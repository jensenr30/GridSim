/// this document contains the necessary functions to evaluate and advance the simulation.
/// It looks at the materials in the cells and then updates the cells based on the dynamics of the materials described in materials_and_cells.h

//used for temporarily storing changes to the cellMat[][] and cellSat[][] arrays.

void apply_grid_changes();
bool *generate_near_by_cell_test_vector(unsigned short);
void evaluate_affectMaterial(unsigned short, unsigned short, struct affectMaterial *);
void reset_grid_changes();

//these are definitions for use with the gravity evaluation code in part 1 of the evaluate grid function.
#define SLOPE_NONE   	0
#define SLOPE_RIGHT  	1
#define SLOPE_LEFT   	2
#define SLOPE_EITHER 	3

#define TUNNEL_NONE 	0
#define TUNNEL_RIGHT 	1
#define TUNNEL_LEFT  	2
#define TUNNEL_EITHER 	3


// this will evaluate the grid. It will run a simulation for the number of generations you tell it to.
void evaluate_grid(){
	
	/// 1. GRAVITY
	//everything having to do with gravity is evaluated here and now.
	
	/////////////////// VARIABLES USED IN GRAVITY AND OTHER SUBSEQUENT PORTIONS OF THE CODE ///////////////////
	
	// i and j are used to index through the current view of the grid.
	short int i,j,a,c; // i = x position in grid. j = y position in grid. k = material type being evaluated. a = type of material being affected. c = which cell around the material is being checked

	// used to temporarily store the gravity value of the cell's material
	int cMat;
	// used to temporarily store the gravity value of the cell material's gravity
	char currentGrav;
	// used to temporarily store the gravity value of the cell's saturation
	int currentSat;
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
	
	
	
	for(j=GRID_HEIGHT+camera_y-1; j>=camera_y; j--){ // cycle through the rows
		// make sure the indexes are not out of bounds
		if(j < 0){
			#if(debug)
				printf("function evaluate_grid():\nError: j is outside of bounds.\nj = %d\n\n",j);
			#endif
			j = 0;
		}
		else if(j >= GRID_HEIGHT_ELEMENTS){
			#if(debug)
				printf("function evaluate_grid():\nError: j is outside of bounds.\nj = %d\n\n",j);
			#endif
			break;
		}
		for(i=camera_x; i<GRID_WIDTH+camera_x; i++){ // cycle through each column for a single row
			
			// make sure the indexes are not out of bounds
			if(i < 0){
				#if(debug)
					printf("function evaluate_grid():\nError: i is outside of bounds.\ni = %d\n\n",i);
				#endif
				i = 0;
			}
			else if(i >= GRID_WIDTH_ELEMENTS){
				#if(debug)
					printf("function evaluate_grid():\nError: i is outside of bounds.\ni = %d\n\n",i);
				#endif
				break;
			}
			
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
			currentSat = grid[i][j].sat;
			
			//this decrements the holdoff mechanism. it acts as a way to police excessive material motion.
			if(holdOffLeft>0)holdOffLeft--;
			else holdOffLeft = 0;
			if(holdOffRight>0)holdOffRight--;
			else holdOffRight = 0;
			
			// if gravity affects this material
			if(currentGrav){
				
				/// material falls out of the bottom of the screen
				if(j >= GRID_HEIGHT+camera_y-1){
					grid[i][j].mat = m_air;
					grid[i][j].sat = m_no_saturation; // remove saturaiton of material once it falls out of the screen.
				}
				/// material falls a single cell
				else if(grid[i][j+1].mat == m_air){
					grid[i][j+1].mat = cMat;
					grid[i][j].mat = m_air;
					//transfer saturation
					grid[i][j+1].sat = currentSat;
					grid[i][j].sat = m_no_saturation;
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
						/*
						#if ( debug )
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
			} // if( gravity )
		} // for i
	} // for j
	
	reset_grid_changes();
	
	
	///variables used in 2. APPLY SATURATION
	// cMat		  :	the current material that we are trying to see if it gets saturated.
	// matIndex	  :	the thing used to matIndex into the matSatOrder array to get a correct material to store int cMat.
	// satEffIndex:	the index for going throught
	// newi, newj :	the coordinates of the cells around the grid[i][j].mat being evaluated.
	// c		  :	used to indicate which cell around our cell is being evaluated.
	//short cMat; // declared earlier
	unsigned short satEffIndex, newi, newj;
	bool firstEncounter;
	
	/// 2.1 EVALUATE SATURATION - the giant ass loop where the saturation is evaluated
	for(i=camera_x ; i<GRID_WIDTH+camera_x && i<GRID_WIDTH_ELEMENTS ; i++){
		for(j=camera_y ; j<GRID_HEIGHT+camera_y && j<GRID_HEIGHT_ELEMENTS; j++){
			
			cMat = grid[i][j].mat; // get correct material
			
			//go through each saturation effect of the material
			for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATIONS ; satEffIndex++){
				
				// if there is no effect, skip ahead to the next saturation effect.
				if(mats[cMat].satEffect[satEffIndex].satMat == m_no_saturation) continue;
				
				//make sure the current cell has the current material, cMat, in it!! if not, continue;
				if(cMat != grid[i][j].mat) continue;
				
				//check for saturation memory
				if(mats[cMat].satEffect[satEffIndex].satMem == false){ // if there is no saturation memory
					// if there is no memory, evaluate the satuation level each time.
					grid[i][j].satLevelChange = 0;
					// it needs to be saturated each cycle of evaluate_grid() when there is no memory.
					grid[i][j].satChange = m_no_saturation;
				}
				/*
				else{ // if there is saturation memory
					// there is no change in the saturation level.
					grid[i][j].satLevelChange = m_no_change;
					// there is no change in the saturation
					grid[i][j].satChange = m_no_change;
				}
				*/
				
				//set this so that you know you have not encountered any other saturating materials around the material you are evaluating
				firstEncounter = 1;
					
				//for every cell around our cell [i][j], evaluate whether or not it gets saturated
				//  	0 1 2
				//  	3 M 4
				//  	5 6 7
				for(c=0 ; c<8 ; c++){
					//get correct newi and newj values to plug into grid[newi][newj].mat
					switch(c){
					case 0: newi = i-1;	newj = j-1;
						break;
					case 1: newi = i;	newj = j-1;
						break;
					case 2: newi = i+1;	newj = j-1;
						break;
					case 3: newi = i-1;	newj = j;
						break;
					case 4: newi = i+1;	newj = j;
						break;
					case 5: newi = i-1;	newj = j+1;
						break;
					case 6: newi = i;	newj = j+1; 
						break;
					case 7: newi = i+1;	newj = j+1;
						break;
					}
					// if newi and newj are in UNACCEPTABLE places, continue to the next neighbor cell
					if(newi < camera_x || newi >= GRID_WIDTH+camera_x || newj < camera_y || newj >= GRID_HEIGHT+camera_y) continue;
				
					// if the material near this cell is the right type to saturate it
					if(grid[newi][newj].mat == mats[cMat].satEffect[satEffIndex].satMat){
						
						if(roll_ht(mats[cMat].satEffect[satEffIndex].chance[c])){ // determine if it will become saturated based on roll_ht function.
							grid[i][j].satChange = mats[cMat].satEffect[satEffIndex].satMat;
							// absorbs the material if required. it only absorbs if it isn't already saturated.
							if(grid[i][j].sat != grid[newi][newj].mat && mats[cMat].satEffect[satEffIndex].absorb) grid[newi][newj].matChange = m_air;
						}
						// increment the satLevel if needed.
						if(firstEncounter){
							grid[i][j].satLevelChange = 1;
							firstEncounter = 0;
							}
						else
							grid[i][j].satLevelChange++;
					}
				}
			}
		}
	}
	
	apply_grid_changes(); // apply changes from the SATURATION APPLY part of this function.
	
	
	
	/// 2.2 SATURATION DECAY
	for(i=camera_x ; i<GRID_WIDTH+camera_x && i<GRID_WIDTH_ELEMENTS ; i++){
		for(j=camera_y ; j<GRID_HEIGHT+camera_y && j<GRID_HEIGHT_ELEMENTS; j++){
			if(grid[i][j].sat != m_no_saturation){ // if there is a valid saturation here
				//store current material here for convenience
				cMat = grid[i][j].mat;
				for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATIONS ; satEffIndex++){
					if(mats[cMat].satEffect[satEffIndex].satMat == grid[i][j].sat){ // if this is the right saturation
						//check for the right saturaion levels. if you don't have the right sat levels, just move on to the next satEffect. (continue)
						if(grid[i][j].satLevel < mats[cMat].satEffect[satEffIndex].decaySatGTE || grid[i][j].satLevel > mats[cMat].satEffect[satEffIndex].decaySatLTE) continue;
						
						// roll for saturation-initiated decay 
						if(roll_ht( mats[cMat].satEffect[satEffIndex].decayChance )){
							grid[i][j].matChange = mats[cMat].satEffect[satEffIndex].decayIntoMat;
							grid[i][j].satChange = mats[cMat].satEffect[satEffIndex].decayIntoSat;
						}
					}
				}
			}
		}
	}
	apply_grid_changes(); // apply changes from the SATURATION AFFECTS AND DECAY part of this function.
	
	
	
	/// 3. AFFECTS AND DECAY - this giant-ass for loop is where we find out which cells need to be changed.
	for(i=camera_x ; i<GRID_WIDTH+camera_x && i<GRID_WIDTH_ELEMENTS ; i++){
		for(j=camera_y ; j<GRID_HEIGHT+camera_y && j<GRID_HEIGHT_ELEMENTS; j++){
			// air doesn't do anything. that is it's definition.
			if(grid[i][j].mat == m_air) continue;
			for(a=0 ; a<MAX_NUMBER_OF_MATERIAL_INTERACTIONS; a++){ // check all the possible interactions
				
				//evaluate the affectMaterial structure (this will apply correct changes to the cellMat array)
				evaluate_affectMaterial(i, j, &mats[grid[i][j].mat].affectMat[a] );
				
			}
			// check for decay.
			if(roll_ht( mats[ grid[i][j].mat ].decayChance) ) grid[i][j].matChange = mats[ grid[i][j].mat ].decayIntoMat; // if, by chance, it is time to decay, then decay into your proper type.
		}
	}
	apply_grid_changes(); // apply changes from the AFFECTS AND DECAY and decay part of this function.
	
}/// end evaluate_grid()





