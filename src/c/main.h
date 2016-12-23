#include <pebble.h>

Window *main_window;
Layer *main_layer;
GRect bounds;
GFont font_digits;

// struct tm *tick_time;
uint8_t hours;
uint8_t minutes;
uint8_t seconds;
