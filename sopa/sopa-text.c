/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-text.c
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

#include "sopa-text.h"

G_DEFINE_TYPE (SopaText, sopa_text, SOPA_TYPE_NODE)

#define TEXT_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), SOPA_TYPE_TEXT, SopaTextPrivate))

struct _SopaTextPrivate
{
  gchar *content;
};

enum {
  PROP_0,

  PROP_CONTENT,

  PROP_LAST
};

static GParamSpec *obj_props[PROP_LAST];

static void
sopa_text_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
  SopaText *text = SOPA_TEXT (object);

  switch (property_id)
    {
    case PROP_CONTENT:
      g_value_set_string (value, text->priv->content);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
sopa_text_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
  SopaText *text = SOPA_TEXT (object);

  switch (property_id)
    {
    case PROP_CONTENT:
      sopa_text_set_content (text, g_value_get_string (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
sopa_text_dispose (GObject *object)
{
  G_OBJECT_CLASS (sopa_text_parent_class)->dispose (object);
}

static void
sopa_text_finalize (GObject *object)
{
  G_OBJECT_CLASS (sopa_text_parent_class)->finalize (object);
}

static void
sopa_text_class_init (SopaTextClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (SopaTextPrivate));

  object_class->get_property = sopa_text_get_property;
  object_class->set_property = sopa_text_set_property;
  object_class->dispose = sopa_text_dispose;
  object_class->finalize = sopa_text_finalize;

  obj_props[PROP_CONTENT] =
    g_param_spec_string ("content",
                         "Content",
                         "The text node content",
                         NULL,
                         G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, PROP_LAST, obj_props);
}

static void
sopa_text_init (SopaText *self)
{
  self->priv = TEXT_PRIVATE (self);
}

/**
 * sopa_text_new:
 *
 * Creates a new #SopaText.
 *
 * A newly created element has a floating reference, which will be sunk
 * when it is added to another element.
 *
 * Return value: the newly created #SopaText
 */
SopaText *
sopa_text_new (void)
{
  return g_object_new (SOPA_TYPE_TEXT, NULL);
}

/**
 * sopa_text_set_content:
 * @self: a #SopaText
 * @content: the content
 *
 * Sets content of @self
 */
void
sopa_text_set_content (SopaText *self,
                       const gchar *content)
{
  g_return_if_fail (SOPA_IS_TEXT (self));

  g_free (self->priv->content);

  if (content != NULL)
    self->priv->content = g_strdup (content);
  else
    self->priv->content = NULL;
}

/**
 * sopa_text_get_content:
 * @self: a #SopaText
 *
 * Retrieves the @self content
 *
 * Return value: (transfer none): the content of node. The returned value
 *      is owned by element and should not be modified or freed
 */
const gchar *
sopa_text_get_content (SopaText *self)
{
  g_return_val_if_fail (SOPA_IS_TEXT (self), NULL);

  return self->priv->content;
}