void reset_grid_changes(){
	int i,j;
	//reset cellMatChanges and cellSatChanges
	for(i=camera_x ; i<GRID_WIDTH+camera_x && i<GRID_WIDTH_ELEMENTS ; i++){
		for(j=camera_y ; j<GRID_HEIGHT+camera_y && j<GRID_HEIGHT_ELEMENTS; j++){
			grid[i][j].matChange = m_no_change;
			grid[i][j].satChange = m_no_change;
			grid[i][j].satLevelChange = m_no_change;
		}
	}
}




void apply_grid_changes(){
	int i,j;
	for(i=camera_x ; i<GRID_WIDTH+camera_x && i<GRID_WIDTH_ELEMENTS ; i++){
		for(j=camera_y ; j<GRID_HEIGHT+camera_y && j<GRID_HEIGHT_ELEMENTS; j++){
			//if the material at [i][j] needs to be changed (updated) then change it
			if(grid[i][j].matChange != m_no_change) grid[i][j].mat = grid[i][j].matChange;
			//if the saturation at [i][j] needs to be changed (updated) then change it
			if(grid[i][j].satChange != m_no_change) grid[i][j].sat = grid[i][j].satChange;
			//if the saturation level at [i][j] needs to be changed (updated) then change it.
			if(grid[i][j].satLevelChange != m_no_change) grid[i][j].satLevel = grid[i][j].satLevelChange;
			//reset grid changes
			grid[i][j].matChange = m_no_change;
			grid[i][j].satChange = m_no_change;
			grid[i][j].satLevelChange = m_no_change;
		}
	}
}











