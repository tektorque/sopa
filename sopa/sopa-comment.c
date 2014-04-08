/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-comment.c
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

#include "sopa-comment.h"

G_DEFINE_TYPE (SopaComment, sopa_comment, SOPA_TYPE_NODE)

#define COMMENT_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), SOPA_TYPE_COMMENT, SopaCommentPrivate))

struct _SopaCommentPrivate
{
};


static void
sopa_comment_get_property (GObject    *object,
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
sopa_comment_set_property (GObject      *object,
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
sopa_comment_dispose (GObject *object)
{
  G_OBJECT_CLASS (sopa_comment_parent_class)->dispose (object);
}

static void
sopa_comment_finalize (GObject *object)
{
  G_OBJECT_CLASS (sopa_comment_parent_class)->finalize (object);
}

static void
sopa_comment_class_init (SopaCommentClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (SopaCommentPrivate));

  object_class->get_property = sopa_comment_get_property;
  object_class->set_property = sopa_comment_set_property;
  object_class->dispose = sopa_comment_dispose;
  object_class->finalize = sopa_comment_finalize;
}

static void
sopa_comment_init (SopaComment *self)
{
  self->priv = COMMENT_PRIVATE (self);
}

SopaComment *
sopa_comment_new (void)
{
  return g_object_new (SOPA_TYPE_COMMENT, NULL);
}
