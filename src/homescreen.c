#include "homescreen.h"
#include <pebble.h>
#include "musclegroup.h"
#include "definitions.h"
#include "upload.h"

char uploadStatus[20] = "Upload Data";
  
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_14;
static GFont s_res_bitham_30_black;
static GFont s_res_roboto_condensed_21;
static TextLayer *s_textlayer_workout;
static TextLayer *s_textlayer_data;
static TextLayer *s_textlayer_player;
static TextLayer *s_textlayer_playername;
TextLayer *s_textlayer_upload;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, false);
  
  s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // s_textlayer_workout
  s_textlayer_workout = text_layer_create(GRect(0, 0, 144, 34));
  text_layer_set_background_color(s_textlayer_workout, GColorClear);
  text_layer_set_text(s_textlayer_workout, "Workout");
  text_layer_set_text_alignment(s_textlayer_workout, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_workout, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_workout);
  
  // s_textlayer_data
  s_textlayer_data = text_layer_create(GRect(0, 108, 144, 34));
  text_layer_set_text(s_textlayer_data, "Table");
  text_layer_set_text_alignment(s_textlayer_data, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_data, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_data);
  
  // s_textlayer_upload
  s_textlayer_upload = text_layer_create(GRect(0, 135, 144, 34));
  text_layer_set_background_color(s_textlayer_upload, GColorClear);
  text_layer_set_text(s_textlayer_upload, uploadStatus);
  text_layer_set_text_alignment(s_textlayer_upload, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_upload, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_upload);
  
  // s_textlayer_player
  s_textlayer_player = text_layer_create(GRect(0, 60, 144, 30));
  text_layer_set_background_color(s_textlayer_player, GColorClear);
  if(varMP==false) text_layer_set_text(s_textlayer_player, "Singleplayer");
  else text_layer_set_text(s_textlayer_player, "Multiplayer");
  text_layer_set_text_alignment(s_textlayer_player, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_player, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_player);
  
  // s_textlayer_playername
  s_textlayer_playername = text_layer_create(GRect(0, 82, 144, 20));
  text_layer_set_background_color(s_textlayer_playername, GColorClear);
  text_layer_set_text(s_textlayer_playername, "First: Marco");
  text_layer_set_text_alignment(s_textlayer_playername, GTextAlignmentCenter);
  //text_layer_set_font(s_textlayer_playername, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_playername);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_workout);
  text_layer_destroy(s_textlayer_data);
  text_layer_destroy(s_textlayer_player);
  text_layer_destroy(s_textlayer_playername);
  text_layer_destroy(s_textlayer_upload);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  choice = VAR_WORKOUT;
  show_menu_musclegroup();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  choice = VAR_TABLE;
  show_menu_musclegroup();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(varMP == false) {
    varMP = true;
    text_layer_set_text(s_textlayer_player, "Multiplayer");
    layer_set_hidden((Layer*)s_textlayer_playername, false);
  } else {
    varMP = false;
    text_layer_set_text(s_textlayer_player, "Singleplayer");  
    layer_set_hidden((Layer*)s_textlayer_playername, true);
  }
}
static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(firstPhil == false) {
    firstPhil = true;
    text_layer_set_text(s_textlayer_playername, "First: Phil");
  } else {
    firstPhil = false;
    text_layer_set_text(s_textlayer_playername, "First: Marco");  
  }
}

static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  // upload
  //text_layer_set_text(s_textlayer_upload, "Uploading ...");
  //set_textlayer_content(s_textlayer_upload, "Uploading ...");
  strcpy(uploadStatus,"Uploading Data ...");
  layer_mark_dirty((Layer*)s_textlayer_upload);
  
  upload_data(0, 9, 0);
   
  //strcpy(uploadStatus,"Data uploaded !");
  //(psleep(1000);
  //text_layer_set_text(s_textlayer_upload, "Data uploaded!");
  //set_textlayer_content(s_textlayer_upload, "Data uploaded!");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 500, select_long_click_handler, NULL);
  window_long_click_subscribe(BUTTON_ID_DOWN, 1000, down_long_click_handler, NULL);
}

void show_homescreen(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "show_homescreen()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  varMP = true;
  firstPhil = false;
  
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_homescreen(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "hide_homescreen()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  window_stack_remove(s_window, true);
}
