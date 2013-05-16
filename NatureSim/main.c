#include "headers_and_globals.h"

int main( int argc, char* args[] )
{
	//get a random seed.
	srand(time(NULL));
	
	//make sure the program waits for a quit
	int quit = false;

    //Initialize
    if( init() == false ) return 1;

    //Load the files
    if( load_files() == false ) return 2;

    //Update the screen
    if( SDL_Flip( screen ) == -1 ) return 3;

    //initialize the cell stuff. This gets the cell system up and running. This also sets all cells to air
    init_cell_stuff();
    
    
    
    
    //put test materials into grid
    int i,j;
    for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			cellData[i][j] = M_grass;
		}
    }
    cellData[45][35] = M_fire;
    //for(i=0 ; i<10 ; i++){
	//	cellData[get_rand(0,89)][get_rand(0,53)] = M_fire;
    //}
    
    
    

   print_cells();//print initial random cells
    Sleep(500);//wait half a second

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



	evaluate_cells(); // evaluates the cells and advances the simulation ahead by one generation.
	print_cells(); // re-prints the
	Sleep(30);

    }// end while(quit == false)

    //Free the surface and quit SDL
    clean_up();

    return 0;
}
