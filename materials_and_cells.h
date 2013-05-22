///------------IMPORTANT TERMS----------------------------------------------
/// a CELL refers to a space in the GRID.
/// the GRID is the collection of cells in the application window.
/// a MATERIAL refers to what thing is occupying a certain cell.
/// for instance, you might fill a CELL with a MATERIAL.
/// you do NOT fill a METERIAL with a CELL. that just doesn't make any sense.
/// if you want to make a cell empty, that means you are filling it with AIR.



// this is how big each square cell is
int CELL_SIZE = 16;

// this is how large the cell grid is.
#define GRID_WIDTH SCREEN_WIDTH/CELL_SIZE
#define GRID_HEIGHT SCREEN_HEIGHT/CELL_SIZE

//this is how many different types of materials there can be in the game.
#define MAX_NUMBER_OF_UNIQUE_MATERIALS 100 // the materials (i.e. M_earth, M_spring) can have values from 0-99

//this holds the number of unique materials that have the ability to be saturated by something.
// this value is detected and set in the set_default_material_attributes() function
short numberOfMaterialsThatCanBeSaturated;

//this is how many different interactions any given material can have with other materials in neighboring cells.
#define MAX_NUMBER_OF_MATERIAL_INTERACTIONS 4
//this is how many different things a given material can be saturated with
#define MAX_NUMBER_OF_SATURATION_EFFECTS 3
//when a material is saturated with some other material, this is how many affects the material can have based on that particular saturation.
//e.g. is grass is saturaed with fire, it may affect at most this many other material types:
#define MAX_NUMBER_OF_SATURATION_EFFECT_INTERACTIONS 2


//this array holds the data for each cell. An integer indicates what material is stored in that cell.
// cellData[0][0] refers to the top left cell (computer coordinates) NOT cartesian.
short cellData[SCREEN_WIDTH][SCREEN_HEIGHT];
//this is what modifier the materials in each cell have. it is soaked? (modified by water) is it on fire? (modified by fire?)
// if it is soaked, then cellSat = M_water. If it is on fire, then cellSat= M_fire.
// being saturated with air does not mean the same thing as being saturated with nothing.
// if there is no saturation, then use M_no_saturation.
short cellSat[SCREEN_WIDTH][SCREEN_HEIGHT];

//this defines the material types. a material type is a
//		signed short
//negative values are not actual materials, but rather flags for conditions used in evaluating the grid.
//for instance, you can use mats[5] to get gunpowder data, or you can use mats[M_gunpowder] to get gunpowder data.
//this is just for ease of code writing.
#define M_dont_care		-3  // this is used to show that we don't care what the material is.
#define M_no_saturation -2  // this is used to signify that a material in a cell has no saturation
#define M_no_change 	-1	// this material is more of a flag. It is used by the cell_engine in checking the changes to the cells in the grid.
#define M_air			0
#define M_earth			1
#define M_grass			2
#define M_water			3
#define M_fire			4
#define M_test			5
#define M_test2			6




#define M_rock			11
#define M_spring		12
#define M_sand			13
#define M_mud			14
#define M_grass_root	15

// this has to be a complete list of all the saturatable materials in the game.
// this is used by the grid evaluator to check and apply saturations of the cells in our grid.
// these are the materials that will be checked for saturation
/// IF YOU MAKE A MATERIAL THAT CAN BE SATURATED WITH SOMETHING, ADD THAT MATERIAL TO THIS LIST.
short matSatOrder[MAX_NUMBER_OF_UNIQUE_MATERIALS];

void set_chance(unsigned *, unsigned);


struct affectMaterial{
	
	// the type of material will be affected by the material in the current cell.
	// default to M_air
	short typeBefore;
	
	// the type of saturation the material must have had before being changed
	//default to M_dont_care
	short satBefore;
	
	// this is material that the above material will turn into.
	// default to M_air
	short typeAfter;
	// this is the type of saturation the material will have after the affectMat
	// default to M_no_change
	short satAfter;
	
	
	
	//the maximum number of changes to nearby materials that can occur per cell_evaluate() cycle.
	// this is a value from 0 to 8 ( 0 = no changes ever. 8 = this block can (has the posibillity. chance permitting) affect all blocks around it simultaniously.)
	// default set to 0 (no changes ever. this means that the affectMaterial entry is non-active. it doesn't do anything if matChanges = 0.)
	short matChanges;
	
