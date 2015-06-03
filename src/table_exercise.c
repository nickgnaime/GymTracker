#include <pebble.h>
#include "table_exercise.h"
#include "definitions.h"
  
  
#define MATRIXCOUNT 48
  
static Window *s_window_table_exercise;
static GBitmap *s_res_bg_table;
static GFont s_res_gothic_14;
static BitmapLayer *s_bitmaplayer_background;
static TextLayer *s_textlayer_dates;
static TextLayer *s_textlayer_set[6];
static TextLayer *s_textlayer_matrix[12];
static TextLayer *s_textlayer_exercise;
static TextLayer *s_textlayer_playername;
static InverterLayer *s_inverterlayer;
static char buf[12][16];
static char buf_date[40];
static uint8_t ExerciseID;
static uint8_t player;
static uint8_t i, x;
static uint8_t y;
static strct_workout myWorkout[2][4] = {{{0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}}, \
                                               {{0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}}};

static void refresh_numbers() {
  snprintf(buf_date, sizeof(buf_date), "%02d. %02d. %02d. %02d. %02d. %02d. %02d. %02d.", myWorkout[player][0].day, myWorkout[player][0].month, myWorkout[player][1].day, myWorkout[player][1].month, myWorkout[player][2].day, myWorkout[player][2].month, myWorkout[player][3].day, myWorkout[player][3].month);
  text_layer_set_text(s_textlayer_dates, buf_date);
  if(player == 0) text_layer_set_text(s_textlayer_playername, "Phil");
  else text_layer_set_text(s_textlayer_playername, "Marco");
  
  for(i = 0; i < 6; i++){
    snprintf(buf[i], sizeof(buf[i]), "%d\n\n%d\n\n%d\n\n%d", myWorkout[player][0].reps[i], myWorkout[player][1].reps[i], myWorkout[player][2].reps[i], myWorkout[player][3].reps[i]);
    text_layer_set_text(s_textlayer_matrix[i], buf[i]);
    
    snprintf(buf[i+6], sizeof(buf[i+6]), "%d\n\n%d\n\n%d\n\n%d", myWorkout[player][0].weight[i], myWorkout[player][1].weight[i], myWorkout[player][2].weight[i], myWorkout[player][3].weight[i]);
    text_layer_set_text(s_textlayer_matrix[i+6], buf[i+6]);
  }
}
static void initialise_ui(void) {
  s_window_table_exercise = window_create();
  window_set_fullscreen(s_window_table_exercise, false);
  
  persist_read_data(ExerciseID, myWorkout, sizeof(myWorkout));
  
  s_res_bg_table = gbitmap_create_with_resource(RESOURCE_ID_bg_table);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  
  // s_textlayer_exercise
  s_textlayer_exercise = text_layer_create(GRect(2, -3, 140, 15));
  text_layer_set_text(s_textlayer_exercise, getExerciseName(ExerciseID));
  text_layer_set_text_alignment(s_textlayer_exercise, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window_table_exercise), (Layer *)s_textlayer_exercise);
  
  // s_textlayer_playername
  s_textlayer_playername = text_layer_create(GRect(2, 10, 140, 15));
  text_layer_set_background_color(s_textlayer_playername, GColorClear);
  text_layer_set_text(s_textlayer_playername, "Phil");
  text_layer_set_text_alignment(s_textlayer_playername, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window_table_exercise), (Layer *)s_textlayer_playername);
  
  // s_bitmaplayer_background
  s_bitmaplayer_background = bitmap_layer_create(GRect(0, 28, 144, 152));
  bitmap_layer_set_bitmap(s_bitmaplayer_background, s_res_bg_table);
  layer_add_child(window_get_root_layer(s_window_table_exercise), (Layer *)s_bitmaplayer_background);
  
 // s_inverterlayer
  s_inverterlayer = inverter_layer_create(GRect(0, 0, 144, 27));
  layer_add_child(window_get_root_layer(s_window_table_exercise), (Layer *)s_inverterlayer);
  
  // s_textlayer_dates
  s_textlayer_dates = text_layer_create(GRect(1, 40, 16, 140));
  text_layer_set_background_color(s_textlayer_dates, GColorClear);
  text_layer_set_text_color(s_textlayer_dates, GColorWhite);
  snprintf(buf_date, sizeof(buf_date), "%02d. %02d. %02d. %02d. %02d. %02d. %02d. %02d.", myWorkout[player][0].day, myWorkout[player][0].month, myWorkout[player][1].day, myWorkout[player][1].month, myWorkout[player][2].day, myWorkout[player][2].month, myWorkout[player][3].day, myWorkout[player][3].month);
  text_layer_set_text(s_textlayer_dates, buf_date);
  text_layer_set_font(s_textlayer_dates, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window_table_exercise), (Layer *)s_textlayer_dates);
  
  // s_textlayer_set[]
  i = 0;
  x = 26;
  for(i = 0; i < 6; i++){
    s_textlayer_set[i] = text_layer_create(GRect(x, 26, 8, 14));
    text_layer_set_background_color(s_textlayer_set[i], GColorClear);
    text_layer_set_text_color(s_textlayer_set[i], GColorWhite);
    text_layer_set_font(s_textlayer_set[i], s_res_gothic_14);
    layer_add_child(window_get_root_layer(s_window_table_exercise), (Layer *)s_textlayer_set[i]);
    x = x + 21;
  }
  text_layer_set_text(s_textlayer_set[0], "1");
  text_layer_set_text(s_textlayer_set[1], "2");
  text_layer_set_text(s_textlayer_set[2], "3");
  text_layer_set_text(s_textlayer_set[3], "4");
  text_layer_set_text(s_textlayer_set[4], "5");
  text_layer_set_text(s_textlayer_set[5], "6");

  x = 19; // 22
  y = 40;
  for(i = 0; i < 6; i++){
    s_textlayer_matrix[i] = text_layer_create(GRect(x, y, 20, 120)); // 12
    text_layer_set_background_color(s_textlayer_matrix[i], GColorClear);
    snprintf(buf[i], sizeof(buf[i]), "%d\n\n%d\n\n%d\n\n%d", myWorkout[player][0].reps[i], myWorkout[player][1].reps[i], myWorkout[player][2].reps[i], myWorkout[player][3].reps[i]);
    text_layer_set_text(s_textlayer_matrix[i], buf[i]);
    text_layer_set_text_alignment(s_textlayer_matrix[i], GTextAlignmentCenter);
    text_layer_set_font(s_textlayer_matrix[i], s_res_gothic_14);
    layer_add_child(window_get_root_layer(s_window_table_exercise), (Layer *)s_textlayer_matrix[i]);
    
    s_textlayer_matrix[i+6] = text_layer_create(GRect(x, y+13, 20, 120)); // 12
    text_layer_set_background_color(s_textlayer_matrix[i+6], GColorClear);
    snprintf(buf[i+6], sizeof(buf[i+6]), "%d\n\n%d\n\n%d\n\n%d", myWorkout[player][0].weight[i], myWorkout[player][1].weight[i], myWorkout[player][2].weight[i], myWorkout[player][3].weight[i]);
    text_layer_set_text(s_textlayer_matrix[i+6], buf[i+6]);
    text_layer_set_text_alignment(s_textlayer_matrix[i+6], GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(s_window_table_exercise), (Layer *)s_textlayer_matrix[i+6]);
    
    x = x + 21;
  }
}
static void destroy_ui(void) {
  window_destroy(s_window_table_exercise);
  bitmap_layer_destroy(s_bitmaplayer_background);
  text_layer_destroy(s_textlayer_dates);
  gbitmap_destroy(s_res_bg_table);
  text_layer_destroy(s_textlayer_exercise);
  text_layer_destroy(s_textlayer_playername);
  inverter_layer_destroy(s_inverterlayer);
  
  int i = 0;
  for(i = 0; i < 6; i++){
    text_layer_destroy(s_textlayer_set[i]);
  }
  for(i = 0; i < 12; i++){
    text_layer_destroy(s_textlayer_matrix[i]);
  }
}

void hide_table_exercise(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "hide_table_exercise()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  window_stack_remove(s_window_table_exercise, true);
}
static void handle_window_unload(Window* window) {
  destroy_ui();
}
static void down_double_click_handler(ClickRecognizerRef recognizer, void *context) {
  hide_table_exercise();
}
static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(player == 0) player = 1;
  else player = 0;
  
  refresh_numbers();
}

static void click_config_provider(void *context) {
  window_multi_click_subscribe(BUTTON_ID_DOWN, 2, 10, 0, true, down_double_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}
void show_table_exercise(uint8_t exercise) {
  APP_LOG(APP_LOG_LEVEL_INFO, "show_table_exercise()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  ExerciseID = exercise;
  player = 0;
  initialise_ui();
  window_set_click_config_provider(s_window_table_exercise, click_config_provider);
  window_set_window_handlers(s_window_table_exercise, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window_table_exercise, true);
}
