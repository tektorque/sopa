/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-node.c (Based on ClutterActor class)
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

/**
 * SECTION:sopa-node
 * @short_description: The basic element of the document
 *
 * The SopaNode class is the basic element of the document in Sopa,
 * and it encapsulates the attributes, childs of a node in
 * the document.
 *
 */

#include "sopa-node.h"
#include "sopa-node-private.h"
#include "sopa-marshal.h"

G_DEFINE_ABSTRACT_TYPE (SopaNode, sopa_node, G_TYPE_INITIALLY_UNOWNED)

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

enum {
  DESTROY,

  LAST_SIGNAL
};

static guint obj_signals[LAST_SIGNAL] = { 0, };

/* Prototypes */
static void               sopa_node_remove_child_internal                       (SopaNode               *self,
                                                                                 SopaNode               *child);

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
sopa_node_real_destroy (SopaNode *node)
{
  SopaNodeIter iter;

  g_object_freeze_notify (G_OBJECT (node));

  sopa_node_iter_init (&iter, node);
  while (sopa_node_iter_next (&iter, NULL))
    sopa_node_iter_destroy (&iter);

  g_object_thaw_notify (G_OBJECT (node));
}

static void
sopa_node_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
  SopaNodePrivate *priv = SOPA_NODE (object)->priv;

  switch (property_id)
    {
    case PROP_NAME:
      g_value_set_string (value, priv->name);
      break;

    case PROP_FIRST_CHILD:
      g_value_set_object (value, priv->first_child);
      break;

    case PROP_LAST_CHILD:
      g_value_set_object (value, priv->last_child);
      break;

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
  SopaNode *node = SOPA_NODE (object);

  switch (property_id)
    {
    case PROP_NAME:
      sopa_node_set_name (node, g_value_get_string (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
sopa_node_dispose (GObject *object)
{
  SopaNode *self = SOPA_NODE (object);
  SopaNodePrivate *priv = self->priv;

  g_signal_emit (self, obj_signals[DESTROY], 0);

  /* avoid recursing when called from sopa_node_destroy() */
  if (priv->parent != NULL)
    {
      SopaNode *parent = priv->parent;

      sopa_node_remove_child_internal (parent, self);
    }

  G_OBJECT_CLASS (sopa_node_parent_class)->dispose (object);
}

static void
sopa_node_finalize (GObject *object)
{
  SopaNode *self = SOPA_NODE (object);
  SopaNodePrivate *priv = self->priv;

  g_free (priv->name);

#ifdef SOPA_ENABLE_DEBUG
  g_free (priv->debug_name);
#endif

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

  klass->destroy = sopa_node_real_destroy;

  /**
   * SopaNode:name:
   *
   * The name of the node
   *
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
   */
  obj_props[PROP_LAST_CHILD] =
    g_param_spec_object ("last-child",
                         "Last child",
                         "The node's last child",
                         SOPA_TYPE_NODE,
                         G_PARAM_READABLE);

  g_object_class_install_properties (object_class, PROP_LAST, obj_props);


  obj_signals[DESTROY] =
    g_signal_new ("destroy",
		              G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_CLEANUP | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
		              G_STRUCT_OFFSET (SopaNodeClass, destroy),
		              NULL, NULL,
		              _sopa_marshal_VOID__VOID,
		              G_TYPE_NONE, 0);
}

static void
sopa_node_init (SopaNode *self)
{
  SopaNodePrivate *priv;

  priv = self->priv = NODE_PRIVATE (self);

  priv->n_children = 0;
  priv->in_destruction = FALSE;
  priv->age = 0;
}

/**
 * sopa_node_destroy:
 * @self: a #SopaNode
 *
 * Destroys a node.  When a node is destroyed, it will break any
 * references it holds to other objects.  If the node is inside a
 * container, the node will be removed.
 *
 * When you destroy a container, its children will be destroyed as well.
 *
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

/**
 * sopa_node_set_name:
 * @self: A #SopaNode
 * @name: Textual tag to apply to actor
 *
 * Sets the given name to @self. The name can be used to identify
 * a #SopaNode.
 */
void
sopa_node_set_name (SopaNode    *self,
                    const gchar *name)
{
  g_return_if_fail (SOPA_IS_NODE (self));

  g_free (self->priv->name);
  self->priv->name = g_strdup (name);

  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_NAME]);
}

/**
 * sopa_node_get_name:
 * @self: A #SopaNode
 *
 * Retrieves the name of @self.
 *
 * Return value: the name of the actor, or %NULL. The returned string is
 *   owned by the node and should not be modified or freed.
 */
const gchar *
sopa_node_get_name (SopaNode *self)
{
  g_return_val_if_fail (SOPA_IS_NODE (self), NULL);

  return self->priv->name;
}

static inline void
remove_child (SopaNode *self,
              SopaNode *child)
{
  SopaNode *prev_sibling, *next_sibling;

  prev_sibling = child->priv->prev_sibling;
  next_sibling = child->priv->next_sibling;

  if (prev_sibling != NULL)
    prev_sibling->priv->next_sibling = next_sibling;

  if (next_sibling != NULL)
    next_sibling->priv->prev_sibling = prev_sibling;

  if (self->priv->first_child == child)
    self->priv->first_child = next_sibling;

  if (self->priv->last_child == child)
    self->priv->last_child = prev_sibling;

  child->priv->parent = NULL;
  child->priv->prev_sibling = NULL;
  child->priv->next_sibling = NULL;
}

static void
sopa_node_remove_child_internal (SopaNode *self,
                                 SopaNode *child)
{
  SopaNode *old_first, *old_last;
  GObject *obj;

  obj = G_OBJECT (self);
  g_object_freeze_notify (obj);

  old_first = self->priv->first_child;
  old_last = self->priv->last_child;

  remove_child (self, child);

  self->priv->n_children -= 1;

  self->priv->age += 1;

  /* we need to emit the signal before dropping the reference */
  //g_signal_emit_by_name (self, "node-removed", child);

  if (old_first != self->priv->first_child)
    g_object_notify_by_pspec (obj, obj_props[PROP_FIRST_CHILD]);

  if (old_last != self->priv->last_child)
    g_object_notify_by_pspec (obj, obj_props[PROP_LAST_CHILD]);

  g_object_thaw_notify (obj);

  /* remove the reference we acquired in sopa_node_add_child() */
  g_object_unref (child);
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

  //g_signal_emit_by_name (self, "node-added", child);

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
 */
void
sopa_node_insert_child_at_index (SopaNode *self,
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

  sopa_node_remove_child_internal (self, child);
}

/**
 * sopa_node_remove_all_children:
 * @self: a #SopaNode
 *
 * Removes all children of @self.
 *
 * This function releases the reference added by inserting a child actor
 * in the list of children of @self.
 *
 * If the reference count of a child drops to zero, the child will be
 * destroyed. If you want to ensure the destruction of all the children
 * of @self, use sopa_node_destroy_all_children().
 *
 */
void
sopa_node_remove_all_children (SopaNode *self)
{
  SopaNodeIter iter;

  g_return_if_fail (SOPA_IS_NODE (self));

  if (self->priv->n_children == 0)
    return;

  g_object_freeze_notify (G_OBJECT (self));

  sopa_node_iter_init (&iter, self);
  while (sopa_node_iter_next (&iter, NULL))
    sopa_node_iter_remove (&iter);

  g_object_thaw_notify (G_OBJECT (self));

  /* sanity check */
  g_assert (self->priv->first_child == NULL);
  g_assert (self->priv->last_child == NULL);
  g_assert (self->priv->n_children == 0);
}

/**
 * sopa_node_destroy_all_children:
 * @self: a #SopaNode
 *
 * Destroys all children of @self.
 *
 * This function releases the reference added by inserting a child
 * node in the list of children of @self, and ensures that the
 * #SopaNode::destroy signal is emitted on each child of the
 * node.
 *
 * By default, #SopaNode will emit the #SopaNode::destroy signal
 * when its reference count drops to 0; the default handler of the
 * #SopaNode::destroy signal will destroy all the children of an
 * node. This function ensures that all children are destroyed, instead
 * of just removed from @self, unlike sopa_node_remove_all_children()
 * which will merely release the reference and remove each child.
 *
 * Unless you acquired an additional reference on each child of @self
 * prior to calling sopa_node_remove_all_children() and want to reuse
 * the actors, you should use sopa_node_destroy_all_children() in
 * order to make sure that children are destroyed and signal handlers
 * are disconnected even in cases where circular references prevent this
 * from automatically happening through reference counting alone.
 *
 */
void
sopa_node_destroy_all_children (SopaNode *self)
{
  SopaNodeIter iter;

  g_return_if_fail (SOPA_IS_NODE (self));

  if (self->priv->n_children == 0)
    return;

  g_object_freeze_notify (G_OBJECT (self));

  sopa_node_iter_init (&iter, self);
  while (sopa_node_iter_next (&iter, NULL))
    sopa_node_iter_destroy (&iter);

  g_object_thaw_notify (G_OBJECT (self));

  /* sanity check */
  g_assert (self->priv->first_child == NULL);
  g_assert (self->priv->last_child == NULL);
  g_assert (self->priv->n_children == 0);
}

/**
 * sopa_node_get_n_children:
 * @self: a #SopaNode
 *
 * Retrieves the number of children of @self
 *
 * Return value: number of children of a node
 */
gint
sopa_node_get_n_children (SopaNode *self)
{
  g_return_val_if_fail (SOPA_IS_NODE (self), 0);

  return self->priv->n_children;
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
  ri->age = root->priv->age;
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

      sopa_node_remove_child_internal (ri->root, cur);

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
