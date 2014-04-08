SUBDIRS = sopa

ACLOCAL_AMFLAGS = -I m4

pcfiles = sopa-$(SOPA_API_VERSION).pc

sopa-$(SOPA_API_VERSION).pc: sopa.pc
	@cp -f $< $@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sopa.pc

EXTRA_DIST = \
  sopa.pc.in

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)
