#include <stdio.h>
#include "Windows.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "time.h"
#include <math.h>

//definitions
#define MAX_COUNT  150
#define false 0 // c++ adaption
#define true 1  // c++ adaption

#define debug 0 // this is the varibale that tells us whether we want to debug or not.

//allowing us to efficiently use the 'bool' type
#define bool char // 1 byte of information for a bool. not perfect, but better than using 4 bytes! :D
#define false 0
#define FALSE 0
#define true 1
#define TRUE 1

//this is for panning the screen (in milliseconds
#define MIN_PAN_INTERVAL 50

//The CONSTANT initial size of the screen
#define DEFAULT_SCREEN_WIDTH 896
#define DEFAULT_SCREEN_HEIGHT 576
//The DYNAMIC  initial size of the screen
unsigned int SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
unsigned int SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;

#define SCREEN_BPP 32

// The surfaces that will be used
SDL_Surface *screen = 	NULL;
SDL_Surface *text = 	NULL;
SDL_Surface *icon = 	NULL;
// The event structure that will be used
SDL_Event event;

// the fint that will be used
TTF_Font *font = NULL;

//The color of the text
SDL_Color textColor = { 255, 255, 255 };

// id for changing materials
// 3 = water
int currentMat = 1;

// mouse moddifier used for brushes
short mouseModifier = 0;

// necessary function prototype.
void setcell(int , int, int);

// include all of the other header files that have functions and variables that will be used throughout the program.
//#include "world_gen.h"
#include "general_functions.h"
#include "materials_and_cells.h"
#include "selection_gui.h"
#include "cell_engine.h"
#include "map_editor.h"



