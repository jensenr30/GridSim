/// this document contains the necessary functions to evaluate and advance the simulation.
/// It looks at the materials in the cells and then updates the cells based on the dynamics of the materials described in materials_and_cells.h

//used for temporarily storing changes to the cellMat[][] and cellSat[][] arrays.

void apply_grid_changes();
bool *generate_near_by_cell_test_vector(unsigned short);
void evaluate_affectMaterial(unsigned short, unsigned short, struct affectMaterial *);
void reset_grid_changes();

//these are definitions for use with the gravity evaluation code in part 1 of the evaluate grid function.
#define SLOPE_NONE   0
#define SLOPE_RIGHT  1
#define SLOPE_LEFT   2
#define SLOPE_EITHER 3


// this will evaluate the grid. It will run a simulation for the number of generations you tell it to.
void evaluate_grid(){
	/// 0. VARIABLES - this is where all the variables used heavily in the loops are declared.
	
	///variables used in 1. GRAVITY, and 3. AFFECT & DECAY.
	short int i,j,a,c; // i = x position in grid. j = y position in grid. k = material type being evaluated. a = type of material being affected. c = which cell around the material is being checked

	/* this array is used to log the changes that will need to be made to the cells in the grid.
	 * I apply the changes that need to be made ONLY AFTER I have evaluated all the cells in the grid.
	 * I do this so that the evaluation is not partial to things that are evaluated earlier than others.
	 * i.e. if two materials destroy each other when they meet, then if one of them is evaluated first AND the effects it produces are applied to cellMat right away, then the other material will not be able to be evaluated and thus the two materials will not destroy each other as was intended.
	 * Using an array to log the changes that need to be made to the cells in the grid is a pretty good way of going abou this problem I think.
	 */
	
	
	
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
	
	
	/* variables used in Gravity:
	 * j			: used to index through the rows
	 * i			: used to index through the columns
	 * jg			: used to index through the rows when checking positive gravity (slopes with magnitude greater than 1)
	 * ig			: used to index through the columns when checking negtive gravity (slopes with magnitude less than 1)
	 * matGrav		: used to temporarily store the gravity value of the material
	 * validSlope	: used to determine if the slope is valid.
					:	SLOPE_NONE   = 0 = no slope to fall down.
					:	SLOPE_RIGHT  = 1 = the material can fall down the right side of the slope
					:	SLOPE_LEFT   = 2 = the material can fall down the  left side of the slope
					:	SLOPE_EITHER = 3 = the material can fall down    either side of the slope ( this is SLOPE_RIGHT | SLOPE_LEFT ) the bitwise OR operator.
	 * holdOff		: this is used to regulate the falling of materials that have a negative gravity (materials that fall down gradual slopes)
	 * moveRight	: this is used as a flag for positive gravity.
	 * moveLeft		:                   ''
	*/
	int currentMat;
	char currentGrav;
	char jg, ig;
	char validSlope;
	char invalidSlope;
	char holdOff = 0;
	char moveRight;
	char moveLeft;
	
	/// 1. NEW GRAVITY
	for(j=GRID_HEIGHT-1; j>=0; j--){ // cycle through the rows
		for(i=0; i<GRID_WIDTH; i++){ // cycle through each column for a single row
			
			//this resets the holdOff if there is a gap between the gravity material
			if(grid[i+camera_x][j+camera_y].mat == m_air && grid[i+camera_x][j+1+camera_y].mat == m_air){
				holdOff = 0;
				continue;
			}
			
			// temporarily store the values of the gravity and material for the current cell
			currentMat = grid[i+camera_x][j+camera_y].mat;
			currentGrav = mats[currentMat].gravity;
			
			//this decrements the holdOff mechanism. it acts as a way to police excessive material motion.
			if(holdOff)holdOff--;
			
			// if gravity affects this material
			if(currentGrav){
				
				// material falls out of the bottom of the screen
				if(j >= GRID_HEIGHT-1){
					grid[i+camera_x][j+camera_y].mat = m_air;
					grid[i+camera_x][j+camera_y].sat = m_no_saturation; // remove saturaiton of material once it falls out of the screen.
				}
				// material falls a single cell
				else if(grid[i+camera_x][j+1+camera_y].mat == m_air){
					grid[i+camera_x][j+1+camera_y].mat = currentMat;
					grid[i+camera_x][j+camera_y].mat = m_air;
					//transfer saturation
					grid[i+camera_x][j+1+camera_y].sat = grid[i+camera_x][j+camera_y].sat;
					grid[i+camera_x][j+camera_y].sat = m_no_saturation;
					holdOff = 2;
				}
				// the material cannot fall directly down. it has to fall down a slope.
				else if(grid[i+1+camera_x][j+camera_y].mat == m_air || grid[i-1+camera_x][j+camera_y].mat == m_air){
					// set the validity of the slope to none as default.
					validSlope = SLOPE_NONE;
					invalidSlope = SLOPE_NONE;
					// the minimum slope that the material can fall down is a 1/1 or steeper
					if(currentGrav > 0){
						// initially set them both to 1. the for() loop will weed out the one(s) that won't work.
						if(grid[i+1+camera_x][j+camera_y].mat == m_air) moveRight = 1;
						else moveRight = 0;
						if(grid[i-1+camera_x][j+camera_y].mat == m_air) moveLeft  = 1;
						else moveLeft = 0;
						// figure out
						for(jg=1; jg<=currentGrav; jg++){
							// if there is an obstruction in the path of the material falling down the slope, then set one of them to zero and break;
							if(grid[i-1+camera_x][j+jg+camera_y].mat != m_air) moveLeft = 0;
							if(grid[i+1+camera_x][j+jg+camera_y].mat != m_air) moveRight= 0;
						}
						// this selects either the right or the left direction
						if(moveLeft && moveRight){
							if(get_rand(0,1)) moveLeft = 0;
							else moveRight = 0;
						}
						if(moveLeft){
							grid[i+camera_x][j+camera_y].mat = m_air; // remove the material
							
							if(i!=0){
								grid[i-1+camera_x][j+currentGrav+camera_y].mat = currentMat; // place the new material only if it is in a valid place
								grid[i-1+camera_x][j+currentGrav+camera_y].sat = grid[i+camera_x][j+camera_y].sat;
							}
							grid[i+camera_x][j+camera_y].sat = m_no_saturation; // remove saturation
						}
						else if(moveRight){
							grid[i+camera_x][j+camera_y].mat = m_air; // remove the material
							if(i<GRID_WIDTH-1){
								grid[i+1+camera_x][j+currentGrav+camera_y].mat = currentMat; // place new material only if it is in a valid place
								grid[i+1+camera_x][j+currentGrav+camera_y].sat = grid[i+camera_x][j+camera_y].sat;
							}
							grid[i+camera_x][j+camera_y].sat = m_no_saturation;
						}
					}
					// the minimum slope that the material can fall down is a 1/1 or less steeper
					else{
						for(ig=1; (-ig>=currentGrav); ig++){
							// if the material can fall to the right and has not yet been shown to be an invalid path
							if( (i+ig<GRID_WIDTH) && (grid[i+ig+camera_x][j+camera_y].mat==m_air)  &&  !(invalidSlope&SLOPE_RIGHT) ){
								if(grid[i+ig+camera_x][j+1+camera_y].mat == m_air)
									validSlope |= SLOPE_RIGHT;
							}
							else invalidSlope |= SLOPE_RIGHT;
							// if the material can fall to the left and has not yet been shown to be an invalid path
							if( !(holdOff) && (i-ig>=0) && (grid[i-ig+camera_x][j+camera_y].mat==m_air)  &&  !(invalidSlope&SLOPE_LEFT) ){
								if(grid[i-ig+camera_x][j+1+camera_y].mat == m_air)
									validSlope |= SLOPE_LEFT;
							}
							else invalidSlope |= SLOPE_LEFT;
							if(invalidSlope == SLOPE_EITHER){		//if the slope is invalid in both directions, quit trying to evaluate the slope.
								break;
							}
							if(validSlope == SLOPE_RIGHT){				//if the material has a valid slope on the right
								grid[i+camera_x][j+camera_y].mat = m_air; 				// remove the material from its current location
								grid[i+ig+camera_x][j+1+camera_y].mat = currentMat; 		// place the material in its new location to the right
								grid[i+ig+camera_x][j+1+camera_y].sat = grid[i+camera_x][j+camera_y].sat;	//copy saturation into new place
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
								break;
							}
							else if(validSlope == SLOPE_LEFT){		//if the material has a valid slope on the left
								grid[i+camera_x][j+camera_y].mat = m_air; 				// remove the material from its current location
								grid[i-ig+camera_x][j+1+camera_y].mat = currentMat; 		// place the material in its new location to the left
								grid[i-ig+camera_x][j+1+camera_y].sat = grid[i+camera_x][j+camera_y].sat;	//copy saturation into new place
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
								holdOff=currentGrav;
								break;
							}
							else if(validSlope == SLOPE_EITHER){	//if the material has a valid slope on either side
								if(get_rand(0,1)){					// goes to the right
									grid[i+camera_x][j+camera_y].mat = m_air;
									grid[i+ig+camera_x][j+1+camera_y].mat = currentMat;
									grid[i+ig+camera_x][j+1+camera_y].sat = grid[i+camera_x][j+camera_y].sat;	//copy saturation into new place
									grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
									break;
								}
								else{								// goes to the left
									grid[i+camera_x][j+camera_y].mat = m_air;
									grid[i-ig+camera_x][j+1+camera_y].mat = currentMat;
									grid[i-ig+camera_x][j+1+camera_y].sat = grid[i+camera_x][j+camera_y].sat;	//copy saturation into new place
									grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
									holdOff=currentGrav;
									break;
								}
							}
						}
					}
				}// if (checking cells around the material)
			} // if gravity
		} // for i
	} // for j
	
	reset_grid_changes();
	
	
	///variables used in 2. APPLY SATURATION
	// cMat		  :	the current material that we are trying to see if it gets saturated.
	// matIndex	  :	the thing used to matIndex into the matSatOrder array to get a correct material to store int cMat.
	// satEffIndex:	the index for going throught
	// newi, newj :	the coordinates of the cells around the grid[i+camera_x][j+camera_y].mat being evaluated.
	// c		  :	used to indicate which cell around our cell is being evaluated.
	short cMat;
	unsigned short matIndex, satEffIndex, newi, newj;
	bool firstEncounter;
	
	/// 2.1 EVALUATE SATURATION - the giant ass loop where the saturation is evaluated
	// go through ONLY the types of materials that can be saturated.
	for(matIndex=0; matIndex<numberOfSatableMats ; matIndex++){
			
		cMat = matSatOrder[matIndex]; // get correct material
		
		for(i=0 ; i<GRID_WIDTH ; i++){ // go through each row
			for(j=0 ; j<GRID_HEIGHT ; j++){ // go through each column
				
				//go through each saturation effect of the material
				for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATIONS ; satEffIndex++){
					
					firstEncounter = 1;
					
					// if there is no effect, skip ahead to the next saturation effect.
					if(mats[cMat].satEffect[satEffIndex].satMat == m_no_saturation) continue;
					
					//make sure the current cell has the current material, cMat, in it!! if not, continue;
					if(cMat != grid[i+camera_x][j+camera_y].mat) continue;
					
					//check for saturation memory
					if(mats[cMat].satEffect[satEffIndex].satMem == false){ // if there is no saturation memory
						// if there is no memory, evaluate the satuation level each time.
						grid[i+camera_x][j+camera_y].satLevelChange = 0;
						// it needs to be saturated each cycle of evaluate_grid() when there is no memory.
						grid[i+camera_x][j+camera_y].satChange = m_no_saturation;
					}
					/*
					else{ // if there is saturation memory
						// there is no change in the saturation level.
						grid[i+camera_x][j+camera_y].satLevelChange = m_no_change;
						// there is no change in the saturation
						grid[i+camera_x][j+camera_y].satChange = m_no_change;
					}
					*/
						
					//for every cell around our cell [i][j], evaluate whether or not it gets saturated
					//  	0 1 2
					//  	3 M 4
					//  	5 6 7
					for(c=0 ; c<8 ; c++){
						//get correct newi and newj values to plug into grid[newi+camera_x][newj+camera_y].mat
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
						if(newi < 0 || newi >= GRID_WIDTH || newj < 0 || newj >= GRID_HEIGHT) continue;
					
						// if the material near this cell is the right type to saturate it
						if(grid[newi+camera_x][newj+camera_y].mat == mats[cMat].satEffect[satEffIndex].satMat){
							
							if(roll_ht(mats[cMat].satEffect[satEffIndex].chance[c])){ // determine if it will become saturated based on roll_ht function.
								grid[i+camera_x][j+camera_y].satChange = mats[cMat].satEffect[satEffIndex].satMat;
								// absorbs the material if required. it only absorbs if it isn't already saturated.
								if(grid[i+camera_x][j+camera_y].sat != grid[newi+camera_x][newj+camera_y].mat && mats[cMat].satEffect[satEffIndex].absorb) grid[newi+camera_x][newj+camera_y].matChange = m_air;
							}
							// increment the satLevel if needed.
							if(firstEncounter){
								grid[i+camera_x][j+camera_y].satLevelChange = 1;
								firstEncounter = 0;
								}
							else
								grid[i+camera_x][j+camera_y].satLevelChange++;
						}
					}
				}
			}
		}
	}
	apply_grid_changes(); // apply changes from the SATURATION APPLY part of this function.
	
	
	
	/// 2.2 SATURATION DECAY
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			if(grid[i+camera_x][j+camera_y].sat != m_no_saturation){ // if there is a valid saturation here
				//store current material here for convenience
				cMat = grid[i+camera_x][j+camera_y].mat;
				for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATIONS ; satEffIndex++){
					if(mats[cMat].satEffect[satEffIndex].satMat == grid[i+camera_x][j+camera_y].sat){ // if this is the right saturation
						//check for the right saturaion levels. if you don't have the right sat levels, just move on to the next satEffect. (continue)
						if(grid[i+camera_x][j+camera_y].satLevel < mats[cMat].satEffect[satEffIndex].decaySatGTE || grid[i+camera_x][j+camera_y].satLevel > mats[cMat].satEffect[satEffIndex].decaySatLTE) continue;
						
						// roll for saturation-initiated decay 
						if(roll_ht( mats[cMat].satEffect[satEffIndex].decayChance )){
							grid[i+camera_x][j+camera_y].matChange = mats[cMat].satEffect[satEffIndex].decayIntoMat;
							grid[i+camera_x][j+camera_y].satChange = mats[cMat].satEffect[satEffIndex].decayIntoSat;
						}
					}
				}
			}
		}
	}
	apply_grid_changes(); // apply changes from the SATURATION AFFECTS AND DECAY part of this function.
	
	
	
	/// 3. AFFECTS AND DECAY - this giant-ass for loop is where we find out which cells need to be changed.
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			// air doesn't do anything. that is it's definition.
			if(grid[i+camera_x][j+camera_y].mat == m_air) continue;
			for(a=0 ; a<MAX_NUMBER_OF_MATERIAL_INTERACTIONS; a++){ // check all the possible interactions
				
				//evaluate the affectMaterial structure (this will apply correct changes to the cellMat array)
				evaluate_affectMaterial(i, j, &mats[grid[i+camera_x][j+camera_y].mat].affectMat[a] );
				
			}
			// check for decay.
			if(roll_ht( mats[ grid[i+camera_x][j+camera_y].mat ].decayChance) ) grid[i+camera_x][j+camera_y].matChange = mats[ grid[i+camera_x][j+camera_y].mat ].decayIntoMat; // if, by chance, it is time to decay, then decay into your proper type.
		}
	}
	apply_grid_changes(); // apply changes from the AFFECTS AND DECAY and decay part of this function.
	
}/// end evaluate_grid()





