/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-tag.h
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

#ifndef __SOPA_TAG_H__
#define __SOPA_TAG_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define SOPA_TYPE_TAG sopa_tag_get_type()

#define SOPA_TAG(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SOPA_TYPE_TAG, SopaTag))

#define SOPA_TAG_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SOPA_TYPE_TAG, SopaTagClass))

#define SOPA_IS_TAG(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SOPA_TYPE_TAG))

#define SOPA_IS_TAG_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SOPA_TYPE_TAG))

#define SOPA_TAG_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SOPA_TYPE_TAG, SopaTagClass))

typedef struct _SopaTag SopaTag;
typedef struct _SopaTagClass SopaTagClass;
typedef struct _SopaTagPrivate SopaTagPrivate;

struct _SopaTag
{
  GObject parent;

  SopaTagPrivate *priv;
};

struct _SopaTagClass
{
  GObjectClass parent_class;
};

GType sopa_tag_get_type (void) G_GNUC_CONST;

SopaTag *                          sopa_tag_new                                 (const gchar *name);

G_END_DECLS

#endif /* __SOPA_TAG_H__ */
