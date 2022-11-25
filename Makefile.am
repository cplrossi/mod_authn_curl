## Makefile.am -- process this file with automake to produce Makefile.in
ACLOCAL_AMFLAGS = -I m4 -I .
AM_CPPFLAGS = $(LIBCURL_CPPFLAGS) $$($(apr_config) --cppflags --includes)
AM_CFLAGS = -Wall -Wextra -pedantic $$($(apr_config) --cflags)
AM_LDFLAGS = -module $$($(apr_config) --ldflags --link-libtool) $(LIBCURL) -version-info 0:0:0

pkglib_LTLIBRARIES = mod_authn_curl.la
mod_authn_curl_la_SOURCES = mod_authn_curl.c
