/// this document contains the necessary functions to evaluate and advance the simulation.
/// It looks at the materials in the cells and then updates the cells based on the dynamics of the materials described in materials_and_cells.h

bool *generate_near_by_cell_test_vector(unsigned short);

// this will evaluate the grid. It will run a simulation for the number of generations you tell it to.
void evaluate_grid(){
/// 0. VARIABLES - this is where all the variables used heavily in the loops are declared.
	
	///variables used in 1. GRAVITY, 3. AFFECT & DECAY, AND 4. APPLY CHANGES
	int i,j,a,c; // i = x position in grid. j = y position in grid. k = material type being evaluated. a = type of material being affected. c = which cell around the material is being checked
	
	/// variables used in lots of places
	bool *testVector;
	
	///variables used in 2. APPLY SATURATION
	// cMat		  :	the current material that we are trying to see if it gets saturated.
	// matIndex	  :	the thing used to matIndex into the matSatOrder array to get a correct material to store int cMat.
	// satEffIndex:	the index for going throught
	// c		  :	used to indicate which cell around our cell is being evaluated.
	// newi		  :	the i value to plug into cellData[][newj] to find the material there.
	// newj		  : the j value to plug into cellData[newi][] to find the material there.
	// affMat	  :	the affectMaterial * type that points at the corrent
	short cMat, matIndex, satEffIndex;
	int newi, newj;
	struct affectMaterial *affMat;

	/* this array is used to log the changes that will need to be made to the cells in the grid.
	 * I apply the changes that need to be made ONLY AFTER I have evaluated all the cells in the grid.
	 * I do this so that the evaluation is not partial to things that are evaluated earlier than others.
	 * i.e. if two materials destroy each other when they meet, then if one of them is evaluated first AND the effects it produces are applied to cellData right away, then the other material will not be able to be evaluated and thus the two materials will not destroy each other as was intended.
	 * Using an array to log the changes that need to be made to the cells in the grid is a pretty good way of going abou this problem I think.
	 */
	short cellChanges[GRID_WIDTH][GRID_HEIGHT];
	short cellSatChanges[GRID_WIDTH][GRID_HEIGHT];
	
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			cellChanges[i][j] = M_no_change;
			cellSatChanges[i][j] = M_no_change;
		}
	}
	
	
	
	
/// 1. GRAVITY - this is where the gravity gets CHECKED and APPLIED.
	for(j=GRID_HEIGHT-1 ; j>=0 ; j--){
		for(i=0 ; i<GRID_WIDTH ; i++){

			// if gravity affects this material...
			if(mats[cellData[i][j]].gravity){
				//dissapears through the bottom
				if(j >= GRID_HEIGHT-1){ // if the gravity material is at the bottom of the screen, get rid of it.
					cellData[i][j] = M_air;
				}
				// falls down
				else if(cellChanges[i][j+1] == M_air || cellData[i][j+1] == M_air){
					cellData[i][j+1] = cellData[i][j];
					cellData[i][j] = M_air;
				}
				
				// if the material could EITHER fall to the LEFT or the RIGHT
				else if( i>0 && cellData[i-1][j] == M_air && cellData[i-1][j+1] == M_air   &&   i<GRID_WIDTH-1 && cellData[i+1][j] == M_air && cellData[i+1][j+1] == M_air ){
					// randomly choose whether to...
					if(get_rand(0,1)){
						cellData[i-1][j+1] = cellData[i][j]; // go to the left or...
						cellData[i][j] = M_air;
					}
					else{
						cellData[i+1][j+1] = cellData[i][j]; // go to the right.
						cellData[i][j] = M_air;
					}
				}
				// if the material can ONLY fall to the LEFT
				else if( i>0 && cellData[i-1][j] == M_air && cellData[i-1][j+1] == M_air){
					cellData[i-1][j+1] = cellData[i][j]; // go to the left or...
					cellData[i][j] = M_air;
				}
				//if the material can ONLY fall the the RIGHT
				else if( i <GRID_WIDTH-1 && cellData[i+1][j] == M_air && cellData[i+1][j+1] == M_air){
					cellData[i+1][j+1] = cellData[i][j]; // go to the right.
					cellData[i][j] = M_air;
				}
			}
		}
	}
	
	
	
	
