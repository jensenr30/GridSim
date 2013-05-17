/// this document contains the necessary functions to evaluate and advance the simulation.
/// It looks at the materials in the cells and then updates the cells based on the dynamics of the materials described in materials_and_cells.h


// this will evaluate the grid. It will run a simulation for the number of generations you tell it to.
void evaluate_cells(){
	
	int i,j,a,c; // i = x position in grid. j = y position in grid. k = material type being evaluated. a = type of block being affected. c = which cell around the block is being checked
	// used for checking how many times gravity has been evaluated. I need to run through checking each row of cells twice because of stupid logistics
	int checkGravCnt;
	
	// this array is used to log the changes that will need to be made to the cells in the grid.
	// I apply the changes that need to be made ONLY AFTER I have evaluated all the cells in the grid.
	// I do this so that the evaluation is not partial to things that are evaluated earlier than others.
	// i.e. if two materials destroy each other when they meet, then if one of them is evaluated first AND the effects it produces are applied to cellData right away, then the other material will not be able to be evaluated and thus the two materials will not destroy each other as was intended.
	// Using an array to log the changes that need to be made to the cells in the grid is a pretty good way of going abou this problem I think.
	short cellChanges[GRID_WIDTH][GRID_HEIGHT];
	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			cellChanges[i][j] = M_no_change;
		}
	}
	
	/// this is where the gravity gets checked
	for(j=GRID_HEIGHT-1 ; j>=0 ; j--){
		for(i=0 ; i<GRID_WIDTH ; i++){
				
			// if gravity affects this material...
			if(mats[cellData[i][j]].gravity){
				//dissapears through the bottom
				if(j >= GRID_HEIGHT-1){ // if the gravity block is at the bottom of the screen, get rid of it.
					cellData[i][j] = M_air;
				}
				// falls down
				else if(cellChanges[i][j+1] == M_air || cellData[i][j+1] == M_air){
					cellData[i][j+1] = cellData[i][j];
					cellData[i][j] = M_air;
				}
				
				// if the material could EITHER fall to the LEFT or the RIGHT
				else if( cellData[i-1][j] == M_air && cellData[i-1][j+1] == M_air   &&   cellData[i+1][j] == M_air && cellData[i+1][j+1] == M_air ){
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
				else if(cellData[i-1][j] == M_air && cellData[i-1][j+1] == M_air){
					cellData[i-1][j+1] = cellData[i][j]; // go to the left or...
					cellData[i][j] = M_air;
				}
				//if the material can ONLY fall the the RIGHT
				else if(cellData[i+1][j] == M_air && cellData[i+1][j+1] == M_air){
					cellData[i+1][j+1] = cellData[i][j]; // go to the right.
					cellData[i][j] = M_air;
				}
			}
		}
	}
	
	///this giant-ass for loop is where we find out which cells need to be changed. DECAY AND AFFECTS.
	for(i=0 ; i<GRID_WIDTH ; i++){
		
		for(j=0 ; j<GRID_HEIGHT ; j++){
			
			if(roll_ht( mats[ cellData[i][j] ].decayChance) ) cellData[i][j] = mats[ cellData[i][j] ].decayInto; // if, by chance, it is time to decay, then decay into your proper type.
			for(a=0 ; a<MAX_NUMBER_OF_MATERIAL_INTERACTIONS; a++){ // check all the possible interactions
				
				if(mats[ cellData[i][j] ].affected[a].typeBefore == M_air && mats[ cellData[i][j] ].affected[a].typeAfter == M_air) break; // air becoming air. stop checking material affects. there are no more.
				for(c=0 ; c<8 ; c++){
					
					//this is a diagram of how the numbers in the chance array correlate to the cells around material in the main cell (M)
					//  0 1 2
					//  3 M 4
					//  5 6 7
					
					switch(c){
					case 0:
						// if the material is the right one to be changed AND if the roll comes back good
						if(  mats[ cellData[i][j] ].affected[a].typeBefore == cellData[i-1][j-1]  &&  roll_ht( mats[ cellData[i][j] ].affected[a].chance[c] ) && i>0 && j>0 ){
							//then change that cell to the material type after the cell affects the neighboring cell.
							cellChanges[i-1][j-1] = mats[ cellData[i][j] ].affected[a].typeAfter;
						}
						break;
					case 1:
						// all of the other cases work very similarly.
						if(  mats[ cellData[i][j] ].affected[a].typeBefore == cellData[i][j-1]  &&  roll_ht( mats[ cellData[i][j] ].affected[a].chance[c] ) && j>0 ){
							cellChanges[i][j-1] = mats[ cellData[i][j] ].affected[a].typeAfter;
						}
						break;
					case 2:
						if(  mats[ cellData[i][j] ].affected[a].typeBefore == cellData[i+1][j-1]  &&  roll_ht( mats[ cellData[i][j] ].affected[a].chance[c] ) && i<(GRID_WIDTH-1) && j>0 ){
							cellChanges[i+1][j-1] = mats[ cellData[i][j] ].affected[a].typeAfter;
						}
						break;
					case 3:
						if(  mats[ cellData[i][j] ].affected[a].typeBefore == cellData[i-1][j]  &&  roll_ht( mats[ cellData[i][j] ].affected[a].chance[c] ) && i>0 ){
							cellChanges[i-1][j] = mats[ cellData[i][j] ].affected[a].typeAfter;
						}
						break;
					case 4:
						if(  mats[ cellData[i][j] ].affected[a].typeBefore == cellData[i+1][j]  &&  roll_ht( mats[ cellData[i][j] ].affected[a].chance[c] ) && i<(GRID_WIDTH-1) ){
							cellChanges[i+1][j] = mats[ cellData[i][j] ].affected[a].typeAfter;
						}
						break;
					case 5:
						if(  mats[ cellData[i][j] ].affected[a].typeBefore == cellData[i-1][j+1]  &&  roll_ht( mats[ cellData[i][j] ].affected[a].chance[c] ) && i>0 && j<(GRID_HEIGHT-1) ){
							cellChanges[i-1][j+1] = mats[ cellData[i][j] ].affected[a].typeAfter;
						}
						break;
					case 6:
						if(  mats[ cellData[i][j] ].affected[a].typeBefore == cellData[i][j+1]  &&  roll_ht( mats[ cellData[i][j] ].affected[a].chance[c] ) && j<(GRID_HEIGHT-1) ){
							cellChanges[i][j+1] = mats[ cellData[i][j] ].affected[a].typeAfter;
						}
						break;
					case 7:
						if(  mats[ cellData[i][j] ].affected[a].typeBefore == cellData[i+1][j+1]  &&  roll_ht( mats[ cellData[i][j] ].affected[a].chance[c] ) && i<(GRID_WIDTH-1) && j<(GRID_HEIGHT-1) ){
							cellChanges[i+1][j+1] = mats[ cellData[i][j] ].affected[a].typeAfter;
						}
						break;
					default:
						break;
					
					}//end switch
				}
			}
		}
	}
	
	/// this is where we use the results collected in the cellChanged array to modify the cellData array.
	
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
