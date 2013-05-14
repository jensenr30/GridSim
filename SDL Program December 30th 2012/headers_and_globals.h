 
/* Ryan Jensen * <date> * <title>
 * <purpose>
 */
 
 
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

//definitions
#define MAX_COUNT  150
#define false 0 // c++ adaption
#define true 1  // c++ adaption

//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface* screen = 	NULL;
SDL_Surface* text = 	NULL;
SDL_Surface* image = 	NULL;

//The event structure that will be used
SDL_Event event;

//the fint that will be used
TTF_Font *font = NULL;

//The color of the text
SDL_Color textColor = { 0, 0, 0 };





#include "functions.h"


