#include <pebble.h>
#include "comm.h"
#include "../settings/settings.h"
#include "../graphics/graphics.h"



void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read tuples for data
  Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);

  // If all data is available, use it
  if(temp_tuple && conditions_tuple) {
    signalSuccessfulWeatherUpdate();
    updateWeather((int)temp_tuple->value->int32, conditions_tuple->value->cstring);
  }
  
  Tuple *app_id_tuple = dict_find(iterator, MESSAGE_KEY_OWM_APPID);
  if (app_id_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received config App-ID: %s", app_id_tuple->value->cstring);
    if (setApiKey(app_id_tuple->value->cstring)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "API key initialized, requesting weather update.");
      checkWeatherUpdate();
    }
  }
  
  Tuple *js_ready_tuple = dict_find(iterator, MESSAGE_KEY_JS_KIT_READY);
  if (js_ready_tuple) {
    setJsReady();
    // TODO request initial weather
  }
}



void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! Reason code: %i", reason);
}



void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! Reason code: %i", reason);
}



void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}



void init_communication() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> init_communication");
  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  AppMessageResult result = app_message_open(inbox_size, outbox_size);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< init_communication, result %i", result);
}


void checkWeatherUpdate() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> checkWeatherUpdate");
  
  char* apiKey = getApiKey();
  if (apiKey == NULL) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "< checkWeatherUpdate - No api key to send.");
    return;
  }
  
  if (!isJsReady()) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "< checkWeatherUpdate - PebbleKit JS is not yet ready.");
    return;    
  }
  
  unsigned int diff = time(NULL) - getLastWeatherUpdate();
  unsigned int maxDiff = 60 * getUpdateFrequencyInMinutes();
  if (diff >= maxDiff) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Last update was %i seconds ago - more than the configured maximum %i. Updating.", diff, maxDiff);
    // Begin dictionary
    DictionaryIterator *iter;
    AppMessageResult result = app_message_outbox_begin(&iter);
    if(result == APP_MSG_OK) {
      // Construct the message - add a key-value pair
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Sending api key.");
      dict_write_cstring(iter, MESSAGE_KEY_OWM_APPID, apiKey);

      // Send the message!
      app_message_outbox_send();
    } else {
      // The outbox cannot be used right now
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
    }      
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< checkWeatherUpdate");
}