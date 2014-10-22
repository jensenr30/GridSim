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

	// used to temporarily store the value of the cell's material
	int cMat;
	// used to temporarily store the value of the cell material's gravity
	short currentGrav;
	// used to temporarily store the value of the cell's saturation
	int currentSat;
	// used to temporarily store the value of the cell's weight
	int currentWeight;
	// used for indexing horizontally when a gravity material has to travel sideways (these are used in the gravity slope portions where the materials fall down slopes).
	// jg is for steep slopes (positive gravity)
	// ig is used for gentle slopes (negative gravity)
	short jg, ig;
	// the holdOff variables are used to police the movement of materials. without them, shit looks really weird.
	// these variables keep track of how long ago materials were moved around and it tells the program how soon it can move materials in the same direction again.
	short holdOffRight = 0;
	short holdOffLeft = 0;
	// these are for storing which direction the material wants to fall
	char moveRight;
	char moveLeft;
	// this stores whether or not the material can flow through similar materials.
	char flowThrough;
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
	// these are true or false values that tell us if we have found valid length_of_mat_right and length_of_mat_left variables.
	bool found_length_of_mat_right;
	bool found_length_of_mat_left;
	
	// these are used to temporarily store data about cells.
	short tempMat, tempSat;
	char tempSatLevel;
	
	/// 1.1 GRAVITY AND SLOPES
	
	for(j=GRID_HEIGHT+camera_y-1; j>=camera_y; j--){ // cycle through the rows
		// make sure the indexes are not out of bounds
		if(j < 0){
			#if(DEBUG_GRIDSIM)
				printf("function evaluate_grid():\nError: j is outside of bounds.\nj = %d\n\n",j);
			#endif
			j = 0;
		}
		else if(j >= GRID_HEIGHT_ELEMENTS){
			#if(DEBUG_GRIDSIM)
				printf("function evaluate_grid():\nError: j is outside of bounds.\nj = %d\n\n",j);
			#endif
			break;
		}
		for(i=camera_x; i<GRID_WIDTH+camera_x; i++){ // cycle through each column for a single row
			
			// make sure the indexes are not out of bounds
			if(i < 0){
				#if(DEBUG_GRIDSIM)
					printf("function evaluate_grid():\nError: i is outside of bounds.\ni = %d\n\n",i);
				#endif
				i = 0;
			}
			else if(i >= GRID_WIDTH_ELEMENTS){
				#if(DEBUG_GRIDSIM)
					printf("function evaluate_grid():\nError: i is outside of bounds.\ni = %d\n\n",i);
				#endif
				break;
			}
			
			// if the current cell is air, just skip ahead to the next one.
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
			currentWeight = mats[cMat].weight;
			
			//this decrements the holdoff mechanism. it acts as a way to police excessive material motion.
			if(holdOffLeft>0)holdOffLeft--;
			else holdOffLeft = 0;
			if(holdOffRight>0)holdOffRight--;
			else holdOffRight = 0;
			
			
			// if this material is affected by gravity
			if(currentGrav){
				
				/// materials DON'T fall out of the bottom of the screen
				if(j > GRID_HEIGHT+camera_y - 2){
					continue;
					//grid[i][j].mat = m_air;
					//grid[i][j].sat = m_no_saturation; // remove saturaiton of material once it falls out of the screen.
				}
				/// material falls a single cell
				else if(mats[grid[i][j].mat].weight > mats[grid[i][j+1].mat].weight && (mats[grid[i][j+1].mat].gravity || grid[i][j+1].mat == m_air) ){
					// store the below cell's mat and sat
					tempMat = grid[i][j+1].mat;
					tempSat = grid[i][j+1].sat;
					// transfer below cell's mat and sat to the current cell
					grid[i][j+1].mat = cMat;
					grid[i][j+1].sat = currentSat;
					// save current cell's original contents with the lower cell's original content.
					grid[i][j].mat = tempMat;
					grid[i][j].sat = tempSat;
					
					// what is this for? I think I remember deleting it and it broke everything.
					holdOffLeft = 2; // is this really necessary? jesus. why is this necessary...?
				}
				
				/// SEE IF THE MATERIAL WILL FALL DOWN A SLOPE
				else{
					
					// if the gravity is a steep slope (steeper than 1:1)
					if(currentGrav > 0){
						// set to false initially (they will be tested for true)
						moveLeft = 0;
						moveRight = 0;
						// set true initially (will be tested for false)
						flowThrough = 1;
						// check for material-flow through.
						for(jg=1; jg<=currentGrav && jg<GRID_HEIGHT+camera_y; jg++)
						{
							if(cMat != grid[i][jg].mat)
							{
								flowThrough = 0;
								break;
							}
						}
						// at this point, we know that the current material has jg-1 cells of the same material underneath it.
						
						// if there is enough material beneath the cell to constitute flow-through
						if(flowThrough)
						{
							if(i < camera_x + GRID_WIDTH - 1)
							{
								if(mats[grid[i+1][jg].mat].weight < currentWeight)
								{
									moveRight = 1;
								}
							}
							if(i > camera_x)
							{
								if(mats[grid[i-1][jg].mat].weight < currentWeight)
								{
									moveLeft = 1;
								}
							}
							if(moveRight && moveLeft)
							{
								if(get_rand(0,1))
									moveLeft = 0;
								else
									moveRight = 0;
							}
							
							if(moveRight)
							{
								// temporarily save data from lower cell
								tempMat = grid[i-1][jg].mat;
								tempSat = grid[i-1][jg].sat;
								// move current cell into new lower cell
								grid[i-1][jg].mat = cMat;
								grid[i-1][jg].sat = currentSat;
								// move lower cell's original data into current cell
								grid[i][j].mat = tempMat;
								grid[i][j].sat = tempSat;
							}
							if(moveLeft)
							{
								// temporarily save data from lower cell
								tempMat = grid[i+1][jg].mat;
								tempSat = grid[i+1][jg].sat;
								// move current cell into new lower cell
								grid[i+1][jg].mat = cMat;
								grid[i+1][jg].sat = currentSat;
								// move lower cell's original data into current cell
								grid[i][j].mat = tempMat;
								grid[i][j].sat = tempSat;
							}
							
						}
						
					}
					// if the gravity of this material is a gradual slope (less steep than 1:1)
					else{
						
					}
				}
				
				
				
			} // if( gravity )
		} // for i
	} // for j
	
	
	/*
	/// 2.2 WEIGHT EVALUATION
	for(i=camera_x ; i<GRID_WIDTH+camera_x && i<GRID_WIDTH_ELEMENTS ; i++){
		//for(j=camera_y ; j<GRID_HEIGHT+camera_y && j<GRID_HEIGHT_ELEMENTS; j++){
		
		// set j to the proper index
		j = camera_y + GRID_HEIGHT - 1;
		// if j is at or below the lowest cell of the global grid,
		if(j> GRID_HEIGHT_ELEMENTS-1){
			// set j to be 1 cell up from the bottom of the global grid
			j = GRID_HEIGHT_ELEMENTS-1;
		}
		for(; j>=camera_y+1 && j>=1; j--){
		
			
			// we don't need to concern ourselves with air.
			if(grid[i][j].mat == m_air) continue;
			// if the material is not affected by gravity, we need not concern ourselves with it.
			if(!mats[grid[i][j].mat].gravity) continue;
			
			// if the material above the current one weighs more, then switch the positions of the two materials.
			if(mats[grid[i][j].mat].weight < mats[grid[i][j-1].mat].weight){
				
				// temporarily store the current cell's data
				tempMat = grid[i][j].mat;
				tempSat = grid[i][j].sat;
				tempSatLevel = grid[i][j].satLevel;
				
				// move the above cell's data into the current cell
				grid[i][j].mat = grid[i][j-1].mat;
				grid[i][j].sat = grid[i][j-1].sat;
				grid[i][j].satLevel = grid[i][j-1].satLevel;
				
				// copy temporary stored data from original state of current cell into above cell
				grid[i][j-1].mat = tempMat;
				grid[i][j-1].sat = tempSat;
				grid[i][j-1].satLevel = tempSatLevel;
				
				j--;
			}
			
			
		}
	}
	*/
	
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
					if( (grid[newi][newj].mat == mats[cMat].satEffect[satEffIndex].satMat) || (grid[newi][newj].mat != m_air && mats[cMat].satEffect[satEffIndex].satMat == m_not_air) ){
						
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
					if(mats[cMat].satEffect[satEffIndex].satMat == grid[i][j].sat || (mats[cMat].satEffect[satEffIndex].satMat == m_not_air && grid[i][j].sat != m_air) ){ // if this is the right saturation
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
		
			// if there is a valid material, or if you can use any material, OR if you can use any material BUT air...
			if( affMat->matBefore == grid[newi][newj].mat || affMat->matBefore == m_dont_care || (affMat->matBefore == m_not_air && grid[newi][newj].mat != m_air)){
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

