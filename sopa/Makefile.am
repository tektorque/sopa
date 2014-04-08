## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(SOPA_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = libsopa.la


libsopa_la_SOURCES = \
	lib.c

libsopa_la_LDFLAGS = 

libsopa_la_LIBADD = $(SOPA_LIBS)

include_HEADERS = \
	sopa.h
