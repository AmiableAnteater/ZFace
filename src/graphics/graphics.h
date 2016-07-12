#pragma once
#include <pebble.h>  
#include "../effects/effects.h"
#include "../effects/effect_layer.h"

void init_graphics(uint32_t resource_id);
void deinit_graphics();

#define MAX_REPLACE_COLORS 2
// structure for color swap effect
typedef struct {
  GColor fromColor[MAX_REPLACE_COLORS];  
  GColor toColorFinishedPart[MAX_REPLACE_COLORS];
  GColor toColorUnfinishedPart[MAX_REPLACE_COLORS];
  int finishedX;
} EffectMultiColorpair;

effect_cb effect_multicolorswap;
