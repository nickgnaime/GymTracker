#pragma once
#include "pebble.h"
  
  
void upload_data(uint8_t player, uint8_t exercise, uint8_t set);

void inbox_received_callback(DictionaryIterator *iterator, void *context) ;

void inbox_dropped_callback(AppMessageResult reason, void *context);

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);

void outbox_sent_callback(DictionaryIterator *iterator, void *context);