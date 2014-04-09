/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-node.c
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

#include "sopa-node.h"

G_DEFINE_TYPE (SopaNode, sopa_node, G_TYPE_INITIALLY_UNOWNED)

#define NODE_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), SOPA_TYPE_NODE, SopaNodePrivate))

struct _SopaNodePrivate
{
  /* a non-unique name, used for debugging */
  gchar     *name;

  SopaNode  *parent;
  SopaNode  *prev_sibling;
  SopaNode  *next_sibling;
  SopaNode  *first_child;
  SopaNode  *last_child;

  gint       n_children;

  /* tracks whenever the children of an actor are changed; the
   * age is incremented by 1 whenever an actor is added or
   * removed. the age is not incremented when the first or the
   * last child pointers are changed, or when grandchildren of
   * an actor are changed.
   */
  gint       age;

#ifdef SOPA_ENABLE_DEBUG
  /* a string used for debugging messages */
  gchar *debug_name;
#endif

  gboolean   in_destruction;
};

enum {
  PROP_0,

  PROP_NAME,

  PROP_FIRST_CHILD,
  PROP_LAST_CHILD,

  PROP_LAST
};

static GParamSpec *obj_props[PROP_LAST];


/*< private >
 * sopa_node_get_debug_name:
 * @node: a #SopaNode
 *
 * Retrieves a printable name of @node for debugging messages
 *
 * Return value: a string with a printable name
 */
const gchar *
_sopa_node_get_debug_name (SopaNode *node)
{
  SopaNodePrivate *priv = node->priv;
  const gchar *retval;

#ifdef SOPA_ENABLE_DEBUG
  if (G_UNLIKELY (priv->debug_name == NULL))
    {
      priv->debug_name = g_strdup_printf ("<%s>[<%s>:%p]",
                                          priv->name != NULL ? priv->name
                                                             : "unnamed",
                                          G_OBJECT_TYPE_NAME (node),
                                          node);
    }

  retval = priv->debug_name;
#else
  retval = priv->name != NULL
         ? priv->name
         : G_OBJECT_TYPE_NAME (node);
#endif

  return retval;
}

