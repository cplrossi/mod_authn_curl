# Apache Module mod_authn_curl

## Every _curlable_ resource can be a source of authentication

What if you run an email service with a user base and want to
authenticate such users against a private resource under your
**[httpd](https://httpd.apache.org/)** instance? I do know that
nowadays there are sophisticated ways to do such things, but
[ZNC](https://wiki.znc.in/ZNC) uses a simple approach with its
[imapauth module](https://wiki.znc.in/Imapauth). _httpd_ doesn't come
with an IMAP provider.

Well, **[libcurl](https://curl.se/libcurl/)** abstracts implementation
details of a wide range of protocols, possibly encrypted with
[GnuTLS](https://www.gnutls.org/) or
[OpenSSL](https://www.openssl.org/). Resource fetching is done by
_libcurl_ with a URL that follows the rules for a certain protocol,
e.g. `https://example.com:8443/private` or `imap://localhost:143/`.

To successfully retrieve such resources, it is sometimes required to
authenticate according to the methods provided for each protocol,
e.g. **basic authentication** for _http_ and **plain** for _imap_.

Here's an _httpd_ authentication module that is not just an IMAP
provider, but a more general provider that relies on any resource
could be accessed by _libcurl_, using the credentials provided by the
_httpd_ users.

Security and performance considerations are left to yourself...

Tested on [Arch Linux](https://archlinux.org/) and [Debian
bullseye](https://www.debian.org/releases/bullseye/).

## Install

Download the latest release from this repository.

You'll need [APR](https://apr.apache.org/) and _httpd_ headers in
order to build this module. Consult your distro documentation to get
this things up.

The standard GNU `INSTALL` file is provided, but tipically it' just a
matter of:

``` shell
cd $YOUR_UNPACKED_TARBALL
mkdir build && cd build
../configure && make && sudo make install
```

The module is installed into `/usr/local/lib/mod_authn_curl/`.

## Configuration

Load the module using an _httpd_ directive like:

``` apacheconf
LoadModule authn_curl_module  /usr/local/lib/mod_authn_curl/mod_authn_curl.so
```

Module specific directives are `AuthCurlURL` and optional
`AuthCurlVerifyPeer` (defaults to `Off`).

An example conf could be:

``` apacheconf
...

<IfModule authn_curl_module>
	<Location "/">
		AuthType basic
		AuthName "suca"
		AuthBasicProvider curl
		AuthCurlURL "imap://localhost:143/"
		Require valid-user
	</Location>
</IfModule>

...
```

You could do some nice self DoSsing by specifying an `AuthCurlURL`
handled by the module itself.

## (Non) Community

Maybe find someone on `#scrocco` on `irc.libera.chat`to complain.
