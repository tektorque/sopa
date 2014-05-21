/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-parser.c
 * Copyright (C) 2014 Tektorque, Lda <geral@tektorque.com>
 * 
 * sopa is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * sopa is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *    Emanuel Fernandes <efernandes@tektorque.com>
 */

#include <string.h>
#include "sopa-parser.h"
#include "sopa-element.h"
#include "sopa-comment.h"
#include "sopa-data.h"
#include "sopa-text.h"

G_DEFINE_TYPE (SopaParser, sopa_parser, G_TYPE_OBJECT)

#define PARSER_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), SOPA_TYPE_PARSER, SopaParserPrivate))

struct _SopaParserPrivate
{
  /* used during parsing */
  SopaDocument  *doc;
  GQueue        *stack;
};

static void
sopa_parser_finalize (GObject *object)
{
  SopaParser *parser = SOPA_PARSER (object);

  /* Free stack */
  g_queue_free_full (parser->priv->stack,
                     (GDestroyNotify) g_object_unref);

  G_OBJECT_CLASS (sopa_parser_parent_class)->finalize (object);
}

static void
sopa_parser_class_init (SopaParserClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (SopaParserPrivate));

  object_class->finalize = sopa_parser_finalize;
}

static void
sopa_parser_init (SopaParser *self)
{
  self->priv = PARSER_PRIVATE (self);

  self->priv->stack = g_queue_new ();
}

static void
sopa_parser_stack_give_parent (SopaParser   *self,
                               SopaElement  *parent,
                               gint          pos)
{
  gint i;
  guint stack_len = g_queue_get_length (self->priv->stack);

  if (pos < 0)
    pos = (gint) stack_len;

  for (i = 0; i < pos; i++)
    {
      sopa_element_insert_child_at_index (parent,
                                          SOPA_NODE (g_queue_pop_head (self->priv->stack)),
                                          0);
    }
}

static void
handle_start_element (GMarkupParseContext *context,
                      const gchar         *element_name,
                      const gchar        **attribute_names,
                      const gchar        **attribute_values,
                      gpointer             user_data,
                      GError             **error)
{
  SopaParser *parser = SOPA_PARSER (user_data);
  SopaElement *elem;

  elem = sopa_element_new (element_name);

  /* Add attributes */
  gint i = 0;
  while (attribute_names[i] && attribute_values[i])
    {
      sopa_element_add_attribute (elem,
                                  attribute_names[i],
                                  attribute_values[i]);
      i++;
    }

  g_queue_push_head (parser->priv->stack, elem);
}

static void
handle_end_element (GMarkupParseContext *context,
                    const gchar         *element_name,
                    gpointer             user_data,
                    GError             **error)
{
  SopaParser *parser = SOPA_PARSER (user_data);
  SopaNode *node = NULL;
  guint idx;
  gboolean f_elem;

  for (idx = 0, f_elem = FALSE;
       idx < g_queue_get_length (parser->priv->stack) && !f_elem;
       idx++)
    {
      node = g_queue_peek_nth (parser->priv->stack, idx);

      if (!SOPA_IS_ELEMENT (node))
        continue;

      if (g_strcmp0 (sopa_element_get_tag (SOPA_ELEMENT (node)),
                    element_name) == 0)
        f_elem = TRUE;
      else
        continue;

      sopa_parser_stack_give_parent (parser,
                                     SOPA_ELEMENT (node),
                                     idx);
    }

  if (f_elem == FALSE)
    {
      //TODO improve this
      g_set_error_literal (error,
                           G_MARKUP_ERROR,
                           G_MARKUP_ERROR_PARSE,
                           "Something goes wrong...");
      return;
    }
}

