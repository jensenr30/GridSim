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

    int x = 0, y = 0;
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
			if( event.type == SDL_MOUSEMOTION ){
				x = event.button.x;
				y = event.button.y;
			}
    	} // end while(event)
		//no more events to handle at the moment.
		
		apply_surface( 640 - x, 480 - y, image, screen );
		// flip screen
		SDL_Flip( screen );
		SDL_FreeSurface( screen );

		SDL_Rect my_rect;
		my_rect.x = 0;
		my_rect.y = 0;
		my_rect.w = 640;
		my_rect.h = 480;
		SDL_FillRect( screen, &my_rect, 0xffffff );
		SDL_FreeSurface( screen );

    }// end while(quit == false)
    
    //Free the surface and quit SDL
    clean_up();
    
    return 0;
}
