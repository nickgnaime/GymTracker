#include "workout.h"
#include <pebble.h>
#include "definitions.h"

static int8_t state, statecount, set, ExerciseID;
static char set_a[2], reps_a[3], weight_a[4];
static char reps_a_0[3], weight_a_0[3];
static char reps_a_1[3], weight_a_1[3];
static char reps_a_2[3], weight_a_2[3];
static char reps_a_3[3], weight_a_3[3];
static uint8_t player = 0;
static bool overwriteFlag;
static bool savedFlag;
static strct_workout tmpWorkout[2] = {{0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}};
static strct_workout myWorkout[2][4] = {{{0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}}, \
                                               {{0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, \
                                                {0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}}};
  
static Window *s_window;
static GFont s_res_gothic_14;
static GFont s_res_gothic_18_bold;
static GFont s_res_bitham_34_medium_numbers;
static GFont s_res_gothic_28;
static GFont s_res_gothic_24;
static GFont s_res_gothic_24_bold;
//static GFont s_res_gothic_18;
static GBitmap *s_res_arrow_up;
static GBitmap *s_res_arrow_right;
static GBitmap *s_res_arrow_down;
//static GFont s_res_bitham_42_medium_numbers;
static GBitmap *s_res_arrow_small_up_black;
static GBitmap *s_res_arrow_small_down_black;
static GBitmap *s_res_checkmark;
// now
static InverterLayer *s_inverterlayer_tmp_1;
static InverterLayer *s_inverterlayer_tmp_2;
static TextLayer *s_textlayer_tmp;
static TextLayer *s_textlayer_tmp_x;
static TextLayer *s_textlayer_tmp_kg;
static TextLayer *s_textlayer_tmp_reps;
static TextLayer *s_textlayer_tmp_weight;
static TextLayer *s_textlayer_overwrite;
static BitmapLayer *s_bitmaplayer_arrow_weight_up;
static BitmapLayer *s_bitmaplayer_arrow_weight_down;
static BitmapLayer *s_bitmaplayer_reps_up;
static BitmapLayer *s_bitmaplayer_reps_down;
static BitmapLayer *s_bitmaplayer_checkmark;
static InverterLayer *s_inverterlayer_old_1;
static InverterLayer *s_inverterlayer_old_2;
static InverterLayer *s_inverterlayer_old_3;
static InverterLayer *s_inverterlayer_old_4;
static InverterLayer *s_inverterlayer_old_5;

static TextLayer *s_textlayer_old_n;
static TextLayer *s_textlayer_old_kg;

// last time
static TextLayer *s_textlayer_0;
static TextLayer *s_textlayer_0_reps;
static TextLayer *s_textlayer_0_weight;

// over last time
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_1_reps;
static TextLayer *s_textlayer_1_weight;

// over over last time
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_2_reps;
static TextLayer *s_textlayer_2_weight;

// over over last time
static TextLayer *s_textlayer_3;
static TextLayer *s_textlayer_3_reps;
static TextLayer *s_textlayer_3_weight;

// general
static TextLayer *s_textlayer_exercise;
static TextLayer *s_textlayer_set;
static TextLayer *s_textlayer_set_num;
static TextLayer *s_textlayer_playername;
static ActionBarLayer *s_actionbarlayer_1;

static void refresh_numbers(){
  snprintf(set_a, sizeof(set_a), "%d", set+1);
  text_layer_set_text(s_textlayer_set_num, set_a);
  text_layer_set_text(s_textlayer_playername, whoIs(player));
  
  // aktuelles Workout
  snprintf(weight_a, sizeof(weight_a), "%d", tmpWorkout[player].weight[set]);
  text_layer_set_text(s_textlayer_tmp_weight, weight_a);
  snprintf(reps_a, sizeof(reps_a), "%d", tmpWorkout[player].reps[set]);
  text_layer_set_text(s_textlayer_tmp_reps, reps_a);
  // fuer letztes Workout
  snprintf(weight_a_0, sizeof(weight_a_0), "%d", myWorkout[player][0].weight[set]);
  text_layer_set_text(s_textlayer_0_weight, weight_a_0);
  snprintf(reps_a_0, sizeof(reps_a_0), "%d", myWorkout[player][0].reps[set]);
  text_layer_set_text(s_textlayer_0_reps, reps_a_0);
  // fuer vorletztes Workout
  snprintf(weight_a_1, sizeof(weight_a_1), "%d", myWorkout[player][1].weight[set]);
  text_layer_set_text(s_textlayer_1_weight, weight_a_1);
  snprintf(reps_a_1, sizeof(reps_a_1), "%d", myWorkout[player][1].reps[set]);
  text_layer_set_text(s_textlayer_1_reps, reps_a_1);
  // fuer vorvorletztes Workout
  snprintf(weight_a_2, sizeof(weight_a_2), "%d", myWorkout[player][2].weight[set]);
  text_layer_set_text(s_textlayer_2_weight, weight_a_2);
  snprintf(reps_a_2, sizeof(reps_a_2), "%d", myWorkout[player][2].reps[set]);
  text_layer_set_text(s_textlayer_2_reps, reps_a_2);
  // fuer vorvorvorletztes Workout
  snprintf(weight_a_3, sizeof(weight_a_3), "%d", myWorkout[player][3].weight[set]);
  text_layer_set_text(s_textlayer_3_weight, weight_a_3);
  snprintf(reps_a_3, sizeof(reps_a_3), "%d", myWorkout[player][3].reps[set]);
  text_layer_set_text(s_textlayer_3_reps, reps_a_3);
}

static void initialise_ui(void) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  //APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  //APP_LOG(APP_LOG_LEVEL_INFO, "Initialize UI");
  s_window = window_create();
  window_set_fullscreen(s_window, false);
  
  persist_read_data(ExerciseID, myWorkout, sizeof(myWorkout));
  
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_bitham_34_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  s_res_gothic_28 = fonts_get_system_font(FONT_KEY_GOTHIC_28);
  s_res_gothic_24 = fonts_get_system_font(FONT_KEY_GOTHIC_24);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  //s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  //s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_arrow_up = gbitmap_create_with_resource(RESOURCE_ID_arrow_up);
  s_res_arrow_right = gbitmap_create_with_resource(RESOURCE_ID_arrow_right);
  s_res_arrow_down = gbitmap_create_with_resource(RESOURCE_ID_arrow_down);
  s_res_arrow_small_up_black = gbitmap_create_with_resource(RESOURCE_ID_arrow_small_up_black);
  s_res_arrow_small_down_black = gbitmap_create_with_resource(RESOURCE_ID_arrow_small_down_black);
  s_res_checkmark = gbitmap_create_with_resource(RESOURCE_ID_checkmark);
   
  // s_textlayer_exercise
  s_textlayer_exercise = text_layer_create(GRect(5, 5, 114, 20));
  text_layer_set_text(s_textlayer_exercise, getExerciseName(ExerciseID));
  text_layer_set_text_alignment(s_textlayer_exercise, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_exercise);
  
  // s_textlayer_tmp
  s_textlayer_tmp = text_layer_create(GRect(1, 106, 16, 46));
  text_layer_set_background_color(s_textlayer_tmp, GColorWhite);
  text_layer_set_text_color(s_textlayer_tmp, GColorBlack);
  text_layer_set_text(s_textlayer_tmp, "N O W");
  text_layer_set_text_alignment(s_textlayer_tmp, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_tmp, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_tmp);
  
  // s_bitmaplayer_checkmark
  s_bitmaplayer_checkmark = bitmap_layer_create(GRect(100, 32, 24, 24));
  bitmap_layer_set_bitmap(s_bitmaplayer_checkmark, s_res_checkmark);
  bitmap_layer_set_background_color(s_bitmaplayer_checkmark, GColorClear);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_checkmark);
  layer_set_hidden((Layer*)s_bitmaplayer_checkmark, true);
  
  // s_textlayer_set
  s_textlayer_set = text_layer_create(GRect(37, 32, 29, 20));
  text_layer_set_background_color(s_textlayer_set, GColorClear);
  text_layer_set_text(s_textlayer_set, "SET:");
  text_layer_set_font(s_textlayer_set, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_set);
  
  // s_textlayer_playername
  s_textlayer_playername = text_layer_create(GRect(5, 17, 50, 20));
  text_layer_set_background_color(s_textlayer_playername, GColorClear);
  text_layer_set_text(s_textlayer_playername, whoIs(player));
  text_layer_set_font(s_textlayer_playername, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_playername);
  
  // s_textlayer_set_num
  s_textlayer_set_num = text_layer_create(GRect(66, 18, 26, 34));
  text_layer_set_background_color(s_textlayer_set_num, GColorClear);
  text_layer_set_text_alignment(s_textlayer_set_num, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_set_num, s_res_bitham_34_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_set_num);
  
  // s_textlayer_tmp_x
  s_textlayer_tmp_x = text_layer_create(GRect(47, 122, 8, 16));
  text_layer_set_background_color(s_textlayer_tmp_x, GColorClear);
  text_layer_set_text(s_textlayer_tmp_x, "X");
  text_layer_set_font(s_textlayer_tmp_x, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_tmp_x);
  
  // s_textlayer_old_n
  s_textlayer_old_n = text_layer_create(GRect(2, 70, 15, 36));
  text_layer_set_background_color(s_textlayer_old_n, GColorBlack);
  text_layer_set_text_color(s_textlayer_old_n, GColorWhite);
  text_layer_set_text(s_textlayer_old_n, "n  kg");
  text_layer_set_font(s_textlayer_old_n, s_res_gothic_14);
  text_layer_set_text_alignment(s_textlayer_old_n, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_old_n);
  
  // s_textlayer_0
  s_textlayer_0 = text_layer_create(GRect(18, 52, 25, 24));
  text_layer_set_background_color(s_textlayer_0, GColorClear);
   text_layer_set_text(s_textlayer_0, "0");
  text_layer_set_font(s_textlayer_0, s_res_gothic_14);
  text_layer_set_text_alignment(s_textlayer_0, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_0);
  
  // s_textlayer_0_reps
  s_textlayer_0_reps = text_layer_create(GRect(18, 62, 23, 24));
  text_layer_set_background_color(s_textlayer_0_reps, GColorClear);
  text_layer_set_font(s_textlayer_0_reps, s_res_gothic_24);
  text_layer_set_text_alignment(s_textlayer_0_reps, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_0_reps);
  
  // s_textlayer_0_weight
  s_textlayer_0_weight = text_layer_create(GRect(18, 80, 23, 24));
  text_layer_set_background_color(s_textlayer_0_weight, GColorClear);
  text_layer_set_font(s_textlayer_0_weight, s_res_gothic_24_bold);
  text_layer_set_text_alignment(s_textlayer_0_weight, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_0_weight);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(44, 52, 25, 24));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, "1");
  text_layer_set_font(s_textlayer_1, s_res_gothic_14);
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_1_reps
  s_textlayer_1_reps = text_layer_create(GRect(44, 62, 23, 24));
  text_layer_set_background_color(s_textlayer_1_reps, GColorClear);
  text_layer_set_font(s_textlayer_1_reps, s_res_gothic_24);
  text_layer_set_text_alignment(s_textlayer_1_reps, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1_reps);
  
  // s_textlayer_1_weight
  s_textlayer_1_weight = text_layer_create(GRect(44, 80, 23, 24));
  text_layer_set_background_color(s_textlayer_1_weight, GColorClear);
  text_layer_set_font(s_textlayer_1_weight, s_res_gothic_24_bold);
  text_layer_set_text_alignment(s_textlayer_1_weight, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1_weight);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(70, 52, 25, 24));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text(s_textlayer_2, "2");
  text_layer_set_font(s_textlayer_2, s_res_gothic_14);
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_2_reps
  s_textlayer_2_reps = text_layer_create(GRect(70, 62, 23, 24));
  text_layer_set_background_color(s_textlayer_2_reps, GColorClear);
  text_layer_set_font(s_textlayer_2_reps, s_res_gothic_24);
  text_layer_set_text_alignment(s_textlayer_2_reps, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2_reps);
  
  // s_textlayer_2_weight
  s_textlayer_2_weight = text_layer_create(GRect(70, 80, 23, 24));
  text_layer_set_background_color(s_textlayer_2_weight, GColorClear);
  text_layer_set_font(s_textlayer_2_weight, s_res_gothic_24_bold);
  text_layer_set_text_alignment(s_textlayer_2_weight, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2_weight);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(96, 52, 25, 24));
  text_layer_set_background_color(s_textlayer_3, GColorClear);
  text_layer_set_text(s_textlayer_3, "3");
  text_layer_set_font(s_textlayer_3, s_res_gothic_14);
  text_layer_set_text_alignment(s_textlayer_3, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
  
  // s_textlayer_3_reps
  s_textlayer_3_reps = text_layer_create(GRect(96, 62, 23, 24));
  text_layer_set_background_color(s_textlayer_3_reps, GColorClear);
  text_layer_set_font(s_textlayer_3_reps, s_res_gothic_24);
  text_layer_set_text_alignment(s_textlayer_3_reps, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3_reps);
  
  // s_textlayer_3_weight
  s_textlayer_3_weight = text_layer_create(GRect(96, 80, 23, 24));
  text_layer_set_background_color(s_textlayer_3_weight, GColorClear);
  text_layer_set_font(s_textlayer_3_weight, s_res_gothic_24_bold);
  text_layer_set_text_alignment(s_textlayer_3_weight, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3_weight);
  
  // s_textlayer_tmp_kg UNTEN
  s_textlayer_tmp_kg = text_layer_create(GRect(106, 122, 16, 18));
  text_layer_set_background_color(s_textlayer_tmp_kg, GColorClear);
  text_layer_set_text(s_textlayer_tmp_kg, "KG");
  text_layer_set_font(s_textlayer_tmp_kg, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_tmp_kg);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_arrow_up);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_arrow_right);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_arrow_down);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  
  // s_textlayer_tmp_reps
  s_textlayer_tmp_reps = text_layer_create(GRect(18, 113, 28, 32));
  text_layer_set_background_color(s_textlayer_tmp_reps, GColorClear);
  text_layer_set_font(s_textlayer_tmp_reps, s_res_gothic_28);
  text_layer_set_text_alignment(s_textlayer_tmp_reps, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_tmp_reps);
  
  // s_textlayer_tmp_weight
  s_textlayer_tmp_weight = text_layer_create(GRect(44, 107, 70, 36));
  text_layer_set_background_color(s_textlayer_tmp_weight, GColorClear);
  text_layer_set_font(s_textlayer_tmp_weight, s_res_bitham_34_medium_numbers);
  text_layer_set_text_alignment(s_textlayer_tmp_weight, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_tmp_weight);
  
  // s_bitmaplayer_arrow_weight_up
  s_bitmaplayer_arrow_weight_up = bitmap_layer_create(GRect(75, 112, 8, 4));
  bitmap_layer_set_bitmap(s_bitmaplayer_arrow_weight_up, s_res_arrow_small_up_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_arrow_weight_up);
  
  // s_bitmaplayer_arrow_weight_down
  s_bitmaplayer_arrow_weight_down = bitmap_layer_create(GRect(75, 142, 8, 4));
  bitmap_layer_set_bitmap(s_bitmaplayer_arrow_weight_down, s_res_arrow_small_down_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_arrow_weight_down);
  
  // s_bitmaplayer_reps_up
  s_bitmaplayer_reps_up = bitmap_layer_create(GRect(29, 115, 8, 4));
  bitmap_layer_set_bitmap(s_bitmaplayer_reps_up, s_res_arrow_small_up_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_reps_up);
  layer_set_hidden((Layer *) s_bitmaplayer_reps_up, true);
  
  // s_bitmaplayer_reps_down
  s_bitmaplayer_reps_down = bitmap_layer_create(GRect(29, 142, 8, 4));
  bitmap_layer_set_bitmap(s_bitmaplayer_reps_down, s_res_arrow_small_down_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_reps_down);
  layer_set_hidden((Layer *) s_bitmaplayer_reps_down, true);
  
  // s_inverterlayer_tmp_1
  s_inverterlayer_tmp_1 = inverter_layer_create(GRect(1, 110, 122, 39));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_tmp_1);
  
  // s_inverterlayer_tmp_2
  s_inverterlayer_tmp_2 = inverter_layer_create(GRect(18, 111, 104, 37));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_tmp_2);
  
  // s_inverterlayer_old_1
  s_inverterlayer_old_1 = inverter_layer_create(GRect(17, 54, 105, 52));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_old_1);
  
  // s_inverterlayer_old_2
  s_inverterlayer_old_2 = inverter_layer_create(GRect(18, 70, 25, 35));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_old_2);
  
  // s_inverterlayer_old_3
  s_inverterlayer_old_3 = inverter_layer_create(GRect(44, 70, 25, 35));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_old_3);
  
  // s_inverterlayer_old_4
  s_inverterlayer_old_4 = inverter_layer_create(GRect(70, 70, 25, 35));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_old_4);
  
  // s_inverterlayer_old_5
  s_inverterlayer_old_5 = inverter_layer_create(GRect(96, 70, 25, 35));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_old_5);
 
  // s_textlayer_overwrite
  s_textlayer_overwrite = text_layer_create(GRect(5, 20, 115, 100));
  text_layer_set_background_color(s_textlayer_overwrite, GColorWhite);
  text_layer_set_text_color(s_textlayer_overwrite, GColorBlack);
  text_layer_set_text(s_textlayer_overwrite, "Save Workout and overwrite existing?");
  text_layer_set_text_alignment(s_textlayer_overwrite, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_overwrite, s_res_gothic_24_bold);
  layer_set_hidden((Layer*)s_textlayer_overwrite, true);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_overwrite);
  
  APP_LOG(APP_LOG_LEVEL_INFO, "initialized");
  APP_LOG(APP_LOG_LEVEL_INFO, "copy last workout");
  tmpWorkout[0].reps[0] = myWorkout[0][0].reps[0];
  tmpWorkout[0].weight[0] = myWorkout[0][0].weight[0];
  time_t now = time(NULL);
  char buf_date[5];
  strftime(buf_date, sizeof(buf_date), "%m%d", localtime(&now));
  tmpWorkout[0].month = (buf_date[0] - '0')*10 + (buf_date[1] - '0');
  tmpWorkout[0].day = (buf_date[2] - '0')*10 + (buf_date[3] - '0');
  
  if(varMP == true) {
    tmpWorkout[1].reps[0] = myWorkout[1][0].reps[0];
    tmpWorkout[1].weight[0] = myWorkout[1][0].weight[0];
    
    tmpWorkout[1].month = (buf_date[0] - '0')*10 + (buf_date[1] - '0');
    tmpWorkout[1].day = (buf_date[2] - '0')*10 + (buf_date[3] - '0');  
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "refresh numbers()");
  refresh_numbers();
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_exercise);
  text_layer_destroy(s_textlayer_set);
  text_layer_destroy(s_textlayer_set_num);
  text_layer_destroy(s_textlayer_playername);
  text_layer_destroy(s_textlayer_overwrite);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_arrow_up);
  gbitmap_destroy(s_res_arrow_right);
  gbitmap_destroy(s_res_arrow_down);
  gbitmap_destroy(s_res_arrow_small_up_black);
  gbitmap_destroy(s_res_arrow_small_down_black);
  gbitmap_destroy(s_res_checkmark);
  
  inverter_layer_destroy(s_inverterlayer_tmp_1);
  inverter_layer_destroy(s_inverterlayer_tmp_2);
  text_layer_destroy(s_textlayer_tmp);
  text_layer_destroy(s_textlayer_tmp_x);
  text_layer_destroy(s_textlayer_tmp_kg);
  text_layer_destroy(s_textlayer_tmp_reps);
  text_layer_destroy(s_textlayer_tmp_weight);
  bitmap_layer_destroy(s_bitmaplayer_arrow_weight_up);
  bitmap_layer_destroy(s_bitmaplayer_arrow_weight_down);
  bitmap_layer_destroy(s_bitmaplayer_reps_up);
  bitmap_layer_destroy(s_bitmaplayer_reps_down);
  bitmap_layer_destroy(s_bitmaplayer_checkmark);
  
  inverter_layer_destroy(s_inverterlayer_old_1);
  inverter_layer_destroy(s_inverterlayer_old_2);
  inverter_layer_destroy(s_inverterlayer_old_3);
  inverter_layer_destroy(s_inverterlayer_old_4);
  inverter_layer_destroy(s_inverterlayer_old_5);

  text_layer_destroy(s_textlayer_old_n);
  text_layer_destroy(s_textlayer_old_kg);
  
  text_layer_destroy(s_textlayer_0);
  text_layer_destroy(s_textlayer_0_reps);
  text_layer_destroy(s_textlayer_0_weight);
  
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_1_reps);
  text_layer_destroy(s_textlayer_1_weight);
 
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_2_reps);
  text_layer_destroy(s_textlayer_2_weight);

  text_layer_destroy(s_textlayer_3);
  text_layer_destroy(s_textlayer_3_reps);
  text_layer_destroy(s_textlayer_3_weight);
  
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}


