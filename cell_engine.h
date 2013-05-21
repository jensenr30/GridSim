/// this document contains the necessary functions to evaluate and advance the simulation.
/// It looks at the materials in the cells and then updates the cells based on the dynamics of the materials described in materials_and_cells.h


// this will evaluate the grid. It will run a simulation for the number of generations you tell it to.
void evaluate_cells(){
/// 0. VARIABLES - this is where all the variables used heavily in the loops are declared.
	
	///variables used in 1. GRAVITY, 3. AFFECT & DECAY, AND 4. APPLY CHANGES
	int i,j,a,c; // i = x position in grid. j = y position in grid. k = material type being evaluated. a = type of material being affected. c = which cell around the material is being checked
	
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
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			cellChanges[i][j] = M_no_change;
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
	
	
/// 2. APPLY SATURATION - the giant ass loop where the saturation is applied to appropriate materials
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
						if(cellData[newi][newj] != cellSat[i][j]){ // if the around our cell is of a different material than our current saturation (this releaves processing power and stops things from sucking up all that can saturate it.
							if(cellData[newi][newj] == mats[cMat].satEffect[satEffIndex].satMat){ // if the material near this cell is the right type to sturate it
								if(roll_ht(mats[cMat].satEffect[satEffIndex].satChance[c])){ // determine if it will become saturated based on roll_ht function.
									cellSat[i][j] = mats[cMat].satEffect[satEffIndex].satMat;
									cellData[newi][newj] = M_air;
								}
							}
						}
					}
				}
			}
		}
	}
/// 2.1 SATURATION DECAY AND AFFECTMATS
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			if(cellSat[i][j] != -2){ // if there is a valid saturation here
				for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATION_EFFECTS ; satEffIndex++){
					if(mats[cellData[i][j]].satEffect[satEffIndex].satMat == cellSat[i][j]) // if this is the right saturation
					{
						//check decay
						if(roll_ht( mats[cellData[i][j]].satEffect[satEffIndex].decayChance )){
							cellData[i][j] = mats[cellData[i][j]].satEffect[satEffIndex].decayInto;
							cellSat[i][j] = mats[cellData[i][j]].satEffect[satEffIndex].decaySatMat;
						}
						for(a=0 ; a<MAX_NUMBER_OF_SATURATION_EFFECT_INTERACTIONS ; a++){ // check all valid affectMat entries for this saturation.
							
							affMat = &mats[cellData[i][j]].satEffect[satEffIndex].affectMat[a]; // set to correct affectMaterial structure
							// diagram of cell placement in relation to our cell that we are evaluating right now.
							//		0 1 2
							//		3 M 4
							//		5 6 7
					
							// 0.
							if(affMat->typeBefore == cellData[i-1][j-1])
								if(roll_ht(affMat->chance[0]))
									cellChanges[i-1][j-1] = affMat->typeAfter;
							// 1.
							if(affMat->typeBefore == cellData[i][j-1])
								if(roll_ht(affMat->chance[1]))
									cellChanges[i][j-1] = affMat->typeAfter;
							// 2.
							if(affMat->typeBefore == cellData[i+1][j-1])
								if(roll_ht(affMat->chance[2]))
									cellChanges[i+1][j-1] = affMat->typeAfter;
							// 3.
							if(affMat->typeBefore == cellData[i-1][j])
								if(roll_ht(affMat->chance[3]))
									cellChanges[i-1][j] = affMat->typeAfter;
							// 4.
							if(affMat->typeBefore == cellData[i+1][j])
								if(roll_ht(affMat->chance[4]))
									cellChanges[i+1][j] = affMat->typeAfter;
							// 5.
							if(affMat->typeBefore == cellData[i-1][j+1])
								if(roll_ht(affMat->chance[5]))
									cellChanges[i-1][j+1] = affMat->typeAfter;
							// 6.
							if(affMat->typeBefore == cellData[i][j+1])
								if(roll_ht(affMat->chance[6]))
									cellChanges[i][j+1] = affMat->typeAfter;
							// 7.
							if(affMat->typeBefore == cellData[i+1][j+1])
								if(roll_ht(affMat->chance[7]))
									cellChanges[i+1][j+1] = affMat->typeAfter;
							
						}
					}
				}
			}
		}
	}
