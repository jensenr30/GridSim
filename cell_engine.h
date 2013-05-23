/// this document contains the necessary functions to evaluate and advance the simulation.
/// It looks at the materials in the cells and then updates the cells based on the dynamics of the materials described in materials_and_cells.h

bool *generate_near_by_cell_test_vector(unsigned short);
void evaluate_affectMaterial(unsigned short, unsigned short, struct affectMaterial *);

// this will evaluate the grid. It will run a simulation for the number of generations you tell it to.
void evaluate_grid(){
/// 0. VARIABLES - this is where all the variables used heavily in the loops are declared.
	
	///variables used in 1. GRAVITY, and 3. AFFECT & DECAY.
	short int i,j,a,c; // i = x position in grid. j = y position in grid. k = material type being evaluated. a = type of material being affected. c = which cell around the material is being checked

	/* this array is used to log the changes that will need to be made to the cells in the grid.
	 * I apply the changes that need to be made ONLY AFTER I have evaluated all the cells in the grid.
	 * I do this so that the evaluation is not partial to things that are evaluated earlier than others.
	 * i.e. if two materials destroy each other when they meet, then if one of them is evaluated first AND the effects it produces are applied to cellData right away, then the other material will not be able to be evaluated and thus the two materials will not destroy each other as was intended.
	 * Using an array to log the changes that need to be made to the cells in the grid is a pretty good way of going abou this problem I think.
	 */
	
	
	
	
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
				else if(cellData[i][j+1] == M_air || cellData[i][j+1] == M_air){
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
	
	
	
	
	
	
	///variables used in 2. APPLY SATURATION
	// cMat		  :	the current material that we are trying to see if it gets saturated.
	// matIndex	  :	the thing used to matIndex into the matSatOrder array to get a correct material to store int cMat.
	// satEffIndex:	the index for going throught
	// newi, newj :	the coordinates of the cells around the cellData[i][j] being evaluated.
	// c		  :	used to indicate which cell around our cell is being evaluated.
	short cMat;
	unsigned short matIndex, satEffIndex, newi, newj;
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

					
					//for every cell around our cell [i][j], evaluate whether or not it gets saturated
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
	
	
	
/// 2.2 SATURATION DECAY AND AFFECTMATS
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			if(cellSat[i][j] != -2){ // if there is a valid saturation here
				for(satEffIndex=0 ; satEffIndex<MAX_NUMBER_OF_SATURATION_EFFECTS ; satEffIndex++){
					if(mats[cellData[i][j]].satEffect[satEffIndex].satMat == cellSat[i][j]){ // if this is the right saturation
						
						// check all valid affectMat entries for this saturation.
						for(a=0 ; a<MAX_NUMBER_OF_SATURATION_EFFECT_INTERACTIONS ; a++){
							
							//evaluate the affectMaterial structure (this will apply correct changes to the cellData array)
							evaluate_affectMaterial(i, j, &mats[cellData[i][j]].satEffect[satEffIndex].affectMat[a] );
							
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
	
	
/// 3. DECAY AND AFFECTS - this giant-ass for loop is where we find out which cells need to be changed.
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			for(a=0 ; a<MAX_NUMBER_OF_MATERIAL_INTERACTIONS; a++){ // check all the possible interactions
				
				//evaluate the affectMaterial structure (this will apply correct changes to the cellData array)
				evaluate_affectMaterial(i, j, &mats[cellData[i][j]].affectMat[a] );
				// check for decay.
				if(roll_ht( mats[ cellData[i][j] ].decayChance) ) cellData[i][j] = mats[ cellData[i][j] ].decayInto; // if, by chance, it is time to decay, then decay into your proper type.
				
			}
		}
	}
}/// end evaluate_grid()

void evaluate_affectMaterial(unsigned short i, unsigned short j, struct affectMaterial *affMat){
	
	static int newi,newj;
	static short c;
	
	static bool *testVector;
	
	//generate a testvector for the cells around the cell being evaluated.
	testVector = generate_near_by_cell_test_vector(affMat->changesPerEval);
	// if there aren't any blocks to be changed, then move on to the next one.
	if(affMat->changesPerEval == 0) return;;
	
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
		if(newi < 0 || newi >= GRID_WIDTH || newj < 0 || newj >= GRID_HEIGHT) return;
		
		if(testVector[c] == true && ( affMat->typeBefore == cellData[newi][newj] || affMat->typeBefore == M_dont_care) && ( affMat->satBefore == cellSat[newi][newj] || affMat->satBefore == M_dont_care) ){
			if(roll_ht(affMat->chance[c])){
				//change the material only if it needs changing.
				if(affMat->matAfter != M_no_change) cellData[newi][newj] = affMat->matAfter;
				//change the saturation only if it needs changing.
				if(affMat->satAfter != M_no_change) cellSat[newi][newj] = affMat->satAfter;
				
				//check to see if the original material will change because of it having completed an affectMat
				if(affMat->changeOrigMat != M_no_change) // if the material changes after it affects neighboring cells
					cellData[i][j] = affMat->changeOrigMat; // change the material
				if(affMat->changeOrigSat != M_no_change) // if the saturation of our material changes after our material affects neighboring cells
					cellSat[i][j] = affMat->changeOrigSat; // change the saturation of our material.
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
			if(cellData[i][j] == M_air) continue; // you don't need to print air. there is a black background being printed at the beginning of this print_cells() function.
			
			myRectangle.x = i*CELL_SIZE;
			myRectangle.y = j*CELL_SIZE;
            SDL_FillRect( screen , &myRectangle , mats[cellData[i][j]].color);
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

