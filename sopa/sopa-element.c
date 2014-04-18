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

#include "sopa-node-private.h"

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

/**
 * sopa_element_new:
 *
 * Creates a new #SopaElement.
 *
 * A newly created element has a floating reference, which will be sunk
 * when it is added to another element.
 *
 * Return value: the newly created #SopaElement
 */
SopaElement *
sopa_element_new (const gchar *tag)
{
  g_return_val_if_fail (tag != NULL, NULL);

  return g_object_new (SOPA_TYPE_ELEMENT,
                       "tag", tag,
                       NULL);
}

/**
 * sopa_element_get_tag:
 * @self: a #SopaElement
 *
 * Retrieves the tag name of @self
 *
 * Return value: (transfer none): the tag name of element. The returned value
 *      is owned by element and should not be modified or freed
 */
const gchar *
sopa_element_get_tag (SopaElement *self)
{
  g_return_val_if_fail (SOPA_IS_ELEMENT (self), NULL);

  return self->priv->tag;
}

/**
 * sopa_element_add_child:
 * @self: a #SopaElement
 * @child: a #SopaNode
 *
 * Adds @child to the children of @self.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling sopa_element_remove_child().
 */
void
sopa_element_add_child (SopaElement *self,
                        SopaNode    *child)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));

  sopa_node_add_child (SOPA_NODE (self), child);
}

/**
 * sopa_element_insert_child_at_index:
 * @self: a #SopaElement
 * @child: a #SopaNode
 * @index_: the index
 *
 * Inserts @child into the list of children of @self, using the
 * given @index_. If @index_ is greater than the number of children
 * in @self, or is less than 0, then the new child is added at the end.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling sopa_element_remove_child().
 */
void
sopa_element_insert_child_at_index (SopaElement *self,
                                    SopaNode    *child,
                                    gint         index_)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));

  sopa_node_insert_child_at_index (SOPA_NODE (self),
                                   child,
                                   index_);
}

/**
 * sopa_element_insert_child_above:
 * @self: a #SopaElement
 * @child: a #SopaNode
 * @sibling: (allow-none): a child of @self, or %NULL
 *
 * Inserts @child into the list of children of @self, above another
 * child of @self or, if @sibling is %NULL, above all the children
 * of @self.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling sopa_element_remove_child().
 */
void
sopa_element_insert_child_above (SopaElement  *self,
                                 SopaNode     *child,
                                 SopaNode     *sibling)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));

  sopa_node_insert_child_above (SOPA_NODE (self),
                                child,
                                sibling);
}

/**
 * sopa_element_insert_child_below:
 * @self: a #SopaElement
 * @child: a #SopaNode
 * @sibling: (allow-none): a child of @self, or %NULL
 *
 * Inserts @child into the list of children of @self, below another
 * child of @self or, if @sibling is %NULL, below all the children
 * of @self.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling sopa_element_remove_child().
 */
void
sopa_element_insert_child_below (SopaElement  *self,
                                 SopaNode     *child,
                                 SopaNode     *sibling)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));

  sopa_node_insert_child_below (SOPA_NODE (self),
                                child,
                                sibling);
}

/**
 * sopa_element_remove_child:
 * @self: a #SopaElement
 * @child: a #SopaNode
 *
 * Removes @child from the children of @self.
 *
 * This function will release the reference added by
 * sopa_element_add_child(), so if you want to keep using @child
 * you will have to acquire a referenced on it before calling this
 * function.
 */
void
sopa_element_remove_child (SopaElement *self,
                           SopaNode    *child)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));

  sopa_node_remove_child (SOPA_NODE (self),
                          child);
}

/**
 * sopa_element_remove_all_children:
 * @self: a #SopaElement
 *
 * Removes all children of @self.
 *
 * This function releases the reference added by inserting a child actor
 * in the list of children of @self.
 *
 * If the reference count of a child drops to zero, the child will be
 * destroyed. If you want to ensure the destruction of all the children
 * of @self, use sopa_element_destroy_all_children().
 */
void
sopa_element_remove_all_children (SopaElement *self)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));

  sopa_node_remove_all_children (SOPA_NODE (self));
}