static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(state%2==0){
    if(tmpWorkout[player].weight[set]<255) tmpWorkout[player].weight[set]++;
    snprintf(weight_a, sizeof(weight_a), "%d", tmpWorkout[player].weight[set]);
    text_layer_set_text(s_textlayer_tmp_weight, weight_a);  
  } else {
    if(tmpWorkout[player].reps[set]<255) tmpWorkout[player].reps[set]++;
    snprintf(reps_a, sizeof(reps_a), "%d", tmpWorkout[player].reps[set]);
    text_layer_set_text(s_textlayer_tmp_reps, reps_a);      
  }
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(state%2==0){
    if(tmpWorkout[player].weight[set]>0) tmpWorkout[player].weight[set]--;
    snprintf(weight_a, sizeof(weight_a), "%d", tmpWorkout[player].weight[set]);
    text_layer_set_text(s_textlayer_tmp_weight, weight_a);  
  } else {
    if(tmpWorkout[player].reps[set]>0) tmpWorkout[player].reps[set]--;
    snprintf(reps_a, sizeof(reps_a), "%d", tmpWorkout[player].reps[set]);
    text_layer_set_text(s_textlayer_tmp_reps, reps_a);      
  }
}
static void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(state%2==0){
    if(tmpWorkout[player].weight[set]<245) tmpWorkout[player].weight[set] += 10;
    snprintf(weight_a, sizeof(weight_a), "%d", tmpWorkout[player].weight[set]);
    text_layer_set_text(s_textlayer_tmp_weight, weight_a);  
  } else {
    if(tmpWorkout[player].reps[set]<245) tmpWorkout[player].reps[set] += 10;
    snprintf(reps_a, sizeof(reps_a), "%d", tmpWorkout[player].reps[set]);
    text_layer_set_text(s_textlayer_tmp_reps, reps_a);      
  }
}
static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(state%2==0){
    if(tmpWorkout[player].weight[set]>9) tmpWorkout[player].weight[set] -= 10;
    snprintf(weight_a, sizeof(weight_a), "%d", tmpWorkout[player].weight[set]);
    text_layer_set_text(s_textlayer_tmp_weight, weight_a);  
  } else {
    if(tmpWorkout[player].reps[set]>9) tmpWorkout[player].reps[set] -= 10;
    snprintf(reps_a, sizeof(reps_a), "%d", tmpWorkout[player].reps[set]);
    text_layer_set_text(s_textlayer_tmp_reps, reps_a);      
  }
}
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(state % 2 == 0 && state < statecount) {
    if(state < 3) state++;
    layer_set_hidden((Layer*)s_bitmaplayer_arrow_weight_up, true);
    layer_set_hidden((Layer*)s_bitmaplayer_arrow_weight_down, true);
    layer_set_hidden((Layer*)s_bitmaplayer_reps_up, false);
    layer_set_hidden((Layer*)s_bitmaplayer_reps_down, false);
    refresh_numbers();
  } else {
    if(varMP == 1) { 
      if(state < 3) state++; 
      if(player==0) player = 1; else player = 0;
    }
    if(state >= statecount) { 
      if(set<5) {
        set++; 
        if(tmpWorkout[0].reps[set] == 0 && myWorkout[0][0].reps[set] != 0) {
          tmpWorkout[0].reps[set] = myWorkout[0][0].reps[set];
          tmpWorkout[0].weight[set] = myWorkout[0][0].weight[set];
        } else if(set>0) {
          tmpWorkout[0].reps[set] = tmpWorkout[0].reps[set-1];
          tmpWorkout[0].weight[set] = tmpWorkout[0].weight[set-1];  
        }
        if(tmpWorkout[1].reps[set] == 0 && myWorkout[1][0].reps[set] != 0) {
          tmpWorkout[1].reps[set] = myWorkout[1][0].reps[set];
          tmpWorkout[1].weight[set] = myWorkout[1][0].weight[set];
        } else if(set>0) {
          tmpWorkout[1].reps[set] = tmpWorkout[1].reps[set-1];
          tmpWorkout[1].weight[set] = tmpWorkout[1].weight[set-1];  
        }
      }
      state = 0; 
      //if(player==0) player = 1; else player = 0;
    }
    
    layer_set_hidden((Layer*)s_bitmaplayer_arrow_weight_up, false);
    layer_set_hidden((Layer*)s_bitmaplayer_arrow_weight_down, false);
    layer_set_hidden((Layer*)s_bitmaplayer_reps_up, true);
    layer_set_hidden((Layer*)s_bitmaplayer_reps_down, true);
    
    refresh_numbers();
  }
}
static void select_multi_click_handler(ClickRecognizerRef recognizer, void *context){
  if(state %2 == 1) {
    if(state>0) state--;
    layer_set_hidden((Layer*)s_bitmaplayer_arrow_weight_up, false);
    layer_set_hidden((Layer*)s_bitmaplayer_arrow_weight_down, false);
    layer_set_hidden((Layer*)s_bitmaplayer_reps_up, true);
    layer_set_hidden((Layer*)s_bitmaplayer_reps_down, true);
  } else {
    if(state > 0) state--;
    if(varMP == true) { if(player==0) player = 1; else player = 0; }
    if(state < 1) { 
      if(set>0) {
        set--;
      } 
      if(varMP==1) {
        state = 3; 
        //if(player==0) player = 1; else player = 0;
      } else { 
        state = 1;
      } 
    }
    
    layer_set_hidden((Layer*)s_bitmaplayer_arrow_weight_up, true);
    layer_set_hidden((Layer*)s_bitmaplayer_arrow_weight_down, true);
    layer_set_hidden((Layer*)s_bitmaplayer_reps_up, false);
    layer_set_hidden((Layer*)s_bitmaplayer_reps_down, false);
    
    refresh_numbers();
  }
}
static void back_click_handler(ClickRecognizerRef recognizer, void *context) { 
  if(overwriteFlag == true) {
    overwriteFlag = false;
    layer_set_hidden((Layer*)s_textlayer_overwrite, true);
  } else {
    //handle_window_unload(s_window);
    hide_workout();
  }
}

