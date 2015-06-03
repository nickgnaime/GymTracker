#include <pebble.h>
#include "upload.h"
#include "definitions.h"

#define K_PLAYER 0
#define K_EXERCISE 1
#define K_MONTH 2
#define K_DAY 3 
#define K_R0 4
#define K_R1 5
#define K_R2 6
#define K_R3 7
#define K_R4 8
#define K_R5 9
#define K_W0 10
#define K_W1 11
#define K_W2 12
#define K_W3 13
#define K_W4 14
#define K_W5 15
  
  
void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}

void upload_data(uint8_t player, uint8_t exercise, uint8_t set) {
  
   
  strct_workout tmpWorkout[2][4];
  
  persist_read_data(exercise, tmpWorkout, sizeof(tmpWorkout));
  
  int i;
  DictionaryIterator *iterator[4];
  
  for(i=0; i<4; i++) {
	  app_message_outbox_begin(&iterator[i]);
		
	  dict_write_uint8(iterator[i], K_PLAYER, player);
	  dict_write_uint8(iterator[i], K_EXERCISE, exercise);
	  dict_write_uint8(iterator[i], K_MONTH, tmpWorkout[player][set].month);
	  dict_write_uint8(iterator[i], K_DAY, tmpWorkout[player][set].day);
	  dict_write_uint8(iterator[i], K_R0, tmpWorkout[player][set].reps[0]);
	  dict_write_uint8(iterator[i], K_R1, tmpWorkout[player][set].reps[1]);
	  dict_write_uint8(iterator[i], K_R2, tmpWorkout[player][set].reps[2]);
	  dict_write_uint8(iterator[i], K_R3, tmpWorkout[player][set].reps[3]);
	  dict_write_uint8(iterator[i], K_R4, tmpWorkout[player][set].reps[4]);
	  dict_write_uint8(iterator[i], K_R5, tmpWorkout[player][set].reps[5]);
	  dict_write_uint8(iterator[i], K_W0, tmpWorkout[player][set].weight[0]);
	  dict_write_uint8(iterator[i], K_W1, tmpWorkout[player][set].weight[1]);
	  dict_write_uint8(iterator[i], K_W2, tmpWorkout[player][set].weight[2]);
	  dict_write_uint8(iterator[i], K_W3, tmpWorkout[player][set].weight[3]);
	  dict_write_uint8(iterator[i], K_W4, tmpWorkout[player][set].weight[4]);
	  dict_write_uint8(iterator[i], K_W5, tmpWorkout[player][set].weight[5]);
	  dict_write_end(iterator[i]); 
  }
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Sending String: %d%02d%02d%02d%03d%03d%03d%03d%03d%03d%03d%03d%03d%03d%03d%03d", player, exercise, tmpWorkout[player][set].month, tmpWorkout[player][set].day, tmpWorkout[player][set].reps[0], tmpWorkout[player][set].reps[1], tmpWorkout[player][set].reps[2], tmpWorkout[player][set].reps[3], tmpWorkout[player][set].reps[4], tmpWorkout[player][set].reps[5], tmpWorkout[player][set].weight[0], tmpWorkout[player][set].weight[1], tmpWorkout[player][set].weight[2], tmpWorkout[player][set].weight[3], tmpWorkout[player][set].weight[4], tmpWorkout[player][set].weight[5]);
  AppMessageResult err_app_send = app_message_outbox_send();
  //APP_LOG(APP_LOG_LEVEL_INFO, "Message Send Result: %u - %s", err_app_send, translate_error(err_app_send));

  // text_layer_set_text(layer, "Data uploaded!");
  strcpy(uploadStatus,"Data uploaded !");
  psleep(300);
}