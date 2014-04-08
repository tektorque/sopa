ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

SUBDIRS = sopa

dist_doc_DATA = \
	COPYING \
	AUTHORS \
	INSTALL \
	README.md


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sopa.pc

EXTRA_DIST = \
  sopa.pc.in \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)
