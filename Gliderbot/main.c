
/* Ryan Jensen * <date> * <title>
 * <purpose>
 */
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <Windows.h>
#include "stdlib.h"
#include "conio.h"
#include <math.h>
#include <string.h>
#include <time.h>

#define MAX_CHARGES 200
int number_of_used_charges;

#define false 0
#define true  1

typedef int bool;

//The attributes of the screen
int SCREEN_WIDTH = 1067;
int SCREEN_HEIGHT = 600;
int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface* screen =		NULL;
SDL_Surface* text = 		NULL;
SDL_Surface* glider = 		NULL;
SDL_Surface* background = 	NULL;

//The event structure that will be used
SDL_Event event;

//the font that will be used
TTF_Font *font = NULL;
int font_size = 15;

//The color of the text
SDL_Color textColor = { 255, 32, 32 };

//this tells if the sim is paused or not. 0 = running, 1 = paused
short int paused = 0;
//this is used to tell time;
double time_el = 0.00;
//this is how much time each increment is.
double time_inc = 0.0005; // one millisecond of game

float posx = 50;
float posy = 50;

float speedx = 0;
float speedy = 0;

float thrusterAccel = 0.001;
float uThruster = 0;
float dThruster = 0;
float lThruster = 0;
float rThruster = 0;

// this is the factor that the impact speed if multiplied by when bouncing off of a surface.
float reflecSpeedMod = 0.00; // default value. The user may specify a different value in settings.txt

// this is the minimum speed that the object must be trvaeling to be considered to bounce. otherwise the speed is set to 0.
// I do this so that the glider doesn't float 1 pixel away from the screen when bouncing off of it.
float reflecSpeedCutOff = 0.04;

int keyUp = 0;
int keyDown = 0;
int keyLeft = 0;
int keyRight = 0;

void frame( int );

#include "functions.h"
#include "graphics_functions.h"
#include "glider_functions.h"


int main( int argc, char* args[] )
{
	
	
    //Initialize
    if( init() == false )
    {
    	return 100;
    }
    
    //Load the files
    load_files();
    
    if( load_physics_settings() == 0){
		MessageBox(NULL, "physics_settings.txt can not be read. No settings loaded.", "Bad Data", MB_OK);
    }
    
    
    //tell the user where he/she can find more instructions 
    //text = TTF_RenderText_Blended( font, "Ctrl + Left Click to place a charge!", textColor );
    //apply_surface( SCREEN_WIDTH/2 - 200 , SCREEN_HEIGHT/2 - 100, text, screen);
    //text = TTF_RenderText_Blended( font, "Ctrl + Right Click to delete it!", textColor );
    //apply_surface( SCREEN_WIDTH/2 - 200 , SCREEN_HEIGHT/2 - 75, text, screen);
    //text = TTF_RenderText_Blended( font, "Check out the Read Me.txt file for", textColor );
    //apply_surface( SCREEN_WIDTH/2 - 200 , SCREEN_HEIGHT/2 - 50, text, screen);
    //text = TTF_RenderText_Blended( font, "a brief guide to this program!", textColor );
    //apply_surface( SCREEN_WIDTH/2 - 200 , SCREEN_HEIGHT/2 - 25, text, screen);
    //SDL_Flip(screen);
	//make sure the program waits for a quit
	bool quit = false;
    //int x, y;
    
    //While the user hasn't quit
    while( quit == false )
    {
    	//While there's an event to handle
    	while( SDL_PollEvent( &event ) )
    	{
    		//If the user has Xed out the window
    		if( event.type == SDL_QUIT )
			{
				//Quit the program
				quit = true;
			}
			//If a key was released
			if( event.type == SDL_KEYUP )
			{
				//short int k = event.key.keysym.sym;
				switch(event.key.keysym.sym){
				case SDLK_w:
					keyUp = 0;
					break;
					
				case SDLK_a:
					keyLeft = 0;
					break;
					
				case SDLK_s:
					keyDown = 0;
					break;
					
				case SDLK_d:
					keyRight = 0;
					break;
					
				default:
					break;
				}
			}
			//If space a key was pressed
			else if( event.type == SDL_KEYDOWN )
			{
				//short int k = event.key.keysym.sym;
				switch(event.key.keysym.sym){
				case SDLK_w:
					keyUp = 1;
					break;
					
				case SDLK_a:
					keyLeft = 1;
					break;
					
				case SDLK_s:
					keyDown = 1;
					break;
					
				case SDLK_d:
					keyRight = 1;
					break;
					
				default:
					break;
				}
			}
			//If a mouse button was pressed
			else if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				int mouse = event.button.button;
				//x = event.button.x;
				//y = event.button.y;
				
				if(mouse == 1) // left click
				{
					
				}
				else if(mouse == 2) //middle click
				{
					
				}
				else if(mouse == 3) //right click
				{
					
				}
				else if(mouse == 4) //scroll up
				{
					
				}
				else if(mouse == 5) //scroll down
				{
					
				}
			}
			else if( event.type == SDL_MOUSEBUTTONUP )
			{
			}
			else if( event.type == SDL_MOUSEMOTION)
			{
				//x = event.motion.x;
				//y = event.motion.y;
			}
    	} // end while(poll event)
    	
    	thrust(keyUp, keyLeft, keyDown, keyRight);
    	
    	
    	
    	if( paused == 0)
    		frame(1);
    }
    
    
    
    //Free the surface and quit SDL
    clean_up();
    return 0;
}







//if you send frame a zero, it doesn't calculate physics behavior. just updates the screen.
void frame( int itterations )
{
	int i;
	for(i = 0; i < itterations; i++){
		move_glider();
		check_glider();
		blit_background();
		blit_glider();
		SDL_Flip(screen);
		return;
	}
}
