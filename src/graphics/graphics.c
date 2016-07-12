#include <stdio.h>
#include <pebble.h>
#include "graphics.h"

static Window *s_main_window;
static GBitmap *s_bitmap;
static Layer *s_canvas_layer;
static EffectLayer *s_effect_layer;
static EffectMultiColorpair* s_multicolorpair;
static float s_progress = .7f;
static TextLayer *s_time_layer;


// changes several colors in one pass
void effect_multicolorswap(GContext* ctx,  GRect position, void* param) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> effect_multicolorswap");
  
#ifdef PBL_COLOR // only logical to do anything on Basalt - otherwise you're just ... doing an invert
  //capturing framebuffer bitmap
  GBitmap *fb = graphics_capture_frame_buffer(ctx);
 
  BitmapInfo bitmap_info;
  bitmap_info.bitmap = fb;
  bitmap_info.bitmap_data =  gbitmap_get_data(fb);
  bitmap_info.bytes_per_row = gbitmap_get_bytes_per_row(fb);
  bitmap_info.bitmap_format = gbitmap_get_format(fb);
  
  EffectMultiColorpair *swap = (EffectMultiColorpair *)param;
  GColor pixel;
  for (int x = 0; x < position.size.w; x++){
    for (int y = 0; y < position.size.h; y++){
       pixel.argb = get_pixel(bitmap_info, y + position.origin.y, x + position.origin.x);
       for (int colIdx = 0; colIdx < MAX_REPLACE_COLORS; colIdx++) {
          if (gcolor_equal(pixel, swap->fromColor[colIdx])) {
            GColor toColor = (x <= swap->finishedX) ? swap->toColorFinishedPart[colIdx] : swap->toColorUnfinishedPart[colIdx];
            if (x%20 == 0 && y %50 == 0)
              APP_LOG(APP_LOG_LEVEL_DEBUG, "x:%03d y:%03d: Swapping %03d to %03d", x, y, pixel.argb, toColor.argb);
            set_pixel(bitmap_info, y + position.origin.y, x + position.origin.x, toColor.argb);
          }
       }
     }
  }
  graphics_release_frame_buffer(ctx, fb);
#endif

  APP_LOG(APP_LOG_LEVEL_DEBUG, "< effect_colorswap");
}



void updateTime(struct tm *time) {
  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}



static void update_proc(Layer *layer, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> update_proc");
  
  GRect bitmap_bounds = gbitmap_get_bounds(s_bitmap);  
  // Set the compositing mode (GCompOpSet is required for transparency)
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, s_bitmap, bitmap_bounds);

  s_multicolorpair->finishedX = (int)(144.0 * s_progress);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "< update_proc");
}



static void main_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> main_window_load");
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_canvas_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(s_canvas_layer, update_proc);
  layer_add_child(window_layer, s_canvas_layer);
  layer_add_child(s_canvas_layer, effect_layer_get_layer(s_effect_layer));
  
  // Create layer to show time
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< main_window_load");
}



static void main_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> main_window_unload");
  layer_destroy(s_canvas_layer);
  text_layer_destroy(s_time_layer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< main_window_unload");
}



void init_graphics(uint32_t resource_id) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "> init_graphics");
  s_main_window = window_create();
  
  s_multicolorpair = (EffectMultiColorpair*)malloc(sizeof(EffectMultiColorpair));
  s_multicolorpair->fromColor[0] = GColorWhite;
  s_multicolorpair->toColorFinishedPart[0] = GColorChromeYellow;
  s_multicolorpair->toColorUnfinishedPart[0] = GColorPastelYellow;

  s_effect_layer = effect_layer_create(GRect(0, 0, 144, 168));
  effect_layer_add_effect(s_effect_layer, effect_multicolorswap, s_multicolorpair);

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
  free(s_multicolorpair);
  s_multicolorpair = NULL;
  
  // Destroy the image data
  gbitmap_destroy(s_bitmap);
  effect_layer_destroy(s_effect_layer);
  free(s_effect_layer);
  s_effect_layer = NULL;
  window_destroy(s_main_window);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "< deinit_graphics");
}
