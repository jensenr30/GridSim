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

    //initialize the cell stuff. This gets the cell system up and running. This also sets all cells to m_air and all the saturation to m_no_saturaion
    init_cell_stuff();
    
    //this sets up some surfaces that the selection gui needs to run efficiently
    if( init_tempGuiScreen() == false){
		MessageBox(NULL, "Couldn't Initialize selection gui surface: tempGuiScreen", "Error", MB_OK);
		return -4;
    }
    
    

	CELL_SIZE = 4;
    sleepTime = 0;
	int i,j;
	//putting test materials into grid
    
    for(i=0; i<GRID_WIDTH; i++){
		for(j=0; j<10; j++)
		grid[i][GRID_HEIGHT-1-get_rand(0,4*j+12)].mat = m_rock;
    }
    /*
    for(i=7 ; i<GRID_WIDTH ; i+=15){
		for(j=26 ; j<GRID_HEIGHT ; j+=20){
			grid[i][j].mat = m_anti_scurge;
		}
    }
    */
    
	//--------------------------------------
	//for(i=0 ; i<GRID_WIDTH*GRID_HEIGHT / 2 ; i++)
	//	grid[get_rand(0,GRID_WIDTH-1)][get_rand(0,GRID_HEIGHT-1)].mat = m_plant;
	int keyw=0, keya=0, keys=0, keyd=0;
	unsigned lastPanTime = 0;
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


            if( event.type == SDL_MOUSEBUTTONDOWN ){						/// mouse down
				x = event.motion.x;
				y = event.motion.y;
                if( event.button.button == SDL_BUTTON_LEFT ){
                    mouseStatusLeft = 1;
                }
                else if( event.button.button == SDL_BUTTON_RIGHT ){
                    mouseStatusRight = 1;
                }
                else if( event.button.button == SDL_BUTTON_WHEELUP )
					zoom_in();
				else if( event.button.button == SDL_BUTTON_WHEELDOWN )
					zoom_out();
            }
            else if(event.type == SDL_MOUSEBUTTONUP){						/// mouse up
				x = event.motion.x;
				y = event.motion.y;
                if( event.button.button == SDL_BUTTON_LEFT ){
                    mouseStatusLeft = 0;
                }
                else if( event.button.button == SDL_BUTTON_RIGHT ){
                    mouseStatusRight = 0;
                }
            }
            else if( event.type == SDL_MOUSEMOTION ){						/// mouse motion
				x = event.motion.x;
				y = event.motion.y;
            }
            else if(event.type == SDL_VIDEORESIZE){							/// window resize
				
				SCREEN_WIDTH = event.resize.w;
				SCREEN_HEIGHT = event.resize.h;
				verify_grid_and_cell_size(); // make sure the window isn't too big for the cell size
				set_window_size(event.resize.w, event.resize.h);
			}

            if( event.type == SDL_KEYDOWN ){								///keyboard event
                switch( event.key.keysym.sym ){
				case SDLK_UP: break; //change block type up
				case SDLK_DOWN: break; // change block type down
				case SDLK_c: reset_cells();  break;//clear the screen
				case SDLK_p: print_saturation_data(); break; // prints the cellSat[][] array to stdout. this is for debuggin purposes.
				case SDLK_r:  randomize_grid(); break; // randomize grid
				case SDLK_LEFT: if(paused != 1) {sleepTime /= 2;} break; //speeds up the game
				case SDLK_RIGHT: if(paused != 1) {if(sleepTime == 0){sleepTime = 1;} {sleepTime *= 2;} if(sleepTime > 2000) {sleepTime = 2000;}} break; //slows down the game
				case SDLK_SPACE: if(paused == 0) {paused = 1;} else if(paused == 1) {paused = 0;} break; //pause the game
				case SDLK_ESCAPE: quit = true; // quit with escape
				case SDLK_w: keyw=1; break; // store key state
				case SDLK_a: keya=1; break;
				case SDLK_s: keys=1; break;
				case SDLK_d: keyd=1; break;
				default: break;
				}
			}
			if( event.type == SDL_KEYUP ){								///keyboard event
                switch( event.key.keysym.sym ){
				case SDLK_w: keyw=0; break;//lastPanTime=0; break; // store key state
				case SDLK_a: keya=0; break;//lastPanTime=0; break;
				case SDLK_s: keys=0; break;//lastPanTime=0; break;
				case SDLK_d: keyd=0; break;//lastPanTime=0; break;
				default: break;
				}
			}
                

    	} // end while(event)
		//no more events to handle at the moment.
		
		//this handles time-controlled panning
		if(SDL_GetTicks() - MIN_PAN_INTERVAL > lastPanTime){
			if(keyw) pan(D_UP);
			if(keya) pan(D_LEFT);
			if(keys) pan(D_DOWN);
			if(keyd) pan(D_RIGHT);
			lastPanTime = SDL_GetTicks();
			printf("\nlastPanTime = %d\n", lastPanTime);
		}
		
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
		SDL_GL_SwapBuffers();
		//clear openGLcolor to get it ready for the next cycle of graphics prints? i think? maybe? probably. it seems to work.
		glClear( GL_COLOR_BUFFER_BIT );

    }// end while(quit == false)


    //Free the surface and quit SDL
    clean_up();

    return 0;
}
