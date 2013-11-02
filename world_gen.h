

// these are the different world types that can be generated.
// these values are not meant to be combined through bit-wise ORing.
#define w_normal			1
#define w_hellscape			2


// these are the different worlds flags that you can send the world_gen function.
// these flags should be powers of 2 and thus are meant to be bit-wise ORed together.
// for example: wf_rains

// no world flags yet  xD  i will have to make some

//#define wf_


/// this function will overwrite the data in grid[][] and generate a world inside it!
// send it a world type and a worldflag and it should 
void world_gen(int worldType, int worldFlag){
	/*
	//clear the grid
	reset_cells();
	//used for indexing through for loops
	int i,j;
	// this is where the normal world type is generated
	if(worldType == w_normal){
		/// generate rock and dirt lines.
		// this described the region in which the top of the rock exists.
		int rockline_min = 280;
		int rockline_max = 480;
		int rockline_height_previous;
		int rockline_height_current = (rockline_max+rockline_min)/2;
		int rockline_var = 3;
		int rockline_chance_of_major_break = 333; // 1/x chance of a  major break.
		int topearth_min = 3;
		int topearth_max = 7;
		int temp_rockHeight;
		
		for(i=0; i<GRID_WIDTH_ELEMENTS; i++){
			rockline_height_previous = rockline_height_current;
			rockline_height_current = get_rand(rockline_height_current - 3, rockline_height_current + 3);
			// make the rock
			for(j=GRID_HEIGHT_ELEMENTS-1; j>=GRID_HEIGHT_ELEMENTS-rockline_height_current; j--){
				grid[i][j].mat = m_rock;
			}
			
		}
		
	}
	*/
}


