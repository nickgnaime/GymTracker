#include <pebble.h>  
  
#define VAR_WORKOUT true
#define VAR_TABLE false

#define NUMBER_OF_EXERCISES_LEGS   6 
#define NUMBER_OF_EXERCISES_CHEST 12
#define NUMBER_OF_EXERCISES_BACK   7 
#define NUMBER_OF_EXERCISES_ARMS   8
#define NUMBER_OF_EXERCISES_TOTAL 32

typedef struct  {
  uint8_t ID;
  char name[21];
} strct_exercise; 
  
typedef struct {
  uint8_t month;
  uint8_t day;
  uint8_t reps[6];
  uint8_t weight[6];
} strct_workout;

extern strct_exercise myLegExercises[NUMBER_OF_EXERCISES_LEGS];
extern strct_exercise myChestExercises[NUMBER_OF_EXERCISES_CHEST];
extern strct_exercise myBackExercises[NUMBER_OF_EXERCISES_BACK];
extern strct_exercise myArmExercises[NUMBER_OF_EXERCISES_ARMS];

bool choice;
bool varMP;
bool firstPhil;
extern char uploadStatus[];

char* getExerciseName(uint8_t id);
char* whoIs(uint8_t playerID);
