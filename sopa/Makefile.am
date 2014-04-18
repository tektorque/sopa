NULL =

BUILT_SOURCES = \
	sopa-enum-types.h                     \
	sopa-enum-types.c                     \
	sopa-marshal.h                        \
	sopa-marshal.c

# keep sorted alphabetically, please
source_h = \
  $(top_srcdir)/sopa/sopa-comment.h     \
  $(top_srcdir)/sopa/sopa-data.h        \
  $(top_srcdir)/sopa/sopa-document.h    \
  $(top_srcdir)/sopa/sopa-element.h     \
  $(top_srcdir)/sopa/sopa-node.h        \
  $(top_srcdir)/sopa/sopa-parser.h      \
  $(top_srcdir)/sopa/sopa-text.h        \
  $(NULL)

source_h_priv = \
  $(top_srcdir)/sopa/sopa-node-private.h\
  $(NULL)

source_c = \
  $(top_srcdir)/sopa/sopa-comment.c     \
  $(top_srcdir)/sopa/sopa-data.c        \
  $(top_srcdir)/sopa/sopa-document.c    \
  $(top_srcdir)/sopa/sopa-element.c     \
  $(top_srcdir)/sopa/sopa-node.c        \
  $(top_srcdir)/sopa/sopa-parser.c      \
  $(top_srcdir)/sopa/sopa-text.c        \
  $(NULL)

EXTRA_DIST = \
	sopa-marshal.list                     \
	sopa-enum-types.h.in                  \
	sopa-enum-types.c.in                  \
	sopa-version.h.in

STAMP_FILES = stamp-sopa-marshal.h stamp-sopa-enum-types.h

CLEANFILES = $(STAMP_FILES) $(BUILT_SOURCES)

sopa-marshal.h: stamp-sopa-marshal.h
	@true
stamp-sopa-marshal.h: Makefile sopa-marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) \
		--prefix=_sopa_marshal \
		--header \
	$(srcdir)/sopa-marshal.list > xgen-tmh && \
	(cmp -s xgen-tmh sopa-marshal.h || cp -f xgen-tmh sopa-marshal.h) && \
	rm -f xgen-tmh && \
	echo timestamp > $(@F)

sopa-marshal.c: Makefile sopa-marshal.list
	$(AM_V_GEN)(echo "#include \"sopa-marshal.h\"" ; \
	 $(GLIB_GENMARSHAL) \
		--prefix=_sopa_marshal \
		--body \
	 $(srcdir)/sopa-marshal.list ) > xgen-tmc && \
	cp -f xgen-tmc sopa-marshal.c && \
	rm -f xgen-tmc

sopa-enum-types.h: stamp-sopa-enum-types.h Makefile
	@true
stamp-sopa-enum-types.h: $(source_h) sopa-enum-types.h.in
	$(AM_V_GEN)( cd $(srcdir) && \
	  $(GLIB_MKENUMS) \
	    --template sopa-enum-types.h.in \
	  $(source_h) ) >> xgen-teth && \
	(cmp -s xgen-teth sopa-enum-types.h || cp xgen-teth sopa-enum-types.h) && \
	rm -f xgen-teth && \
	echo timestamp > $(@F)

sopa-enum-types.c: stamp-sopa-enum-types.h sopa-enum-types.c.in
	$(AM_V_GEN)( cd $(srcdir) && \
	  $(GLIB_MKENUMS) \
	    --template sopa-enum-types.c.in \
	  $(source_h) ) >> xgen-tetc && \
	cp xgen-tetc sopa-enum-types.c && \
	rm -f xgen-tetc

# libsopa library

sopaincludedir = $(includedir)/sopa-$(SOPA_API_VERSION)/sopa
sopainclude_DATA = \
	$(source_h) \
	$(top_srcdir)/sopa/sopa.h \
	$(top_builddir)/sopa/sopa-version.h \
	$(top_builddir)/sopa/sopa-enum-types.h \
	$(NULL)

lib_LTLIBRARIES = libsopa-@SOPA_API_VERSION@.la

common_includes = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" 	\
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" 				\
	-DPACKAGE_DATA_DIR=\""$(datadir)"\"				\
	-DSOPA_COMPILATION

common_ldflags = \
	-export-dynamic			\
	-export-symbols-regex "^sopa.*"	\
	-rpath $(libdir)

libsopa_@SOPA_API_VERSION@_la_CFLAGS =	\
	$(common_includes)		\
	-DG_LOG_DOMAIN=\"Sopa\"		\
	$(SOPA_MAINTAINER_CFLAGS)	\
	$(SOPA_DEBUG_CFLAGS)		\
	$(SOPA_CFLAGS)			\
	$(NULL)

libsopa_@SOPA_API_VERSION@_la_LDFLAGS = $(SOPA_LT_LDFLAGS) $(common_ldflags)

libsopa_@SOPA_API_VERSION@_la_SOURCES =	\
	$(BUILT_SOURCES) \
	$(source_h)	\
	$(source_h_priv) \
	$(source_c)	\
	$(top_srcdir)/sopa/sopa.h \
	$(NULL)

libsopa_@SOPA_API_VERSION@_la_LIBADD = $(SOPA_LIBS)

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)

INTROSPECTION_GIRS = Sopa-@SOPA_API_VERSION@.gir

Sopa-@SOPA_API_VERSION@.gir: libsopa-@SOPA_API_VERSION@.la Makefile $(INTROSPECTION_SCANNER)

Sopa_@SOPA_API_VERSION_AM@_gir_NAMESPACE = Sopa
Sopa_@SOPA_API_VERSION_AM@_gir_VERSION   = @SOPA_API_VERSION@
Sopa_@SOPA_API_VERSION_AM@_gir_LIBS      = libsopa-@SOPA_API_VERSION@.la
Sopa_@SOPA_API_VERSION_AM@_gir_FILES     = \
	$(source_h) \
	$(top_srcdir)/sopa/sopa.h \
	$(top_builddir)/sopa/sopa-version.h \
	$(top_builddir)/sopa/sopa-enum-types.h \
	$(source_c)
Sopa_@SOPA_API_VERSION_AM@_gir_CFLAGS = $(common_includes)
Sopa_@SOPA_API_VERSION_AM@_gir_INCLUDES = GObject-2.0 Gio-2.0
Sopa_@SOPA_API_VERSION_AM@_gir_SCANNERFLAGS = \
	--warn-all \
	--c-include='sopa/sopa.h' \
	--pkg-export=sopa-@SOPA_API_VERSION@

girdir = $(datadir)/gir-1.0/
gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0/
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)
endif # HAVE_INTROSPECTION
