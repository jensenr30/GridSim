///------------IMPORTANT TERMS----------------------------------------------
/// a CELL refers to a space in the GRID.
/// the GRID is the collection of cells in the application window.
/// a MATERIAL refers to what thing is occupying a certain cell.
/// for instance, you might fill a CELL with a MATERIAL.
/// you do NOT fill a METERIAL with a CELL. that just doesn't make any sense.
/// if you want to make a cell empty, that means you are filling it with AIR.


// this is how big each square cell is
int CELL_SIZE = 8;

// this is how large the cell grid is.
#define GRID_WIDTH SCREEN_WIDTH/CELL_SIZE
#define GRID_HEIGHT SCREEN_HEIGHT/CELL_SIZE

//this is how many different types of materials there can be.
#define MAX_NUMBER_OF_UNIQUE_MATERIALS 100
//this is how many different interactions any given material can have with other materials in neighboring cells.
#define MAX_NUMBER_OF_MATERIAL_INTERACTIONS 4
//this is how many different things a given material can be saturated with
#define MAX_NUMBER_OF_SATURATION_EFFECTS 3
//when a material is saturated with some other material, this is how many affects the block can have based on that particular saturation.
//e.g. is grass is saturaed with fire, it may affect at most this many other material types:
#define MAX_NUMBER_OF_SATURATION_EFFECT_INTERACTIONS 2


//this array holds the data for each cell. An integer indicates what material is stored in that cell.
// cellData[0][0] refers to the top left cell (computer coordinates) NOT cartesian.
short cellData[SCREEN_WIDTH][SCREEN_HEIGHT];
//this is what modifier the materials in each cell have. it is soaked? (modified by water) is it on fire? (modified by fire?)
// if it is soaked, then cellMod = M_water. If it is on fire, then cellMod= M_fire.
// being saturated with air does not mean the same thing as being saturated with nothing.
// if there is no saturation, then use M_no_saturation.
short cellMod[SCREEN_WIDTH][SCREEN_HEIGHT];

//this defines the material types. a material type is a
//		signed short
//negative values are not actual materials, but rather flags for conditions used in evaluating the grid.
//for instance, you can use mats[5] to get gunpowder data, or you can use mats[M_gunpowder] to get gunpowder data.
//this is just for ease of code writing.
#define M_no_saturation -3  // this is used to signify that a block has no saturation
#define M_no_modifier	-2	// this is used to flag that there is no modifier on the block.
#define M_no_change 	-1	// this material is more of a flag. It is used by the cell_engine in checking the changes to the cells in the grid.
#define M_air			0
#define M_earth			1
#define M_grass			2
#define M_water			3
#define M_fire			4
#define M_gunpowder		5
#define M_cement		6
#define M_concrete		7
#define M_tree			8
#define M_animal		9
#define M_human			10
#define M_rock			11
#define M_spring		12



struct affectMaterial{
	short typeBefore; // the type of material will be affected by the material in the current cell.
	short typeAfter; // this is material that the above material will turn into.
	unsigned chance[8]; // int value from 0-100000 describes the likelyhood of current material affecting the material in the cell next to it.
	// = 0 never happens. 100000 = always happens.
	//this is a diagram of how the numbers in the chance array correlate to the cells around material in the main cell (M)
	//  0 1 2
	//  3 M 4
	//  5 6 7
	// if we set  chance[2] =  53700;   then the material in the cell up and to the right from the main cell (M) has a 53.700% chance of being changed into typeAfter.
	// if we set  chance[6] =     12;   then the material in the cell directly below the main cell (M) will have a 0.012% chance of being changed into typeAfter.
	// if we set  chance[3] = 100000;  then the material in the cell to the left of the main cell (M) will have a 100.000% chance of being changed into typeAfter.
};


struct saturationEffect{
	//this is some material that is saturating our material.
	short satMat;
	
	// does the saturation material get absorbed (i.e. turn into air)? or does it not get absorbed?
	//1 = gets absorbed
	//0 = doesn't get absorbed
	unsigned short absorb;
	
	//the chance that our saturated material will decay into something else.
	//from 0-100000
	unsigned decayChance;
	//the thing that our saturated material may decay into now that it is saturated.
	short decayInto;
	
	//these are other materials that our saturated material can affect now that it is saturated.
	struct affectMaterial affectMat[MAX_NUMBER_OF_SATURATION_EFFECT_INTERACTIONS];
};


///this is the data structure for materials:
struct material {

	// this is an array of affectMaterial structures.
	//Each element of the structure array describes one type of interaction this material can have with materials in neighboring cells.
	struct affectMaterial affectMat[MAX_NUMBER_OF_MATERIAL_INTERACTIONS];
	
	//this is an array (list) of things that can saturate this material and how it affects it.
	struct saturationEffect satEffect[MAX_NUMBER_OF_SATURATION_EFFECTS];
	
	// the color of the material
	unsigned int color;

	//value between 0 and 100000 describing the likelyhood of this material decaying on its own.
	// 467 would mean there is a 0.467% chance of decay on each evaluation cycle.
	unsigned decayChance;
	//this is the type of material that the current material may decay into.
	short decayInto;

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
	int i,j,k,l;
	
