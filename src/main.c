#include <pebble.h>
#include "graphics/graphics.h"


static void init() {
  init_graphics(RESOURCE_ID_SUNNY);
}

static void deinit() {
  deinit_graphics();
}

int main() {
  init();
  app_event_loop();
  deinit();
}
