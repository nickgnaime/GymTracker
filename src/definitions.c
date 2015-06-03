#include <pebble.h>
#include "definitions.h"

strct_exercise myLegExercises[NUMBER_OF_EXERCISES_LEGS] = {    {  9, "Squats"}, \
                                                                      { 10, "Wadenheben sitzend"}, \
                                                                      { 11, "Wadenheben stehend"}, \
                                                                      { 12, "Adduktoren"}, \
                                                                      { 13, "Abduktoren"},\
                                                                      { 33, "Beinpresse"} \
};
strct_exercise myChestExercises[NUMBER_OF_EXERCISES_CHEST] = { {14, "Flachbank"}, \
                                                                      {15, "Flachbank schräg"}, \
                                                                      {16, "Flachbank negativ"}, \
                                                                      {17, "Flachbank KH"}, \
                                                                      {18, "Flachbank KH schräg"}, \
                                                                      {19, "Pushups"}, \
                                                                      {20, "Butterfly"}, \
                                                                      {21, "Überzieher"}, \
                                                                      {22, "Dips"}, \
                                                                      {30, "Bauchmaschine"}, \
                                                                      {31, "Crunches"}, \
                                                                      {32, "Legraises"} \
};
strct_exercise myBackExercises[NUMBER_OF_EXERCISES_BACK] = {   {23, "Klimmzüge weit"}, \
                                                                      {24, "Klimmzüge eng"}, \
                                                                      {25, "Rudern LH"}, \
                                                                      {26, "Rudern SZ"}, \
                                                                      {27, "Kreuzheben"}, \
                                                                      {28, "Lat-ziehen"}, \
                                                                      {29, "Klimmzugmaschine"} \
};
strct_exercise myArmExercises[NUMBER_OF_EXERCISES_ARMS] = {    { 1, "Bizepscurl LH"}, \
                                                                      { 2, "Bizepscurl KH"}, \
                                                                      { 3, "Trizeps SZ"}, \
                                                                      { 4, "Frenchpress"}, \
                                                                      { 5, "Militarypress"}, \
                                                                      { 6, "Schulterdrücken"}, \
                                                                      { 7, "Frontheben SZ"}, \
                                                                      { 8, "Seitheben SZ"} \
};

char* getExerciseName(uint8_t id){
  int i;
  
  for(i=0; i < NUMBER_OF_EXERCISES_LEGS; i++) {
    if(myLegExercises[i].ID == id) return myLegExercises[i].name;
  }
  for(i=0; i < NUMBER_OF_EXERCISES_CHEST; i++) {
    if(myChestExercises[i].ID == id) return myChestExercises[i].name;
  }
  for(i=0; i < NUMBER_OF_EXERCISES_BACK; i++) {
    if(myBackExercises[i].ID == id) return myBackExercises[i].name;
  }
  for(i=0; i < NUMBER_OF_EXERCISES_ARMS; i++) {
    if(myArmExercises[i].ID == id) return myArmExercises[i].name;
  }
  
  return "unknown Exercise";
}

char* whoIs(uint8_t playerID) {
  if(playerID == 0) return "Phil";
  else return "Marco";
}
