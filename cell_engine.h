

/// this document contains the necessary functions to evaluate the grid

//these are definitions for use with the gravity evaluation code in part 1 of the evaluate grid function.
#define SLOPE_NONE   	0
#define SLOPE_RIGHT  	1
#define SLOPE_LEFT   	2
#define SLOPE_EITHER 	3

#define TUNNEL_NONE 	0
#define TUNNEL_RIGHT 	1
#define TUNNEL_LEFT  	2
#define TUNNEL_EITHER 	3


// this will evaluate the gravity of materials in their cells.
void evaluate_gravity(int x_start, int y_start, int width, int height){
	
	/////////////////// VARIABLES USED IN GRAVITY AND OTHER SUBSEQUENT PORTIONS OF THE CODE ///////////////////
	
	// i and j are used to index through the current view of the grid.
	short int i,j; // i = x position in grid. j = y position in grid. k = material type being evaluated. a = type of material being affected. c = which cell around the material is being checked
	
	// used to temporarily store the gravity value of the cell's material
	int cMat;
	// used to temporarily store the gravity value of the cell material's gravity
	char currentGrav;
	// used for indexing horizontally when a gravity material has to travel sideways (these are used in the gravity slope portions where the materials fall down slopes).
	// jg is for steep slopes (positive gravity)
	// ig is used for gentle slopes (negative gravity)
	char jg, ig;
	// the holdOff variables are used to police the movement of materials. without them, shit looks really weird.
	// these variables keep track of how long ago materials were moved around and it tells the program how soon it can move materials in the same direction again.
	short holdOffRight = 0;
	short holdOffLeft = 0;
	// these are for storing which direction the material wants to fall
	char moveRight;
	char moveLeft;
		// the following variables are used in the part of the code that deals with slopes that are less than or equal to 1 (liquid-like materials: gravity < 0)
		// they help keep track of what the material's surroundings are like.
		// based on what these variables are calculted to be, the material will be put in different places, or not moved at all.
	// these count how many cells away an obstruction is from the cell (not counting cMat as an obstruction)
	// these are used for checking tunneling 
	int cells_right_to_obstruction;
	int cells_left_to_obstruction;
	// these count how many cells away an obstruction is from the cell (counting cMat as an obstruction)
	// these are used for checking gradual grade slope
	int cells_right_to_obstruction_sloping;
	int cells_left_to_obstruction_sloping;
	// these tell us how far away a cell of air is from one cell below our material (how far away, on the row below our material's cell, is the first air cell)
	int cells_right_to_air;
	int cells_left_to_air;
	// these tell us how much material there is underneath our cell that is the same type as cMat before there is some other material.
	// these basically record how long of a stretch of cMat there is under our material.
	int length_of_mat_right;
	int length_of_mat_left;
	// these are true or false valuse that tell us if we have found valid length_of_mat_right and length_of_mat_left variables.
	bool found_length_of_mat_right;
	bool found_length_of_mat_left;
	
	
	
	for(j=y_start+height-1; j>=y_start; j--){ // cycle through the rows
		
		// make sure the y indexes are not out of bounds
		if(j < 0) break;	// the j values only get smaller. quit now.
		else if(j >= GRID_HEIGHT_ELEMENTS) continue; // continue on in search of a smaller j that is within the boundaries
		
		for(i=x_start; i<x_start+width; i++){ // cycle through each column for a single row
			
			// make sure the x indexes are not out of bounds
			if(i < 0) continue; // continue in search of a larger i value
			else if(i >= GRID_WIDTH_ELEMENTS) break;	// the i values only get bigger. quit now.
			
			//if the material is air (no material) then just move past it.
			if(grid[i][j].mat == m_air) continue;
			
			// this resets the holdoff variables if there is a gap between the gravity material
			// this polices the movement of material
			if(grid[i][j].mat == m_air && grid[i][j+1].mat == m_air){
				holdOffLeft = 0;
				holdOffRight = 0;
				continue;
			}
			
			// temporarily store the values of the gravity and material for the current cell
			cMat = grid[i][j].mat;
			currentGrav = mats[cMat].gravity;
			
			//this decrements the holdoff mechanism. it acts as a way to police excessive material motion.
			if(holdOffLeft>0)holdOffLeft--;
			else holdOffLeft = 0;
			if(holdOffRight>0)holdOffRight--;
			else holdOffRight = 0;
			
			// if gravity affects this material
			if(currentGrav){
				
				/// material falls out of the bottom of MAP. not out of the bottom of the screen.
				if(j >= GRID_HEIGHT_ELEMENTS){
					grid[i][j].mat = m_air;
				}
				/// material falls a single cell
				else if(grid[i][j+1].mat == m_air){
					grid[i][j+1].mat = cMat;
					grid[i][j].mat = m_air;
					holdOffLeft = 2; // is this really necessary? jesus. why is this necessary...?
				}
				
				
				/// THE MATERIAL CANNOT FALL DIRECTLY DOWN. IT HAS TO FALL DOWN A SLOPE.
				else{
					
					// the minimum slope that the material can fall down is a 1/1 or steeper
					if(currentGrav > 0){
						if(grid[i+1][j].mat == m_air || grid[i-1][j].mat == m_air){
							// initially set them both to 1. the for() loop will weed out the one(s) that won't work.
							if(grid[i+1][j].mat == m_air) moveRight = 1;
							else moveRight = 0;
							if(grid[i-1][j].mat == m_air) moveLeft  = 1;
							else moveLeft = 0;
							// figure out
							for(jg=1; jg<=currentGrav; jg++){
								// if there is an obstruction in the path of the material falling down the slope, then set one of them to zero and break;
								if(grid[i-1][j+jg].mat != m_air) moveLeft = 0;
								if(grid[i+1][j+jg].mat != m_air) moveRight= 0;
							}
							// this selects either the right or the left direction
							if(moveLeft && moveRight){
								if(get_rand(0,1)) moveLeft = 0;
								else moveRight = 0;
							}
							if(moveLeft){
								grid[i][j].mat = m_air; // remove the material
								
								if(i!=0){
									grid[i-1][j+currentGrav].mat = cMat; // place the new material only if it is in a valid place
								}
							}
							else if(moveRight){
								grid[i][j].mat = m_air; // remove the material
								if(i<GRID_WIDTH-1){
									grid[i+1][j+currentGrav].mat = cMat; // place new material only if it is in a valid place
								}
							}
						}
					} // if( gravity is positive) steep slope
					
					
					else{ /// NEW GRADUAL-GRADED AND TUNNELING GRAVITY.
						
						// gravity is when the material falls down a slope
						// tunneling is defined as the material getting to a new place through material of its kind.
						
						//this treats other cells with the same material as obstruction
						cells_right_to_obstruction = -currentGrav+1;
						cells_left_to_obstruction = -currentGrav+1;
						//this includes materials of the same kind in the definition of 
						cells_right_to_obstruction_sloping = -currentGrav+1;
						cells_left_to_obstruction_sloping = -currentGrav+1;
						
						cells_right_to_air = 0;
						cells_left_to_air = 0;
						// if the material underneath the cell we are evaluating is the same as the material in the cell we are evaluating,
						// then the length of mat is 0 to start. otherwise the length of the material is invalid and is set to -1.
						if(grid[i][j+1].mat == cMat){
							length_of_mat_right = 0;
							length_of_mat_left =  0;
							found_length_of_mat_right = false;
							found_length_of_mat_left = false;
						}
						else{
							length_of_mat_right = -1;
							length_of_mat_left = -1;
							found_length_of_mat_right = true;
							found_length_of_mat_left = true;
						}
						
						
						moveRight = false;
						moveLeft = false;
					
						// generate the figures we need to evaluate gravity.
						for(ig=1; ig<=-currentGrav; ig++){
							
							// get how far it is to the nearest obstruction to the right. only set this if it has not been set before.
							//this set is for tunneling
							if( (cells_right_to_obstruction==-currentGrav+1) && grid[i+ig][j].mat != m_air && grid[i+ig][j].mat != cMat) cells_right_to_obstruction = ig;
							// get how far it is to the nearest obstruction to the  left. only set this if it has not been set before.
							if( (cells_left_to_obstruction==-currentGrav+1)  && grid[i-ig][j].mat != m_air && grid[i-ig][j].mat != cMat )  cells_left_to_obstruction = ig;
							
							// get how far it is to the nearest obstruction to the right. only set this if it has not been set before.
							// this set is for gravity slope testing.
							if( (cells_right_to_obstruction_sloping==-currentGrav+1) && grid[i+ig][j].mat != m_air) cells_right_to_obstruction_sloping = ig;
							// get how far it is to the nearest obstruction to the  left. only set this if it has not been set before.
							if( (cells_left_to_obstruction_sloping==-currentGrav+1)  && grid[i-ig][j].mat != m_air) cells_left_to_obstruction_sloping = ig;
							
							// get how far it is to the nearest air cell to the right. only set this if it has not been set before.
							if( !cells_right_to_air && grid[i+ig][j+1].mat == m_air ) cells_right_to_air = ig;
							// get how far it is to the nearest air cell to the  left. only set this if it has not been set before.
							if( !cells_left_to_air  && grid[i-ig][j+1].mat == m_air )  cells_left_to_air = ig;
							
							// get how far it is to the nearest non-cMat cell to the right. only set this if it has not been set before.
							if( !found_length_of_mat_right && grid[i+ig][j+1].mat != cMat ) { length_of_mat_right = ig-1; found_length_of_mat_right = true; }
							// get how far it is to the nearest non-cMat cell to the  left. only set this if it has not been set before.
							if( !found_length_of_mat_left  && grid[i-ig][j+1].mat != cMat )  { length_of_mat_left = ig-1; found_length_of_mat_left = true; }
							
							//break if all of these values have been found
							if( (cells_right_to_obstruction_sloping != -currentGrav+1) && (cells_left_to_obstruction_sloping != -currentGrav+1) && (cells_right_to_obstruction != -currentGrav+1) && (cells_left_to_obstruction != -currentGrav+1) && cells_right_to_air && cells_left_to_air && length_of_mat_right<0 && length_of_mat_left<0 ) break;
						}//end for looping through the horizontal elements around the material
						/*
						#if ( DEBUG_GRIDSIM )
							printf("\ncells_right_to_obstruction = %d\n", cells_right_to_obstruction);
							printf("cells_left_to_obstruction = %d\n", cells_left_to_obstruction);
							printf("cells_right_to_air = %d\n", cells_right_to_air);
							printf("cells_left_to_air = %d\n", cells_left_to_air);
							printf("length_of_mat_right = %d\n", length_of_mat_right);
							printf("length_of_mat_left = %d\n\n", length_of_mat_left);
						#endif
						*/
						// can the material tunnel to the right?
						if(length_of_mat_right >= 0 && cells_right_to_air - length_of_mat_right == 1 && cells_right_to_obstruction <= cells_right_to_air) moveRight = true;
						// can the material tunnel to the left?
						if(length_of_mat_left  >= 0 &&  cells_left_to_air -  length_of_mat_left == 1 &&  cells_left_to_obstruction <= cells_left_to_air) moveLeft = true;
						// can the material fall down the slope to the right?
						if(cells_right_to_air && cells_right_to_obstruction_sloping > cells_right_to_air) moveRight = true;
						// can the material fall down the slope to the left?
						if(cells_left_to_air && cells_left_to_obstruction_sloping > cells_left_to_air) moveLeft = true;
						
						
						// if both directions of motion are valid. this will ensure that either moveRight or moveLeft will be 1. NOT both.
						if(moveLeft && moveRight){
							// preform a random weighted coin flip to see which the water goes off.
							if( get_rand(1,cells_left_to_air+cells_right_to_air) > cells_left_to_air)
								moveRight = 0;
							else
								moveLeft  = 0;
						}
						
						// the material goes to the right
						if(moveRight && !holdOffRight && i+cells_left_to_air < GRID_WIDTH_ELEMENTS){
							// put current cell data into new cell
							grid[i+cells_right_to_air][j+1].mat = cMat;
							// erase current cell data
							grid[i][j].mat = m_air;
							holdOffRight = -currentGrav;
						}
						// the material goes to the left.
						if(moveLeft && !holdOffLeft && i-cells_left_to_air >= 0){
							// put current cell data into new cell
							grid[i-cells_left_to_air][j+1].mat = cMat;
							// erase current cell data
							grid[i][j].mat = m_air;
							holdOffLeft = -currentGrav;
						}
					} // tunneling and gradual grade slope
				} // if( the material cannot fall straight down )
			} // if( gravity )
		} // for i
	} // for j
	
	
}/// end evaluate_gravity()