void evaluate_affectMaterial(unsigned short i, unsigned short j, struct affectMaterial *affMat){
	
	// if the affectMaterial will not do anything, return;
	if(affMat->matAfter == m_no_change && affMat->satAfter == m_no_change && affMat->changeOrigSat == m_no_change && affMat->changeOrigMat == m_no_change) return;
	
	static int newi,newj;
	// c is used to increment through all either chance[] elements.
	//c is also used to check if a material is saturated by any of its saturations
	static short c;
	static bool validSat;
	
	static bool *testVector;
	
	// if there aren't any blocks to be changed, then move on to the next one.
	if(affMat->changesPerEval == 0) return;
	
	//generate a testvector for the cells around the cell being evaluated.
	testVector = generate_near_by_cell_test_vector(affMat->changesPerEval);
	
	//these checks inside this if() statements only apply if the saturation needed is a valid material saturation. i.e. not one of these flags.
	switch(affMat->satNeeded){
	case m_dont_care:
		break;
	case m_no_saturation:
		if(grid[i][j].sat != m_no_saturation) return;
		break;
	case m_any_of_my_sats:
		validSat = false; // by default, we have not yet detected a valid saturation.
		for(c=0 ; c<MAX_NUMBER_OF_SATURATIONS ; c++){
			if(mats[grid[i][j].mat].satEffect[c].satMat == m_no_saturation) // if you have reached an null satEffect...
				break; // ...break out of the for(c) loop
				
			// if there is a valid saturation, set validSat true and break out of the for(c) loop.
			if(grid[i][j].sat == mats[grid[i][j].mat].satEffect[c].satMat){
					validSat = true;
				break;
			}
		}
		if(validSat == false) return; // false condition. if this material is not saturated with one of its valid saturations, then quit this affectMat.
		//if our material isn't saturated enough, return
		if(grid[i][j].satLevel < affMat->satGTE) return;
		//if our material is too saturated, return
		if(grid[i][j].satLevel > affMat->satLTE) return;
		break;
	default: // default conditions. there is a single saturation we are looking for. and we have to be the saturation range.
		//if the saturation here isn't right, return
		if(grid[i][j].sat != affMat->satNeeded) return;
		//if our material isn't saturated enough, return
		if(grid[i][j].satLevel < affMat->satGTE) return;
		//if our material is too saturated, return
		if(grid[i][j].satLevel > affMat->satLTE) return;
		break;
	}
	
	//for each chance
	for(c=0 ; c<8 ; c++){
		
		// if test vector is true
		if(testVector[c] == true){
			
			//generate correct test coordinates. newi and newj are used to index into a neighboring cell and get information about it.
			// newi and newj are used in the myriad of if() statements below...
			switch(c){
			case 0: newi = i-1;	newj = j-1;
				break;
			case 1: newi = i;	newj = j-1;
				break;
			case 2: newi = i+1;	newj = j-1;
				break;
			case 3: newi = i-1;	newj = j;
				break;
			case 4: newi = i+1;	newj = j;
				break;
			case 5: newi = i-1;	newj = j+1;
				break;
			case 6: newi = i;	newj = j+1;
				break;
			case 7: newi = i+1;	newj = j+1;
				break;
			}
			// if newi and newj are in UNACCEPTABLE places, continue to the next neighboring cell
			if(newi < 0 || newi >= GRID_WIDTH_ELEMENTS || newj < 0 || newj >= GRID_HEIGHT_ELEMENTS) continue;
		
			// if there is a valid material or if you can use any material
			if( affMat->matBefore == grid[newi][newj].mat || affMat->matBefore == m_dont_care){
				// if there is a valid saturation or if you can use any saturation
				if( affMat->satBefore == grid[newi][newj].sat || affMat->satBefore == m_dont_care){
					// rolling
					if(roll_ht(affMat->chance[c])){
						// change the material only if it needs changing.
						if(affMat->matAfter != m_no_change) grid[newi][newj].matChange = affMat->matAfter;
						// change the saturation only if it needs changing. also change the saturation level to a default of 1
						if(affMat->satAfter != m_no_change) {
								grid[newi][newj].satChange = affMat->satAfter;
								grid[newi][newj].satLevelChange = 1;
						}
						
						// check to see if the original material will change because of it having completed an affectMat
						if(affMat->changeOrigMat != m_no_change) 			// if the material changes after it affects neighboring cells
							grid[i][j].matChange = affMat->changeOrigMat; 		// change the material
						if(affMat->changeOrigSat != m_no_change) 			// if the saturation of our material changes after our material affects neighboring cells
							grid[i][j].satChange = affMat->changeOrigSat; 		// change the saturation of our material.
					}
				}
			}
		}
	}
}








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
    for(i = 0; i < GRID_WIDTH - wPos/CELL_SIZE && i+camera_x<GRID_WIDTH_ELEMENTS; i++){
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













// this will return a pointer to an array that has picks out "changesPerEval" of random elements in the testVector array.
// they correspond to the materials in the cells around our cell that will be checked.
bool *generate_near_by_cell_test_vector(unsigned short changesPerEval){
	static bool testVector[8];
	unsigned short emelentsChanged = 0; // this holds how many cells have been changed from 0's into 1's.
	unsigned short temp;
	
	testVector[0] = testVector[1] = testVector[2] = testVector[3] = testVector[4] = testVector[5] = testVector[6] = testVector[7] = false; // set to 0.
	
	if(changesPerEval == 0)
	{
		return testVector;
	}
	else if(changesPerEval == 8){
		testVector[0] = testVector[1] = testVector[2] = testVector[3] = testVector[4] = testVector[5] = testVector[6] = testVector[7] = true;
	}
	else{ // 1,2,3,4,5,6,7
		while(emelentsChanged < changesPerEval){ // while the number of entries in the textVector[] array are less than how many we need, try to add more to the array.
			temp = get_rand(0,7);
			if( testVector[temp] == false){
				testVector[temp] = true;
				emelentsChanged++;
			}
		}
	}
	return testVector;
}

