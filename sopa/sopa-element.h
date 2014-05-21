/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-element.h
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

#ifndef __SOPA_ELEMENT_H__
#define __SOPA_ELEMENT_H__

#include <glib-object.h>
#include <sopa/sopa-node.h>

G_BEGIN_DECLS

#define SOPA_TYPE_ELEMENT sopa_element_get_type()

#define SOPA_ELEMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SOPA_TYPE_ELEMENT, SopaElement))

#define SOPA_ELEMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SOPA_TYPE_ELEMENT, SopaElementClass))

#define SOPA_IS_ELEMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SOPA_TYPE_ELEMENT))

#define SOPA_IS_ELEMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SOPA_TYPE_ELEMENT))

#define SOPA_ELEMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SOPA_TYPE_ELEMENT, SopaElementClass))

typedef struct _SopaElement SopaElement;
typedef struct _SopaElementClass SopaElementClass;
typedef struct _SopaElementPrivate SopaElementPrivate;

struct _SopaElement
{
  SopaNode parent;

  SopaElementPrivate *priv;
};

struct _SopaElementClass
{
  SopaNodeClass parent_class;
};

GType sopa_element_get_type (void) G_GNUC_CONST;

SopaElement *                       sopa_element_new                            (const gchar                *tag);
const gchar *                       sopa_element_get_tag                        (SopaElement                *self);
void                                sopa_element_add_child                      (SopaElement                *self,
                                                                                 SopaNode                   *child);
void                                sopa_element_insert_child_at_index          (SopaElement                *self,
                                                                                 SopaNode                   *child,
                                                                                 gint                        index_);
void                                sopa_element_insert_child_above             (SopaElement                *self,
                                                                                 SopaNode                   *child,
                                                                                 SopaNode                   *sibling);
void                                sopa_element_insert_child_below             (SopaElement                *self,
                                                                                 SopaNode                   *child,
                                                                                 SopaNode                   *sibling);
void                                sopa_element_remove_child                   (SopaElement                *self,
                                                                                 SopaNode                   *child);
void                                sopa_element_remove_all_children            (SopaElement                *self);
void                                sopa_element_destroy_all_children           (SopaElement                *self);
gint                                sopa_element_get_n_children                 (SopaElement                *self);
void                                sopa_element_add_attribute                  (SopaElement                *self,
                                                                                 const gchar                *key,
                                                                                 const gchar                *value);
gboolean                            sopa_element_remove_attribute               (SopaElement                *self,
                                                                                 const gchar                *key);
void                                sopa_element_set_attribute                  (SopaElement                *self,
                                                                                 const gchar                *key,
                                                                                 const gchar                *value);
const gchar *                       sopa_element_get_attribute                  (SopaElement                *self,
                                                                                 const gchar                *key);
guint                               sopa_element_get_n_attributes               (SopaElement                *self);
gboolean                            sopa_element_has_attribute                  (SopaElement                *self,
                                                                                 const gchar                *key);
gchar *                             sopa_element_to_string                      (SopaElement                *self,
                                                                                 guint                       indent_width);

G_END_DECLS

#endif /* __SOPA_ELEMENT_H__ */
