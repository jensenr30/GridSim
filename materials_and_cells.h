///------------IMPORTANT TERMS----------------------------------------------
/// a CELL refers to a space in the GRID.
/// the GRID is the collection of cells in the application window.
/// a MATERIAL refers to what thing is occupying a certain cell.
/// for instance, you might fill a CELL with a MATERIAL.
/// you do NOT fill a METERIAL with a CELL. that just doesn't make any sense.
/// if you want to make a cell empty, that means you are filling it with AIR.


// this is how big each square cell is
int CELL_SIZE = 16;
#define MAX_CELL_SIZE 16
#define MIN_CELL_SIZE 1

/// this is how large the cell grid is INSIDE OF THE USER'S WINDOW
#define GRID_WIDTH (SCREEN_WIDTH/CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT/CELL_SIZE)

// these are the maximum dimensions inside of the user's window.
#define MAX_GRID_WIDTH  1920
#define MAX_GRID_HEIGHT 1080
// this is the max size the grid[][] material array can be. this should definitely be larger than MAX_GRID_WIDTH and MAX_GRID_HEIGHT.
#define GRID_WIDTH_ELEMENTS 1920
#define GRID_HEIGHT_ELEMENTS 1080

//this is how many different types of materials there can be in the game.
#define MAX_NUMBER_OF_UNIQUE_MATERIALS 100 // the materials (i.e. m_earth, m_spring) can have values from 0-99

struct cellData{
	short mat; // the material in a cell. 	// default to m_air
	//short sat; // the saturaiton of a cell. // default to m_no_saturation
	//char satLevel;  // the saturation level of a cell. i.e. how saturated is it? (integer range 1-8) default to 1
	
	//short matChange; // does the material need to change?
	//short satChange; // does the saturation need to change?
	//char satLevelChange; // does the saturationLevel need to change?
};
//this is the array of structures that the data for the grid is stored in.
// your entire map exists in here.
struct cellData grid[GRID_WIDTH_ELEMENTS][GRID_HEIGHT_ELEMENTS];


//this defines the material types. a material type is a
//		signed short
//negative values are not actual materials, but rather flags for conditions used in evaluating the grid.
//for instance, you can use mats[5] to get gunpowder data, or you can use mats[m_gunpowder] to get gunpowder data.
//this is just for ease of code writing.
#define m_any_of_my_sats	-4	// this is used when checking affectMat[].satNeeded to see if a material has ANY of its valid saturations. if it does have any, then evaluate_affectMaterial() will allow the affect to occur.
#define m_dont_care			-3  // this is used to show that we don't care what the material is.
#define m_no_saturation 	-2  // this is used to signify that a material in a cell has no saturation
#define m_no_change 		-1	// this material is more of a flag. It is used by the cell_engine in checking the changes to the cells in the grid.
#define m_air			0

/// materials you will see

#define m_earth			1

#define m_rock			3
#define m_rubble		4
#define m_sand			5
#define m_water			6
#define m_plant			7

#define m_fire			9
#define m_log			10
#define m_leaves		11

#define m_test			23
#define m_test2			24

/// non-menu materials

#define m_valid_but_null_material (MAX_NUMBER_OF_UNIQUE_MATERIALS-1)

// this has to be a complete list of all the saturatable materials in the game.
// this is used by the grid evaluator to check and apply saturations of the cells in our grid.
// these are the materials that will be checked for saturation
/// IF YOU MAKE A MATERIAL THAT CAN BE SATURATED WITH SOMETHING, ADD THAT MATERIAL TO THIS LIST.
short matSatOrder[MAX_NUMBER_OF_UNIQUE_MATERIALS];

//void set_chance(unsigned *, unsigned);
//void set_chance_symmetrical(unsigned *chance, unsigned c1, unsigned c02, unsigned c34, unsigned c57, unsigned c6);




///this is the data structure for materials:
struct material {
	
	// the color of the material
	unsigned int color;
	
	
	
	// 0 = no gravity. any other value means the object is subject to gravity.
	// POSITIVE numbers 1,2,3,... make the material need a steep slope to fall down.
	// for instance, if the material has a gravity of 2, that means the material must have an empty space two cells below it and one cell to the right or the left of it.
	// so relative coordinates would be y-2, x+-1
	//
	// NEGATIVE numbers -1,-2,-3,... make the material need at minimum a more gradual slope to fall down.
	// for instance, if the material has a gravity of -3, that means the material must have an empty space one cell beneath it and three blocks to the right or the left of it.
	// the relative coordinates would be y-1 x+-3
	//
	// of course, these two examples represent the minimum slope the material must have.
	// A material can travel down slopes that are steeper than it's minimum.
	// interestingly, a gravity value of 1 and -1 behave the same way.
	// it can help to mathematically think of the negative sign as instead the exponent of the gravity magnitude.
	// when there is a negative value, it is actually a -1 in the exponent of the number.
	// therefore, a gravity of -4 is actually a slope of 4^(-1) = 1/4.
	// isn't math fun?
	char gravity;
	