static void save_data(bool overwrite) {
  if(overwrite == false) {
    myWorkout[0][3] = myWorkout[0][2];
    myWorkout[0][2] = myWorkout[0][1];
    myWorkout[0][1] = myWorkout[0][0];
    myWorkout[0][0] = tmpWorkout[0];
    
    if(varMP == true) {
      myWorkout[1][3] = myWorkout[1][2];
      myWorkout[1][2] = myWorkout[1][1];
      myWorkout[1][1] = myWorkout[1][0];
      myWorkout[1][0] = tmpWorkout[1];
    }
  } else {
    myWorkout[0][0] = tmpWorkout[0];
    if(varMP == true) { myWorkout[1][0] = tmpWorkout[1]; }
  }
    
  persist_write_data(ExerciseID, myWorkout, sizeof(myWorkout));
  
  savedFlag = true;
  layer_set_hidden((Layer*)s_bitmaplayer_checkmark, false);  
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(tmpWorkout[0].month == myWorkout[0][0].month && tmpWorkout[0].day == myWorkout[0][0].day) {
    if(overwriteFlag == false && savedFlag == false) {
      overwriteFlag = true;
      layer_set_hidden((Layer*)s_textlayer_overwrite, false);
    } else if(overwriteFlag == true) {
      overwriteFlag = false;
      layer_set_hidden((Layer*)s_textlayer_overwrite, true);
      save_data(false);
    } else if(savedFlag == true) {
      save_data(true);
    }
  } else {
    save_data(false);
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  
  window_long_click_subscribe(BUTTON_ID_UP, 300, up_long_click_handler, NULL);
  window_long_click_subscribe(BUTTON_ID_DOWN, 300, down_long_click_handler, NULL);
  window_long_click_subscribe(BUTTON_ID_SELECT, 1000, select_long_click_handler, NULL);
  
  window_multi_click_subscribe(BUTTON_ID_SELECT,2, 10, 0, true, select_multi_click_handler);
}

void show_workout(uint8_t exercise) {
  APP_LOG(APP_LOG_LEVEL_INFO, "show_workout()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  set = 0;
  state = 0;
  overwriteFlag = false;
  savedFlag = false;
  if(varMP==false) statecount = 1; else statecount = 3;
  if(firstPhil == true || varMP == false) player = 0;
  else player = 1;
  ExerciseID = exercise;

  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_workout(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "hide_workout()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  window_stack_remove(s_window, true);
}
