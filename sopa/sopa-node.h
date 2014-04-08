/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-node.h
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

#ifndef __SOPA_NODE_H__
#define __SOPA_NODE_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define SOPA_TYPE_NODE sopa_node_get_type()

#define SOPA_NODE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SOPA_TYPE_NODE, SopaNode))

#define SOPA_NODE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SOPA_TYPE_NODE, SopaNodeClass))

#define SOPA_IS_NODE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SOPA_TYPE_NODE))

#define SOPA_IS_NODE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SOPA_TYPE_NODE))

#define SOPA_NODE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SOPA_TYPE_NODE, SopaNodeClass))

typedef struct _SopaNode SopaNode;
typedef struct _SopaNodeClass SopaNodeClass;
typedef struct _SopaNodePrivate SopaNodePrivate;

struct _SopaNode
{
  GInitiallyUnowned parent;

  SopaNodePrivate *priv;
};

struct _SopaNodeClass
{
  GInitiallyUnownedClass parent_class;
};

GType sopa_node_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* __SOPA_NODE_H__ */