/// 2.1 EVALUATE SATURATION - the giant ass loop where the saturation is evaluated
	for(i=0 ; i<GRID_WIDTH ; i++){ // go through each row
		for(j=0 ; j<GRID_HEIGHT ; j++){ // go through each column
			for(matIndex=0; matIndex<numberOfMaterialsThatCanBeSaturated ; matIndex++){ // go through every type of material that can be saturated
				
				cMat = matSatOrder[matIndex]; // get correct material
				//make sure the current cell has the current material, cMat, in it!! if not, continue;
				if(cMat != cellData[i][j]) continue;
				for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATION_EFFECTS ; satEffIndex++){
					
					// if there is no saturation effect at the satEffIndex-th saturation effect, then there aren't any more by definition.
					//break out of this look and move on the the next material that may have saturation effects.
					if(mats[cMat].satEffect[satEffIndex].satMat == M_no_saturation) break;

					
					//for every cell around our cell [i][j]
					//  	0 1 2
					//  	3 M 4
					//  	5 6 7
					for(c=0 ; c<8 ; c++){
						//get correct newi and newj values to plug into cellData[newi][newj]
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
						
						if(cellData[newi][newj] != cellSatChanges[i][j]){ // if the around our cell is of a different material than our current saturation (this releaves processing power and stops things from sucking up all that can saturate it.
							if(cellData[newi][newj] == mats[cMat].satEffect[satEffIndex].satMat){ // if the material near this cell is the right type to sturate it
								if(roll_ht(mats[cMat].satEffect[satEffIndex].satChance[c])){ // determine if it will become saturated based on roll_ht function.
									cellSatChanges[i][j] = mats[cMat].satEffect[satEffIndex].satMat;
									cellData[newi][newj] = M_air;
								}
							}
						}
					}
				}
			}
		}
	}
	
	
	
	
/// 2.1.5  APPLY NEW SATURATION BASED ON WHAT WAS FOUND IN 2.1
	for(i=0 ; i<GRID_WIDTH ; i++){
		if(cellSatChanges[i][j] != M_no_change) // only apply new saturation if it is, in fact, new!
			cellSat[i][j] = cellSatChanges[i][j];
	}
	
	
/// 2.2 SATURATION DECAY AND AFFECTMATS
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			if(cellSat[i][j] != -2){ // if there is a valid saturation here
				for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATION_EFFECTS ; satEffIndex++){
					if(mats[cellData[i][j]].satEffect[satEffIndex].satMat == cellSat[i][j]){ // if this is the right saturation
						
						// check all valid affectMat entries for this saturation.
						for(a=0 ; a<MAX_NUMBER_OF_SATURATION_EFFECT_INTERACTIONS ; a++){
							
							// set to correct affectMaterial structure for ease of writing in this particular for loop.
							affMat = &mats[cellData[i][j]].satEffect[satEffIndex].affectMat[a];
							
							//generate a testvector for the cells around the cell being evaluated.
							testVector = generate_near_by_cell_test_vector(affMat->matChanges);
							// if there aren't any blocks to be changed, then move on to the next one.
							if(affMat->matChanges == 0) continue;
							
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
								// if newi and newj are in UNACCEPTABLE places, continue to the next neighbor cell
								if(newi < 0 || newi >= GRID_WIDTH || newj < 0 || newj >= GRID_HEIGHT) continue;
								
								if(testVector[c] == true && ( affMat->typeBefore == cellData[newi][newj] || affMat->typeBefore == M_dont_care) && ( affMat->satBefore == cellSat[newi][newj] || affMat->satBefore == M_dont_care) ){
									if(roll_ht(affMat->chance[a])){
										//change the cell
										cellChanges[newi][newj] = affMat->typeAfter;
										cellSatChanges[newi][newj] = affMat->satAfter;
										
										//check to see if the original material will change because of it having completed an affectMat
										if(affMat->changeOrigMat != M_no_change) // if the material changes after it affects neighboring cells
											cellChanges[i][j] = affMat->changeOrigMat; // change the material
										if(affMat->changeOrigSat != M_no_change) // if the saturation of our material changes after our material affects neighboring cells
											cellSatChanges[i][j] = affMat->changeOrigSat; // change the saturation of our material.
									}
								}
							}
						}
						
						//check saturation-initiated decay 
						if(roll_ht( mats[cellData[i][j]].satEffect[satEffIndex].decayChance )){
							cellData[i][j] = mats[cellData[i][j]].satEffect[satEffIndex].decayInto;
							cellSat[i][j] = mats[cellData[i][j]].satEffect[satEffIndex].decaySatMat;
						}
					}
				}
			}
		}
	}
	
	
	
	
