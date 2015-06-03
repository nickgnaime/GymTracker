#include <pebble.h>
#include "menu_exercise.h"
#include "definitions.h"
#include "workout.h"
#include "table_exercise.h"

#define NUM_MENU_SECTIONS 1
  
static Window *s_menu_exercise_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[13];
static uint8_t musclegroup;
static uint8_t numExercises;
static char myTitle[16];
static strct_exercise *myExercises;

static void menu_select_callback(int index, void *ctx) {
  if(choice == VAR_WORKOUT) show_workout(myExercises[index].ID); 
  else show_table_exercise(myExercises[index].ID);
}

static void menu_exercise_window_load(Window *window) {
  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int i;
  
  for(i = 0; i<numExercises; i++) {
    s_first_menu_items[i] = (SimpleMenuItem) {
      .title = myExercises[i].name,
      .callback = menu_select_callback,
    };
  }
    
  s_menu_sections[0] = (SimpleMenuSection) {
    .title = myTitle,
    .num_items = numExercises,
    .items = s_first_menu_items,
  };
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

void menu_exercise_window_unload(Window *window) {
  window_destroy(s_menu_exercise_window);
  simple_menu_layer_destroy(s_simple_menu_layer);
}

void show_menu_exercise(uint8_t myMusclegroup) {
  APP_LOG(APP_LOG_LEVEL_INFO, "show_menu_exercise()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  musclegroup = myMusclegroup;
  switch(musclegroup) {
    case 0: numExercises = NUMBER_OF_EXERCISES_LEGS;  strcpy(myTitle, "Beinübungen");     myExercises = myLegExercises; break;
    case 1: numExercises = NUMBER_OF_EXERCISES_CHEST; strcpy(myTitle, "Brust und Bauch"); myExercises = myChestExercises; break;
    case 2: numExercises = NUMBER_OF_EXERCISES_BACK;  strcpy(myTitle, "Rückenübungen");   myExercises = myBackExercises; break;
    case 3: numExercises = NUMBER_OF_EXERCISES_ARMS;  strcpy(myTitle, "Armübungen");      myExercises = myArmExercises; break;
    default: break;
  }
  s_menu_exercise_window = window_create();
  window_set_window_handlers(s_menu_exercise_window, (WindowHandlers) {
    .load = menu_exercise_window_load,
    .unload = menu_exercise_window_unload,
  });
  window_stack_push(s_menu_exercise_window, true);
}

void hide_menu_exercise() {
  //window_destroy(s_menu_exercise_window);
  APP_LOG(APP_LOG_LEVEL_INFO, "hide_menu_exercise()");
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes used: %d", heap_bytes_used());
  APP_LOG(APP_LOG_LEVEL_INFO, "heap bytes free: %d", heap_bytes_free());
  window_stack_remove(s_menu_exercise_window, true);
}