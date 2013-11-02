

/// this will print to the screen each material in each cell.
void print_cells(){
	
    int i,j;
	SDL_Rect myRectangle, myRectangleMat, myRectangleSat;
	myRectangle.x = 0;
	myRectangle.y = 0;
	myRectangle.w = SCREEN_WIDTH;
	myRectangle.h = SCREEN_HEIGHT;
	// INITIAL BLACK BACKGROUND
	SDL_FillRect( screen , &myRectangle , 0x000000);
	
	// constant cell sizes
	myRectangleMat.w = CELL_SIZE;
	myRectangleMat.h = CELL_SIZE;
	myRectangleSat.w = CELL_SIZE/2;
	myRectangleSat.h = CELL_SIZE/2;
	
	// print out the grid
    for(i = 0; i < GRID_WIDTH - GUI_W/CELL_SIZE; i++){
        for(j = 0; j < GRID_HEIGHT && j+camera_y<GRID_HEIGHT_ELEMENTS; j++){
        	//only print the material if it is not air
			if(grid[i+camera_x][j+camera_y].mat != m_air){
				myRectangleMat.x = i*CELL_SIZE;
				myRectangleMat.y = j*CELL_SIZE;
				SDL_FillRect( screen , &myRectangleMat , mats[grid[i+camera_x][j+camera_y].mat].color);
			}
			//only print valid saturations
			if( grid[i+camera_x][j+camera_y].sat != m_no_saturation ){
				myRectangleSat.x = i*CELL_SIZE + myRectangleSat.w/2;
				myRectangleSat.y = j*CELL_SIZE + myRectangleSat.h/2;
				SDL_FillRect( screen , &myRectangleSat , mats[grid[i+camera_x][j+camera_y].sat].color);
			}
        }
    }
    
    /*
    // print the color of the saturations of each material in each cell
    for(i = 0; i < GRID_WIDTH; i++){
        for(j = 0; j < GRID_HEIGHT; j++){
			if( grid[i][j].sat == m_no_saturation || grid[i][j].sat == m_air ) continue; // you don't need to print air. there is a black background being printed at the beginning of this print_cells() function.
			myRectangleSat.x = i*CELL_SIZE + myRectangleSat.w/2;
			myRectangleSat.y = j*CELL_SIZE + myRectangleSat.h/2;
            SDL_FillRect( screen , &myRectangleSat , mats[grid[i][j].sat].color);
        }
    }
    */
}