	//DEFAULT MATERIAL VALUES:
	for(i=0 ; i<MAX_NUMBER_OF_UNIQUE_MATERIALS ; i++){
		mats[i].gravity = 0; // is not affected by gravity
		mats[i].color = 0x000000;//default color is black
		mats[i].decayChance = 0; // 0% chance to decay.
		mats[i].decayInto = M_air;	 // decay into air (this is irrelevant because there is a 0% decayChance anyway)
		
		 // for every saturation effect, set it to the default of not being able to be saturated by anything.
		for(j=0 ; j<MAX_NUMBER_OF_SATURATION_EFFECTS ; j++){
			mats[i].satEffect[j].satMat = M_no_saturation; // by default, nothing can be saturated with anything.
			mats[i].satEffect[j].absorb = 0; // does not absorb by default.
			mats[i].satEffect[j].decayChance = 0; // be default, nothing saturated will decay into anything.
			mats[i].satEffect[j].decayInto = M_air; // default decay into air. kind of irrelevant because the chance of decay is already 0. oh well. better safe than sorry.
			
			for(k=0; k<MAX_NUMBER_OF_SATURATION_EFFECT_INTERACTIONS ; k++){ // for every interaction a saturated material can have with another material.
				mats[i].satEffect[j].affectMat[k].typeBefore = M_air;// affects air
				mats[i].satEffect[j].affectMat[k].typeAfter = M_air; // turns air into air
				
				for(l=0 ; l<8 ; l++){
					mats[i].affectMat[k].chance[l] = 0; // no chance of affecting anything (so it never affects air. it never will actually go through the process of turning air into air)
				}
			}
		}
		
		// for every affect that our material can have on other materials, set it to default (default = air changes to air with a 0% chance. nothing happens.)
		for(k=0 ; k<MAX_NUMBER_OF_MATERIAL_INTERACTIONS ; k++){
			mats[i].affectMat[k].typeBefore = M_air;// affects air
			mats[i].affectMat[k].typeAfter = M_air; // turns air into air
			for(l=0 ; l<8 ; l++){
				mats[i].affectMat[k].chance[l] = 0; // no chance of affecting anything (so it never affects air. it never will actually go through the process of turning air into air)
			}
		}
	}
}

void specify_material_attributes(void){

	mats[M_air].name = "Air (empty)";
	//everything else for air has already been

	mats[M_earth].name = "Earth";
	mats[M_earth].decayInto = M_rock;
	mats[M_earth].color = 0x885607;
	mats[M_earth].decayChance = 1;

	mats[M_grass].name = "Grass";
    mats[M_grass].color = 0x20e112;
    mats[M_grass].affectMat[0].typeBefore = M_water;
	mats[M_grass].affectMat[0].typeAfter  = M_grass;
	mats[M_grass].affectMat[0].chance[0] = 1000;
	mats[M_grass].affectMat[0].chance[1] = 1000;
	mats[M_grass].affectMat[0].chance[2] = 1000;
	mats[M_grass].affectMat[0].chance[3] = 1000;
	mats[M_grass].affectMat[0].chance[4] = 1000;
	mats[M_grass].affectMat[0].chance[5] = 1000;
	mats[M_grass].affectMat[0].chance[6] = 1000;
	mats[M_grass].affectMat[0].chance[7] = 1000;

	mats[M_water].name = "Water";
	mats[M_water].gravity = 1;
    mats[M_water].color = 0x158ad4;

    mats[M_spring].name = "Spring";
	mats[M_spring].color = 0x97bcbb;
	mats[M_spring].affectMat[0].typeBefore = M_air;
	mats[M_spring].affectMat[0].typeAfter = M_water;
	mats[M_spring].affectMat[0].chance[0] = 0;
	mats[M_spring].affectMat[0].chance[1] = 0;
	mats[M_spring].affectMat[0].chance[2] = 0;
	mats[M_spring].affectMat[0].chance[3] = 300;
	mats[M_spring].affectMat[0].chance[4] = 300;
	mats[M_spring].affectMat[0].chance[5] = 700;
	mats[M_spring].affectMat[0].chance[6] = 1000;
	mats[M_spring].affectMat[0].chance[7] = 700;

	mats[M_fire].name = "Fire";
	mats[M_fire].decayInto = M_air;
    mats[M_fire].color = 0xd83313;
	mats[M_fire].decayChance = 9000;
	mats[M_fire].affectMat[0].typeBefore = M_grass;
	mats[M_fire].affectMat[0].typeAfter  = M_fire;
	mats[M_fire].affectMat[0].chance[0] = 12000;
	mats[M_fire].affectMat[0].chance[1] = 15000;
	mats[M_fire].affectMat[0].chance[2] = 12000;
	mats[M_fire].affectMat[0].chance[3] =  9000;
	mats[M_fire].affectMat[0].chance[4] =  9000;
	mats[M_fire].affectMat[0].chance[5] =  4500;
	mats[M_fire].affectMat[0].chance[6] =  6500;
	mats[M_fire].affectMat[0].chance[7] =  4500;

	mats[M_rock].name = "Rock";
	mats[M_rock].color = 0x5a5651;


}



///this function resets all the cells to default state. Nothing
void reset_cells(void){
	int i; int j;

	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			cellData[i][j] = 0; // air
			cellMod[i][j]  = M_no_modifier;
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
