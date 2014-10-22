//--------------------------------------------------------------------------------------------------------------------------------------------------------------

			//if a key was pressed
			if( event.type == SDL_KEYDOWN ){
				//nothing
			}
			//if a key was released
			if( event.type == SDL_KEYUP ){
				//nothing
			}
			
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

		//keeps track of how long you have been pressing the button.
		if(keyup == 1) countup++;
		if(keydown == 1) countdown++;
		if(keyleft == 1) countleft++;
		if(keyright == 1) countright++;

		if(countup >= MAX_COUNT) { y-=20; countup = 0; }
		if(countdown >= MAX_COUNT) { y+=20; countdown = 0; }
		if(countleft >= MAX_COUNT) { x-=20; countleft = 0; }
		if(countright >= MAX_COUNT) { x+=20; countright = 0; }

		//apply white background
		SDL_Rect my_rect;
		my_rect.x = 0;
		my_rect.y = 0;
		my_rect.w = 640;
		my_rect.h = 480;
		SDL_FillRect( screen, &my_rect, 0xffffff );
		SDL_FreeSurface( screen );

		//apply images
		apply_surface(x, y, image, screen ); //apply image
		apply_surface(x + 20, y, image, screen ); //apply image
		apply_surface(x - 20, y, image, screen ); //apply image
		apply_surface(x, y + 20, image, screen ); //apply image
		apply_surface(x, y - 20, image, screen ); //apply image
		// flip screen
		SDL_Flip( screen ); // update screen
		//SDL_FreeSurface( screen ); // saves memory
		
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/// OLD GRAVITY
/*
	/// 1. GRAVITY - this is where the gravity gets CHECKED and APPLIED.
	for(j=GRID_HEIGHT-1 ; j>=0 ; j--){
		for(i=0 ; i<GRID_WIDTH ; i++){

			// if gravity affects this material...
			if(mats[grid[i][j].mat].gravity){
				// dissapears through the bottom
				if(j >= GRID_HEIGHT-1){ // if the gravity material is at the bottom of the screen, get rid of it.
					grid[i][j].mat = m_air;
				}
				// falls down
				else if(grid[i][j+1].mat == m_air || grid[i][j+1].mat == m_air){
					grid[i][j+1].mat = grid[i][j].mat;
					grid[i][j].mat = m_air;
				}
				// if the material could EITHER fall to the LEFT or the RIGHT
				else if( i>0 && grid[i-1][j].mat == m_air && grid[i-1][j+1].mat == m_air   &&   i<GRID_WIDTH-1 && grid[i+1][j].mat == m_air && grid[i+1][j+1].mat == m_air ){
					// randomly choose whether to...
					if(get_rand(0,1)){
						grid[i-1][j+1].mat = grid[i][j].mat; // go to the left or...
						grid[i][j].mat = m_air;
					}
					else{
						grid[i+1][j+1].mat = grid[i][j].mat; // go to the right.
						grid[i][j].mat = m_air;
					}
				}
				// if the material can ONLY fall to the LEFT
				else if( i>0 && grid[i-1][j].mat == m_air && grid[i-1][j+1].mat == m_air){
					grid[i-1][j+1].mat = grid[i][j].mat; // go to the left or...
					grid[i][j].mat = m_air;
				}
				//if the material can ONLY fall the the RIGHT
				else if( i <GRID_WIDTH-1 && grid[i+1][j].mat == m_air && grid[i+1][j+1].mat == m_air){
					grid[i+1][j+1].mat = grid[i][j].mat; // go to the right.
					grid[i][j].mat = m_air;
				}
			} // if gravity
		} // for columns
	} // for rows
*/
	
/// LESS OLD GRAVITY

