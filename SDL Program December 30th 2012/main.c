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
			if( event.type == SDL_MOUSEBUTTONDOWN ){
				if(event.button.button == 1){
					// user clicked left mouse button.
					x++;
				}
				else if(event.button.button == 2){ // middle click
					y++;
				}
				else if(event.button.button == 3){
					// user clicked right mouse button
					x--;
				}
			}
    	} // end while(event)
    }// end while(quit == false
    
    //Free the surface and quit SDL
    clean_up();
    
    return 0;
}
