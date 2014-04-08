/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * sopa-macros.h
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
 */
 
 #if !defined(SOPA_H_INSIDE) && !defined(SOPA_COMPILATION)
#error "Only <sopa/sopa.h> can be included directly.h"
#endif

#ifndef __SOPA_MACROS_H__
#define __SOPA_MACROS_H__

#include <sopa/sopa-version.h>

/* some structures are meant to be opaque and still be allocated on the stack;
 * in order to avoid people poking at their internals, we use this macro to
 * ensure that users don't accidentally access a struct private members.
 *
 * we use the SOPA_COMPILATION define to allow us easier access, though.
 */
#ifdef SOPA_COMPILATION
#define SOPA_PRIVATE_FIELD(x)        x
#else
#define SOPA_PRIVATE_FIELD(x)        sopa_private_ ## x
#endif

#endif /* __SOPA_MACROS_H__ */