/*
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
						grid[i-1][j+currentGrav].sat = currentSat;
					}
					grid[i][j].sat = m_no_saturation; // remove saturation
				}
				else if(moveRight){
					grid[i][j].mat = m_air; // remove the material
					if(i<GRID_WIDTH-1){
						grid[i+1][j+currentGrav].mat = cMat; // place new material only if it is in a valid place
						grid[i+1][j+currentGrav].sat = currentSat;
					}
					grid[i][j].sat = m_no_saturation;
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
			
			//#if ( DEBUG_GRIDSIM )
			//	printf("\ncells_right_to_obstruction = %d\n", cells_right_to_obstruction);
			//	printf("cells_left_to_obstruction = %d\n", cells_left_to_obstruction);
			//	printf("cells_right_to_air = %d\n", cells_right_to_air);
			//	printf("cells_left_to_air = %d\n", cells_left_to_air);
			//	printf("length_of_mat_right = %d\n", length_of_mat_right);
			//	printf("length_of_mat_left = %d\n\n", length_of_mat_left);
			//#endif
			
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
			if(moveRight && !holdOffRight){
				// put current cell data into new cell
				grid[i+cells_right_to_air][j+1].mat = cMat;
				grid[i+cells_right_to_air][j+1].sat = currentSat;
				// erase current cell data
				grid[i][j].mat = m_air;
				grid[i][j].sat = m_no_saturation;
				holdOffRight = -currentGrav;
			}
			// the material goes to the left.
			if(moveLeft && !holdOffLeft){
				// put current cell data into new cell
				grid[i-cells_left_to_air][j+1].mat = cMat;
				grid[i-cells_left_to_air][j+1].sat = currentSat;
				// erase current cell data
				grid[i][j].mat = m_air;
				grid[i][j].sat = m_no_saturation;
				holdOffLeft = -currentGrav;
			}
		} // tunneling and gradual grade slope
		
		
		
	} // if( the material cannot fall straight down )
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/// FIRST ATTEMPT AT TUNNELING MATERIALS

/* OLD GRADED GRAVITY
					// the minimum slope that the material can fall down is a 1/1 or less steep
					else{
						for(ig=1; (-ig>=currentGrav); ig++){
							// if the material can fall to the right and has not yet been shown to be an invalid path
							if( (i+ig<GRID_WIDTH) && (grid[i+ig+camera_x][j+camera_y].mat==m_air)  &&  !(invalidSlope&SLOPE_RIGHT) ){
								if(grid[i+ig+camera_x][j+1+camera_y].mat == m_air)
									validSlope |= SLOPE_RIGHT;
							}else invalidSlope |= SLOPE_RIGHT;
							
							// if the material can fall to the left and has not yet been shown to be an invalid path
							if( !(holdOffLeft) && (i-ig>=0) && (grid[i-ig+camera_x][j+camera_y].mat==m_air)  &&  !(invalidSlope&SLOPE_LEFT) ){
								if(grid[i-ig+camera_x][j+1+camera_y].mat == m_air)
									validSlope |= SLOPE_LEFT;
							}else invalidSlope |= SLOPE_LEFT;
							
							if(invalidSlope == SLOPE_EITHER){		//if the slope is invalid in both directions, quit trying to evaluate the slope.
								break;
							}
							if(validSlope == SLOPE_RIGHT){				//if the material has a valid slope on the right
								grid[i+camera_x][j+camera_y].mat = m_air; 				// remove the material from its current location
								grid[i+ig+camera_x][j+1+camera_y].mat = currentMat; 		// place the material in its new location to the right
								grid[i+ig+camera_x][j+1+camera_y].sat = currentSat;	//copy saturation into new place
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
								break;
							}
							else if(validSlope == SLOPE_LEFT){		//if the material has a valid slope on the left
								grid[i+camera_x][j+camera_y].mat = m_air; 				// remove the material from its current location
								grid[i-ig+camera_x][j+1+camera_y].mat = currentMat; 		// place the material in its new location to the left
								grid[i-ig+camera_x][j+1+camera_y].sat = currentSat;	//copy saturation into new place
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
								holdOffLeft=currentGrav;
								break;
							}
							else if(validSlope == SLOPE_EITHER){	//if the material has a valid slope on either side
								if(get_rand(0,1)){					// goes to the right
									grid[i+camera_x][j+camera_y].mat = m_air;
									grid[i+ig+camera_x][j+1+camera_y].mat = currentMat;
									grid[i+ig+camera_x][j+1+camera_y].sat = currentSat;	//copy saturation into new place
									grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
									break;
								}
								else{								// goes to the left
									grid[i+camera_x][j+camera_y].mat = m_air;
									grid[i-ig+camera_x][j+1+camera_y].mat = currentMat;
									grid[i-ig+camera_x][j+1+camera_y].sat = currentSat;	//copy saturation into new place
									grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
									holdOffLeft=currentGrav;
									break;
								}
							}
						}
					}
				else{ // if all else fails, try to TUNNEL
					// if the material has not declined, and if the material one cell down is the same as the one being evaluated...
					if(currentGrav < 0 && grid[i+camera_x][j+1+camera_y].mat == currentMat){
						
						//these valuse will be overwritten as the cell is evaluated.
						validTunnel = TUNNEL_NONE;
						invalidTunnel = TUNNEL_NONE;
						foundObstacleRight = false;
						foundObstacleLeft  = false;
						
						for(ig=1; ig<=-currentGrav; ig++){
							
							// check to see if there valid tunnel RIGHT
							if( (invalidTunnel&TUNNEL_RIGHT) == 0 && grid[i+camera_x+ig][j+camera_y+1].mat == m_air) validTunnel |= TUNNEL_RIGHT;
							// if there is no where to tunnel yet, check to see if there is an invalid material blocking the way.
							else if( (validTunnel&TUNNEL_RIGHT) == 0 && grid[i+camera_x+ig][j+camera_y+1].mat != currentMat) invalidTunnel |= TUNNEL_RIGHT;
							//check to see if there is an obstacle to the right
							
							//check to see if there is any valid obstruction to the right
							if(grid[i+camera_x+ig][j+camera_y].mat != m_air) foundObstacleRight = true;
							
							// check to see if there valid tunnel LEFT
							if( (invalidTunnel&TUNNEL_LEFT) == 0 && grid[i+camera_x-ig][j+camera_y+1].mat == m_air) validTunnel |= TUNNEL_LEFT;
							// if there is no where to tunnel yet, check to see if there is an invalid material blocking the way.
							else if( (validTunnel&TUNNEL_LEFT) == 0 && grid[i+camera_x-ig][j+camera_y+1].mat != currentMat) invalidTunnel |= TUNNEL_LEFT;
							
							//check to see if there is any valid obstruction to the left
							if(grid[i+camera_x-ig][j+camera_y].mat != m_air) foundObstacleLeft = true;
							
							
							// if there is no valid tunnel, quit
							if( invalidTunnel == TUNNEL_EITHER) break;
							
							// if the material can tunnel left or right.
							if(validTunnel == TUNNEL_EITHER && foundObstacleLeft && foundObstacleRight){
								//decide to go left or right.
								if(get_rand(0,1)) ig *= -1;
								grid[i+camera_x+ig][j+camera_y+1].mat = currentMat;
								grid[i+camera_x+ig][j+camera_y+1].sat = currentSat;
								// erase old cell data
								grid[i+camera_x][j+camera_y].mat = m_air;
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;
								break;
								
								break;
							}
							else if(validTunnel == TUNNEL_RIGHT && foundObstacleRight){
								// overwrite new cell data
								grid[i+camera_x+ig][j+camera_y+1].mat = currentMat;
								grid[i+camera_x+ig][j+camera_y+1].sat = currentSat;
								// erase old cell data
								grid[i+camera_x][j+camera_y].mat = m_air;
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;
								break;
							}
							else if(validTunnel == TUNNEL_LEFT && foundObstacleLeft){
								// overwrite new cell data
								grid[i+camera_x-ig][j+camera_y+1].mat = currentMat;
								grid[i+camera_x-ig][j+camera_y+1].sat = currentSat;
								// erase old cell data
								grid[i+camera_x][j+camera_y].mat = m_air;
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;
								break;
							}
							
						} // for (check for tunneling)
					} // if tunneling if possible
				} // if all else fails, try to tunnel
				*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/// OLD PANNING
void pan(int direction){
	switch(direction){
	case D_UP:
		camera_y -= MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	case D_LEFT:
		camera_x -= MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	case D_DOWN:
		camera_y += MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	case D_RIGHT:
		camera_x += MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	}
	verify_camera();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
