/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-node-private.h
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

#ifndef __SOPA_NODE_PRIVATE_H__
#define __SOPA_NODE_PRIVATE_H__

#include <glib.h>

G_BEGIN_DECLS

/* "protected" methods for subclasses */
void                                sopa_node_add_child                         (SopaNode                 *self,
                                                                                 SopaNode                 *child);
void                                sopa_node_insert_child_at_index             (SopaNode                 *self,
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

G_END_DECLS

#endif /* __SOPA_NODE_PRIVATE_H__ */
