#include <pebble.h>
#include "graphics/graphics.h"
#include "health/health.h"

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  updateTime(tick_time);
}



static void init() {
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
}



int main() {
  init();
  app_event_loop();
  deinit();
}
