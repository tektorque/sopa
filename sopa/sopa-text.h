/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-text.h
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

#if !defined(SOPA_H_INSIDE) && !defined(SOPA_COMPILATION)
#error "Only <sopa/sopa.h> can be included directly.h"
#endif

#ifndef __SOPA_TEXT_H__
#define __SOPA_TEXT_H__

#include <glib-object.h>
#include <sopa/sopa-node.h>

G_BEGIN_DECLS

#define SOPA_TYPE_TEXT sopa_text_get_type()

#define SOPA_TEXT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SOPA_TYPE_TEXT, SopaText))

#define SOPA_TEXT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SOPA_TYPE_TEXT, SopaTextClass))

#define SOPA_IS_TEXT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SOPA_TYPE_TEXT))

#define SOPA_IS_TEXT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SOPA_TYPE_TEXT))

#define SOPA_TEXT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SOPA_TYPE_TEXT, SopaTextClass))

typedef struct _SopaText SopaText;
typedef struct _SopaTextClass SopaTextClass;
typedef struct _SopaTextPrivate SopaTextPrivate;

struct _SopaText
{
  SopaNode parent;

  SopaTextPrivate *priv;
};

struct _SopaTextClass
{
  SopaNodeClass parent_class;
};

GType sopa_text_get_type (void) G_GNUC_CONST;

SopaText *                          sopa_text_new                               (void);
void                                sopa_text_set_content                       (SopaText               *self,
                                                                                 const gchar            *content);
const gchar *                       sopa_text_get_content                       (SopaText               *self);

G_END_DECLS

#endif /* __SOPA_TEXT_H__ */
