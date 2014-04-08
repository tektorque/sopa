/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-data.c
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

#include "sopa-data.h"

G_DEFINE_TYPE (SopaData, sopa_data, SOPA_TYPE_NODE)

#define DATA_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), SOPA_TYPE_DATA, SopaDataPrivate))

struct _SopaDataPrivate
{
};


static void
sopa_data_get_property (GObject    *object,
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
sopa_data_set_property (GObject      *object,
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
sopa_data_dispose (GObject *object)
{
  G_OBJECT_CLASS (sopa_data_parent_class)->dispose (object);
}

static void
sopa_data_finalize (GObject *object)
{
  G_OBJECT_CLASS (sopa_data_parent_class)->finalize (object);
}

static void
sopa_data_class_init (SopaDataClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (SopaDataPrivate));

  object_class->get_property = sopa_data_get_property;
  object_class->set_property = sopa_data_set_property;
  object_class->dispose = sopa_data_dispose;
  object_class->finalize = sopa_data_finalize;
}

static void
sopa_data_init (SopaData *self)
{
  self->priv = DATA_PRIVATE (self);
}

SopaData *
sopa_data_new (void)
{
  return g_object_new (SOPA_TYPE_DATA, NULL);
}
