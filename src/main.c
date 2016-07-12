#include <pebble.h>
#include "graphics/graphics.h"

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
}

static void deinit() {
  deinit_graphics();
}

int main() {
  init();
  app_event_loop();
  deinit();
}