void reset_grid_changes(){
	int i,j;
	//reset cellMatChanges and cellSatChanges
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			grid[i+camera_x][j+camera_y].matChange = m_no_change;
			grid[i+camera_x][j+camera_y].satChange = m_no_change;
			grid[i+camera_x][j+camera_y].satLevelChange = m_no_change;
		}
	}
}




void apply_grid_changes(){
	int i,j;
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			//if the material at [i][j] needs to be changed (updated) then change it
			if(grid[i+camera_x][j+camera_y].matChange != m_no_change) grid[i+camera_x][j+camera_y].mat = grid[i+camera_x][j+camera_y].matChange;
			//if the saturation at [i][j] needs to be changed (updated) then change it
			if(grid[i+camera_x][j+camera_y].satChange != m_no_change) grid[i+camera_x][j+camera_y].sat = grid[i+camera_x][j+camera_y].satChange;
			//if the saturation level at [i][j] needs to be changed (updated) then change it.
			if(grid[i+camera_x][j+camera_y].satLevelChange != m_no_change) grid[i+camera_x][j+camera_y].satLevel = grid[i+camera_x][j+camera_y].satLevelChange;
		}
	}
	//reset cellMatChanges and cellSatChanges
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			grid[i+camera_x][j+camera_y].matChange = m_no_change;
			grid[i+camera_x][j+camera_y].satChange = m_no_change;
			grid[i+camera_x][j+camera_y].satLevelChange = m_no_change;
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
		if(grid[i+camera_x][j+camera_y].sat != m_no_saturation) return;
		break;
	case m_any_of_my_sats:
		validSat = false; // by default, we have not yet detected a valid saturation.
		for(c=0 ; c<MAX_NUMBER_OF_SATURATIONS ; c++){
			if(mats[grid[i+camera_x][j+camera_y].mat].satEffect[c].satMat == m_no_saturation) // if you have reached an null satEffect...
				break; // ...break out of the for(c) loop
				
			// if there is a valid saturation, set validSat true and break out of the for(c) loop.
			if(grid[i+camera_x][j+camera_y].sat == mats[grid[i+camera_x][j+camera_y].mat].satEffect[c].satMat){
					validSat = true;
				break;
			}
		}
		if(validSat == false) return; // false condition. if this material is not saturated with one of its valid saturations, then quit this affectMat.
		//if our material isn't saturated enough, return
		if(grid[i+camera_x][j+camera_y].satLevel < affMat->satGTE) return;
		//if our material is too saturated, return
		if(grid[i+camera_x][j+camera_y].satLevel > affMat->satLTE) return;
		break;
	default: // default conditions. there is a single saturation we are looking for. and we have to be the saturation range.
		//if the saturation here isn't right, return
		if(grid[i+camera_x][j+camera_y].sat != affMat->satNeeded) return;
		//if our material isn't saturated enough, return
		if(grid[i+camera_x][j+camera_y].satLevel < affMat->satGTE) return;
		//if our material is too saturated, return
		if(grid[i+camera_x][j+camera_y].satLevel > affMat->satLTE) return;
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
			if(newi < 0 || newi >= GRID_WIDTH || newj < 0 || newj >= GRID_HEIGHT) continue;
		
			// if there is a valid material or if you can use any material
			if( affMat->matBefore == grid[newi+camera_x][newj+camera_y].mat || affMat->matBefore == m_dont_care){
				// if there is a valid saturation or if you can use any saturation
				if( affMat->satBefore == grid[newi+camera_x][newj+camera_y].sat || affMat->satBefore == m_dont_care){
					// rolling
					if(roll_ht(affMat->chance[c])){
						// change the material only if it needs changing.
						if(affMat->matAfter != m_no_change) grid[newi+camera_x][newj+camera_y].matChange = affMat->matAfter;
						// change the saturation only if it needs changing. also change the saturation level to a default of 1
						if(affMat->satAfter != m_no_change) {
								grid[newi+camera_x][newj+camera_y].satChange = affMat->satAfter;
								grid[newi+camera_x][newj+camera_y].satLevelChange = 1;
						}
						
						// check to see if the original material will change because of it having completed an affectMat
						if(affMat->changeOrigMat != m_no_change) 			// if the material changes after it affects neighboring cells
							grid[i+camera_x][j+camera_y].matChange = affMat->changeOrigMat; 		// change the material
						if(affMat->changeOrigSat != m_no_change) 			// if the saturation of our material changes after our material affects neighboring cells
							grid[i+camera_x][j+camera_y].satChange = affMat->changeOrigSat; 		// change the saturation of our material.
					}
				}
			}
		}
	}
}








