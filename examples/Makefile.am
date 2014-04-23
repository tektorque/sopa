NULL =

AM_CFLAGS = $(SOPA_CFLAGS) $(SOPA_MAINTAINER_CFLAGS)
LDADD = $(top_builddir)/sopa/libsopa-$(SOPA_API_VERSION).la $(SOPA_LIBS)

INCLUDES = \
	-I$(top_srcdir)               \
	-I$(top_builddir)

noinst_PROGRAMS =               \
  parse_and_dump                \
	$(NULL)

dump_SOURCES = parse_and_dump.c

EXTRA_DIST =