static void
sopa_node_get_property (GObject    *object,
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
sopa_node_set_property (GObject      *object,
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
sopa_node_dispose (GObject *object)
{
  G_OBJECT_CLASS (sopa_node_parent_class)->dispose (object);
}

static void
sopa_node_finalize (GObject *object)
{
  G_OBJECT_CLASS (sopa_node_parent_class)->finalize (object);
}

static void
sopa_node_class_init (SopaNodeClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (SopaNodePrivate));

  object_class->get_property = sopa_node_get_property;
  object_class->set_property = sopa_node_set_property;
  object_class->dispose = sopa_node_dispose;
  object_class->finalize = sopa_node_finalize;

  /**
   * SopaNode:name:
   *
   * The name of the node
   *
   * Since: 0.2
   */
  obj_props[PROP_NAME] =
    g_param_spec_string ("name",
                         "Name",
                         "Name of the node",
                         NULL,
                         G_PARAM_READWRITE);

  /**
   * SopaNode:first-child:
   *
   * The node's first child
   *
   * Since: 0.2
   */
  obj_props[PROP_FIRST_CHILD] =
    g_param_spec_object ("first-child",
                         "First child",
                         "The node's first child",
                         SOPA_TYPE_NODE,
                         G_PARAM_READABLE);

  /**
   * SopaNode:last-child:
   *
   * The node's last child
   *
   * Since: 0.2
   */
  obj_props[PROP_LAST_CHILD] =
    g_param_spec_object ("last-child",
                         "Last child",
                         "The node's last child",
                         SOPA_TYPE_NODE,
                         G_PARAM_READABLE);

  g_object_class_install_properties (object_class, PROP_LAST, obj_props);
}

static void
sopa_node_init (SopaNode *self)
{
  SopaNodePrivate *priv;

  priv = self->priv = NODE_PRIVATE (self);

  priv->n_children = 0;
  priv->in_destruction = FALSE;
}

/**
 * sopa_node_new:
 *
 * Creates a new #SopaNode.
 *
 * A newly created node has a floating reference, which will be sunk
 * when it is added to another node.
 *
 * Return value: the newly created #SopaNode
 *
 * Since: 0.2
 */
SopaNode *
sopa_node_new (void)
{
  return g_object_new (SOPA_TYPE_NODE, NULL);
}

/**
 * sopa_node_destroy:
 * @self: a #ClutterActor
 *
 * Destroys a node.  When a node is destroyed, it will break any
 * references it holds to other objects.  If the node is inside a
 * container, the node will be removed.
 *
 * When you destroy a container, its children will be destroyed as well.
 *
 * Since: 0.2
 */
void
sopa_node_destroy (SopaNode *self)
{
  g_return_if_fail (SOPA_IS_NODE (self));

  g_object_ref (self);

  /* avoid recursion while destroying */
  if (!self->priv->in_destruction)
    {
      self->priv->in_destruction = TRUE;

      g_object_run_dispose (G_OBJECT (self));

      self->priv->in_destruction = FALSE;
    }

  g_object_unref (self);
}

static void
insert_child_at_index (SopaNode *self,
                       SopaNode *child,
                       gpointer  data_)
{
  gint index_ = GPOINTER_TO_INT (data_);

  child->priv->parent = self;

  if (index_ == 0)
    {
      SopaNode *tmp = self->priv->first_child;

      if (tmp != NULL)
        tmp->priv->prev_sibling = child;

      child->priv->prev_sibling = NULL;
      child->priv->next_sibling = tmp;
    }
  else if (index_ < 0 || index_ >= self->priv->n_children)
    {
      SopaNode *tmp = self->priv->last_child;

      if (tmp != NULL)
        tmp->priv->next_sibling = child;

      child->priv->prev_sibling = tmp;
      child->priv->next_sibling = NULL;
    }
  else
    {
      SopaNode *iter;
      int i;

      for (iter = self->priv->first_child, i = 0;
           iter != NULL;
           iter = iter->priv->next_sibling, i += 1)
        {
          if (index_ == i)
            {
              SopaNode *tmp = iter->priv->prev_sibling;

              child->priv->prev_sibling = tmp;
              child->priv->next_sibling = iter;

              iter->priv->prev_sibling = child;

              if (tmp != NULL)
                tmp->priv->next_sibling = child;

              break;
            }
        }
    }

  if (child->priv->prev_sibling == NULL)
    self->priv->first_child = child;

  if (child->priv->next_sibling == NULL)
    self->priv->last_child = child;
}

static void
insert_child_above (SopaNode *self,
                    SopaNode *child,
                    gpointer  data)
{
  SopaNode *sibling = data;

  child->priv->parent = self;

  if (sibling == NULL)
    sibling = self->priv->last_child;

  child->priv->prev_sibling = sibling;

  if (sibling != NULL)
    {
      SopaNode *tmp = sibling->priv->next_sibling;

      child->priv->next_sibling = tmp;

      if (tmp != NULL)
        tmp->priv->prev_sibling = child;

      sibling->priv->next_sibling = child;
    }
  else
    child->priv->next_sibling = NULL;

  if (child->priv->prev_sibling == NULL)
    self->priv->first_child = child;

  if (child->priv->next_sibling == NULL)
    self->priv->last_child = child;
}

static void
insert_child_below (SopaNode *self,
                    SopaNode *child,
                    gpointer  data)
{
  SopaNode *sibling = data;

  child->priv->parent = self;

  if (sibling == NULL)
    sibling = self->priv->first_child;

  child->priv->next_sibling = sibling;

  if (sibling != NULL)
    {
      SopaNode *tmp = sibling->priv->prev_sibling;

      child->priv->prev_sibling = tmp;

      if (tmp != NULL)
        tmp->priv->next_sibling = child;

      sibling->priv->prev_sibling = child;
    }
  else
    child->priv->prev_sibling = NULL;

  if (child->priv->prev_sibling == NULL)
    self->priv->first_child = child;

  if (child->priv->next_sibling == NULL)
    self->priv->last_child = child;
}

typedef void (* SopaNodeAddChildFunc) (SopaNode     *parent,
                                       SopaNode     *child,
                                       gpointer      data);

/*< private >
 * sopa_node_add_child_internal:
 * @self: a #SopaNode
 * @child: a #SopaNode
 * @add_func: delegate function
 * @data: (closure): data to pass to @add_func
 *
 * Adds @child to the list of children of @self.
 *
 * The actual insertion inside the list is delegated to @add_func: this
 * function will just set up the state, perform basic checks, and emit
 * signals.
 */
static inline void
sopa_node_add_child_internal (SopaNode              *self,
                              SopaNode              *child,
                              SopaNodeAddChildFunc   add_func,
                              gpointer               data)
{
  SopaNode *old_first_child, *old_last_child;
  GObject *obj;

  if (child->priv->parent != NULL)
    {
      g_warning ("The node '%s' already has a parent, '%s'. You must "
                 "use sopa_node_remove_child() first.",
                 _sopa_node_get_debug_name (child),
                 _sopa_node_get_debug_name (child->priv->parent));
      return;
    }

  if (child->priv->in_destruction)
    {
      g_warning ("The node '%s' is currently being destroyed, and "
                 "cannot be added as a child of another node.",
                 _sopa_node_get_debug_name (child));

      return;
    }

  old_first_child = self->priv->first_child;
  old_last_child = self->priv->last_child;

  obj = G_OBJECT (self);
  g_object_freeze_notify (obj);

  g_object_ref_sink (child);
  child->priv->parent = NULL;
  child->priv->next_sibling = NULL;
  child->priv->prev_sibling = NULL;

  /* delegate the actual insertion */
  add_func (self, child, data);

  g_assert (child->priv->parent == self);

  self->priv->n_children += 1;

  self->priv->age += 1;

  if (old_first_child != self->priv->first_child)
    g_object_notify_by_pspec (obj, obj_props[PROP_FIRST_CHILD]);

  if (old_last_child != self->priv->last_child)
    g_object_notify_by_pspec (obj, obj_props[PROP_LAST_CHILD]);

  g_object_thaw_notify (obj);
}

/**
 * sopa_node_add_child:
 * @self: a #SopaNode
 * @child: a #SopaNode
 *
 * Adds @child to the children of @self.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling sopa_node_remove_child().
 *
 * Since: 0.2
 */
void
sopa_node_add_child (SopaNode *self,
                     SopaNode *child)
{
  g_return_if_fail (SOPA_IS_NODE (self));
  g_return_if_fail (SOPA_IS_NODE (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child->priv->parent == NULL);

  sopa_node_add_child_internal (self,
                                child,
                                insert_child_at_index,
                                GINT_TO_POINTER (-1));
}

/**
 * sopa_node_insert_child_at_index:
 * @self: a #SopaNode
 * @child: a #SopaNode
 * @index_: the index
 *
 * Inserts @child into the list of children of @self, using the
 * given @index_. If @index_ is greater than the number of children
 * in @self, or is less than 0, then the new child is added at the end.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling sopa_node_remove_child().
 *
 * Since: 0.2
 */
void
sopa_node_add_child_at_index (SopaNode *self,
                              SopaNode *child,
                              gint index_)
{
  g_return_if_fail (SOPA_IS_NODE (self));
  g_return_if_fail (SOPA_IS_NODE (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child->priv->parent == NULL);

  sopa_node_add_child_internal (self, child,
                                insert_child_at_index,
                                GINT_TO_POINTER (index_));
}

/**
 * sopa_node_insert_child_above:
 * @self: a #SopaNode
 * @child: a #SopaNode
 * @sibling: (allow-none): a child of @self, or %NULL
 *
 * Inserts @child into the list of children of @self, above another
 * child of @self or, if @sibling is %NULL, above all the children
 * of @self.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling sopa_node_remove_child().
 *
 * Since: 0.2
 */
void
sopa_node_insert_child_above (SopaNode *self,
                              SopaNode *child,
                              SopaNode *sibling)
{
  g_return_if_fail (SOPA_IS_NODE (self));
  g_return_if_fail (SOPA_IS_NODE (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child != sibling);
  g_return_if_fail (child->priv->parent == NULL);
  g_return_if_fail (sibling == NULL ||
                    (SOPA_IS_NODE (sibling) &&
                     sibling->priv->parent == self));

  sopa_node_add_child_internal (self, child,
                                insert_child_above,
                                sibling);
}

/**
 * sopa_node_insert_child_below:
 * @self: a #SopaNode
 * @child: a #SopaNode
 * @sibling: (allow-none): a child of @self, or %NULL
 *
 * Inserts @child into the list of children of @self, below another
 * child of @self or, if @sibling is %NULL, below all the children
 * of @self.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling sopa_node_remove_child().
 *
 * Since: 0.2
 */
void
sopa_node_insert_child_below (SopaNode *self,
                              SopaNode *child,
                              SopaNode *sibling)
{
  g_return_if_fail (SOPA_IS_NODE (self));
  g_return_if_fail (SOPA_IS_NODE (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child != sibling);
  g_return_if_fail (child->priv->parent == NULL);
  g_return_if_fail (sibling == NULL ||
                    (SOPA_IS_NODE (sibling) &&
                     sibling->priv->parent == self));

  sopa_node_add_child_internal (self, child,
                                insert_child_below,
                                sibling);
}

/**
 * sopa_node_remove_child:
 * @self: a #SopaNode
 * @child: a #SopaNode
 *
 * Removes @child from the children of @self.
 *
 * This function will release the reference added by
 * sopa_node_add_child(), so if you want to keep using @child
 * you will have to acquire a referenced on it before calling this
 * function.
 *
 * Since: 0.2
 */
void
sopa_node_remove_child (SopaNode *self,
                        SopaNode *child)
{
  g_return_if_fail (SOPA_IS_NODE (self));
  g_return_if_fail (SOPA_IS_NODE (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child->priv->parent == NULL);
  g_return_if_fail (child->priv->parent == self);

  //TODO
}

/**
 * sopa_node_get_parent:
 * @self: A #SopaNode
 *
 * Retrieves the parent of @self.
 *
 * Return Value: (transfer none): The #SopaNode parent, or %NULL
 *  if no parent is set
 */
SopaNode *
sopa_node_get_parent (SopaNode *self)
{
  g_return_val_if_fail (SOPA_IS_NODE (self), NULL);

  return self->priv->parent;
}

/* easy way to have properly named fields instead of the dummy ones
 * we use in the public structure
 */
typedef struct _RealNodeIter
{
  SopaNode *root;               /* dummy1 */
  SopaNode *current;            /* dummy2 */
  gpointer padding_1;           /* dummy3 */
  gint age;                     /* dummy4 */
  gpointer padding_2;           /* dummy5 */
} RealNodeIter;

/**
 * sopa_node_iter_init:
 * @iter: a #SopaNodeIter
 * @root: a #SopaNode
 *
 * Initializes a #SopaNodeIter, which can then be used to iterate
 * efficiently over a section of the document, and associates it
 * with @root.
 *
 * Modifying the document section that contains @root will invalidate
 * the iterator.
 *
 * |[
 *   SopaNodeIter iter;
 *   SopaNode *child;
 *
 *   sopa_node_iter_init (&iter, container);
 *   while (sopa_node_iter_next (&iter, &child))
 *     {
 *       /&ast; do something with child &ast;/
 *     }
 * ]|
 *
 * Since: 0.2
 */
void
sopa_node_iter_init (SopaNodeIter *iter,
                     SopaNode     *root)
{
  RealNodeIter *ri = (RealNodeIter *) iter;

  g_return_if_fail (iter != NULL);
  g_return_if_fail (SOPA_IS_NODE (root));

  ri->root = root;
  ri->current = NULL;
  ri->age = 0;
}

/**
 * sopa_node_iter_is_valid:
 * @iter: a #SopaNodeIter
 *
 * Checks whether a #SopaNodeIter is still valid.
 *
 * An iterator is considered valid if it has been initialized, and
 * if the #SopaNode that it refers to hasn't been modified after
 * the initialization.
 *
 * Return value: %TRUE if the iterator is valid, and %FALSE otherwise
 *
 * Since: 0.2
 */
gboolean
sopa_node_iter_is_valid (const SopaNodeIter *iter)
{
  RealNodeIter *ri = (RealNodeIter *) iter;

  g_return_val_if_fail (iter != NULL, FALSE);

  if (ri->root == NULL)
    return FALSE;

  return ri->root->priv->age == ri->age;
}

/**
 * sopa_node_iter_next:
 * @iter: a #SopaNodeIter
 * @child: (out) (transfer none): return location for a #SopaNode
 *
 * Advances the @iter and retrieves the next child of the root #SopaNode
 * that was used to initialize the #SopaNodeIterator.
 *
 * If the iterator can advance, this function returns %TRUE and sets the
 * @child argument.
 *
 * If the iterator cannot advance, this function returns %FALSE, and
 * the contents of @child are undefined.
 *
 * Return value: %TRUE if the iterator could advance, and %FALSE otherwise.
 *
 * Since: 0.2
 */
gboolean
sopa_node_iter_next (SopaNodeIter   *iter,
                     SopaNode     **child)
{
  RealNodeIter *ri = (RealNodeIter *) iter;

  g_return_val_if_fail (iter != NULL, FALSE);
  g_return_val_if_fail (ri->root != NULL, FALSE);
#ifndef G_DISABLE_ASSERT
  g_return_val_if_fail (ri->age == ri->root->priv->age, FALSE);
#endif

  if (ri->current == NULL)
    ri->current = ri->root->priv->first_child;
  else
    ri->current = ri->current->priv->next_sibling;

  if (child != NULL)
    *child = ri->current;

  return ri->current != NULL;
}

/**
 * sopa_node_iter_prev:
 * @iter: a #SopaNodeIter
 * @child: (out) (transfer none): return location for a #SopaNode
 *
 * Advances the @iter and retrieves the previous child of the root
 * #SopaNode that was used to initialize the #SopaNodeIterator.
 *
 * If the iterator can advance, this function returns %TRUE and sets the
 * @child argument.
 *
 * If the iterator cannot advance, this function returns %FALSE, and
 * the contents of @child are undefined.
 *
 * Return value: %TRUE if the iterator could advance, and %FALSE otherwise.
 *
 * Since: 0.2
 */
gboolean
sopa_node_iter_prev (SopaNodeIter   *iter,
                     SopaNode      **child)
{
  RealNodeIter *ri = (RealNodeIter *) iter;

  g_return_val_if_fail (iter != NULL, FALSE);
  g_return_val_if_fail (ri->root != NULL, FALSE);
#ifndef G_DISABLE_ASSERT
  g_return_val_if_fail (ri->age == ri->root->priv->age, FALSE);
#endif

  if (ri->current == NULL)
    ri->current = ri->root->priv->last_child;
  else
    ri->current = ri->current->priv->prev_sibling;

  if (child != NULL)
    *child = ri->current;

  return ri->current != NULL;
}

/**
 * sopa_node_iter_remove:
 * @iter: a #SopaNodeIter
 *
 * Safely removes the #SopaNode currently pointer to by the iterator
 * from its parent.
 *
 * This function can only be called after sopa_node_iter_next() or
 * sopa_node_iter_prev() returned %TRUE, and cannot be called more
 * than once for the same actor.
 *
 * This function will call sopa_node_remove_child() internally.
 *
 * Since: 0.2
 */
void
sopa_node_iter_remove (SopaNodeIter *iter)
{
  RealNodeIter *ri = (RealNodeIter *) iter;
  SopaNode *cur;

  g_return_if_fail (iter != NULL);
  g_return_if_fail (ri->root != NULL);
#ifndef G_DISABLE_ASSERT
  g_return_if_fail (ri->age == ri->root->priv->age);
#endif
  g_return_if_fail (ri->current != NULL);

  cur = ri->current;

  if (cur != NULL)
    {
      ri->current = cur->priv->prev_sibling;

      //TODO class sopa_node_remove_child (cur)

      ri->age += 1;
    }
}

/**
 * sopa_node_iter_destroy:
 * @iter: a #SopaNodeIter
 *
 * Safely destroys the #SopaNode currently pointer to by the iterator
 * from its parent.
 *
 * This function can only be called after sopa_node_iter_next() or
 * sopa_node_iter_prev() returned %TRUE, and cannot be called more
 * than once for the same actor.
 *
 * This function will call sopa_node_destroy() internally.
 *
 * Since: 0.2
 */
void
sopa_node_iter_destroy (SopaNodeIter *iter)
{
  RealNodeIter *ri = (RealNodeIter *) iter;
  SopaNode *cur;

  g_return_if_fail (iter != NULL);
  g_return_if_fail (ri->root != NULL);
#ifndef G_DISABLE_ASSERT
  g_return_if_fail (ri->age == ri->root->priv->age);
#endif
  g_return_if_fail (ri->current != NULL);

  cur = ri->current;

  if (cur != NULL)
    {
      ri->current = cur->priv->prev_sibling;

      sopa_node_destroy (cur);

      ri->age += 1;
    }
}
