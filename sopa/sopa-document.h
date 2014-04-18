/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-document.h
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

#ifndef __SOPA_DOCUMENT_H__
#define __SOPA_DOCUMENT_H__

#include <glib-object.h>
#include <sopa/sopa-element.h>
#include <sopa/sopa-enum-types.h>

G_BEGIN_DECLS

#define SOPA_TYPE_DOCUMENT sopa_document_get_type()

#define SOPA_DOCUMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SOPA_TYPE_DOCUMENT, SopaDocument))

#define SOPA_DOCUMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SOPA_TYPE_DOCUMENT, SopaDocumentClass))

#define SOPA_IS_DOCUMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SOPA_TYPE_DOCUMENT))

#define SOPA_IS_DOCUMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SOPA_TYPE_DOCUMENT))

#define SOPA_DOCUMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SOPA_TYPE_DOCUMENT, SopaDocumentClass))

typedef struct _SopaDocument SopaDocument;
typedef struct _SopaDocumentClass SopaDocumentClass;
typedef struct _SopaDocumentPrivate SopaDocumentPrivate;

/**
 * SopaDocumentType:
 * @SOPA_DOCUMENT_TYPE_UNKNOWN: Unknown document type
 * @SOPA_DOCUMENT_TYPE_HTML_401_STRICT: This DTD contains all HTML elements
 * and attributes, but does NOT INCLUDE presentational or deprecated
 * elements (like font). Framesets are not allowed.
 * @SOPA_DOCUMENT_TYPE_HTML_401_TRANSITIONAL: This DTD contains all HTML
 * elements and attributes, INCLUDING presentational and deprecated
 * elements (like font). Framesets are not allowed.
 * @SOPA_DOCUMENT_TYPE_HTML_401_FRAMESET: This DTD is equal to
 * HTML 4.01 Transitional, but allows the use of frameset content.
 * @SOPA_DOCUMENT_TYPE_XHTML_10_STRICT: This DTD contains all HTML elements
 * and attributes, but does NOT INCLUDE presentational or deprecated
 * elements (like font). Framesets are not allowed. The markup must also
 * be written as well-formed XML.
 * @SOPA_DOCUMENT_TYPE_XHTML_10_TRANSITIONAL: This DTD contains all HTML
 * elements and attributes, INCLUDING presentational and deprecated
 * elements (like font). Framesets are not allowed. The markup must also
 * be written as well-formed XML.
 * @SOPA_DOCUMENT_TYPE_XHTML_10_FRAMESET: This DTD is equal to
 * XHTML 1.0 Transitional, but allows the use of frameset content.
 * @SOPA_DOCUMENT_TYPE_XHTML_11: This DTD is equal to XHTML 1.0 Strict,
 * but allows you to add modules (for example to provide ruby support
 * for East-Asian languages).
 * @SOPA_DOCUMENT_TYPE_HTML_5: <!DOCTYPE html>
 *
 * DOCTYPE declaration
 *
 * Since: 0.2
 */
typedef enum { /*< PREFIX=SOPA_DOCUMENT_TYPE >*/
  SOPA_DOCUMENT_TYPE_UNKNOWN,
  SOPA_DOCUMENT_TYPE_HTML_401_STRICT,
  SOPA_DOCUMENT_TYPE_HTML_401_TRANSITIONAL,
  SOPA_DOCUMENT_TYPE_HTML_401_FRAMESET,
  SOPA_DOCUMENT_TYPE_XHTML_10_STRICT,
  SOPA_DOCUMENT_TYPE_XHTML_10_TRANSITIONAL,
  SOPA_DOCUMENT_TYPE_XHTML_10_FRAMESET,
  SOPA_DOCUMENT_TYPE_XHTML_11,
  SOPA_DOCUMENT_TYPE_HTML_5
} SopaDocumentType;

struct _SopaDocument
{
  SopaElement parent;

  SopaDocumentPrivate *priv;
};

struct _SopaDocumentClass
{
  SopaElementClass parent_class;
};

GType sopa_document_get_type (void) G_GNUC_CONST;

SopaDocument *sopa_document_new (void);

G_END_DECLS

#endif /* __SOPA_DOCUMENT_H__ */