/// 2.2 APPLY SATURATION CHANGES
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

			if(roll_ht( mats[ cellData[i][j] ].decayChance) ) cellData[i][j] = mats[ cellData[i][j] ].decayInto; // if, by chance, it is time to decay, then decay into your proper type.
			for(a=0 ; a<MAX_NUMBER_OF_MATERIAL_INTERACTIONS; a++){ // check all the possible interactions
				
				//because a starts at 0, the first
				if(mats[ cellData[i][j] ].affectMat[a].typeBefore == M_air && mats[ cellData[i][j] ].affectMat[a].typeAfter == M_air) break; // air becoming air. stop checking material affects. there are no more.
				for(c=0 ; c<8 ; c++){

					//this is a diagram of how the numbers in the chance array correlate to the cells around material in the main cell (M)
					//		0 1 2
					//		3 M 4
					//		5 6 7

					switch(c){
					case 0:
						// if the material is the right one to be changed AND if the roll comes back good
						if(  mats[ cellData[i][j] ].affectMat[a].typeBefore == cellData[i-1][j-1]  &&  roll_ht( mats[ cellData[i][j] ].affectMat[a].chance[c] ) && i>0 && j>0 ){
							//then change that cell to the material type after the cell affects the neighboring cell.
							cellChanges[i-1][j-1] = mats[ cellData[i][j] ].affectMat[a].typeAfter;
						}
						break;
					case 1:
						// all of the other cases work very similarly.
						if(  mats[ cellData[i][j] ].affectMat[a].typeBefore == cellData[i][j-1]  &&  roll_ht( mats[ cellData[i][j] ].affectMat[a].chance[c] ) && j>0 ){
							cellChanges[i][j-1] = mats[ cellData[i][j] ].affectMat[a].typeAfter;
						}
						break;
					case 2:
						if(  mats[ cellData[i][j] ].affectMat[a].typeBefore == cellData[i+1][j-1]  &&  roll_ht( mats[ cellData[i][j] ].affectMat[a].chance[c] ) && i<(GRID_WIDTH-1) && j>0 ){
							cellChanges[i+1][j-1] = mats[ cellData[i][j] ].affectMat[a].typeAfter;
						}
						break;
					case 3:
						if(  mats[ cellData[i][j] ].affectMat[a].typeBefore == cellData[i-1][j]  &&  roll_ht( mats[ cellData[i][j] ].affectMat[a].chance[c] ) && i>0 ){
							cellChanges[i-1][j] = mats[ cellData[i][j] ].affectMat[a].typeAfter;
						}
						break;
					case 4:
						if(  mats[ cellData[i][j] ].affectMat[a].typeBefore == cellData[i+1][j]  &&  roll_ht( mats[ cellData[i][j] ].affectMat[a].chance[c] ) && i<(GRID_WIDTH-1) ){
							cellChanges[i+1][j] = mats[ cellData[i][j] ].affectMat[a].typeAfter;
						}
						break;
					case 5:
						if(  mats[ cellData[i][j] ].affectMat[a].typeBefore == cellData[i-1][j+1]  &&  roll_ht( mats[ cellData[i][j] ].affectMat[a].chance[c] ) && i>0 && j<(GRID_HEIGHT-1) ){
							cellChanges[i-1][j+1] = mats[ cellData[i][j] ].affectMat[a].typeAfter;
						}
						break;
					case 6:
						if(  mats[ cellData[i][j] ].affectMat[a].typeBefore == cellData[i][j+1]  &&  roll_ht( mats[ cellData[i][j] ].affectMat[a].chance[c] ) && j<(GRID_HEIGHT-1) ){
							cellChanges[i][j+1] = mats[ cellData[i][j] ].affectMat[a].typeAfter;
						}
						break;
					case 7:
						if(  mats[ cellData[i][j] ].affectMat[a].typeBefore == cellData[i+1][j+1]  &&  roll_ht( mats[ cellData[i][j] ].affectMat[a].chance[c] ) && i<(GRID_WIDTH-1) && j<(GRID_HEIGHT-1) ){
							cellChanges[i+1][j+1] = mats[ cellData[i][j] ].affectMat[a].typeAfter;
						}
						break;
					default:
						break;

					}//end switch
				}
			}
		}
	}

/// 4. APPLY CHANGES
	/// this is where we use the results collected in the cellChanged array to modify the cellData array.
	/// this applies the changes from the saturation checking looop and the affectMat/decay loop.
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			if(cellChanges[i][j] == M_no_change) continue; // if there is no change in this cell, move on to the next one, asshole!
			//otherwise, these was a change in the cell. so copy cellChanges into cellData
			cellData[i][j] = cellChanges[i][j];
		}
	}
}


// this will print to the screen each material in each cell.
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