/**
 * sopa_element_destroy_all_children:
 * @self: a #SopaElement
 *
 * Destroys all children of @self.
 *
 * This function releases the reference added by inserting a child
 * node in the list of children of @self, and ensures that the
 * #SopaElement::destroy signal is emitted on each child of the
 * node.
 *
 * By default, #SopaElement will emit the #SopaElement::destroy signal
 * when its reference count drops to 0; the default handler of the
 * #SopaElement::destroy signal will destroy all the children of a
 * node. This function ensures that all children are destroyed, instead
 * of just removed from @self, unlike sopa_element_remove_all_children()
 * which will merely release the reference and remove each child.
 *
 * Unless you acquired an additional reference on each child of @self
 * prior to calling sopa_element_remove_all_children() and want to reuse
 * the actors, you should use sopa_element_destroy_all_children() in
 * order to make sure that children are destroyed and signal handlers
 * are disconnected even in cases where circular references prevent this
 * from automatically happening through reference counting alone.
 */
void
sopa_element_destroy_all_children (SopaElement *self)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));

  sopa_node_destroy_all_children (SOPA_NODE (self));
}

/**
 * sopa_element_add_attribute:
 * @self: a #SopaElement
 * @key: the attribute name
 * @value: the attribute value
 *
 * Adds a new @key and @value to the #SopaElement attributes
 *
 * If the @key already exists in the #SopaElement its current value is replaced
 * with a new @value
 */
void
sopa_element_add_attribute (SopaElement *self,
                            const gchar *key,
                            const gchar *value)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));
  g_return_if_fail (key != NULL);
  g_return_if_fail (value != NULL);

  g_hash_table_insert (self->priv->attributes,
                       (gpointer) g_strdup (key),
                       (gpointer) g_strdup (value));
}

/**
 * sopa_element_remove_attribute:
 * @self: a #SopaElement
 * @key: the attribute to remove
 *
 * Removes an attribute ans its associated value from a #SopaElement
 *
 * Return value: %TRUE if the attribute was found and removed from
 *      the #SopaElement, %FALSE otherwise
 */
gboolean
sopa_element_remove_attribute (SopaElement *self,
                               const gchar *key)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));
  g_return_if_fail (key != NULL);

  return g_hash_table_remove (self->priv->attributes,
                              (gconstpointer) key);
}

/**
 * sopa_element_set_attribute:
 * @self: a #SopaElement
 * @key: the attribute name
 * @value: the attribute value
 *
 * Sets @key attribute value in the #SopaElement
 *
 * If the @key doesn't exists in the #SopaElement its creates a new attribute
 * with @key and @value
 */
void
sopa_element_set_attribute (SopaElement *self,
                            const gchar *key,
                            const gchar *value)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));
  g_return_if_fail (key != NULL);
  g_return_if_fail (value != NULL);

  g_hash_table_insert (self->priv->attributes,
                       (gpointer) g_strdup (key),
                       (gpointer) g_strdup (value));
}

/**
 * sopa_element_get_attribute:
 * @self: a #SopaElement
 * @key: the attribute name
 *
 * Retrieves the attribute value
 *
 * Return value: (transfer none): the associated value, or %NULL if the
 *      attribute is not found
 */
const gchar *
sopa_element_get_attribute (SopaElement *self,
                            const gchar *key)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));
  g_return_if_fail (key != NULL);

  return g_hash_table_lookup (self->priv->attributes,
                              (gconstpointer) key);
}

/**
 * sopa_element_get_n_attributes:
 * @self: a #SopaElement
 *
 * Retrieves the number of attributes of @self
 *
 * Return value: the number of attributes of an element
 */
guint
sopa_element_get_n_attributes (SopaElement *self)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));

  return g_hash_table_size (self->priv->attributes);
}

/**
 * sopa_element_has_attribute:
 * @self: a #SopaElement
 * @key: the attribute name
 *
 * Returns whether the element has an attribute name equals to @key
 *
 * Return value: %TRUE if the element has @key attribute, %FALSE otherwise
 */
gboolean
sopa_element_has_attribute (SopaElement *self,
                            const gchar *key)
{
  g_return_if_fail (SOPA_IS_ELEMENT (self));
  g_return_if_fail (key != NULL);

  return g_hash_table_contains (self->priv->attributes,
                                (gconstpointer) key);
}
