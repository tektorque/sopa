dnl Process this file with autoconf to produce a configure script.

AC_INIT(sopa, 0.0.1)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([sopa/sopa.h])
AC_CONFIG_MACRO_DIR([build])

AM_INIT_AUTOMAKE([1.11 no-define -Wno-portability dist-bzip2 foreign])

AM_SILENT_RULES([yes])

AC_PROG_CC_C_O


AM_PATH_GLIB_2_0

# Initialize libtool
LT_PREREQ([2.2.6])
LT_INIT([disable-static])

dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
#IT_PROG_INTLTOOL([0.35.0])

#GETTEXT_PACKAGE=sopa
#AC_SUBST(GETTEXT_PACKAGE)
#AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
#AM_GLIB_GNU_GETTEXT



dnl ***************************************************************************
dnl Check for Windows
dnl ***************************************************************************
AC_CANONICAL_HOST

case $host_os in
  *mingw*)
    platform_win32=yes
    native_win32=yes
    ;;
  pw32* | *cygwin*)
    platform_win32=yes
    native_win32=no
    ;;
  *)
    platform_win32=no
    native_win32=no
    ;;
esac
AM_CONDITIONAL(PLATFORM_WIN32, test x"$platform_win32" = "xyes")
AM_CONDITIONAL(NATIVE_WIN32, test x"$native_win32" = "xyes")


LT_INIT([win32-dll])


PKG_CHECK_MODULES(SOPA, [gobject-introspection-1.0 gobject-2.0 glib-2.0 ])

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

GOBJECT_INTROSPECTION_CHECK([0.6.5])

AC_OUTPUT([
Makefile
sopa.pc
sopa/Makefile])
