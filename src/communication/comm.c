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
    updateWeather((int)temp_tuple->value->int32, conditions_tuple->value->cstring);
  }
  
  Tuple *app_id_tuple = dict_find(iterator, MESSAGE_KEY_OWM_APPID);
  if (app_id_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received config App-ID: %s", app_id_tuple->value->cstring);
    setApiKey(app_id_tuple->value->cstring);
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

