#include <pebble.h>
#include "graphics.h"

static Window *s_main_window;
static GBitmap *s_bitmap;
static Layer *s_canvas_layer;
static EffectLayer *s_effect_layer_left;
static EffectColorpair* s_colorpair_left;
static EffectLayer* s_effect_layer_right;
static EffectColorpair* s_colorpair_right;
static float s_progress = .7f;

static EffectLayer* create_progress_effect_layer(EffectColorpair* colorpair) {
  EffectLayer* effect_layer = effect_layer_create(GRect(0, 0, 10, 10));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "    creating effect layer %p", effect_layer);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "    Colorpair adress: %p, swap argb:%d to argb:%d", colorpair, colorpair->firstColor.argb, colorpair->secondColor.argb);
  effect_layer_add_effect(effect_layer, effect_colorswap, colorpair);
  return effect_layer;
}

static void update_proc(Layer *layer, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> update_proc");
  
  GRect bitmap_bounds = gbitmap_get_bounds(s_bitmap);  
  // Set the compositing mode (GCompOpSet is required for transparency)
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, s_bitmap, bitmap_bounds);
  
  const int progress = (int)(144.0 * s_progress);
  GRect frame = GRect(0, 0, progress, 168);
  effect_layer_set_frame(s_effect_layer_left, frame);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "< update_proc");
}

static void main_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> main_window_load");
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_canvas_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(s_canvas_layer, update_proc);
  layer_add_child(window_layer, s_canvas_layer);
  layer_add_child(s_canvas_layer, effect_layer_get_layer(s_effect_layer_left));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< main_window_load");
}

static void main_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> main_window_unload");
  layer_destroy(s_canvas_layer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< main_window_unload");
}

void init_graphics(uint32_t resource_id) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> init_graphics");
  s_main_window = window_create();
  
  s_colorpair_left = (EffectColorpair*)malloc(sizeof(EffectColorpair));
  s_colorpair_left->firstColor = GColorWhite;
  s_colorpair_left->secondColor = GColorYellow;
  s_effect_layer_left = create_progress_effect_layer(s_colorpair_left);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "    effect layer adress: %p", s_effect_layer_left);

  s_colorpair_right = (EffectColorpair*)malloc(sizeof(EffectColorpair));
  s_colorpair_right->firstColor = GColorWhite;
  s_colorpair_right->secondColor = GColorPastelYellow;  

  // Create the object from resource file
  s_bitmap = gbitmap_create_with_resource(resource_id);
  
  window_set_background_color(s_main_window, GColorBlueMoon);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< init_graphics");
}

void deinit_graphics() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> deinit_graphics");
  free(s_colorpair_left);
  free(s_colorpair_right);
  
  // Destroy the image data
  gbitmap_destroy(s_bitmap);
  effect_layer_destroy(s_effect_layer_left);
  effect_layer_destroy(s_effect_layer_right);
  window_destroy(s_main_window);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< deinit_graphics");
}
