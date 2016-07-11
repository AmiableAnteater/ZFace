#include <pebble.h>
#include "effects/effect_layer.h"

static Window *s_main_window;
static GBitmap *s_bitmap;
static Layer *s_canvas_layer;
EffectLayer* h1;


static void update_proc(Layer *layer, GContext *ctx) {
  // Get the bounds of the image
  GRect bitmap_bounds = gbitmap_get_bounds(s_bitmap);
  
  // Set the compositing mode (GCompOpSet is required for transparency)
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  
  // Draw the image
  graphics_draw_bitmap_in_rect(ctx, s_bitmap, bitmap_bounds);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

   // Create the canvas Layer
  s_canvas_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));

  // Set the LayerUpdateProc
  layer_set_update_proc(s_canvas_layer, update_proc);

  // Add to parent Window
  layer_add_child(window_layer, s_canvas_layer);
}

static void main_window_unload(Window *window) {
  layer_destroy(s_canvas_layer);
}

static void init() {
  // Create the object from resource file
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SUNNY);
  
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlueMoon);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
  
  h1 = effect_layer_create(GRect(0,0,110,168));
  
  EffectColorpair colorpair;
  colorpair.firstColor = GColorWhite;
  colorpair.secondColor = GColorGreen;  
  effect_layer_add_effect(h1, effect_colorswap, &colorpair);
  layer_add_child(window_get_root_layer(s_main_window), effect_layer_get_layer(h1));
}

static void deinit() {
  // Destroy the image data
  gbitmap_destroy(s_bitmap);
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
