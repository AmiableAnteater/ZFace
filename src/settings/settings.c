#include <pebble.h>
#include "settings/settings.h"

static char* s_api_key = NULL;
static bool s_js_ready = false;


void freeApiKey() {
    if (s_api_key != NULL) {
      free(s_api_key);
    }
    s_api_key = NULL;  
}



void setApiKey(char *owmApiKey) {
  if (owmApiKey == NULL) {
    freeApiKey();
    persist_write_string(MESSAGE_KEY_OWM_APPID, "");
  } else {
    int len = strlen(owmApiKey) * sizeof(char);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Got an api key of length %i[bytes]", len);
    s_api_key = malloc(len + 1);
    strcpy(s_api_key, owmApiKey);
    
    persist_write_string(MESSAGE_KEY_OWM_APPID, s_api_key);
    persist_write_int(MESSAGE_KEY_OWM_APPID_LEN, len + 1);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Persisted API key.");
  }
}



char* getApiKey() {
  return s_api_key;
}



void init_settings(){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> init_settings");
  int api_key_len = persist_read_int(MESSAGE_KEY_OWM_APPID_LEN);
  if (api_key_len > 0) {
    s_api_key = malloc(api_key_len);
    int bytes_read = persist_read_string(MESSAGE_KEY_OWM_APPID, s_api_key, api_key_len);
    if (bytes_read == E_DOES_NOT_EXIST) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "API key did not exist in persistent storage.");
    } else {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Read API key %s from persistent storage.", s_api_key);
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "    api_key_len was 0");
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< init_settings");
}



bool isJsReady() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "checking PebbleKit JS readyness: %s", (s_js_ready ? "true" : "false"));
  return s_js_ready;
}



void setJsReady() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "PebbleKit JS is ready.");
  s_js_ready = true;
}



void deinit_settings() {
  freeApiKey();
}


