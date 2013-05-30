
#include "Windows.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "time.h"

//definitions
#define MAX_COUNT  150
#define false 0 // c++ adaption
#define true 1  // c++ adaption

//allowing us to efficiently use the 'bool' type
#define bool char // 1 byte of information for a bool. not perfect, but better than using 4 bytes! :D
#define false 0
#define FALSE 0
#define true 1
#define TRUE 1

//The attributes of the screen
#define SCREEN_WIDTH 896
#define SCREEN_HEIGHT 576
#define SCREEN_BPP 32

//The surfaces that will be used
SDL_Surface *screen = 	NULL;
SDL_Surface *text = 	NULL;
//SDL_Surface *image = 	NULL;
//The event structure that will be used
SDL_Event event;

//the fint that will be used
TTF_Font *font = NULL;

//The color of the text
SDL_Color textColor = { 255, 255, 255 };

//id for changing materials
//3 = water
int currentMat = 1;

//mouse moddifier used for brushes
short mouseModifier = 0;



#include "general_functions.h"
#include "materials_and_cells.h"
#include "cell_engine.h"
#include "map_editor.h"
#include "selection_gui.h"