	unsigned chance[8]; // int value from 0-100000 describes the likelyhood of current material affecting the material in the cell next to it.
	// 0 = never happens. 100000 = always happens.
	// this is a diagram of how the numbers in the chance array correlate to the cells around material in the main cell (M)
	//		0 1 2
	//		3 M 4
	//		5 6 7
	// if we set  chance[2] =  53700;   then the material in the cell up and to the right from the main cell (M) has a 53.700% chance of being changed into typeAfter.
	// if we set  chance[6] =     12;   then the material in the cell directly below the main cell (M) will have a 0.012% chance of being changed into typeAfter.
	// if we set  chance[3] = 100000;  then the material in the cell to the left of the main cell (M) will have a 100.000% chance of being changed into typeAfter.
	
	// does the original material (the one that is affecting the surrounding material) change after and only after affecting the surrounding material?
	// default set to M_no_change.
	short changeOrigMat;
	
	// what saturation will the original material have after and only after changing as a result of affecting the nearby material?
	// default set to M_no_change.
	short changeOrigSat;
	
};


struct saturationEffect{
	
	//this is some material that is saturating our material.
	//default to M_no_saturation
	//if satMat == M_no_saturation for a given material, that means there is no saturation effect for that material.
	short satMat;

	// does the saturation material get absorbed (i.e. turn into air) when our material gets saturated by it? or is it not affected by saturating our material?
	// 1 = gets absorbed
	// 0 = doesn't get absorbed
	unsigned short absorb;

	// chance of our material getting saturated by the surrounding material in these locations:
	//		0 1 2
	//		3 M 4
	//		5 6 7
	// chance of 0-100000. 0 = never 100000 = always
	// default to each space around our material to have a 100% chance (satChance[0 thru 7] = 100000)
	unsigned satChance[8];

	// the chance that our saturated material will decay into something else.
	// from 0-100000
	unsigned decayChance;
	
	// the thing that our saturated material may decay into now that it is saturated.
	short decayInto;
	
	// what will the saturation be after the block decays?
	// set to M_no_saturation by default.
	short decaySatMat;

	// these are other materials that our saturated material can affect now that it is saturated.
	// start with affectMat[0]. If you want to add another, put that effect in affectMat[1] then affectMat[2] and affectMat[3] (etc...)
	struct affectMaterial affectMat[MAX_NUMBER_OF_SATURATION_EFFECT_INTERACTIONS];
};


///this is the data structure for materials:
struct material {

	// this is an array of affectMaterial structures.
	//Each element of the structure array describes one type of interaction this material can have with materials in neighboring cells.
	//start with affectMat[0] and put other effects into affectMat[1] and affectMat[2] and so on.
	struct affectMaterial affectMat[MAX_NUMBER_OF_MATERIAL_INTERACTIONS];