	// true or false: does this material have collision properties?
	bool collision;

	//material name
	char *name;

} mats[MAX_NUMBER_OF_UNIQUE_MATERIALS]; // this mats array holds all the different types of materials.



///this sets the default properties for all matss to default (air, basically)
///after this is done, the "init_material_attributes" specifies everything that needs to be specified.
///this is just here to be a catch all that assigns a value to everything.
///so basically, if you don't specify a material's behavior, it will basically be air.
///if you want to make a material, but you don't want it to appear in the menu, give it a NULL for its mats[].name value.
void set_default_material_attributes(){
	
	int i;
	//DEFAULT MATERIAL VALUES:
	for(i=0 ; i<MAX_NUMBER_OF_UNIQUE_MATERIALS ; i++){
        mats[i].name = NULL;			// default no name
		mats[i].gravity = 0;			// is not affected by gravity
		mats[i].color = 0x000000;		//default color is black
		mats[i].collision = false;		// default is a non-collision type material
	}
}

void init_material_attributes(void){
	
	// I don't need to specify anything for air because air doesn't do anything.
	// all of the elements of the mats[m_air] structure are initialized in the set_default_material_attributes() function.
	mats[m_air].name = "Air";
	///DON'T YOU DARE CHANGE ANYTHING ABOUT AIR! you SACK of SHIT!
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_earth].name = "Earth";
	mats[m_earth].color = 0x8b672d;
	mats[m_earth].collision = true;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_plant].name = "Plant";
    mats[m_plant].color = 0x008951;//0x20e112;
    mats[m_plant].collision = true;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_water].name = "Water";
	mats[m_water].gravity = -32;
    mats[m_water].color = 0x52a9e0;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_fire].name = "Fire";
    mats[m_fire].color = 0xd83313;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_test].name = "test";
	mats[m_test].color = 0xCCFF00;
	mats[m_test].gravity = 1;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_test2].name = "test2";
	mats[m_test2].color = 0x00FFCC;
	mats[m_test2].gravity = 2;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_rock].name = "Rock";
	mats[m_rock].color = 0x5A5651;
	mats[m_rock].collision = true;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_rubble].name = "Rubble";
	mats[m_rubble].color = 0x715A63;
	mats[m_rubble].gravity = 3;
	mats[m_rubble].collision = true;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_log].name = "Log";
	mats[m_log].color = 0x7b5126;
	mats[m_log].collision = true;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_sand].name = "Sand";
	mats[m_sand].gravity = -2;
	mats[m_sand].color = 0xcfc1aa;
	mats[m_sand].collision = true;
//-------------------------------------------------------------------------------------------------------------------------------
	
//-------------------------------------------------------------------------------------------------------------------------------
	
//-------------------------------------------------------------------------------------------------------------------------------
	
} // end init_material_attributes



///this function resets all the cells to default state. Nothing
void reset_cells(void){
	int i; int j;
	for(i=0 ; i<GRID_WIDTH_ELEMENTS ; i++){
		for(j=0 ; j<GRID_HEIGHT_ELEMENTS ; j++){
			grid[i][j].mat = m_air;
		}
	}
}



///this function is called when the program starts.
///It initializes everything that needs to be done to get the cell stuff working.
void init_cell_stuff(void){
	set_default_material_attributes();
	init_material_attributes();
	reset_cells();
}

/*
///this randomizes the materials and saturations in the grid.
///this basically randomizes cellMat[][] and cellSat[][].
void randomize_grid(){
	int i, j, temp;
	for(i=0 ; i<SCREEN_WIDTH ; i++){
		for(j=0 ; j<SCREEN_HEIGHT ; j++){
			if(get_rand(1,10) < 10){
				grid[i+camera_x][j+camera_y].mat = m_air;
				continue;
			}
			//get random material
			temp = m_valid_but_null_material;
			while(mats[temp].name == NULL){ // get a material that has a valid name
				temp = get_rand(0, MAX_NUMBER_OF_UNIQUE_MATERIALS-1);
			}
			grid[i+camera_x][j+camera_y].mat = temp;
		}
	}
}
*/
