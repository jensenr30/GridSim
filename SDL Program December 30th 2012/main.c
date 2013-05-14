#include "headers_and_globals.h"
 
int main( int argc, char* args[] )
{
	//make sure the program waits for a quit
	int quit = false;
	
    //Initialize
    if( init() == false )
    {
    	return 1;
    }
    
    //Load the files
    if( load_files() == false )
    {
    	return 2;
    }
    
    //Apply the surface to the screen
    apply_surface( 0, 0, image, screen );
    
    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
    	return 3;
    }

	int keyup = 0, keydown = 0, keyleft = 0, keyright = 0;
	int x=0, y=0;
	int countup=0, countdown=0, countleft=0, countright=0;
	
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
			//if a key was pressed
			if( event.type == SDL_KEYDOWN ){

		        //Set the proper message surface
        		switch( event.key.keysym.sym ){
            		case SDLK_UP:
            			keyup = 1;
            			break;
            		case SDLK_DOWN:
            			keydown = 1;
            			break;
            		case SDLK_LEFT:
            			keyleft = 1;
            			break;
            		case SDLK_RIGHT:
            			keyright = 1;
            			break;
            		default:
            			break;
        		}
			}
			if( event.type == SDL_KEYUP ){

		        //Set the proper message surface
        		switch( event.key.keysym.sym ){
            		case SDLK_UP:
            			keyup = 0;
            			break;
            		case SDLK_DOWN:
            			keydown = 0;
            			break;
            		case SDLK_LEFT:
            			keyleft = 0;
            			break;
            		case SDLK_RIGHT:
            			keyright = 0;
            			break;
            		default:
            			break;
        		}
			}
    	} // end while(event)
		//no more events to handle at the moment.

		//keeps track of how long you have been pressing the button.
		if(keyup == 1) countup++;
		if(keydown == 1) countdown++;
		if(keyleft == 1) countleft++;
		if(keyright == 1) countright++;

		if(countup >= MAX_COUNT) { y-=20; countup = 0; }
		if(countdown >= MAX_COUNT) { y+=20; countdown = 0; }
		if(countleft >= MAX_COUNT) { x-=20; countleft = 0; }
		if(countright >= MAX_COUNT) { x+=20; countright = 0; }

		//apply white background
		SDL_Rect my_rect;
		my_rect.x = 0;
		my_rect.y = 0;
		my_rect.w = 640;
		my_rect.h = 480;
		SDL_FillRect( screen, &my_rect, 0xffffff );
		SDL_FreeSurface( screen );

		//apply images
		apply_surface(x, y, image, screen ); //apply image
		apply_surface(x + 20, y, image, screen ); //apply image
		apply_surface(x - 20, y, image, screen ); //apply image
		apply_surface(x, y + 20, image, screen ); //apply image
		apply_surface(x, y - 20, image, screen ); //apply image
		// flip screen
		SDL_Flip( screen ); // update screen
		//SDL_FreeSurface( screen ); // saves memory
		

		
    }// end while(quit == false)
    
    //Free the surface and quit SDL
    clean_up();
    
    return 0;
}
