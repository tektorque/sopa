/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-parser.h
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

#ifndef __SOPA_PARSER_H__
#define __SOPA_PARSER_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <sopa/sopa-document.h>

G_BEGIN_DECLS

#define SOPA_TYPE_PARSER sopa_parser_get_type()

#define SOPA_PARSER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SOPA_TYPE_PARSER, SopaParser))

#define SOPA_PARSER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SOPA_TYPE_PARSER, SopaParserClass))

#define SOPA_IS_PARSER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SOPA_TYPE_PARSER))

#define SOPA_IS_PARSER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SOPA_TYPE_PARSER))

#define SOPA_PARSER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SOPA_TYPE_PARSER, SopaParserClass))

typedef struct _SopaParser SopaParser;
typedef struct _SopaParserClass SopaParserClass;
typedef struct _SopaParserPrivate SopaParserPrivate;

struct _SopaParser
{
  GObject parent;

  SopaParserPrivate *priv;
};

struct _SopaParserClass
{
  GObjectClass parent_class;
};

GType sopa_parser_get_type (void) G_GNUC_CONST;

SopaParser *                      sopa_parser_new                               (void);
SopaDocument *                    sopa_parser_parse                             (SopaParser             *self,
                                                                                 const gchar            *text,
                                                                                 gssize                  text_len,
                                                                                 GError                **error);
void                              sopa_parser_parse_async                       (SopaParser             *self,
                                                                                 const gchar            *text,
                                                                                 gssize                  text_len,
                                                                                 GCancellable           *cancellable,
                                                                                 GAsyncReadyCallback     callback,
                                                                                 gpointer                user_data);

G_END_DECLS

#endif /* __SOPA_PARSER_H__ */
