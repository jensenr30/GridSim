/// this document contains the necessary functions to evaluate and advance the simulation.
/// It looks at the materials in the cells and then updates the cells based on the dynamics of the materials described in materials_and_cells.h

//used for temporarily storing changes to the cellMat[][] and cellSat[][] arrays.

void apply_grid_changes();
bool *generate_near_by_cell_test_vector(unsigned short);
void evaluate_affectMaterial(unsigned short, unsigned short, struct affectMaterial *);
void reset_grid_changes();

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
	
	
	
	
	/// 1. GRAVITY - this is where the gravity gets CHECKED and APPLIED.
	for(j=GRID_HEIGHT-1 ; j>=0 ; j--){
		for(i=0 ; i<GRID_WIDTH ; i++){

			// if gravity affects this material...
			if(mats[grid[i][j].mat].gravity){
				//dissapears through the bottom
				if(j >= GRID_HEIGHT-1){ // if the gravity material is at the bottom of the screen, get rid of it.
					grid[i][j].mat = M_air;
				}
				// falls down
				else if(grid[i][j+1].mat == M_air || grid[i][j+1].mat == M_air){
					grid[i][j+1].mat = grid[i][j].mat;
					grid[i][j].mat = M_air;
				}
				
				// if the material could EITHER fall to the LEFT or the RIGHT
				else if( i>0 && grid[i-1][j].mat == M_air && grid[i-1][j+1].mat == M_air   &&   i<GRID_WIDTH-1 && grid[i+1][j].mat == M_air && grid[i+1][j+1].mat == M_air ){
					// randomly choose whether to...
					if(get_rand(0,1)){
						grid[i-1][j+1].mat = grid[i][j].mat; // go to the left or...
						grid[i][j].mat = M_air;
					}
					else{
						grid[i+1][j+1].mat = grid[i][j].mat; // go to the right.
						grid[i][j].mat = M_air;
					}
				}
				// if the material can ONLY fall to the LEFT
				else if( i>0 && grid[i-1][j].mat == M_air && grid[i-1][j+1].mat == M_air){
					grid[i-1][j+1].mat = grid[i][j].mat; // go to the left or...
					grid[i][j].mat = M_air;
				}
				//if the material can ONLY fall the the RIGHT
				else if( i <GRID_WIDTH-1 && grid[i+1][j].mat == M_air && grid[i+1][j+1].mat == M_air){
					grid[i+1][j+1].mat = grid[i][j].mat; // go to the right.
					grid[i][j].mat = M_air;
				}
			}
		}
	}
	
	
	
	reset_grid_changes();
	
	
	///variables used in 2. APPLY SATURATION
	// cMat		  :	the current material that we are trying to see if it gets saturated.
	// matIndex	  :	the thing used to matIndex into the matSatOrder array to get a correct material to store int cMat.
	// satEffIndex:	the index for going throught
	// newi, newj :	the coordinates of the cells around the grid[i][j].mat being evaluated.
	// c		  :	used to indicate which cell around our cell is being evaluated.
	short cMat;
	unsigned short matIndex, satEffIndex, newi, newj;
	
	/// 2.1 EVALUATE SATURATION - the giant ass loop where the saturation is evaluated
	for(matIndex=0; matIndex<numberOfSatableMats ; matIndex++){ // go through every type of material that can be saturated and ONLY the types of materials that can be saturated.
			
		cMat = matSatOrder[matIndex]; // get correct material
		
		for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATIONS ; satEffIndex++){
					
			// if there is no saturation effect at the satEffIndex-th saturation effect, then there aren't any more by definition.
			//break out of this look and move on the the next material that may have saturation effects.
			if(mats[cMat].satEffect[satEffIndex].satMat == M_no_saturation) break;
		
			for(i=0 ; i<GRID_WIDTH ; i++){ // go through each row
				for(j=0 ; j<GRID_HEIGHT ; j++){ // go through each column
					
					//make sure the current cell has the current material, cMat, in it!! if not, continue;
					if(cMat != grid[i][j].mat) continue;
					
					//check for saturation memory
					if(mats[cMat].satEffect[satEffIndex].satMem == false){ // if there is no saturation memory
						// if there is no memory, evaluate the satuation level each time.
						grid[i][j].satLevelChange = 0;
						// it needs to be saturated each cycle of evaluate_grid() when there is no memory.
						grid[i][j].satChange = M_no_saturation;
					}
					else{ // if there is saturation memory
						// there is no change in the saturation level.
						grid[i][j].satLevelChange = M_no_change;
						// there is no change in the saturation
						grid[i][j].satChange = M_no_change;
					}
						
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
						if(newi < 0 || newi >= GRID_WIDTH || newj < 0 || newj >= GRID_HEIGHT) continue;
					
						// if the material near this cell is the right type to saturate it
						if(grid[newi][newj].mat == mats[cMat].satEffect[satEffIndex].satMat){
							
							if(roll_ht(mats[cMat].satEffect[satEffIndex].chance[c])){ // determine if it will become saturated based on roll_ht function.
								grid[i][j].satChange = mats[cMat].satEffect[satEffIndex].satMat;
								// absorbs the material if required. it only absorbs if it isn't already saturated.
								if(grid[i][j].sat != grid[newi][newj].mat && mats[cMat].satEffect[satEffIndex].absorb) grid[newi][newj].matChange = M_air;
							}
							// increment the satLevel if needed.
							if(grid[i][j].satLevelChange == M_no_change)
								grid[i][j].satLevelChange=1;
							else
								grid[i][j].satLevelChange++;
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
			if(grid[i][j].sat != -2){ // if there is a valid saturation here
				//store current material here for convenience
				cMat = grid[i][j].mat;
				for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATIONS ; satEffIndex++){
					if(mats[cMat].satEffect[satEffIndex].satMat == grid[i][j].sat){ // if this is the right saturation
						//check for the right saturaion levels. if you don't have the right sat levels, just move on to the next satEffect. (continue)
						if(grid[i][j].satLevel < mats[cMat].satEffect[satEffIndex].decaySatGTE || grid[i][j].satLevel > mats[cMat].satEffect[satEffIndex].decaySatLTE) continue;
						
						// roll for saturation-initiated decay 
						if(roll_ht( mats[cMat].satEffect[satEffIndex].decayChance )){
							grid[i][j].matChange = mats[cMat].satEffect[satEffIndex].decayInto;
							grid[i][j].satChange = mats[cMat].satEffect[satEffIndex].decaySatMat;
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
			if(grid[i][j].mat == M_air) continue;
			for(a=0 ; a<MAX_NUMBER_OF_MATERIAL_INTERACTIONS; a++){ // check all the possible interactions
				
				//evaluate the affectMaterial structure (this will apply correct changes to the cellMat array)
				evaluate_affectMaterial(i, j, &mats[grid[i][j].mat].affectMat[a] );
				// check for decay.
				if(roll_ht( mats[ grid[i][j].mat ].decayChance) ) grid[i][j].matChange = mats[ grid[i][j].mat ].decayInto; // if, by chance, it is time to decay, then decay into your proper type.
				
			}
		}
	}
	apply_grid_changes(); // apply changes from the AFFECTS AND DECAY and decay part of this function.
	
}/// end evaluate_grid()





void reset_grid_changes(){
	int i,j;
	//reset cellMatChanges and cellSatChanges
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			grid[i][j].matChange = M_no_change;
			grid[i][j].satChange = M_no_change;
			grid[i][j].satLevelChange = M_no_change;
		}
	}
}




void apply_grid_changes(){
	int i,j;
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			//if the material at [i][j] needs to be changed (updated) then change it
			if(grid[i][j].matChange != M_no_change) grid[i][j].mat = grid[i][j].matChange;
			//if the saturation at [i][j] needs to be changed (updated) then change it
			if(grid[i][j].satChange != M_no_change) grid[i][j].sat = grid[i][j].satChange;
			//if the saturation level at [i][j] needs to be changed (updated) then change it.
			if(grid[i][j].satLevelChange != M_no_change) grid[i][j].satLevel = grid[i][j].satLevelChange;
		}
	}
	//reset cellMatChanges and cellSatChanges
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			grid[i][j].matChange = M_no_change;
			grid[i][j].satChange = M_no_change;
			grid[i][j].satLevelChange = M_no_change;
		}
	}
}











