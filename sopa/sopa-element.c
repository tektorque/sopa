/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-element.c
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

#include "sopa-element.h"

G_DEFINE_TYPE (SopaElement, sopa_element, SOPA_TYPE_NODE)

#define ELEMENT_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), SOPA_TYPE_ELEMENT, SopaElementPrivate))

struct _SopaElementPrivate
{
  gchar         *tag;
  GHashTable    *attributes;
};

enum {
  PROP_0,

  PROP_TAG,

  PROP_LAST
};

static GParamSpec *obj_props[PROP_LAST];

static void
sopa_element_get_property (GObject    *object,
                           guint       property_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
  SopaElement *elem = SOPA_ELEMENT (object);

  switch (property_id)
    {
    case PROP_TAG:
      g_value_set_string (value, elem->priv->tag);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
sopa_element_set_property (GObject      *object,
                           guint         property_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
  SopaElement *elem = SOPA_ELEMENT (object);

  switch (property_id)
    {
    case PROP_TAG:
      g_free (elem->priv->tag);
      elem->priv->tag = g_value_dup_string (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
sopa_element_dispose (GObject *object)
{
  SopaElement *elem = SOPA_ELEMENT (object);

  if (elem->priv->tag != NULL)
    {
      g_object_unref (elem->priv->tag);
      elem->priv->tag = NULL;
    }

  G_OBJECT_CLASS (sopa_element_parent_class)->dispose (object);
}

static void
sopa_element_finalize (GObject *object)
{
  G_OBJECT_CLASS (sopa_element_parent_class)->finalize (object);
}

static void
sopa_element_class_init (SopaElementClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (SopaElementPrivate));

  object_class->get_property = sopa_element_get_property;
  object_class->set_property = sopa_element_set_property;
  object_class->dispose = sopa_element_dispose;
  object_class->finalize = sopa_element_finalize;

  /**
   * SopaElement:tag:
   *
   * The element's tag
   *
   * Since: 0.2
   */
  obj_props[PROP_TAG] =
    g_param_spec_string ("tag",
                         "Element tag",
                         "The element's tag",
                         NULL,
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, PROP_LAST, obj_props);
}

static void
sopa_element_init (SopaElement *self)
{
  SopaElementPrivate *priv;

  priv = self->priv = ELEMENT_PRIVATE (self);

  priv->attributes = g_hash_table_new_full (g_str_hash,
                                            g_str_equal,
                                            g_free,
                                            g_free);
}

SopaNode *
sopa_element_new (const gchar *tag)
{
  g_return_val_if_fail (tag != NULL, NULL);

  return g_object_new (SOPA_TYPE_ELEMENT,
                       "tag", tag,
                       NULL);
}
