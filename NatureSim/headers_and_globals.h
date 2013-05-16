

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "Windows.h"
#include "time.h"

//definitions
#define MAX_COUNT  150
#define false 0 // c++ adaption
#define true 1  // c++ adaption

//The attributes of the screen
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 540
#define SCREEN_BPP 32

//The surfaces that will be used
SDL_Surface* screen = 	NULL;
//SDL_Surface* text = 	NULL;
//SDL_Surface* image = 	NULL;
/// DON'T FORGET TO ADD THESE SURFACES TO "void clean_up"!
//The event structure that will be used
SDL_Event event;

//the fint that will be used
//TTF_Font *font = NULL;

//The color of the text
//SDL_Color textColor = { 0, 0, 0 };






#include "general_functions.h"
#include "materials_and_cells.h"
#include "cell_engine.h"


