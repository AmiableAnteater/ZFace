#include <pebble.h>
#include "settings/settings.h"
#include "graphics/graphics.h"
#include "health/health.h"
#include "communication/comm.h"
#include "weather/weather.h"

/*
TODOs:
- include sunrise/sunset/twilight: http://sunrise-sunset.org/api

- Configuration
  - step target
  - locale
  https://developer.pebble.com/guides/user-interfaces/app-configuration/
  https://github.com/pebble/clay
  
  
- Show weekday + date
*/


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  updateTime(tick_time);
}



static void init() {
  // Register AppMessage callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  init_communication();
  init_settings();

  init_graphics(RESOURCE_ID_SUNNY);
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  updateTime(tick_time);
  
#if defined(PBL_HEALTH)
  // Attempt to subscribe 
  if(health_service_events_subscribe(health_handler, NULL)) {
    updateSteps(get_todays_steps());
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Health subscription failed!");
  }
#else
  APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
#endif
}



static void deinit() {
  deinit_graphics();
  deinit_settings();
}



int main() {
  init();
  app_event_loop();
  deinit();
}
