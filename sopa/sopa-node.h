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
#include <sopa/sopa-macros.h>

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

typedef struct _SopaNodeIter SopaNodeIter;

struct _SopaNode
{
  GInitiallyUnowned parent;

  SopaNodePrivate *priv;
};

struct _SopaNodeClass
{
  GInitiallyUnownedClass parent_class;
};

/**
 * SopaNodeIter:
 *
 * An iterator structure that allows to efficiently iterate over a
 * section of the document.
 *
 * The contents of the <structname>SopaNodeIter</structname> structure
 * are private and should only be accessed using the provided API.
 *
 * Since: 0.2
 */
struct _SopaNodeIter {
  /*< private >*/
  gpointer SOPA_PRIVATE_FIELD (dummy1);
  gpointer SOPA_PRIVATE_FIELD (dummy2);
  gpointer SOPA_PRIVATE_FIELD (dummy3);
  gint     SOPA_PRIVATE_FIELD (dummy4);
  gpointer SOPA_PRIVATE_FIELD (dummy5);
};

GType sopa_node_get_type (void) G_GNUC_CONST;

SopaNode *                          sopa_node_new                               (void);
void                                sopa_node_destroy                           (SopaNode                 *self);
void                                sopa_node_add_child                         (SopaNode                 *self,
                                                                                 SopaNode                 *child);
void                                sopa_node_add_child_at_index                (SopaNode                 *self,
                                                                                 SopaNode                 *child,
                                                                                 gint                      index_);
void                                sopa_node_insert_child_above                (SopaNode                 *self,
                                                                                 SopaNode                 *child,
                                                                                 SopaNode                 *sibling);
void                                sopa_node_insert_child_below                (SopaNode                 *self,
                                                                                 SopaNode                 *child,
                                                                                 SopaNode                 *sibling);
void                                sopa_node_remove_child                      (SopaNode                 *self,
                                                                                 SopaNode                 *child);
void                                sopa_node_remove_all_children               (SopaNode                 *self);
void                                sopa_node_destroy_all_children              (SopaNode                 *self);
SopaNode *                          sopa_node_get_parent                        (SopaNode                 *self);
void                                sopa_node_iter_init                         (SopaNodeIter             *iter,
                                                                                 SopaNode                 *root);
gboolean                            sopa_node_iter_is_valid                     (const SopaNodeIter       *iter);
gboolean                            sopa_node_iter_next                         (SopaNodeIter             *iter,
                                                                                 SopaNode                **child);
gboolean                            sopa_node_iter_prev                         (SopaNodeIter             *iter,
                                                                                 SopaNode                **child);
void                                sopa_node_iter_remove                       (SopaNodeIter             *iter);
void                                sopa_node_iter_destroy                      (SopaNodeIter             *iter);

G_END_DECLS

#endif /* __SOPA_NODE_H__ */
