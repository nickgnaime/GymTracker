#include "pebble.h"
//#include "definitions.h"
#include "homescreen.h"
  
void handle_init(void) {
  show_homescreen();
}

void handle_deinit(void) {
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}