/// 2.2.5 APPLY SATURATION CHANGES BASED ON WHAT WAS FOUND IN 2.2
	// this applies the changes put into
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			if(cellChanges[i][j] == M_no_change) continue; // if there is no change in this cell, move on to the next one, asshole!
			//otherwise, these was a change in the cell. so copy cellChanges into cellData
			cellData[i][j] = cellChanges[i][j];
		}
	}
	
	
	
	
/// 3. DECAY AND AFFECTS - this giant-ass for loop is where we find out which cells need to be changed.
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			for(a=0 ; a<MAX_NUMBER_OF_MATERIAL_INTERACTIONS; a++){ // check all the possible interactions
				
				//copy current affectMaterial into this pointer so that it is easy to write it and read it in this for(a) loop.
				affMat = &mats[cellData[i][j]].affectMat[a];
				//generate test vector
				testVector = generate_near_by_cell_test_vector(mats[cellData[i][j]].affectMat[a].matChanges);
			
				//check to see if we need to check for materialAffects
				if( mats[cellData[i][j]].affectMat[a].matChanges == 0 || ( mats[ cellData[i][j] ].affectMat[a].typeBefore == M_air && mats[ cellData[i][j] ].affectMat[a].typeAfter == M_air) ) break; // no blocks effected OR air becoming air. stop checking material affects. there are no more.
				for(c=0 ; c<8 ; c++){
					if(testVector[c] != true) continue; // if you aren't supposed to check this one, try the next one.
					
					//generate how many
					//this is a diagram of how the numbers in the chance array correlate to the cells around material in the main cell (M)
					//		0 1 2
					//		3 M 4
					//		5 6 7
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
					
					//if the material
					if(  mats[ cellData[i][j] ].affectMat[a].typeBefore == cellData[newi][newj]  &&  roll_ht( mats[ cellData[i][j] ].affectMat[a].chance[c] )){
						//then change that cell to the material type after the cell affects the neighboring cell.
						cellChanges[newi][newj] = mats[ cellData[i][j] ].affectMat[a].typeAfter;
					}
				}
				
				// check for decay.
				if(roll_ht( mats[ cellData[i][j] ].decayChance) ) cellData[i][j] = mats[ cellData[i][j] ].decayInto; // if, by chance, it is time to decay, then decay into your proper type.
			}
		}
	}
	
	
	
	
	
/// 4. APPLY CHANGES - this collects the changes from part 3 and applies those changes to the cellData[][] array and the cellSat[][] array.
	// this is where we use the results collected in the cellChanged array to modify the cellData array.
	// this applies the changes from the saturation checking looop and the affectMat/decay loop.
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			if(cellChanges[i][j] != M_no_change) // only apply the new material type if it is different from the last. save on processing power.
				cellData[i][j] = cellChanges[i][j];
			if(cellSatChanges[i][j] != M_no_change) // only apply new saturation type if it is different from before.
				cellSat[i][j] = cellSatChanges[i][j];
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
			if(cellData[i][j] == M_air) continue; // you don't need to print air. there is a black background being printed at the beginning of this print_cells() function.
			
			myRectangle.x = i*CELL_SIZE;
			myRectangle.y = j*CELL_SIZE;
            SDL_FillRect( screen , &myRectangle , mats[cellData[i][j]].color);
        }
    }
}




// this will return a pointer to an array that has picks out "matChanges" of random elements in the testVector array.
// they correspond to the materials in the cells around our cell that will be checked.
bool *generate_near_by_cell_test_vector(unsigned short matChanges){
	static bool testVector[8];
	unsigned short emelentsChanged = 0; // this holds how many cells have been changed from 0's into 1's.
	unsigned short temp;
	
	testVector[0] = testVector[1] = testVector[2] = testVector[3] = testVector[4] = testVector[5] = testVector[6] = testVector[7] = false; // set to 0.
	
	if(matChanges == 0)
	{
		return testVector;
	}
	else if(matChanges == 8){
		testVector[0] = testVector[1] = testVector[2] = testVector[3] = testVector[4] = testVector[5] = testVector[6] = testVector[7] = true;
	}
	else{ // 1,2,3,4,5,6,7
		while(emelentsChanged < matChanges){ // while the number of entries in the textVector[] array are less than how many we need, try to add more to the array.
			temp = get_rand(0,7);
			if( testVector[temp] == false){
				testVector[temp] = true;
				emelentsChanged++;
			}
		}
	}
	return testVector;
}