/// this will print to the screen each material in each cell.
void print_cells(){
    int i,j;
	//quad vertex (quadrilateral. four sides.)
	glBegin(GL_QUADS);
	int matColor, satColor;
	// print out the grid
    for(i = 0; i < GRID_WIDTH - wPos/CELL_SIZE; i++){
        for(j = 0; j < GRID_HEIGHT; j++){
        	//only print the material if it is not air
			if(grid[i+camera_x][j+camera_y].mat != m_air){
				matColor = mats[grid[i+camera_x][j+camera_y].mat].color;
				glColor3ub(matColor/0x10000, (matColor/0x100)&0xFF, matColor&0xFF);
				//these are the verticies of the material
				glVertex2i(    i*CELL_SIZE, j*CELL_SIZE);
				glVertex2i((i+1)*CELL_SIZE, j*CELL_SIZE);
				glVertex2i((i+1)*CELL_SIZE, (j+1)*CELL_SIZE);
				glVertex2i(    i*CELL_SIZE, (j+1)*CELL_SIZE);
			}
			//only print valid saturations
			if( grid[i+camera_x][j+camera_y].sat != m_no_saturation ){
				// put the color into something easier to write
				satColor = mats[grid[i+camera_x][j+camera_y].sat].color;
				//get the right color values
				glColor3ub(satColor/0x10000, (satColor/0x100)&0xFF, satColor&0xFF);
				//print the cell saturation color inside the cell itself.
				glVertex2i((i+0.25)*CELL_SIZE, (j+0.25)*CELL_SIZE);
				glVertex2i((i+0.75)*CELL_SIZE, (j+0.25)*CELL_SIZE);
				glVertex2i((i+0.75)*CELL_SIZE, (j+0.75)*CELL_SIZE);
				glVertex2i((i+0.25)*CELL_SIZE, (j+0.75)*CELL_SIZE);
			}
        }
    }
    
	//end the printing of material cells
    glEnd();
    /*
    // print the color of the saturations of each material in each cell
    for(i = 0; i < GRID_WIDTH; i++){
        for(j = 0; j < GRID_HEIGHT; j++){
			if( grid[i][j].sat == m_no_saturation || grid[i][j].sat == m_air ) continue; // you don't need to print air. there is a black background being printed at the beginning of this print_cells() function.
			myRectangleSat.x = i*CELL_SIZE + myRectangleSat.w/2;
			myRectangleSat.y = j*CELL_SIZE + myRectangleSat.h/2;
            GL_FillRect( screen , &myRectangleSat , mats[grid[i][j].sat].color);
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

