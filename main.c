#include "headers_and_globals.h"

int main( int argc, char* args[] ) 
{
	
	//get a random seed.
	srand(time(NULL));

    //mouse variables and cell types
    int x, y, sleepTime = 64, paused = 0, countVar = 0;

    //mouse is held variables
    int mouseStatusLeft = 0, mouseStatusRight = 0;

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
    
    //this sets up some surfaces that the selection gui needs to run efficiently
    if( init_selection_gui() == false){
		MessageBox(NULL, "Couldn't Initialize selection gui surface: tempGuiScreen", "Error", MB_OK);
		return -4;
    }


	int i;
///-------------------------------------
 //putting test materials into grid
    
    for(i=0; i<20; i++) grid[i+10][23].mat = m_bedrock;
    
    /*
    for(i=7 ; i<GRID_WIDTH ; i+=15){
		for(j=26 ; j<GRID_HEIGHT ; j+=20){
			grid[i][j].mat = m_anti_scurge;
		}
    }
    */
    sleepTime = 256;
 //--------------------------------------
	//for(i=0 ; i<GRID_WIDTH*GRID_HEIGHT / 2 ; i++)
	//	grid[get_rand(0,GRID_WIDTH-1)][get_rand(0,GRID_HEIGHT-1)].mat = m_grass;

    //While the user hasn't quit
    while(1){

    	//While there's an event to handle
    	while( SDL_PollEvent( &event ) ){

    		//If the user has Xed out the window
    		if( event.type == SDL_QUIT || quit == true ){
				//Quit the program
				clean_up();
				return 0;
			}


            if( event.type == SDL_MOUSEBUTTONDOWN ){
				x = event.motion.x;
				y = event.motion.y;
                if( event.button.button == SDL_BUTTON_LEFT ){
                    mouseStatusLeft = 1;
                }

                else if( event.button.button == SDL_BUTTON_RIGHT ){
                    mouseStatusRight = 1;
                }
            }

            if(event.type == SDL_MOUSEBUTTONUP){
				x = event.motion.x;
				y = event.motion.y;
                if( event.button.button == SDL_BUTTON_LEFT ){
                    mouseStatusLeft = 0;
                }
                else if( event.button.button == SDL_BUTTON_RIGHT ){
                    mouseStatusRight = 0;
                }
            }
            if( event.type == SDL_MOUSEMOTION ){
				x = event.motion.x;
				y = event.motion.y;
            }

            if( event.type == SDL_KEYDOWN ){
                switch( event.key.keysym.sym ){
                    case SDLK_UP: break; //change block type up
                    case SDLK_DOWN: break; // change block type down
                    case SDLK_c: reset_cells();  break;//clear the screen
                    case SDLK_p: print_saturation_data(); break; // prints the cellSat[][] array to stdout. this is for debuggin purposes.
                    case SDLK_r:  randomize_grid(); break; // randomize grid
                    case SDLK_LEFT: if(paused != 1) {sleepTime /= 2;} break; //speeds up the game
                    case SDLK_RIGHT: if(paused != 1) {if(sleepTime == 0){sleepTime = 1;} {sleepTime *= 2;} if(sleepTime > 2000) {sleepTime = 2000;}} break; //slows down the game
                    case SDLK_SPACE: if(paused == 0) {paused = 1;} else if(paused == 1) {paused = 0;} break; //pause the game
                    case SDLK_EQUALS: zoomplus(); break; //zoom in
                    case SDLK_MINUS: zoomminus(); break; //zoom out
                    case SDLK_ESCAPE: quit = true; // quit with escape
                    default: break;
                    }
                }

    	} // end while(event)
		//no more events to handle at the moment.
		
		//checks if the mouse is held or not
        if(mouseStatusLeft == 1 && mouseModifier == 0){
			//make sure the mouse isn't inside either of the two GUIs.
			if(y >= 50 && x < SCREEN_WIDTH - 200)
            setcell(x, y, currentMat);
            }

        else if(mouseStatusRight == 1 && mouseModifier == 0){
            deletecell(x, y, currentMat);
        }
        
        //speed of gameplay
        countVar++;
        if(sleepTime <= 0)
        {
            sleepTime = 0;
        }

        //evealuate cells
        if(countVar >= sleepTime && paused != 1){
            evaluate_grid();
            countVar = 0;
        }

        //updates screen with cells
        print_cells();

        //displays selection gui
        selectionGUI(x, y, mouseStatusLeft);
        
        //displays brushes and misc gui
        brushesGUI(x, y, mouseStatusLeft);
        
        //displays cursor
        cursorDisplay(x, y);

        //updates the screen
        SDL_Flip( screen );

    }// end while(quit == false)


    //Free the surface and quit SDL
    clean_up();

    return 0;
}