static void
handle_text (GMarkupParseContext *context,
             const gchar         *text,
             gsize                text_len,
             gpointer             user_data,
             GError             **error)
{
  if (text_len <= 0)
    return;

  SopaParser *parser = SOPA_PARSER (user_data);
  SopaText *elem;
  gchar *content;

  /* Create a nul-terminated string */
  content = g_strchomp (g_strndup (text, text_len));

  if (strlen (content))
    {
      elem = sopa_text_new ();
      sopa_text_set_content (elem, text);

      g_queue_push_head (parser->priv->stack, elem);
    }

  g_free (content);
}

static void
handle_passthrough (GMarkupParseContext *context,
                    const gchar         *passthrough_text,
                    gsize                text_len,
                    gpointer             user_data,
                    GError             **error)
{
  //SopaParser *parser = SOPA_PARSER (user_data);
  g_print ("handle_passthrough\n");
}

static void
handle_error (GMarkupParseContext *context,
              GError              *error,
              gpointer             user_data)
{
  //SopaParser *parser = SOPA_PARSER (user_data);
  const gchar *cur_elem;

  cur_elem = g_markup_parse_context_get_element (context);
  g_print ("handle_error elem:%s\n", cur_elem);
}

static gboolean
sopa_parser_parse_data (SopaParser   *self,
                        const gchar  *text,
                        gssize        text_len,
                        GError      **error)
{
  GMarkupParser parser = {
    handle_start_element,
    handle_end_element,
    handle_text,
    handle_passthrough,
    handle_error
  };

  GMarkupParseContext *context = NULL;
  GError *internal_error = NULL;

  context = g_markup_parse_context_new (&parser, 0, self, NULL);

  if (!g_markup_parse_context_parse (context, text, text_len, &internal_error));


  if (!g_markup_parse_context_end_parse (context, &internal_error));


  /* Adds remaining stack elements to the root (document) element */
  self->priv->doc = sopa_document_new ();
  sopa_parser_stack_give_parent (self,
                                 SOPA_ELEMENT (self->priv->doc),
                                 -1);
  goto exit;
exit:
  g_markup_parse_context_free (context);

  if (internal_error != NULL)
    {
      g_propagate_error (error, internal_error);
      //g_clear_error (&internal_error);
      return FALSE;
    }

  return TRUE;
}

/**
 * sopa_parser_new:
 *
 * Creates a new #SopaParser
 *
 * Return value: the newly created #SopaParser
 */
SopaParser *
sopa_parser_new (void)
{
  return g_object_new (SOPA_TYPE_PARSER, NULL);
}

/**
 * sopa_parser_parse:
 * @self: a #SopaParser
 * @text: chunk of text to parse
 * @text_len: length of text in bytes
 * @error: return location for a #GError
 *
 * Tries to parse @text
 *
 * The data need not be valid UTF-8; an error will be signaled if it's invalid.
 *
 * Return value: (transfer full): the newly created #SopaDocument if successful
 *      or %NULL otherwise
 */
SopaDocument *
sopa_parser_parse (SopaParser   *self,
                   const gchar  *text,
                   gssize        text_len,
                   GError      **error)
{
  g_return_val_if_fail (SOPA_IS_PARSER (self), NULL);
  g_return_val_if_fail (text != NULL, NULL);

  if (sopa_parser_parse_data (self,
                              text,
                              text_len,
                              error))
    return self->priv->doc;

  return NULL;
}

/**
 * sopa_parser_parse_async:
 * @self: a #SopaParser
 * @text: chunk of text to parse
 * @text_len: length of text in bytes
 * @cancellable: a #GCancellable
 * @callback: the callback
 * @user_data: user data
 *
 * Tries to parse @text asynchronously.
 *
 * The data need not be valid UTF-8; an error will be signaled if it's invalid.
 */
void
sopa_parser_parse_async (SopaParser           *self,
                         const gchar          *text,
                         gssize                text_len,
                         GCancellable         *cancellable,
                         GAsyncReadyCallback   callback,
                         gpointer              user_data)
{
  g_return_if_fail (SOPA_IS_PARSER (self));
  g_return_if_fail (text != NULL);

  //TODO
}