	// this is an array (list) of things that can saturate this material and how it affects it.
	// you must enter them in order starting at the 0th element. i.e. satEffect[0]. once the cell evalutator finds a default saturation effect, it breaks from the saturation effect checking loop.
	// basically, if you want to make a saturation effect for a material, you have to put it in the satEffect[0] spot. if you want to make another, put it in the satEffect[1] spot. and so on and so forth.
	// once the grid evaluator finds a saturation effect that is the default (does nothing)
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
///after this is done, the "init_material_attributes" specifies everything that needs to be specified.
///this is just here to be a catch all that assigns a value to everything.
///so basically, if you don't specify a material's behavior, it will basically be air.
void set_default_material_attributes(){
	int i,j,k,l;

	//DEFAULT MATERIAL VALUES:
	for(i=0 ; i<MAX_NUMBER_OF_UNIQUE_MATERIALS ; i++){
        mats[i].name = NULL;
		mats[i].gravity = 0; // is not affected by gravity
		mats[i].color = 0x000000;//default color is black
		mats[i].decayChance = 0; // 0% chance to decay.
		mats[i].decayInto = M_air;	 // decay into air (this is irrelevant because there is a 0% decayChance anyway)

		 // for every saturation effect, set it to the default of not being able to be saturated by anything.
		for(j=0 ; j<MAX_NUMBER_OF_SATURATION_EFFECTS ; j++){
			mats[i].satEffect[j].satMat = M_no_saturation; // by default, nothing can be saturated with anything.
			mats[i].satEffect[j].absorb = 0; // does not absorb by default.
			mats[i].satEffect[j].decayChance = 0; // be default, nothing saturated will decay into anything.
			mats[i].satEffect[j].decayInto =	M_air; // default decay into air. kind of irrelevant because the chance of decay is already 0. oh well. better safe than sorry.
			mats[i].satEffect[j].decaySatMat =	M_no_saturation; // the default saturation the material will have after decaying is nothing.
			for(l=0 ; l<8 ; l++){ // set all the absorb chances to 100% be default
				mats[i].satEffect[j].satChance[l] = 100000; // 100%
			}
			for(k=0; k<MAX_NUMBER_OF_SATURATION_EFFECT_INTERACTIONS ; k++){ // for every interaction a saturated material can have with another material.
				mats[i].satEffect[j].affectMat[k].typeBefore = M_air;// affects air
				mats[i].satEffect[j].affectMat[k].typeAfter  = M_air; // turns air into air
				mats[i].satEffect[j].affectMat[k].satBefore  = M_dont_care; // by default, the saturation before doesn't matter in an affectMat.
				mats[i].satEffect[j].affectMat[k].satAfter   = M_no_change; // by default, the saturation afterwards doesn't chance in an affectMat.
				mats[i].satEffect[j].affectMat[k].matChanges = 8; // by default, any material can affect the stuff around it all at once.
				mats[i].satEffect[j].affectMat[k].changeOrigMat = M_no_change; // by default, the original material will not change after affecting the material around it.
				mats[i].satEffect[j].affectMat[k].changeOrigSat  = M_no_change; // by default, the material that the original material turns into will have the same saturation.
				for(l=0 ; l<8 ; l++){
					mats[i].satEffect[j].affectMat[k].chance[l] = 0; // no chance of affecting anything (so it never affects air. it never will actually go through the process of turning air into air)
				}
			}
		}

		// for every affect that our material can have on other materials, set it to default (default = air changes to air with a 0% chance. nothing happens.)
		for(k=0 ; k<MAX_NUMBER_OF_MATERIAL_INTERACTIONS ; k++){
			mats[i].affectMat[k].typeBefore = M_air;// affects air.
			mats[i].affectMat[k].typeAfter = M_air; // turns air into air.
			mats[i].affectMat[k].satBefore = M_dont_care; // by default, it doesn't matter what the affected material had for saturation before the incident.
			mats[i].affectMat[k].satAfter  = M_no_change; // by default, there is no change in saturation after the affectMat.
			mats[i].affectMat[k].matChanges = 8; // by default, any material can affect the stuff around it all at once.
			mats[i].affectMat[k].changeOrigMat = M_no_change; // by default, doesn't change the original block type.
			mats[i].affectMat[k].changeOrigSat = M_no_change; // by default, the original materials saturation does not change.
			for(l=0 ; l<8 ; l++){
				mats[i].affectMat[k].chance[l] = 0; // no chance of affecting anything (so it never affects air. it never will actually go through the process of turning air into air)
			}
		}
	}
}

void init_material_attributes(void){
	
	mats[M_air].name = "Air (empty)";
	//everything else for air has already been
	
	mats[M_earth].name = "Earth";
	mats[M_earth].color = 0x8b672d;
	mats[M_earth].satEffect[0].satMat = M_water;    // earth turns into mud when soaked
	mats[M_earth].satEffect[0].absorb = 1;
	mats[M_earth].satEffect[0].satChance[0] = 125;
	mats[M_earth].satEffect[0].satChance[1] = 125;
	mats[M_earth].satEffect[0].satChance[2] = 125;
	mats[M_earth].satEffect[0].satChance[3] = 125;
	mats[M_earth].satEffect[0].satChance[4] = 125;
	mats[M_earth].satEffect[0].satChance[5] = 125;
	mats[M_earth].satEffect[0].satChance[6] = 125;
	mats[M_earth].satEffect[0].satChance[7] = 125;
	mats[M_earth].satEffect[0].decayInto = M_mud;
	mats[M_earth].satEffect[0].decayChance = 100000;
	
	
	
	mats[M_grass].name = "Grass";
    mats[M_grass].color = 0x20e112;
    mats[M_grass].affectMat[0].typeBefore = M_water; // grass grows into water
	mats[M_grass].affectMat[0].typeAfter  = M_grass;
	mats[M_grass].affectMat[0].chance[0] = 1000;
	mats[M_grass].affectMat[0].chance[1] = 1000;
	mats[M_grass].affectMat[0].chance[2] = 1000;
	mats[M_grass].affectMat[0].chance[3] = 1000;
	mats[M_grass].affectMat[0].chance[4] = 1000;
	mats[M_grass].affectMat[0].chance[5] = 1000;
	mats[M_grass].affectMat[0].chance[6] = 1000;
	mats[M_grass].affectMat[0].chance[7] = 1000;
	mats[M_grass].affectMat[1].typeBefore = M_mud; // grass grows roots into into mud
	mats[M_grass].affectMat[1].typeAfter  = M_grass_root;
	mats[M_grass].affectMat[1].chance[0] = 1000;
	mats[M_grass].affectMat[1].chance[1] = 1000;
	mats[M_grass].affectMat[1].chance[2] = 1000;
	mats[M_grass].affectMat[1].chance[3] = 3000;
	mats[M_grass].affectMat[1].chance[4] = 3000;
	mats[M_grass].affectMat[1].chance[5] = 4500;
	mats[M_grass].affectMat[1].chance[6] = 4500;
	mats[M_grass].affectMat[1].chance[7] = 4500;
	
	mats[M_water].name = "Water";
	mats[M_water].gravity = 1;
    mats[M_water].color = 0x158ad4;
	
    mats[M_spring].name = "Spring";
	mats[M_spring].color = 0x97bcbb;
	mats[M_spring].affectMat[0].typeBefore = M_air;  // spring generates water
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
	mats[M_fire].affectMat[0].typeBefore = M_grass; // fire burns grass
	mats[M_fire].affectMat[0].typeAfter  = M_fire;
	mats[M_fire].affectMat[0].chance[0] = 12000;
	mats[M_fire].affectMat[0].chance[1] = 15000;
	mats[M_fire].affectMat[0].chance[2] = 12000;
	mats[M_fire].affectMat[0].chance[3] =  9000;
	mats[M_fire].affectMat[0].chance[4] =  9000;
	mats[M_fire].affectMat[0].chance[5] =  4500;
	mats[M_fire].affectMat[0].chance[6] =  6500;
	mats[M_fire].affectMat[0].chance[7] =  4500;
	
	mats[M_test].name = "TestMat"; // the material that jensen tests evaluate_grid() with
	mats[M_test].color = 0xccff00;
	mats[M_test].affectMat[0].typeBefore = M_air;
	mats[M_test].affectMat[0].typeAfter  = M_test;
	mats[M_test].affectMat[0].changeOrigMat = M_air;
	mats[M_test].affectMat[0].matChanges = 1;		// this material moves itself around once per grid evaluation.
	mats[M_test].affectMat[0].chance[0] = 100000;
	mats[M_test].affectMat[0].chance[1] = 100000;
	mats[M_test].affectMat[0].chance[2] = 100000;
	mats[M_test].affectMat[0].chance[3] = 100000;
	mats[M_test].affectMat[0].chance[4] = 100000;
	mats[M_test].affectMat[0].chance[5] = 100000;
	mats[M_test].affectMat[0].chance[6] = 100000;
	mats[M_test].affectMat[0].chance[7] = 100000;
	
	mats[M_test2].name = "TestMat2"; // the material that jensen tests evaluate_grid() with
	mats[M_test2].color = 0x00ffcc;
	mats[M_test2].satEffect[0].absorb = 1;
	set_chance(&mats[M_test2].satEffect[0].satChance[0], 100000);
	mats[M_test2].satEffect[0].satMat = M_earth;
	mats[M_test2].satEffect[0].affectMat[0].typeBefore = M_air;
	mats[M_test2].satEffect[0].affectMat[0].typeAfter  = M_test2;
	mats[M_test2].satEffect[0].affectMat[0].satAfter   = mats[M_test2].satEffect[0].satMat;
	mats[M_test2].satEffect[0].affectMat[0].changeOrigMat = M_air;
	mats[M_test2].satEffect[0].affectMat[0].matChanges = 1;		// this material moves itself around once per grid evaluation.
	mats[M_test2].satEffect[0].affectMat[0].chance[0] = 100000;
	mats[M_test2].satEffect[0].affectMat[0].chance[1] = 100000;
	mats[M_test2].satEffect[0].affectMat[0].chance[2] = 100000;
	mats[M_test2].satEffect[0].affectMat[0].chance[3] = 100000;
	mats[M_test2].satEffect[0].affectMat[0].chance[4] = 100000;
	mats[M_test2].satEffect[0].affectMat[0].chance[5] = 100000;
	mats[M_test2].satEffect[0].affectMat[0].chance[6] = 100000;
	mats[M_test2].satEffect[0].affectMat[0].chance[7] = 100000;
	
	mats[M_rock].name = "Rock";
	mats[M_rock].color = 0x5a5651;
	
	mats[M_sand].name = "Sand";
	mats[M_sand].gravity = 1;
	mats[M_sand].color = 0xcfc1aa;
	
	mats[M_mud].name = "Mud";
	mats[M_mud].gravity = 1;
	mats[M_mud].color = 0x644310;
	
	mats[M_grass_root].name = "Grass Root";
	mats[M_grass_root].color = 0xbfc69e;
	mats[M_grass_root].affectMat[0].typeBefore = M_mud;
	mats[M_grass_root].affectMat[0].typeAfter  = M_grass_root;
	mats[M_grass_root].affectMat[0].chance[0] = 70;
	mats[M_grass_root].affectMat[0].chance[1] = 95; 
	mats[M_grass_root].affectMat[0].chance[2] = 70;
	mats[M_grass_root].affectMat[0].chance[3] = 250;
	mats[M_grass_root].affectMat[0].chance[4] = 250;
	mats[M_grass_root].affectMat[0].chance[5] = 400;
	mats[M_grass_root].affectMat[0].chance[6] = 400;
	mats[M_grass_root].affectMat[0].chance[7] = 400;
	mats[M_grass_root].affectMat[0].typeBefore = M_mud;
	mats[M_grass_root].affectMat[0].typeAfter  = M_grass_root;
	mats[M_grass_root].affectMat[0].chance[0] = 70;
	mats[M_grass_root].affectMat[0].chance[1] = 95; 
	mats[M_grass_root].affectMat[0].chance[2] = 70;
	mats[M_grass_root].affectMat[0].chance[3] = 250;
	mats[M_grass_root].affectMat[0].chance[4] = 250;
	mats[M_grass_root].affectMat[0].chance[5] = 400;
	mats[M_grass_root].affectMat[0].chance[6] = 400;
	mats[M_grass_root].affectMat[0].chance[7] = 400;
	
	
	
	// find how many saturatable materials there are:
	numberOfMaterialsThatCanBeSaturated = 0; // set this to 0 by default. it will get incremented in the for loop and brought to the correct value.
	int i;
	for(i=0 ; i<MAX_NUMBER_OF_UNIQUE_MATERIALS ; i++){
		// if the saturationMaterial is an invalid choice (like M_no_saturation or M_no_change) skip and move on.
		if(mats[i].satEffect[0].satMat == M_no_saturation) continue;
		
		matSatOrder[numberOfMaterialsThatCanBeSaturated] = i; // put the material type into the array at the right point.
		
		printf("matSatOrder[%d] = %d\n", numberOfMaterialsThatCanBeSaturated, matSatOrder[numberOfMaterialsThatCanBeSaturated]);
		
		numberOfMaterialsThatCanBeSaturated++; // increment the number of materials that be saturated that we have.
	}
	printf("numberOfMaterialsThatCanBeSaturated = %d\n\n\n\n\n", numberOfMaterialsThatCanBeSaturated);
}



///this function resets all the cells to default state. Nothing
void reset_cells(void){
	int i; int j;

	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			cellData[i][j] = 0; // air
			cellSat[i][j]  = M_no_saturation;
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


void print_saturation_data(){
	int i,j; // indexes
	static int printTime = 1;
	printf("printTime = %d\n",printTime);
	for(j=0 ; j<GRID_HEIGHT ; j++){
		for(i=0 ; i<GRID_WIDTH ; i++){
			if(cellSat[i][j] >= 0)printf("%2d ",cellSat[i][j]);
			else printf(" . ");
		}
		printf("\n");
	}
	printf("\n\n");
	printTime++;
}




//this function will take in a 1-Dimensional array of 8 elements (such as a 'chance[8]' array from affectMat) and give them all the same values that you choose.
void set_chance(unsigned *chanceArray, unsigned chance){
	chanceArray[0] = chance;
	chanceArray[1] = chance;
	chanceArray[2] = chance;
	chanceArray[3] = chance;
	chanceArray[4] = chance;
	chanceArray[5] = chance;
	chanceArray[6] = chance;
	chanceArray[7] = chance;
}
