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

#include "sopa-parser.h"

G_DEFINE_TYPE (SopaParser, sopa_parser, G_TYPE_OBJECT)

#define PARSER_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), SOPA_TYPE_PARSER, SopaParserPrivate))

struct _SopaParserPrivate
{
  gint x; /* avoid warn */
};


static void
sopa_parser_get_property (GObject    *object,
                          guint       property_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
  switch (property_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
sopa_parser_set_property (GObject      *object,
                          guint         property_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
  switch (property_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
sopa_parser_dispose (GObject *object)
{
  G_OBJECT_CLASS (sopa_parser_parent_class)->dispose (object);
}

static void
sopa_parser_finalize (GObject *object)
{
  G_OBJECT_CLASS (sopa_parser_parent_class)->finalize (object);
}

static void
sopa_parser_class_init (SopaParserClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (SopaParserPrivate));

  object_class->get_property = sopa_parser_get_property;
  object_class->set_property = sopa_parser_set_property;
  object_class->dispose = sopa_parser_dispose;
  object_class->finalize = sopa_parser_finalize;
}

static void
sopa_parser_init (SopaParser *self)
{
  self->priv = PARSER_PRIVATE (self);
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
}
