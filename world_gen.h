
#define SHOW_WORLD_GEN 1

// these are the different world types that can be generated.
// these values are not meant to be combined through bit-wise ORing.
#define w_normal			1
#define w_hellscape			2


// these are the different worlds flags that you can send the world_gen function.
// these flags should be powers of 2 and thus are meant to be bit-wise ORed together.
// for example: wf_rains

// no world flags yet  xD  i will have to make some

//#define wf_


/// recursive function that generates a landscape inside an array
// the top_material is an array to a pointer that tell you the height of the landscape at any given point.
// the function will evaluate from lowIndex to highIndex in the top_material[] array.
// landscape material is only a necessary variable if you plan on showing the creation of the landscape.
// if you don't care about visually showing the landscape being built, then just send landscapeMaterial anything (e.g. m_air, m_earth, m_mud, etc...)
void gen_landscape(int *top_material, int lowIndex, int highIndex, int matLowBound, int matHighBound, float slope, int landscapeMaterial){

	// stop evaluating if there is no space between the lowIndex and the highIndex (or if, for some crazy reason, the lowIndex because HIGHER than the highIndex...)
	if(lowIndex+1 >= highIndex) return;
	// find the middleIndex between the low and high Indexes.
	int middleIndex = (lowIndex+highIndex)/2;

	//calculate the value at the midpoint
	top_material[middleIndex] = (top_material[lowIndex] + top_material[highIndex])/2;		// take the average value between the indexes (linear interpolation)
	top_material[middleIndex] += get_rand((int)(((float)(lowIndex-highIndex)*slope)/2.0), (int)(((float)(highIndex-lowIndex)*slope)/2.0f));		// generate some variance to the average value between the indexes.

	// if the new top_material value is out of bounds, try again
	if(top_material[middleIndex] >= matHighBound || top_material[middleIndex] < matLowBound){
		gen_landscape(top_material, lowIndex, highIndex, matLowBound, matHighBound, slope, landscapeMaterial);
		return;
	}
	// if the user wants to visually see the world getting built, then so be it.
	/* THIS IS THE OLD WAY OF DISPLAYING THE GENERATION OF THE WORLD
	#if(SHOW_WORLD_GEN)
	grid[middleIndex][GRID_HEIGHT_ELEMENTS-1-top_material[middleIndex]].mat = landscapeMaterial;
	if(highIndex-lowIndex > GRID_WIDTH_ELEMENTS/64){
		print_cells();
		SDL_Flip(screen);
	}
	#endif // SHOW_WORLD_GEN
	*/
	//now recursively preform the function on the two new midpoints
	gen_landscape(top_material, lowIndex, middleIndex, matLowBound, matHighBound, slope, landscapeMaterial);
	gen_landscape(top_material, middleIndex, highIndex, matLowBound, matHighBound, slope, landscapeMaterial);
}


void gen_landscape_relative(int *top_material, int top_material_relative, int lowIndex, int highIndex, int matLowBoundRelative, int matHighBoundRelative, int slope){

}


/// this function will overwrite the data in grid[][] and generate a world inside it!
// send it a world type and a worldflag and it should
void gen_world(int worldType, int worldFlag){
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

	// initial settings. zoom all the way out and check out the map in all its glory
	paused = 1;
	CELL_SIZE = 1;
	verify_grid_and_cell_size();
	camera_x = MAX_camera_x;
	camera_y = MAX_camera_y;
	verify_camera();

	//clean grid
	reset_cells();

	int i,j;
	/// all of these numbers are relative to the bottom cell in the grid (grid[i][GRID_WIDTH_ELEMENTS-1])

	// these just tell us the height band of where top of the rock layer can be generated
	// rockline_max is exclusive, rockline_min is inclusive. the domain would be [rockling_min,rockline_max) for integer numbers.
	int rockline_max=500, rockline_min=200;
	// this is an array that tells us for myColumn, the highest point where there is rock = top_rock[myColumn]
	int top_rock[GRID_WIDTH_ELEMENTS];
	// the larger the slope is,  the more steep and sharp the slopes can be.
	// the smaller the slope is, the smoother and duller the landscape will be.
	float rock_slope = 0.9;

	//set default values to top_rock array (this is just so that the elements are not left un-initialized. they should all be overwritten
	for(i=0; i<GRID_WIDTH_ELEMENTS; i++){
		top_rock[i] = 0; // default value. this will get overwritten.
	}

	//get initial points at the left and right sides of the map
	top_rock[0]						= get_rand(rockline_min, rockline_max-1);
	top_rock[GRID_WIDTH_ELEMENTS-1]	= get_rand(rockline_min, rockline_max-1);

	// generate the top_rock array
	gen_landscape(top_rock, 0, GRID_WIDTH_ELEMENTS-1, rockline_min, rockline_max, rock_slope, m_rock);

	/// this only works (well or at all) with a screen width of 1080 pixels
	#if(SHOW_WORLD_GEN)
	SDL_Rect screenRect;
	screenRect.x = screenRect.y = 0;
	screenRect.w = SCREEN_WIDTH;
	screenRect.h = SCREEN_HEIGHT;
	// dist is the distance between the sample points in the grid[][] array
	int sections;

	for(sections = 1; sections<=SCREEN_WIDTH/2; sections<<=1){
		//blank screen
		SDL_FillRect(screen, &screenRect, 0x000000);
		//draw_line(screen, 0,GRID_HEIGHT_ELEMENTS-1-top_rock[0], GRID_WIDTH_ELEMENTS/sections, GRID_HEIGHT_ELEMENTS-1-top_rock[GRID_WIDTH_ELEMENTS/sections], 1, mats[m_rock].color);
		for(i=0; i<sections; i++){
			draw_line(screen, i*SCREEN_WIDTH/sections,  SCREEN_HEIGHT-1-top_rock[i*SCREEN_WIDTH/sections],  (i+1)*(SCREEN_WIDTH-1)/sections,  SCREEN_HEIGHT-1-top_rock[(i+1)*(SCREEN_WIDTH-1)/sections],  1, 0x0bf100);
			draw_line(screen, i*SCREEN_WIDTH/sections,  SCREEN_HEIGHT-1-top_rock[i*SCREEN_WIDTH/sections]-4,  (i+1)*(SCREEN_WIDTH-1)/sections,  SCREEN_HEIGHT-1-top_rock[(i+1)*(SCREEN_WIDTH-1)/sections]-4,  1, 0x0bf100);
		}

		// display the lines that were just printed
		//SDL_Flip(screen);
		SDL_RenderPresent(screen);
		// wait for a bit
		SDL_Delay(500);
	}
	#endif


	//fill up the rock area.
	for(i=0; i<GRID_WIDTH_ELEMENTS; i++){
		for(j=GRID_HEIGHT_ELEMENTS-top_rock[i]-1; j<GRID_HEIGHT_ELEMENTS; j++){
			grid[i][j].mat = m_rock;
		}
	}

	int top_earth[GRID_WIDTH_ELEMENTS]; // this keeps track of the top layer of earth.

	int earth_average_above_rock = 4; // average of 4 blocks of dirt above rock.

	for(i=0; i<GRID_WIDTH_ELEMENTS; i++){
		for(j=GRID_HEIGHT_ELEMENTS-top_rock[i]-2; j>=GRID_HEIGHT_ELEMENTS-top_rock[i]-2-earth_average_above_rock; j--){
			grid[i][j].mat = m_earth;
		}
	}

}





