#include "main.h"
#include <pebble.h>

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // update_time(tick_time);
}

static void update_time() {
  // hours = tick_time->tm_hour;
  // minutes = tick_time->tm_min;
  // seconds = tick_time->tm_sec;
  // layer_mark_dirty(s_layer);
}

static void layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorTiffanyBlue);
  graphics_context_set_antialiased(ctx, true);
  graphics_context_set_stroke_width(ctx, 2);
  GPoint pixel1 = (GPoint){.x = 122, .y = 84};
  GPoint pixel2 = (GPoint){.x = 91, .y = 84};
  graphics_draw_circle(ctx, pixel1, 20);
  graphics_fill_circle(ctx, pixel1, 18);
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);
  // Set this Layer's update procedure
  layer_set_update_proc(root_layer, layer_update_proc);
}

static void main_window_unload(Window *window) { layer_destroy(root_layer); }

static void init() {
  // Create main Window element and assign to pointer
  main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(
      main_window,
      (WindowHandlers){.load = main_window_load, .unload = main_window_unload});

  // Show the Window on the watch, with animated=true
  window_stack_push(main_window, true);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
