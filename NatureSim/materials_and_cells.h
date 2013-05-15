///------------IMPORTANT TERMS----------------------------------------------
/// a CELL refers to a space in the GRID.
/// the GRID is the collection of cells in the application window.
/// a MATERIAL refers to what thing is occupying a certain cell.
/// for instance, you might fill a CELL with a MATERIAL.
/// you do NOT fill a METERIAL with a CELL. that just doesn't make any sense.
/// if you want to make a cell empty, that means you are filling it with AIR.


// this is how big each square cell is
#define CELL_SIZE 10

// this is how large the cell grid is.
#define CELL_WIDTH (SCREEN_WIDTH/CELL_SIZE)
#define CELL_HEIGHT (SCREEN_HEIGHT/CELL_SIZE)

//this is how many different types of materials there can be.
#define MAX_NUMBER_OF_UNIQUE_MATERIALS 100
//this is how many different interactions any given material can have with other materials in neighboring cells.
#define MAX_NUMBER_OF_MATERIAL_INTERACTIONS 10


//this array holds the data for each cell. An integer indicates what material is stored in that cell.
// cellData[0][0] refers to the top left cell (computer coordinates) NOT cartesian.
int cellData[CELL_WIDTH][CELL_HEIGHT];

//this defines the material types.
//for instance, you can use mats[5] to get gunpowder data, or you can use mats[M_gunpowder] to get gunpowder data.
//this is just for ease of code writing.
#define M_no_change -1  // this material is more of a flag. It is used by the cell_engine in checking the changes to the cells in the grid.
#define M_air 0
#define M_earth 1
#define M_grass 2
#define M_water 3
#define M_fire 4
#define M_gunpowder 5
#define M_cement 6
#define M_concrete 7
#define M_tree 8
#define M_animal 9
#define M_human 10



struct affectMaterial{
	short typeBefore; // the type of material will be affected by the material in the current cell.
	short typeAfter; // this is material that the above material will turn into.
	short chance[8]; // int value from 0-10000 describes the likelyhood of current material affecting the material in the cell next to it.
	//this is a diagram of how the numbers in the array correlate to the cells around material in the main cell (M)
	//  0 1 2
	//  3 M 4
	//  5 6 7
	// if we set  chance[2] = 5370;   then the material in the cell up and to the right from the main cell (M) has a 53.70% chance of being changed into typeAfter.
	// if we set  chance[6] = 0012;   then the material in the cell directly below the main cell (M) will have a 00.12% chance of being changed into typeAfter.
	// if we set  chance[3] = 10000;  then the material in the cell to the left of the main cell (M) will have a 100.00% chance of being changed into typeAfter.
};

///this is the data structure for materials:
struct material {

	// this is an array of affectMaterial structures.
	//Each element of the structure array describes one type of interaction this material can have with materials in neighboring cells.
	struct affectMaterial affected[MAX_NUMBER_OF_MATERIAL_INTERACTIONS];

	// the color of the material
	SDL_Color color;

	//value between 0 and 10000 describing the likelyhood of this material decaying on its own.
	// 467 would mean there is a 4.67% chance of decay on each evaluation cycle.
	int decayChance;
	//this is the type of material that the current material may decay into.
	int decayInto;

	// 0 = no gravity. 1 = material is subject to gravity.
	int gravity;

	//material name
	char *name;

} mats[MAX_NUMBER_OF_UNIQUE_MATERIALS]; // this mats array holds all the different types of materials.



///this sets the default properties for all matss to default (air, basically)
///after this is done, the "specify_material_attributes" specifies everything that needs to be specified.
///this is just here to be a catch all that assigns a value to everything.
///so basically, if you don't specify a material's behavior, it will basically be air.
void set_default_material_attributes(){
	//air, grass, fire, water, earth, gunpowder, cement, concrete, tree, animal, human
	int i,j,k;
	//DEFAULT MATERIAL VALUES:
	for(i=0 ; i<MAX_NUMBER_OF_UNIQUE_MATERIALS ; i++){
		mats[i].gravity = 0; // is not affected by gravity
		mats[i].color.r = 0;// red=0	default color is black
		mats[i].color.g = 0;// green=0
		mats[i].color.b = 0;// blue=0
		mats[i].decayChance = 0; // 0% chance to decay.
		mats[i].decayInto = 0;	 // decay into air (this is irrelevant because there is a 0% decayChance anyway)
		for(j=0 ; j<MAX_NUMBER_OF_MATERIAL_INTERACTIONS ; j++){
			mats[i].affected[j].typeBefore = M_air;// affects air
			mats[i].affected[j].typeAfter = M_air; // turns air into air
			for(k=0 ; k<8 ; k++){
				mats[i].affected[j].chance[k] = 0; // no chance of affecting anything (so it never affects air. it never will actually go through the process of turning air into air)
			}
		}
	}
}

void specify_material_attributes(void){

	mats[M_air].name = "Air (empty)";
	//everything else for air has already been

	mats[M_earth].name = "Earth";
	mats[M_earth].gravity = 0;

	mats[M_grass].name = "Grass";
	mats[M_grass].gravity = 0;

	mats[M_water].name = "Water";
	mats[M_water].gravity = 1;


	mats[M_fire].name = "Fire";
	mats[M_fire].gravity = 0;

}



///this function resets all the cells to default state. Nothing
void reset_cells(void){
	int i; int j;

	for(i=0 ; i<CELL_WIDTH ; i++){
		for(j=0 ; j<CELL_HEIGHT ; j++){
			cellData[i][j] = 0; // air
		}
	}
}



///this function is called when the program starts.
///It initializes everything that needs to be done to get the cell stuff working.
void init_cell_stuff(void){

	set_default_material_attributes();
	specify_material_attributes();
	reset_cells();
}
