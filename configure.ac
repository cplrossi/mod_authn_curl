#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([mod_authn_curl], [0.1.0], [cplrossi@scroc.co])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([mod_authn_curl.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
LT_PREREQ([2.4.7-dirty])
LT_INIT

# Checks for programs.
AC_PROG_CPP
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

# Checks for libraries.
LIBCURL_CHECK_CONFIG([], [], [], AC_MSG_ERROR([libcurl not found]))

APR_FIND_APR([], [], 1, 1, [apr_acceptable=yes])
if test x"${apr_found}" != x"yes"; then
   AC_MSG_ERROR([apr not found])
fi
AC_SUBST([apr_config])

# Checks for header files.
CPPFLAGS_OLD="$CPPFLAGS"
CPPFLAGS="$($apr_config --cppflags --includes) $CPPFLAGS"
AC_CHECK_HEADERS([httpd/httpd.h apache2/httpd.h], [httpd_found="yes"])

if test x"${httpd_found}" != x"yes"; then
   AC_MSG_ERROR([httpd headers not found])
fi
CPPFLAGS="$CPPFLAGS_OLD"

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([strcasecmp])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
