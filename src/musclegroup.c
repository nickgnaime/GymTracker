#include <pebble.h>
#include "musclegroup.h"
//#include "beine.h"
//#include "brust.h"
//#include "ruecken.h"
//#include "arme.h"
#include "definitions.h"
#include "menu_exercise.h"


#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 4

static Window *s_menu_musclegroup_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];

static void menu_select_callback(int index, void *ctx) {
  //s_first_menu_items[index].subtitle = "You've hit select here!";
  //layer_mark_dirty(simple_menu_layer_get_layer(s_simple_menu_layer));
  
  show_menu_exercise(index);
  
  /*switch(index) {
    case 0: show_menu_beine(); break;
    case 1: show_menu_brust(); break;
    case 2: show_menu_ruecken(); break;
    case 3: show_menu_arme(); break;
    default: break;
  }*/
}

static void menu_musclegroup_window_load(Window *window) {
  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Beine",
    .callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Brust/Bauch",
    .callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Rücken",
    .callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Arme",
    .callback = menu_select_callback,
  };
  
  s_menu_sections[0] = (SimpleMenuSection) {
    .title = "Welche Körperregion?",
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_first_menu_items,
  };
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

static void menu_musclegroup_window_unload(Window *window) {
  window_destroy(s_menu_musclegroup_window);
  simple_menu_layer_destroy(s_simple_menu_layer);
}

void show_menu_musclegroup(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "show_menu_musclegroup()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  s_menu_musclegroup_window = window_create();
  window_set_window_handlers(s_menu_musclegroup_window, (WindowHandlers) {
    .load = menu_musclegroup_window_load,
    .unload = menu_musclegroup_window_unload,
  });
  window_stack_push(s_menu_musclegroup_window, true);
}
void hide_menu_musclegroup(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "hide_menu_musclegroup()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  window_stack_remove(s_menu_musclegroup_window, true);
}
