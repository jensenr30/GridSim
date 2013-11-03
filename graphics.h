

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




//this draws a line on the screen from point (x1,y1) to point (x2,y2)
// this draws the line (mathematically) from the centers of the two pixels.
void draw_line(SDL_Surface *theSurface, int x1, int y1, int x2, int y2, int thickness, int lineColor){
	//this is the rectangle describing the pixel to color in.
	SDL_Rect myPixel;
	myPixel.w = 1;
	myPixel.h = 1;
	
	//handle lines with no x-difference
	if(x1 == x2){
		// this handles the special case that the line is a point
		if(y1 == y2){
			// draw a point
			myPixel.x = x1;
			myPixel.y = y1;
			SDL_FillRect(theSurface, &myPixel, lineColor);
			// done drawing a point. that's all folks!
			return;
		}
		//this handles the special case the the line is a verticle line (slope infinity)
		// this swaps the x and y values so that the function draws a line with a finite slope (zero)
		draw_line(theSurface,y1,x1,y2,x2,thickness,lineColor);
		return;
	}
	
	float slope = (float)(y2-y1)/(float)(x2-x1);
	//generate an absolute value of the slope
	float absval_slope = slope;
	if(slope < 1) absval_slope *= -1;
	float x,y;
	//this is used to mathematically determine where the line should be.
	float line_value;
	
	// if the absolute value of the slope is less than 1, index through the x values
	if(absval_slope < 1){
		/* OLD CODE FOR GRADUAL SLOPE LINES
		// swap the points around if x1 > x2
		if(x1 > x2){
			
			int tempval=x1;	// store x1
			x1 = x2;		// put x2 into x1
			x2 = tempval;	// put the value that used to be in x1 into x2.
			tempval = y1;	// store y1
			y1 = y2;		// put y2 into y1
			y2 = tempval;	// put the value that used to be in y1 into y2.
		}
		
		for(x=x1+0.5,myPixel.x=x1; x<x2; x+=1.0,myPixel.x++){
			line_value = slope*x +y1;
			myPixel.y = line_value; // integer assignment. truncate decimal places
			// if the line_value has decimal place value greater than or equal to 0.5, then round up.
			if(line_value-(int)line_value >= 0.5) myPixel.y++;
				
			SDL_FillRect(theSurface, &myPixel, lineColor);
		}
		*/
		//all of this assumes that x1 <= x2
		float pixel_offset = 0.5;
		//x2 is greater than x1
		int low_x = x1;
		int high_x = x2;
		int ref_y = y1;		// this is the reference y. this is where we start
		
		// this takes care of the special case of x1 being bigger than x2.
		if(x1 > x2){
			pixel_offset *= -1;
			//x2 is less than x1
			low_x = x2;
			high_x = x1;
			ref_y = y2;
		}
		
		
		for(x=low_x+pixel_offset,myPixel.x=low_x; x<high_x; x+=1,myPixel.x++){
			line_value = (x-low_x)*slope + ref_y;
			myPixel.y = line_value; // integer assignment. truncate decimal places
			// if the line_value has decimal place value greater than or equal to 0.5, then round up.
			if(line_value-(int)line_value > 0.5) myPixel.y++;
				
			SDL_FillRect(theSurface, &myPixel, lineColor);
		}
	}
	// otherwise, the absolute value of the slope is greater to or equal to one, so index through the y values
	else{
		/*
		// swap the points around if y1 > y2
		if(x1 > x2){
			int tempval=x1;	// store x1
			x1 = x2;		// put x2 into x1
			x2 = tempval;	// put the value that used to be in x1 into x2.
			tempval = y1;	// store y1
			y1 = y2;		// put y2 into y1
			y2 = tempval;	// put the value that used to be in y1 into y2.
		}
		*/
		
		
		//all of this assumes that y1 <= y2
		float pixel_offset = 0.5;
		//y2 is greater than y1
		int low_y = y1;
		int high_y = y2;
		int ref_x = x1;		// this is the reference x. this is where we start
		
		// this takes care of the special case of y1 being bigger than y2.
		if(y1 > y2){
			pixel_offset *= -1;
			//y2 is less than y1
			low_y = y2;
			high_y = y1;
			ref_x = x2;
		}
		
		
		for(y=low_y+pixel_offset,myPixel.y=low_y; y<high_y; y+=1,myPixel.y++){
			line_value = (y-low_y)/slope + ref_x;
			myPixel.x = line_value; // integer assignment. truncate decimal places
			// if the line_value has decimal place value greater than or equal to 0.5, then round up.
			if(line_value-(int)line_value > 0.5) myPixel.x++;
				
			SDL_FillRect(theSurface, &myPixel, lineColor);
		}
	}
	
}




