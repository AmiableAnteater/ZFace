#include <pebble.h>
#include "settings.h"

static char* s_api_key = NULL;
static bool s_js_ready = false;
static time_t s_last_weather_update = 0;
static unsigned int s_freq_in_minutes = 0;
static bool s_use_celsius = true;
static bool s_weather_config_changed = false;
static WeatherLocale_t s_weather_locale = ENGLISH;


void setUpdateFrequencyInMinutes(unsigned int frequency) {
  if (frequency > 0 && frequency != s_freq_in_minutes) {
    s_freq_in_minutes = frequency;
    persist_write_int(MESSAGE_KEY_UPDATE_FREQ, s_freq_in_minutes);
    s_weather_config_changed = true;
  }
}



unsigned int getUpdateFrequencyInMinutes() {
  return (unsigned int) s_freq_in_minutes;
}



void freeApiKey() {
    if (s_api_key != NULL) {
      free(s_api_key);
    }
    s_api_key = NULL;  
}



bool setApiKey(char *owmApiKey) {
  if (owmApiKey == NULL) {
    freeApiKey();
    persist_write_string(MESSAGE_KEY_OWM_APPID, "");
  } else {
    bool apiKeyWasNull = s_api_key == NULL;
    int len = strlen(owmApiKey) * sizeof(char);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Got an api key of length %i[bytes]", len);
    s_api_key = malloc(len + 1);
    strcpy(s_api_key, owmApiKey);
    
    persist_write_string(MESSAGE_KEY_OWM_APPID, s_api_key);
    persist_write_int(MESSAGE_KEY_OWM_APPID_LEN, len + 1);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Persisted API key.");
    s_weather_config_changed = true;
    return apiKeyWasNull;
  }
  return false;
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
  
  s_freq_in_minutes = persist_read_int(MESSAGE_KEY_UPDATE_FREQ);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Read update frequence %i from persistent storage.", s_freq_in_minutes);
  if (s_freq_in_minutes == 0) {
    setUpdateFrequencyInMinutes(10);
  }
  
  s_use_celsius = persist_read_int(MESSAGE_KEY_UNIT_TEMP) < 2;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Read temperature unit %s from persistent storage.", s_use_celsius ? "Celsius" : "Fahrenheit");
  
  
  s_weather_locale = persist_read_int(MESSAGE_KEY_WEATHER_LOCALE);
  if (s_weather_locale == 0) {
    s_weather_locale = ENGLISH;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Read weather locale %i from persistent storage.", s_weather_locale);
  
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



void signalSuccessfulWeatherUpdate() {
  s_last_weather_update = time(NULL);
  s_weather_config_changed = false;
  
  struct tm *localtm = localtime(&s_last_weather_update);
  static char s_buffer[128];
  strftime(s_buffer, sizeof(s_buffer), "%Y/%m/%d %H:%M:%S", localtm);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather updated at %s.", s_buffer);
}


time_t getLastWeatherUpdate() {
  return s_last_weather_update;
}


void setTemperatureUnitToCelsius(bool useDegreesCelsius) {
  if (useDegreesCelsius != s_use_celsius) {
    s_weather_config_changed = true;
    s_use_celsius = useDegreesCelsius;
    persist_write_int(MESSAGE_KEY_UNIT_TEMP, s_use_celsius ? 1 : 2);
  }
}


bool useCelsius() {
  return s_use_celsius;
}


bool weatherConfigChanged() {
  return s_weather_config_changed;
}


void setWeatherLocale(WeatherLocale_t loc) {
  if (loc != s_weather_locale) {
    s_weather_config_changed = true;
    s_weather_locale = loc;
    persist_write_int(MESSAGE_KEY_WEATHER_LOCALE, s_weather_locale);
  }
}


WeatherLocale_t getWeatherLocale() {
  return s_weather_locale;
}

