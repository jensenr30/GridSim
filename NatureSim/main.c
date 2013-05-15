#include "headers_and_globals.h"
 
int main( int argc, char* args[] )
{
	//make sure the program waits for a quit
	int quit = false;
	
    //Initialize
    if( init() == false ) return 1;
    
    //Load the files
    if( load_files() == false ) return 2;
    
    //Update the screen
    if( SDL_Flip( screen ) == -1 ) return 3;
    
    //initialize the cell stuff. This gets the cell system up and running.
    init_cell_stuff();
    
    
	
    //While the user hasn't quit
    while( quit == false ){
			
    	//While there's an event to handle
    	while( SDL_PollEvent( &event ) ){
    		
    		//If the user has Xed out the window
    		if( event.type == SDL_QUIT ){
				//Quit the program
				quit = true;
			}
			
			
			
    	} // end while(event)
		//no more events to handle at the moment.

		
		

		
    }// end while(quit == false)
    
    //Free the surface and quit SDL
    clean_up();
    
    return 0;
}
