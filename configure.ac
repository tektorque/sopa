# bump micro_version to the next even number for each point release
# bump micro_version to the next odd number after each release
m4_define([sopa_major_version], [0])
m4_define([sopa_minor_version], [0])
m4_define([sopa_micro_version], [1])

m4_define([sopa_version], [sopa_major_version.sopa_minor_version.sopa_micro_version])
m4_define([sopa_api_version], [0.2])
m4_define([sopa_api_version_am], [0\_2])

m4_define([sopa_release_status],
          [m4_if(m4_eval(sopa_micro_version % 2), [1], [git],
                 [m4_if(m4_eval(sopa_minor_version % 2), [1], [snapshot],
                                                              [release])])])

# bump up by 1 for every micro release with no API changes, otherwise
# set to 0. after release, bump up by 1
m4_define([sopa_interface_age], [1])
m4_define([sopa_binary_age], [m4_eval(100 * sopa_minor_version + sopa_micro_version)])

m4_define([lt_current], [m4_eval(100 * sopa_minor_version + sopa_micro_version - sopa_interface_age)])
m4_define([lt_revision], [sopa_interface_age])
m4_define([lt_age], [m4_eval(sopa_binary_age - sopa_interface_age)])

m4_define([glib_req_version], [2.28])

AC_PREREQ([2.63])

AC_INIT([sopa],
        [sopa_version],
        [],
        [sopa])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([sopa/sopa.h])

AM_INIT_AUTOMAKE([1.11 no-define -Wno-portability dist-bzip2 foreign])

AM_SILENT_RULES([yes])

AC_PROG_CC_C_O

AM_PATH_GLIB_2_0

# Initialize libtool
LT_PREREQ([2.2.6])
LT_INIT([disable-static])

SOPA_MAJOR_VERSION=sopa_major_version
SOPA_MINOR_VERSION=sopa_minor_version
SOPA_MICRO_VERSION=sopa_micro_version
SOPA_VERSION=sopa_version
SOPA_API_VERSION=sopa_api_version
SOPA_API_VERSION_AM=sopa_api_version_am
SOPA_RELEASE_STATUS=sopa_release_status
AC_SUBST(SOPA_MAJOR_VERSION)
AC_SUBST(SOPA_MICRO_VERSION)
AC_SUBST(SOPA_MINOR_VERSION)
AC_SUBST(SOPA_VERSION)
AC_SUBST(SOPA_API_VERSION)
AC_SUBST(SOPA_API_VERSION_AM)
AC_SUBST(SOPA_RELEASE_STATUS)

SOPA_LT_CURRENT=lt_current
SOPA_LT_REVISION=lt_revision
SOPA_LT_AGE=lt_age
SOPA_LT_VERSION="$SOPA_LT_CURRENT:$SOPA_LT_REVISION:$SOPA_LT_AGE"
SOPA_LT_LDFLAGS="-version-info $SOPA_LT_VERSION"
AC_SUBST(SOPA_LT_LDFLAGS)

dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
#IT_PROG_INTLTOOL([0.35.0])

#GETTEXT_PACKAGE=sopa
#AC_SUBST(GETTEXT_PACKAGE)
#AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
#AM_GLIB_GNU_GETTEXT

PKG_CHECK_MODULES(SOPA, [gobject-introspection-1.0 gobject-2.0 glib-2.0 ])

dnl ***************************************************************************
dnl Enable debug level
dnl ***************************************************************************

m4_define([debug_default], [m4_if(m4_eval(sopa_minor_version % 2), [1], [yes], [minimum])])

AC_ARG_ENABLE([debug],
              [AC_HELP_STRING([--enable-debug=@<:@no/minimum/yes@:>@],
                              [turn on debugging @<:@default=debug_default@:>@])],
              [],
              [enable_debug=debug_default])

AS_CASE([$enable_debug],

        [yes],
        [
          test "$cflags_set" = set || CFLAGS="$CFLAGS -g"
          SOPA_DEBUG_CFLAGS="-DSOPA_ENABLE_DEBUG"
        ],

        [minimum],
        [
          SOPA_DEBUG_CFLAGS="-DSOPA_ENABLE_DEBUG -DG_DISABLE_CAST_CHECKS"
        ],

        [no],
        [
          SOPA_DEBUG_CFLAGS="-DG_DISABLE_ASSERT -DG_DISABLE_CHECKS -DG_DISABLE_CAST_CHECKS"
        ],

        [AC_MSG_ERROR([Unknown argument to --enable-debug])]
)

AC_SUBST(SOPA_DEBUG_CFLAGS)

dnl ***************************************************************************
dnl = Enable strict compiler flags
dnl ***************************************************************************

# use strict compiler flags only on development releases
m4_define([maintainer_flags_default], [m4_if(m4_eval(sopa_minor_version % 2), [1], [yes], [no])])
AC_ARG_ENABLE([maintainer-flags],
              [AC_HELP_STRING([--enable-maintainer-flags=@<:@no/yes@:>@],
                              [Use strict compiler flags @<:@default=maintainer_flags_default@:>@])],
              [],
              [enable_maintainer_flags=maintainer_flags_default])

AS_IF([test "x$enable_maintainer_flags" = "xyes" && test "x$GCC" = "xyes"],
      [
        AS_COMPILER_FLAGS([SOPA_MAINTAINER_CFLAGS],
                          ["-Wall -Wshadow -Wcast-align
                            -Wno-uninitialized -Wformat-security
                            -Wno-strict-aliasing -Winit-self
                            -Wempty-body -Wdeclaration-after-statement
                            -Wno-strict-aliasing -Wno-strict-aliasing"])
      ],
      [
        AS_COMPILER_FLAGS([SOPA_MAINTAINER_CFLAGS],
                          ["-Wall"])
      ]
)
AC_SUBST([SOPA_MAINTAINER_CFLAGS])

dnl ***************************************************************************
dnl GCOV coverage checks
dnl ***************************************************************************
AC_MSG_CHECKING([whether to build with gcov testing])
AC_ARG_ENABLE([gcov],
              [AS_HELP_STRING([--enable-gcov],
                              [Whether to enable coverage testing (requires gcc and gcov)])],
              [],
              [enable_gcov=no])

AS_IF([test "x$enable_gcov" = "xyes" && test "x$GCC" = "xyes"],
      [
        GCOV_CFLAGS="$CFLAGS -O0 -g -fprofile-arcs -ftest-coverage"
        GCOV_LDFLAGS="$LDFLAGS -lgcov"
      ]
)

AM_CONDITIONAL([GCOV_ENABLED], [test "x$enable_gcov" = "xyes"])
AC_SUBST([GCOV_CFLAGS])
AC_SUBST([GCOV_LDFLAGS])
AC_MSG_RESULT([$enable_gcov])

GTK_DOC_CHECK([1.14])

GOBJECT_INTROSPECTION_CHECK([0.6.5])

AC_CONFIG_FILES([
  Makefile
  sopa.pc
  sopa/sopa-version.h
  sopa/Makefile
])

AC_OUTPUT

dnl === Summary ===

echo ""
echo " Sopa - $VERSION (${SOPA_RELEASE_STATUS})"
echo ""
echo " • Prefix: ${prefix}"
echo ""
echo " • Debug level: ${enable_debug}"
echo " • Compiler flags: ${SOPA_MAINTAINER_CFLAGS}"
echo ""
echo " • API reference: ${enable_gtk_doc}"
echo ""
echo " • Build introspection data: ${enable_introspection}"
echo " • Enable coverage test: ${enable_gcov}"
echo ""
