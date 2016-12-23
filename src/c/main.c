#include "main.h"
#include <pebble.h>

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  hours = tick_time->tm_hour;
  minutes = tick_time->tm_min;
  // seconds = tick_time->tm_sec;
  layer_mark_dirty(window_get_root_layer(main_window));
}

static void tick_handler(struct tm *s_tick_time, TimeUnits units_changed) {
  update_time();
}

static int calculate_angle_12h_minutes(int s_hours, int s_minutes) {
  int s_angle = (((s_hours % 12) * 60) + s_minutes) / 2;
  return s_angle;
}

static GPoint calculate_dial_center(int s_angle) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GRect s_inset = grect_inset(bounds, GEdgeInsets(25));
  GPoint s_center = gpoint_from_polar(s_inset, GOvalScaleModeFitCircle,
                                      DEG_TO_TRIGANGLE(s_angle));
  return s_center;
}

static void draw_dial(GContext *s_ctx, GColor s_stroke_color,
                      GColor s_fill_color, GPoint s_center) {
  graphics_context_set_stroke_color(s_ctx, s_stroke_color);
  graphics_context_set_fill_color(s_ctx, s_fill_color);
  graphics_context_set_stroke_width(s_ctx, 2);
  graphics_draw_circle(s_ctx, s_center, 20);
  graphics_fill_circle(s_ctx, s_center, 18);
}

static GRect calculate_dial_box(int s_angle) {
  GRect bounds = layer_get_bounds(window_get_root_layer(main_window));
  GRect s_inset = grect_inset(bounds, GEdgeInsets(25));
  GRect s_rect = grect_centered_from_polar(s_inset, GOvalScaleModeFitCircle,
                                           DEG_TO_TRIGANGLE(s_angle),
                                           (GSize){.w = 27, .h = 27});
  return s_rect;
}

static void draw_digits(GContext *s_ctx, GRect s_box) {
  char s_digits[4];
  snprintf(s_digits, sizeof(s_digits), "%d", minutes);
  graphics_context_set_text_color(s_ctx, GColorBlack);
  graphics_draw_text(s_ctx, s_digits, font_digits, s_box,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

static void layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_antialiased(ctx, true);
  int angle = calculate_angle_12h_minutes(hours, minutes);
  GPoint center = calculate_dial_center(angle);
  draw_dial(ctx, GColorBlack, GColorYellow, center);
  GRect box = calculate_dial_box(angle);
  // graphics_draw_rect(ctx, box);
  draw_digits(ctx, box);
}

// static void unobstructed_area_did_change_handler() {
//   layer_mark_dirty(main_layer);
// }

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *main_layer =
      layer_create(layer_get_bounds(window_get_root_layer(main_window)));
  layer_add_child(window_get_root_layer(main_window), main_layer);
  layer_set_update_proc(main_layer, layer_update_proc);
  font_digits = fonts_load_custom_font(
      resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_REGULAR_23));
  update_time();
}

static void main_window_unload(Window *window) {
  layer_destroy(main_layer);
  fonts_unload_custom_font(font_digits);
  // unobstructed_area_service_unsubscribe();
}

static void init() {
  main_window = window_create();
  window_set_background_color(main_window, GColorPictonBlue);
  window_set_window_handlers(
      main_window,
      (WindowHandlers){.load = main_window_load, .unload = main_window_unload});
  window_stack_push(main_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // update_time(localtime(&temp));

  // unobstructed_area_service_subscribe(
  //     (UnobstructedAreaHandlers){.did_change =
  //                                    unobstructed_area_did_change_handler},
  //     NULL);
}

static void deinit() {
  window_destroy(main_window);
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
