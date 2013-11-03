#include "headers_and_globals.h"

int main( int argc, char* args[] ) 
{
	
	//get a random seed.
	srand(time(NULL));

    //mouse variables and cell types
    int x, y, sleepTime = 0, paused = 0, countVar = 0;

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
    
	/*
	CELL_SIZE = 4;
	int i;//
	//putting test materials into grid
    
    for(i=0; i<GRID_WIDTH; i++){
		if(get_rand(1,4)==1)
			grid[i+camera_x][GRID_HEIGHT-1-get_rand(7,15)+camera_y].mat = m_plant_root;
		if(get_rand(1,10)==10)
			grid[i+camera_x][camera_y+get_rand(20,35)].mat = m_spring;
		grid[i+camera_x][camera_y+get_rand(30,34)+30].mat = m_earth;
		grid[i+camera_x][camera_y+get_rand(30,34)+30].mat = m_earth;
		grid[i+camera_x][camera_y+get_rand(30,34)+30].mat = m_earth;
    }
    */
    CELL_SIZE = 8;
    sleepTime = 0;
	//int i;//
	//putting test materials into grid
    
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
	
	
	//int keyw=0, keya=0, keys=0, keyd=0;
	//unsigned lastPanTime = 0;
	bool alt = 0; // this keeps track of the state of the alt key
	//these variables store the position of the user's cursor at the moment that the user decided to pan the screen
	int mouse_x_when_pan=0;
	int mouse_y_when_pan=0;
	// these are for keeping track of the motion of the mouse when the user is panning. these keep the panning from being really jumpy.
	// without them, there is a truncation error when adjusting the camera_x and camera_y materials and re-setting the mouse coordinates to its original state.
	// these keep track of the truncation error and add it to the next operation so that the mouse motion feels fluid.
	int remainder_panning_motion_x=0;
	int remainder_panning_motion_y=0;
	
	//last minute verification that the initial start up values for the grid size and the camera positions are valid.
	verify_grid_and_cell_size();
	verify_camera();
	
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
					zoom_in(x,y);
				else if( event.button.button == SDL_BUTTON_WHEELDOWN )
					zoom_out(x,y);
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
				// if the alt key (camera panning key) is down and the coordinates have changed, then let the screen be panned!
				if(alt && x != mouse_x_when_pan && y != mouse_y_when_pan){
					// this adjusts the x-axis camera (this scales with the CELL_SIZE)
					camera_x += (x-mouse_x_when_pan+remainder_panning_motion_x)/CELL_SIZE;
					camera_y += (y-mouse_y_when_pan+remainder_panning_motion_y)/CELL_SIZE;
					//calculate the remainders of the mouse motion.
					// these values represent the motion of the mouse that is not utilized by the discrete nature of the operation on the camera_x and camera_y values.
					remainder_panning_motion_x = (x-mouse_x_when_pan+remainder_panning_motion_x) - (int)((x-mouse_x_when_pan+remainder_panning_motion_x)/CELL_SIZE)*CELL_SIZE;
					remainder_panning_motion_y = (y-mouse_y_when_pan+remainder_panning_motion_y) - (int)((y-mouse_y_when_pan+remainder_panning_motion_y)/CELL_SIZE)*CELL_SIZE;
					// make sure the camera is not out of bounds.
					verify_camera();
					
					//reset the user's curcor position to the original position the curcor was in when the user started panning the camera
					SDL_WarpMouse(mouse_x_when_pan, mouse_y_when_pan);
				}
            }
            else if(event.type == SDL_VIDEORESIZE){							/// window resize
				
				float new_cell_size = CELL_SIZE * event.resize.h/((float)SCREEN_HEIGHT); // adjust the pixel size.
				if(new_cell_size - ((int)new_cell_size) >= 0.5f) CELL_SIZE = new_cell_size + 1;
				else CELL_SIZE = new_cell_size;
				SCREEN_WIDTH = event.resize.w;
				SCREEN_HEIGHT = event.resize.h;
				verify_grid_and_cell_size(); // make sure the window isn't too big for the cell size
				
				set_window_size(event.resize.w, event.resize.h);
				verify_camera();
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
				case SDLK_w: gen_world(w_normal,0); break; // generate a world
				/*
				case SDLK_w: keyw=1; break; // store key state
				case SDLK_a: keya=1; break;
				case SDLK_s: keys=1; break;
				case SDLK_d: keyd=1; break;
				*/
				case SDLK_LALT:
				case SDLK_RALT:
					// store the state of the alt key.
					alt = 1;
					// turn off the cursor
					SDL_ShowCursor(SDL_DISABLE);
					// store the position of the mouse. this will allow the program to make the cursor stay stationary
					mouse_x_when_pan = x;
					mouse_y_when_pan = y;
					//reset the remainder mouse motion variables for x and y.
					remainder_panning_motion_x = 0;
					remainder_panning_motion_y = 0;
					break;
				default: break;
				}
			}
			if( event.type == SDL_KEYUP ){								///keyboard event
                switch( event.key.keysym.sym ){
				/*
				case SDLK_w: keyw=0; break;//lastPanTime=0; break; // store key state
				case SDLK_a: keya=0; break;//lastPanTime=0; break;
				case SDLK_s: keys=0; break;//lastPanTime=0; break;
				case SDLK_d: keyd=0; break;//lastPanTime=0; break;
				*/
				case SDLK_LALT:
				case SDLK_RALT:
					// show the cursor again.
					SDL_ShowCursor(SDL_ENABLE);
					alt = 0;
					break;
				default: break;
				}
			}
                

    	} // end while(event)
		//no more events to handle at the moment.
		/*
		//this handles time-controlled panning
		if(SDL_GetTicks() - MIN_PAN_INTERVAL > lastPanTime){
			if(keyw) pan(D_UP);
			if(keya) pan(D_LEFT);
			if(keys) pan(D_DOWN);
			if(keyd) pan(D_RIGHT);
			lastPanTime = SDL_GetTicks();
			//#if (DEBUG_GRIDSIM)
			//	printf("\nlastPanTime = %d\n", lastPanTime);
			//#endif
		}
		*/
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
        
        // If the user is not panning, then it is fine to show the cursor.
        if(!alt){
			//displays cursor
			cursorDisplay(x, y);
        }
        draw_line(screen,100,200,200,300,1,0xff00ff);
        //updates the screen
        SDL_Flip( screen );

    }// end while(quit == false)


    //Free the surface and quit SDL
    clean_up();

    return 0;
}
