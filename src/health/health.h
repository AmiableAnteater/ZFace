#pragma once
#include <pebble.h>  
#include "../graphics/graphics.h"

void health_handler(HealthEventType event, void *context);
int get_todays_steps();