#include <pebble.h>
#include "health.h"
#include "../graphics/graphics.h"

void health_handler(HealthEventType event, void *context) {
#if defined(PBL_HEALTH)
  // Which type of event occurred?
  switch(event) {
    case HealthEventSignificantUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO, "New HealthService HealthEventSignificantUpdate event");
      break;
    case HealthEventMovementUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO, "New HealthService HealthEventMovementUpdate event");
      break;
    case HealthEventSleepUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO, 
              "New HealthService HealthEventSleepUpdate event");
    break;
  } 

  HealthMetric metric = HealthMetricStepCount;
  time_t start = time_start_of_today();
  time_t end = time(NULL);
  
  // Check the metric has data available for today
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, start, end);
  
  if(mask & HealthServiceAccessibilityMaskAvailable) {
    // Data is available!
    APP_LOG(APP_LOG_LEVEL_INFO, "Steps today: %d", (int)health_service_sum_today(metric));
    
  } else {
    // No data recorded yet today
    APP_LOG(APP_LOG_LEVEL_ERROR, "Step data unavailable!");
  }
  
#endif
}



int get_todays_steps() {
  int retval = -1;
#if defined(PBL_HEALTH)
  HealthMetric metric = HealthMetricStepCount;
  time_t start = time_start_of_today();
  time_t end = time(NULL);
  
  // Check the metric has data available for today
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, start, end);
  
  if(mask & HealthServiceAccessibilityMaskAvailable) {
    // Data is available!
    retval = (int)health_service_sum_today(metric);
    APP_LOG(APP_LOG_LEVEL_INFO, "Steps today: %d", retval);
  } else {
    // No data recorded yet today
    APP_LOG(APP_LOG_LEVEL_ERROR, "Step data unavailable!");
  }
#endif
  return retval;
}