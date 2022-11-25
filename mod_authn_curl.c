#ifdef HAVE_CONFIG_H
# include <config.h>
#endif	/* HAVE_CONFIG_H */

#ifdef HAVE_HTTPD_HTTPD_H
# include <httpd/httpd.h>
# include <httpd/http_request.h>
# include <httpd/http_log.h>
# include <httpd/mod_auth.h>
#else
# include <apache2/httpd.h>
# include <apache2/http_request.h>
# include <apache2/http_log.h>
# include <apache2/mod_auth.h>
#endif	/* HAVE_HTTPD_HTTPD_H */

#include <curl/curl.h>

typedef struct {
  char *url;
  unsigned short verify_peer;
} authn_curl_config;

static authn_curl_config config;

static const char *set_url(cmd_parms *cmd, void *cfg, const char *arg) {
  (void) cfg;

  apr_uri_t url;
  size_t bufsize = strlen(arg) + 1;
  
  if (apr_uri_parse(cmd->pool, arg, &url) != APR_SUCCESS) {
    /* TODO(cplrossi) log some errors somewhere */
  }

  if ((config.url = apr_pcalloc(cmd->pool, bufsize))) {
    strncpy(config.url, arg, bufsize);
  } else {
    /* TODO(cplrossi) log some errors somewhere */
  }

  return NULL;
}

static const char *set_verify_peer(cmd_parms *cmd, void *cfg, const char *arg) {
  (void) cmd; (void) cfg;

  if (!strcasecmp(arg, "on")) {
    config.verify_peer = 1;
  } else {
    config.verify_peer = 0;
  }

  return NULL;
}

static const command_rec cmds[] =
  {
    AP_INIT_TAKE1("AuthCurlURL", set_url, NULL, OR_AUTHCFG, "Target URL"),
    AP_INIT_TAKE1("AuthCurlVerifyPeer", set_verify_peer, NULL, OR_AUTHCFG, "Enable or disable peer certificate verification"),
    { NULL }
  };

static size_t dummy_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  /* discard contents and make libcurl happy */
  (void) ptr; (void) size; (void) userdata;

  return nmemb;
}

static authn_status check_password(request_rec *r, const char *user,
					      const char *password)
{
  (void) r;

  if (config.url) {
    CURL *curl = curl_easy_init();

    if (curl) {
      authn_status ret = AUTH_DENIED;
      CURLcode res;

      curl_easy_setopt(curl, CURLOPT_USERNAME, user);
      curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
      curl_easy_setopt(curl, CURLOPT_URL, config.url);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, config.verify_peer);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_callback);

      if ((res = curl_easy_perform(curl)) == CURLE_OK) {
	ret = AUTH_GRANTED;
      } else {
	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(00000)
		      "%s", curl_easy_strerror(res));
      }

      curl_easy_cleanup(curl);

      return ret;
    }
  }

  return AUTH_GENERAL_ERROR;
}

static const authn_provider provider =
  {
    &check_password,
    NULL,
  };

static void register_hooks(apr_pool_t *p)
{
  ap_register_auth_provider(p, AUTHN_PROVIDER_GROUP, "curl",
			    AUTHN_PROVIDER_VERSION,
			    &provider, AP_AUTH_INTERNAL_PER_CONF);
}

AP_DECLARE_MODULE(authn_curl) =
  {
    STANDARD20_MODULE_STUFF,
    NULL,			/* dir config creater */
    NULL,			/* dir merger --- default is to override */
    NULL,			/* server config */
    NULL,			/* merge server config */
    cmds,		        /* command apr_table_t */
    register_hooks		/* register hooks */
  };