void evaluate_affectMaterial(unsigned short i, unsigned short j, struct affectMaterial *affMat){
	
	// if the affectMaterial will not do anything, return;
	if(affMat->matAfter == M_no_change) return;
	//don't do anything if you don't need to.
	if(affMat->matBefore == M_air && affMat->matAfter == M_air) return;
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
	case M_dont_care:
		break;
	case M_no_saturation:
		if(grid[i][j].sat != M_no_saturation) return;
		break;
	case M_any_of_my_sats:
		validSat = false; // by default, we have not yet detected a valid saturation.
		for(c=0 ; c<MAX_NUMBER_OF_SATURATIONS ; c++){
			if(mats[grid[i][j].mat].satEffect[c].satMat == M_no_saturation) // if you have reached an null satEffect...
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
		// if newi and newj are in UNACCEPTABLE places, continue to the next neighbor cell (continue;)
		if(newi < 0 || newi >= GRID_WIDTH || newj < 0 || newj >= GRID_HEIGHT) continue;
		
		if(testVector[c] == true && ( affMat->matBefore == grid[newi][newj].mat || affMat->matBefore == M_dont_care) && ( affMat->satBefore == grid[newi][newj].sat || affMat->satBefore == M_dont_care) ){
			if(roll_ht(affMat->chance[c])){
				//change the material only if it needs changing.
				if(affMat->matAfter != M_no_change) grid[newi][newj].matChange = affMat->matAfter;
				//change the saturation only if it needs changing. also change the saturation level to a default of 1
				if(affMat->satAfter != M_no_change) {
						grid[newi][newj].satChange = affMat->satAfter;
						grid[newi][newj].satLevelChange = 1;
				}
				
				//check to see if the original material will change because of it having completed an affectMat
				if(affMat->changeOrigMat != M_no_change) // if the material changes after it affects neighboring cells
					grid[i][j].matChange = affMat->changeOrigMat; // change the material
				if(affMat->changeOrigSat != M_no_change) // if the saturation of our material changes after our material affects neighboring cells
					grid[i][j].satChange = affMat->changeOrigSat; // change the saturation of our material.
				grid[newi][newj].satLevelChange = 0;
					
			}
		}
	}
}








/// this will print to the screen each material in each cell.
void print_cells(){
	
    int i,j;
	SDL_Rect myRectangle;
	myRectangle.x = 0;
	myRectangle.y = 0;
	myRectangle.w = SCREEN_WIDTH;
	myRectangle.h = SCREEN_HEIGHT;
	//INITIAL BLACK BACGROUND
	SDL_FillRect( screen , &myRectangle , 0x000000);
	
	//constant cell sizes
	myRectangle.w = CELL_SIZE;
	myRectangle.h = CELL_SIZE;
	
    for(i = 0; i < GRID_WIDTH; i++){
        for(j = 0; j < GRID_HEIGHT; j++){
			if(grid[i][j].mat == M_air) continue; // you don't need to print air. there is a black background being printed at the beginning of this print_cells() function.
			
			myRectangle.x = i*CELL_SIZE;
			myRectangle.y = j*CELL_SIZE;
            SDL_FillRect( screen , &myRectangle , mats[grid[i][j].mat].color);
        }
    }
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

