#include "headers_and_globals.h"




int main( int argc, char* args[] ) 
{
	
	//get a random seed.
	srand(time(NULL));
	
    //mouse variables and cell types
    int x, y, sleepTime = 0, countVar = 0;
	
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
    
    // create the surface that the grid data will be printed to.
    gridSurface = create_surface(GRID_WIDTH_ELEMENTS,GRID_HEIGHT_ELEMENTS);
    
    // these are used for displaying the grid and the sky
    SDL_Rect screenRect;
    
    //create the skySurface
    skySurface = create_surface(GRID_WIDTH_ELEMENTS,GRID_HEIGHT_ELEMENTS);
    
    //generate sky gradient
    generate_sky(skySurface, SCREEN_WIDTH, SCREEN_HEIGHT);
    
	// these keep track of the WASD keys.
	int keyw=0, keya=0, keys=0, keyd=0;
	bool keyF3=true;
	
	//these are used to calculating and keeping track of the FPS
	int ticksSinceLastFPSUpdate = 0;
	int cumulativeFrames = 0;
	int currentTicks = 0;
	
	//last minute verification that the initial start up values for the grid size and the camera positions are valid.
	verify_grid_and_cell_size();
	
	//generate a world to begin the game
	gen_world(w_normal,0);
	
	CELL_SIZE = 8;
	
	// get default player data.
	init_player_attributes(&player);
	
	player.x_pos = GRID_WIDTH_ELEMENTS/2;
	player.y_pos = 1080-500;
	
	
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
				/*
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
				*/
            }
            else if(event.type == SDL_VIDEORESIZE){							/// window resize
				
				float new_cell_size = CELL_SIZE * event.resize.h/((float)SCREEN_HEIGHT); // adjust the pixel size.
				if(new_cell_size - ((int)new_cell_size) >= 0.5f) CELL_SIZE = new_cell_size + 1;
				else CELL_SIZE = new_cell_size;
				SCREEN_WIDTH = event.resize.w;
				SCREEN_HEIGHT = event.resize.h;
				verify_grid_and_cell_size(); // make sure the window isn't too big for the cell size
				set_window_size(event.resize.w, event.resize.h);		// set window to correct dimensions
				generate_sky(skySurface, SCREEN_WIDTH, SCREEN_HEIGHT);	// render a new sky background
			}
			
            if( event.type == SDL_KEYDOWN ){		///keyboard event
                switch( event.key.keysym.sym ){
				case SDLK_c:		reset_cells();  break;	//clear the screen
				case SDLK_LEFT:		if(paused != 1) {sleepTime /= 2;} break; 									//speeds up the game
				case SDLK_RIGHT:	if(paused != 1) {if(sleepTime == 0){sleepTime = 1;} {sleepTime *= 2;} if(sleepTime > 2000) {sleepTime = 2000;}} break; //slows down the game
				case SDLK_SPACE:	if(paused == 0) {paused = 1;} else if(paused == 1) {paused = 0;} break; 	//pause the game
				//case SDLK_ESCAPE: quit = true; 		// quit with escape
				case SDLK_F1:		gen_world(w_normal,wf_display_generation); break; // generate a world
				case SDLK_F3:		keyF3 ^= 1; break;	// toggle printing debugging information
				case SDLK_w:		keyw=1; break; 		// store key state
				case SDLK_a:		keya=1; break;
				case SDLK_s:		keys=1; break;
				case SDLK_d:		keyd=1; break;
				
				/*
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
					*/
				default: break;
				}
			}
			if( event.type == SDL_KEYUP ){								///keyboard event
                switch( event.key.keysym.sym ){
				case SDLK_w: keyw=0; break; // store key state
				case SDLK_a: keya=0; break;
				case SDLK_s: keys=0; break;
				case SDLK_d: keyd=0; break;
				/*
				case SDLK_LALT:
				case SDLK_RALT:
					// show the cursor again.
					SDL_ShowCursor(SDL_ENABLE);
					alt = 0;
					break;
				*/
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
		
		
		//evaluate the player's movements.
		evaluate_player_movement(&player, keyw, keya, keys, keyd);
		
		//grid[(int)player.x_pos][(int)player.y_pos].mat = m_test2; // this can produce segmentation fault errors without the proper checks and balances (which it currently does not have)
		
		//apply/remove material (test feature for debugging)
		#if(1)
		int xcell,ycell;
		xcell = x/CELL_SIZE+(int)player.x_pos-GRID_WIDTH/2;
		ycell = y/CELL_SIZE+(int)player.y_pos-GRID_HEIGHT/2;
		if(within_grid_elements(xcell,ycell)){
			//checks if the mouse is held or not
			if(mouseStatusRight == 1 && mouseModifier == 0){
				grid[xcell][ycell].mat = m_earth;
			}
			if(mouseStatusLeft == 1 && mouseModifier == 0){
				grid[xcell][ycell].mat = m_air;
			}
		}
		#endif
        
        //speed of gameplay
        countVar++;
        if(sleepTime <= 0)
        {
            sleepTime = 0;
        }
		
        //evealuate cells
        if(countVar >= sleepTime && paused != 1){
            evaluate_gravity(0,0,200,200);
            countVar = 0;
        }
        /// ----------------------------------------------------------------
        /// GRID PRINTING STUFF
        /// ----------------------------------------------------------------
        
        
        // apply initial black background
		screenRect.x = 0;
		screenRect.y = 0;
		screenRect.w = SCREEN_WIDTH;
		screenRect.h = SCREEN_HEIGHT;
		SDL_FillRect( screen , &screenRect , 0x000000);
		
		//generate background
		//generate_sky(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
		//apply background
		apply_surface(0,0,skySurface,screen);
		
		//gradient(screen, &screenRect, 0, 0, 100, 200, 0x2561a9, 0x6cb8f6, 0);
		
		// generate the grid image
		SDL_FillRect( gridSurface , &screenRect , 0x000000);
        generate_grid_surface(gridSurface);
		// apply the grid surface to the screen
        apply_surface(0,0, gridSurface, screen);
        
        
        //generate player rectangle
        SDL_Rect playerRect;
		playerRect.x = CELL_SIZE*(GRID_WIDTH/2);
		playerRect.y = CELL_SIZE*(GRID_HEIGHT/2 - player.height);
		playerRect.w = CELL_SIZE*player.width;
		playerRect.h = CELL_SIZE*player.height;
        // print the character
        SDL_FillRect(screen, &playerRect, player.color);
        
        
        
        // print the debugging information to the screen.
        if(keyF3) print_debugging_information(x,y);
        
        
        
        //updates the screen
        SDL_Flip( screen );
        
        //----------------------------------------------------
		// FPS calculation and variable handling
		//----------------------------------------------------
        currentTicks = SDL_GetTicks();
        // it is officially the next second
        if(currentTicks >= ticksSinceLastFPSUpdate + 1000){
			// calculate the FPS
			FPS = cumulativeFrames;//(cumulativeFrames*1000 ) / (currentTicks-ticksSinceLastFPSUpdate);
			cumulativeFrames=0;				// reset cumulative amount of frames
			ticksSinceLastFPSUpdate = currentTicks;	// reset the last FPS update to the number of ticks now.
        }
        cumulativeFrames++;
		
    }// end while(quit == false)


    //Free the surface and quit SDL
    clean_up();

    return 0;
}
