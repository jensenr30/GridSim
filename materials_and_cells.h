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

//this is how many different types of materials there can be in the game.
#define MAX_NUMBER_OF_UNIQUE_MATERIALS 100 // the materials (i.e. M_earth, M_spring) can have values from 0-99

//this holds the number of unique materials that have the ability to be saturated by something.
// this value is detected and set in the set_default_material_attributes() function
short numberOfSatableMats;

//this is how many different interactions any given material can have with other materials in neighboring cells.
#define MAX_NUMBER_OF_MATERIAL_INTERACTIONS 6
//this is how many different things a given material can be saturated with
#define MAX_NUMBER_OF_SATURATIONS 3

struct cellData{
	short mat; // the material in a cell. 	// default to M_air
	short sat; // the saturaiton of a cell. // default to M_no_saturation
	char satLevel;  // the saturation level of a cell. i.e. how saturated is it? (integer range 1-8) default to 1
	
	short matChange; // does the material need to change?
	short satChange; // does the saturation need to change?
	char satLevelChange; // does the saturationLevel need to change?
};
//this is the array of structures that the data for the grid is stored in.
// your entire map exists in here.
struct cellData grid[SCREEN_WIDTH][SCREEN_HEIGHT];


//this defines the material types. a material type is a
//		signed short
//negative values are not actual materials, but rather flags for conditions used in evaluating the grid.
//for instance, you can use mats[5] to get gunpowder data, or you can use mats[M_gunpowder] to get gunpowder data.
//this is just for ease of code writing.
#define M_any_of_my_sats	-4	// this is used when checking affectMat[].satNeeded to see if a material has ANY of its valid saturations. if it does have any, then evaluate_affectMaterial() will allow the affect to occur.
#define M_dont_care			-3  // this is used to show that we don't care what the material is.
#define M_no_saturation 	-2  // this is used to signify that a material in a cell has no saturation
#define M_no_change 		-1	// this material is more of a flag. It is used by the cell_engine in checking the changes to the cells in the grid.
#define M_air			0
#define M_earth			1
#define M_grass			2
#define M_water			3
#define M_fire			4
#define M_tree_base		5
#define M_test			6

#define M_rock			11
#define M_spring		12
#define M_sand			13
#define M_mud			14
#define M_grass_root	15

#define M_scurge		21
#define M_anti_scurge	22
#define M_dead_scurge	23

//tree stuff
#define M_tree_trunk		80
#define M_tree_trunk_top	81
#define M_tree_branch_left	82
#define M_tree_branch_right 83
#define M_tree_branch_end	84
#define M_tree_leaves		85
#define M_tree_leaves_end	86
#define M_tree_fruit		87


#define M_valid_but_null_material (MAX_NUMBER_OF_UNIQUE_MATERIALS-1)

// this has to be a complete list of all the saturatable materials in the game.
// this is used by the grid evaluator to check and apply saturations of the cells in our grid.
// these are the materials that will be checked for saturation
/// IF YOU MAKE A MATERIAL THAT CAN BE SATURATED WITH SOMETHING, ADD THAT MATERIAL TO THIS LIST.
short matSatOrder[MAX_NUMBER_OF_UNIQUE_MATERIALS];

void set_chance(unsigned *, unsigned);


struct affectMaterial{
	
	//this is the type of saturation our material needs to have in order to carry out the desired affect.
	//default to M_dont_care
	short satNeeded;
	
	// this is how much the saturation level must be in order to do the desired affect to be carried out.
	// saturaiton Greater Than or Equal
	//default to 1 (no minumum)
	char satGTE;
	//this is the highest yoru saturation level can be in order for the desired affect to be caried our.
	//saturation Less Than or Equal
	//default to 8 (no maximum)
	char satLTE;
	
	
	
	// the type of material will be affected by the material in the current cell.
	// default to M_air
	short matBefore;
	
	// the type of saturation the material must have had before being changed
	//default to M_dont_care
	short satBefore;
	
	// this is material that the above material will turn into.
	// default to M_air
	short matAfter;
	// this is the type of saturation the material will have after the affectMat
	// default to M_no_change
	short satAfter;
	
	
	//the maximum number of changes to nearby materials that can occur per cell_evaluate() cycle.
	// this is a value from 0 to 8 ( 0 = no changes ever. 8 = this block can (has the posibillity. chance permitting) affect all blocks around it simultaniously.)
	// default set to 0 (no changes ever. this means that the affectMaterial entry is non-active. it doesn't do anything if changesPerEval = 0.)
	short changesPerEval;
	
	unsigned chance[8]; // int value from 0-100000 describes the likelyhood of current material affecting the material in the cell next to it.
	// 0 = never happens. 100000 = always happens.
	// this is a diagram of how the numbers in the chance array correlate to the cells around material in the main cell (M)
	//		0 1 2
	//		3 M 4
	//		5 6 7
	// if we set  chance[2] =  53700;   then the material in the cell up and to the right from the main cell (M) has a 53.700% chance of being changed into matAfter.
	// if we set  chance[6] =     12;   then the material in the cell directly below the main cell (M) will have a 0.012% chance of being changed into matAfter.
	// if we set  chance[3] = 100000;  then the material in the cell to the left of the main cell (M) will have a 100.000% chance of being changed into matAfter.
	
	// does the original material (the one that is affecting the surrounding material) change after and only after affecting the surrounding material?
	// default set to M_no_change.
	short changeOrigMat;
	
	// what saturation will the original material have after and only after changing as a result of affecting the nearby material?
	// default set to M_no_change.
	short changeOrigSat;
	
};

void copy_affMat( struct affectMaterial *, struct affectMaterial *);

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
	// default to each space around our material to have a 100% chance (chance[0 thru 7] = 100000)
	unsigned chance[8];

	// the chance that our saturated material will decay into something else.
	// from 0-100000
	unsigned decayChance;
	//the minimum saturation level needed to decay. default 1
	char decaySatGTE;
	//the maximum saturation level that will allow decay. default 8
	char decaySatLTE;
	// the thing that our saturated material may decay into now that it is saturated.
	short decayInto;
	// what will the saturation be after the block decays?
	// set to M_no_saturation by default.
	short decaySatMat;
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
	struct saturationEffect satEffect[MAX_NUMBER_OF_SATURATIONS];

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
///if you want to make a material, but you don't want it to appear in the menu, give it a NULL for its mats[].name value.
void set_default_material_attributes(){
	int i,s,k,m;

	//DEFAULT MATERIAL VALUES:
	for(i=0 ; i<MAX_NUMBER_OF_UNIQUE_MATERIALS ; i++){
        mats[i].name = NULL;
		mats[i].gravity = 0; // is not affected by gravity
		mats[i].color = 0x000000;//default color is black
		mats[i].decayChance = 0; // 0% chance to decay.
		mats[i].decayInto = M_air;	 // decay into air (this is irrelevant because there is a 0% decayChance anyway)

		 // for every saturation effect, set it to the default of not being able to be saturated by anything.
		for(s=0 ; s<MAX_NUMBER_OF_SATURATIONS ; s++){
			mats[i].satEffect[s].satMat = M_no_saturation; // by default, nothing can be saturated with anything.
			mats[i].satEffect[s].absorb = 0; // does not absorb by default.
			mats[i].satEffect[s].decayChance = 0; // be default, nothing saturated will decay into anything.
			mats[i].satEffect[s].decayInto =	M_air; // default decay into air. kind of irrelevant because the chance of decay is already 0. oh well. better safe than sorry.
			mats[i].satEffect[s].decaySatMat =	M_no_saturation; // the default saturation the material will have after decaying is nothing.
			mats[i].satEffect[s].decaySatGTE = 1;
			mats[i].satEffect[s].decaySatLTE = 8;
			for(k=0 ; k<8 ; k++){ // set all the absorb chances to 0% be default
				mats[i].satEffect[s].chance[k] = 0; // 0%
			}
		}

		// for every affect that our material can have on other materials, set it to default (default = air changes to air with a 0% chance. nothing happens.)
		for(m=0 ; m<MAX_NUMBER_OF_MATERIAL_INTERACTIONS ; m++){
			mats[i].affectMat[m].matBefore = M_dont_care;// affects everything
			mats[i].affectMat[m].matAfter = M_no_change; // doesn't do anything to anything
			mats[i].affectMat[m].satBefore = M_dont_care; // by default, it doesn't matter what the affected material had for saturation before the incident.
			mats[i].affectMat[m].satAfter  = M_no_saturation; // by default, there is no change in saturation after the affectMat.
			mats[i].affectMat[m].changesPerEval = 8; // by default, any material can affect the stuff around it all at once.
			mats[i].affectMat[m].satNeeded = M_dont_care; // by default, there is no required saturation.
			mats[i].affectMat[m].satGTE = 1;	// the Saturation can be Greater Than or Equal to 1.
			mats[i].affectMat[m].satLTE = 8;	// the Saturation can be Less Than or Equal to 8.
			mats[i].affectMat[m].changeOrigMat = M_no_change; // by default, doesn't change the original block type.
			mats[i].affectMat[m].changeOrigSat = M_no_change; // by default, the original materials saturation does not change.
			for(k=0 ; k<8 ; k++){
				mats[i].affectMat[m].chance[k] = 0; // no chance of affecting anything (so it never affects air. it never will actually go through the process of turning air into air)
			}
		}
	}
}

void init_material_attributes(void){
	
	// I don't need to specify anything for air because air doesn't do anything.
	// all of the elements of the mats[M_air] structure are initialized in the set_default_material_attributes() function.
	mats[M_air].name = "Air";
	//DON'T YOU DARE CHANGE ANYTHING ABOUT AIR!
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_earth].name = "Earth";
	mats[M_earth].color = 0x8b672d;
	mats[M_earth].satEffect[0].satMat = M_water;    /// earth turns into mud when soaked
	mats[M_earth].satEffect[0].absorb = 1;
	mats[M_earth].satEffect[0].chance[0] = 125;
	mats[M_earth].satEffect[0].chance[1] = 125;
	mats[M_earth].satEffect[0].chance[2] = 125;
	mats[M_earth].satEffect[0].chance[3] = 125;
	mats[M_earth].satEffect[0].chance[4] = 125;
	mats[M_earth].satEffect[0].chance[5] = 125;
	mats[M_earth].satEffect[0].chance[6] = 125;
	mats[M_earth].satEffect[0].chance[7] = 125;
	mats[M_earth].satEffect[0].decayInto = M_mud;
	mats[M_earth].satEffect[0].decayChance = 100000;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_grass].name = "Grass";
    mats[M_grass].color = 0x20e112;
    mats[M_grass].affectMat[0].matBefore = M_water; /// grass grows into water
	mats[M_grass].affectMat[0].matAfter  = M_grass;
	mats[M_grass].affectMat[0].chance[0] = 1000;
	mats[M_grass].affectMat[0].chance[1] = 1000;
	mats[M_grass].affectMat[0].chance[2] = 1000;
	mats[M_grass].affectMat[0].chance[3] = 1000;
	mats[M_grass].affectMat[0].chance[4] = 1000;
	mats[M_grass].affectMat[0].chance[5] = 1000;
	mats[M_grass].affectMat[0].chance[6] = 1000;
	mats[M_grass].affectMat[0].chance[7] = 1000;
	mats[M_grass].affectMat[1].matBefore = M_mud; /// grass grows grass_roots into into mud
	mats[M_grass].affectMat[1].matAfter  = M_grass_root;
	mats[M_grass].affectMat[1].chance[0] = 1000;
	mats[M_grass].affectMat[1].chance[1] = 1000;
	mats[M_grass].affectMat[1].chance[2] = 1000;
	mats[M_grass].affectMat[1].chance[3] = 3000;
	mats[M_grass].affectMat[1].chance[4] = 3000;
	mats[M_grass].affectMat[1].chance[5] = 4500;
	mats[M_grass].affectMat[1].chance[6] = 4500;
	mats[M_grass].affectMat[1].chance[7] = 4500;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_water].name = "Water";
	mats[M_water].gravity = true;
    mats[M_water].color = 0x158ad4;
//-------------------------------------------------------------------------------------------------------------------------------
    mats[M_spring].name = "Spring";
	mats[M_spring].color = 0x97bcbb;
	mats[M_spring].affectMat[0].matBefore = M_air;  /// spring generates water
	mats[M_spring].affectMat[0].matAfter = M_water;
	mats[M_spring].affectMat[0].chance[0] = 0;
	mats[M_spring].affectMat[0].chance[1] = 0;
	mats[M_spring].affectMat[0].chance[2] = 0;
	mats[M_spring].affectMat[0].chance[3] = 300;
	mats[M_spring].affectMat[0].chance[4] = 300;
	mats[M_spring].affectMat[0].chance[5] = 700;
	mats[M_spring].affectMat[0].chance[6] = 1000;
	mats[M_spring].affectMat[0].chance[7] = 700;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_fire].name = "Fire";
	mats[M_fire].decayInto = M_air;
    mats[M_fire].color = 0xd83313;
	mats[M_fire].decayChance = 9000;
	mats[M_fire].affectMat[0].matBefore = M_grass; /// fire burns grass
	mats[M_fire].affectMat[0].matAfter  = M_fire;
	mats[M_fire].affectMat[0].chance[0] = 12000;
	mats[M_fire].affectMat[0].chance[1] = 15000;
	mats[M_fire].affectMat[0].chance[2] = 12000;
	mats[M_fire].affectMat[0].chance[3] =  9000;
	mats[M_fire].affectMat[0].chance[4] =  9000;
	mats[M_fire].affectMat[0].chance[5] =  4500;
	mats[M_fire].affectMat[0].chance[6] =  6500;
	mats[M_fire].affectMat[0].chance[7] =  4500;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_test].name = "test"; // the material that jensen tests evaluate_grid() with
	mats[M_test].color = 0xccff00;
	mats[M_test].affectMat[0].chance[1] = 100000;
	mats[M_test].affectMat[0].matBefore = M_air;
	mats[M_test].affectMat[0].matAfter  = M_tree_branch_right;
/*/-------------------------------------------------------------------------------------------------------------------------------
	mats[M_test2].name = "test2"; // the material that jensen tests evaluate_grid() with
	mats[M_test2].color = 0x00ffcc;
	mats[M_test2].affectMat[0].matBefore = M_air; /// test2 turns air into test
	mats[M_test2].affectMat[0].matAfter  = M_test;
	mats[M_test2].affectMat[0].changesPerEval = 1;
	set_chance( mats[M_test2].affectMat[0].chance, 12500);
	mats[M_test2].satEffect[0].absorb = 0;
	set_chance(&mats[M_test2].satEffect[0].chance[0], 100000); /// test2 turns the test above it into test2. it grows upwards into test.
	mats[M_test2].satEffect[0].satMat = M_test;
	mats[M_test2].affectMat[1].matBefore = M_test;
	mats[M_test2].affectMat[1].matAfter  = M_test2;
	mats[M_test2].affectMat[1].satGTE = 4;
	mats[M_test2].affectMat[1].chance[1] = 5000;
*///-------------------------------------------------------------------------------------------------------------------------------
	mats[M_rock].name = "Rock";
	mats[M_rock].color = 0x5a5651;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_tree_base].name = "Tree";	/// this is the start of the tree. this is what you palce and watch a tree grow.
	mats[M_tree_base].color = 0x7b5126;
	mats[M_tree_base].affectMat[0].matBefore = M_air;
	mats[M_tree_base].affectMat[0].matAfter  = M_tree_trunk;
	mats[M_tree_base].affectMat[0].chance[1] = 850;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_tree_trunk].color = mats[M_tree_base].color;
	
	mats[M_tree_trunk].satEffect[0].satMat = M_tree_trunk; /// tree_trunk can tell when there is tree_trunk around it.
	mats[M_tree_trunk].satEffect[0].chance[1] = 100000;
	
	mats[M_tree_trunk].affectMat[0].matBefore = M_air;  /// turns air into leaves only when there isn't a tree_trunk above it.
	mats[M_tree_trunk].affectMat[0].matAfter  = M_tree_leaves_end;
	mats[M_tree_trunk].affectMat[0].chance[0] = 350;
	mats[M_tree_trunk].affectMat[0].chance[1] = 1000;
	mats[M_tree_trunk].affectMat[0].chance[2] = 350;
	mats[M_tree_trunk].affectMat[0].chance[3] = 500;
	mats[M_tree_trunk].affectMat[0].chance[4] = 500;
	mats[M_tree_trunk].affectMat[0].satNeeded = M_no_saturation;
	copy_affMat(&mats[M_tree_trunk].affectMat[0], &mats[M_tree_trunk].affectMat[1]);
	
	mats[M_tree_trunk].affectMat[2].matBefore = M_tree_leaves_end;		/// tree_trunk grows upwards into tree_leaves_end
	mats[M_tree_trunk].affectMat[2].matAfter  = M_tree_trunk;
	mats[M_tree_trunk].affectMat[2].chance[1] = 500;
	
	mats[M_tree_trunk].affectMat[3].matBefore = M_tree_leaves_end;		/// tree trunk grows tree_trunk_end into tree_leaves_end
	mats[M_tree_trunk].affectMat[3].matAfter  = M_tree_trunk_top;
	mats[M_tree_trunk].affectMat[3].chance[1] = 200;
	
	mats[M_tree_trunk].affectMat[4].matBefore = M_tree_leaves_end; /// once tree has grown, it sheds the leaves lower on it's trunk.
	mats[M_tree_trunk].affectMat[4].matAfter  = M_air;
	mats[M_tree_trunk].affectMat[4].satBefore = M_tree_trunk;		/// tree_trunk can only remove leaves that are saturated with tree_trunk.
	mats[M_tree_trunk].affectMat[4].chance[3] = 450;				/// if end_leaves are saturated with tree_trunk_top, they are not removed.
	mats[M_tree_trunk].affectMat[4].chance[4] = 450;
	mats[M_tree_trunk].affectMat[4].satNeeded = M_tree_trunk;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_tree_trunk_top].color = mats[M_tree_base].color; /// this is the top of the tree. the trunk will stop growing when this material is spawned.
	
	mats[M_tree_trunk_top].affectMat[0].matBefore = M_air;		///turns air into leaves_end
	mats[M_tree_trunk_top].affectMat[0].matAfter  = M_tree_leaves_end;
	mats[M_tree_trunk_top].affectMat[0].chance[0] = 500;
	mats[M_tree_trunk_top].affectMat[0].chance[1] = 750;
	mats[M_tree_trunk_top].affectMat[0].chance[2] = 500;
	
	mats[M_tree_trunk_top].affectMat[1].matBefore = M_tree_leaves_end;
	mats[M_tree_trunk_top].affectMat[1].matAfter  = M_tree_branch_right;
	mats[M_tree_trunk_top].affectMat[1].chance[2] = 250;		/// spawns tree_branch_right on its rght side.
	
	mats[M_tree_trunk_top].affectMat[2].matBefore = M_tree_leaves_end;
	mats[M_tree_trunk_top].affectMat[2].matAfter  = M_tree_branch_left;
	mats[M_tree_trunk_top].affectMat[2].chance[0] = 250;		/// spawns tree_branch_right on its rght side.
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_tree_leaves].color = 0x708D23;
	
	mats[M_tree_leaves].satEffect[0].satMat = M_tree_branch_left; /// tree_leaves saturated by tree_branches_right
	set_chance(mats[M_tree_leaves].satEffect[0].chance, 100000);
	
	mats[M_tree_leaves].satEffect[1].satMat = M_tree_branch_right;/// and tree_leaves saturated by tree_branches_right
	set_chance(mats[M_tree_leaves].satEffect[1].chance, 100000);
	
	mats[M_tree_leaves].affectMat[0].matBefore = M_air;
	mats[M_tree_leaves].affectMat[0].matAfter  = M_tree_leaves;
	mats[M_tree_leaves].affectMat[0].satNeeded = M_any_of_my_sats;
	mats[M_tree_leaves].affectMat[0].chance[1] = 100;
	mats[M_tree_leaves].affectMat[0].chance[0] = 
	mats[M_tree_leaves].affectMat[0].chance[2] = 54;
	mats[M_tree_leaves].affectMat[0].chance[3] = 
	mats[M_tree_leaves].affectMat[0].chance[4] = 40;
	mats[M_tree_leaves].affectMat[0].chance[5] = 
	mats[M_tree_leaves].affectMat[0].chance[7] = 35;
	mats[M_tree_leaves].affectMat[0].chance[6] = 20;
	
	
	mats[M_tree_leaves].affectMat[1].matBefore = M_air;
	mats[M_tree_leaves].affectMat[1].matAfter  = M_tree_leaves_end;
	mats[M_tree_leaves].affectMat[1].chance[1] = 350;
	mats[M_tree_leaves].affectMat[1].chance[0] = 
	mats[M_tree_leaves].affectMat[1].chance[2] = 250;
	mats[M_tree_leaves].affectMat[1].chance[3] = 
	mats[M_tree_leaves].affectMat[1].chance[4] = 200;
	mats[M_tree_leaves].affectMat[1].chance[5] = 
	mats[M_tree_leaves].affectMat[1].chance[7] = 165;
	mats[M_tree_leaves].affectMat[1].chance[6] = 100;
	
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_tree_leaves_end].color = 0x708D23;
	
	mats[M_tree_leaves_end].satEffect[0].satMat = M_tree_trunk;		/// tree_leaves_end can be saturated by both tree_trunk 
	set_chance(mats[M_tree_leaves_end].satEffect[0].chance, 100000);
	
	mats[M_tree_leaves_end].satEffect[1].satMat = M_tree_trunk_top; /// and tree_trunk_top.
	set_chance(mats[M_tree_leaves_end].satEffect[1].chance, 100000);
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_tree_branch_right].color = mats[M_tree_base].color;
	
	mats[M_tree_branch_right].affectMat[0].matBefore = M_air;			/// tree_branch_right makes leaves around it.
	mats[M_tree_branch_right].affectMat[0].matAfter  = M_tree_leaves;
	set_chance( &mats[M_tree_branch_right].affectMat[0].chance[0], 433);
	
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_tree_branch_left].color = mats[M_tree_base].color;
	
	/// tree_branch_left makes leaves around it as well
	copy_affMat(&mats[M_tree_branch_right].affectMat[0], &mats[M_tree_branch_left].affectMat[0]);
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_sand].name = "Sand";
	mats[M_sand].gravity = 1;
	mats[M_sand].color = 0xcfc1aa;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_mud].name = "Mud";
	mats[M_mud].gravity = true;
	mats[M_mud].color = 0x644310;
	mats[M_mud].satEffect[0].absorb = 1;
	mats[M_mud].satEffect[0].satMat = M_water;
	mats[M_mud].satEffect[0].chance[0] = 1000; /// mud absorbs water
	mats[M_mud].satEffect[0].chance[1] = 1100;
	mats[M_mud].satEffect[0].chance[2] = 1000;
	mats[M_mud].satEffect[0].chance[3] = 500;
	mats[M_mud].satEffect[0].chance[4] = 500;
	mats[M_mud].satEffect[0].chance[5] = 150;
	mats[M_mud].satEffect[0].chance[6] = 200;
	mats[M_mud].satEffect[0].chance[7] = 150;
	mats[M_mud].affectMat[1].changesPerEval = 1; /// when mud is saturated with water, it will leak water into other mud that is NOT saturated with water.
	mats[M_mud].affectMat[1].changeOrigSat = M_no_saturation;
	mats[M_mud].affectMat[1].satNeeded = M_water;
	mats[M_mud].affectMat[1].matBefore = M_mud;
	mats[M_mud].affectMat[1].matAfter = M_mud;
	mats[M_mud].affectMat[1].satBefore = M_no_saturation;
	mats[M_mud].affectMat[1].satAfter = M_water;
	mats[M_mud].affectMat[1].chance[3] = 400;
	mats[M_mud].affectMat[1].chance[4] = 400;
	mats[M_mud].affectMat[1].chance[5] = 700;
	mats[M_mud].affectMat[1].chance[6] = 950;
	mats[M_mud].affectMat[1].chance[7] = 700;
	mats[M_mud].affectMat[2].changesPerEval = 1; 	/// mud leaks water into dry earth.
	mats[M_mud].affectMat[2].satNeeded = M_water;
	mats[M_mud].affectMat[2].changeOrigSat = M_no_saturation;
	mats[M_mud].affectMat[2].matBefore = M_earth;
	mats[M_mud].affectMat[2].matAfter = M_mud;
	mats[M_mud].affectMat[2].satBefore = M_no_saturation;
	mats[M_mud].affectMat[1].chance[0] = 45;
	mats[M_mud].affectMat[1].chance[1] = 70;
	mats[M_mud].affectMat[1].chance[2] = 45;
	mats[M_mud].affectMat[1].chance[3] = 150;
	mats[M_mud].affectMat[1].chance[4] = 150;
	mats[M_mud].affectMat[1].chance[5] = 325;
	mats[M_mud].affectMat[1].chance[6] = 650;
	mats[M_mud].affectMat[1].chance[7] = 325;
	mats[M_mud].affectMat[0].changeOrigMat = M_earth;		/// mud turns into dirt if it can make the dirt below it turn into earth.
	mats[M_mud].affectMat[0].changesPerEval = 1;
	mats[M_mud].affectMat[0].satBefore = M_no_saturation;
	mats[M_mud].affectMat[0].satAfter = M_no_saturation;
	mats[M_mud].affectMat[0].matBefore = M_earth;
	mats[M_mud].affectMat[0].matAfter = M_mud;
	mats[M_mud].affectMat[0].chance[0] = 20;
	mats[M_mud].affectMat[0].chance[1] = 15;
	mats[M_mud].affectMat[0].chance[2] = 20;
	mats[M_mud].affectMat[0].chance[3] = 55;
	mats[M_mud].affectMat[0].chance[4] = 55;
	mats[M_mud].affectMat[0].chance[5] = 100;
	mats[M_mud].affectMat[0].chance[6] = 175;
	mats[M_mud].affectMat[0].chance[7] = 100;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_grass_root].name = "Grass Root";
	mats[M_grass_root].color = 0xbfc69e;
	mats[M_grass_root].affectMat[0].matBefore = M_mud;
	mats[M_grass_root].affectMat[0].matAfter  = M_grass_root;
	mats[M_grass_root].affectMat[0].chance[0] = 70;
	mats[M_grass_root].affectMat[0].chance[1] = 95; 
	mats[M_grass_root].affectMat[0].chance[2] = 70;
	mats[M_grass_root].affectMat[0].chance[3] = 250;
	mats[M_grass_root].affectMat[0].chance[4] = 250;
	mats[M_grass_root].affectMat[0].chance[5] = 400;
	mats[M_grass_root].affectMat[0].chance[6] = 400;
	mats[M_grass_root].affectMat[0].chance[7] = 400;
	mats[M_grass_root].affectMat[0].matBefore = M_mud;
	mats[M_grass_root].affectMat[0].matAfter  = M_grass_root;
	mats[M_grass_root].affectMat[0].chance[0] = 70;
	mats[M_grass_root].affectMat[0].chance[1] = 95; 
	mats[M_grass_root].affectMat[0].chance[2] = 70;
	mats[M_grass_root].affectMat[0].chance[3] = 250;
	mats[M_grass_root].affectMat[0].chance[4] = 250;
	mats[M_grass_root].affectMat[0].chance[5] = 400;
	mats[M_grass_root].affectMat[0].chance[6] = 400;
	mats[M_grass_root].affectMat[0].chance[7] = 400;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_scurge].name = "Scurge";
	mats[M_scurge].color = 0x561377;
	mats[M_scurge].affectMat[0].matBefore = M_air; /// scurge turns empty space into scurge.
	mats[M_scurge].affectMat[0].matAfter = M_scurge;
	mats[M_scurge].affectMat[0].changesPerEval = 1;
	set_chance( &mats[M_scurge].affectMat[0].chance[0], 100000 );
	mats[M_scurge].satEffect[0].satMat = M_scurge; /// scurge turns other scurge into empty space
	mats[M_scurge].satEffect[0].decayInto = M_air;
	mats[M_scurge].satEffect[0].decayChance = 100000;
	set_chance( &mats[M_scurge].satEffect[0].chance[0], 8700);
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_anti_scurge].name = "Anti Scurge";
	mats[M_anti_scurge].color = 0x0b94a0;
	mats[M_anti_scurge].affectMat[0].matBefore = M_air; /// anti scurge turns empty space into anti scurge.
	mats[M_anti_scurge].affectMat[0].matAfter = M_anti_scurge;
	mats[M_anti_scurge].affectMat[0].changesPerEval = 1;
	set_chance( &mats[M_anti_scurge].affectMat[0].chance[0], 100000 );
	mats[M_anti_scurge].satEffect[0].satMat = M_anti_scurge; /// anti scurge turns other anti scurge into empty space
	mats[M_anti_scurge].satEffect[0].decayInto = M_air;
	mats[M_anti_scurge].satEffect[0].decayChance = 100000;
	set_chance( &mats[M_anti_scurge].satEffect[0].chance[0], mats[M_scurge].satEffect[0].chance[0]); // make the anti scuge survive just as well as the scurge
	mats[M_anti_scurge].affectMat[1].matBefore = M_scurge;		/// anti scurge will fight the scurge to the death
	mats[M_anti_scurge].affectMat[1].matAfter = M_dead_scurge;
	mats[M_anti_scurge].affectMat[1].changeOrigMat = M_dead_scurge;
	mats[M_anti_scurge].affectMat[1].changesPerEval = 1;
	set_chance( &mats[M_anti_scurge].affectMat[1].chance[0], 100000);
//-------------------------------------------------------------------------------------------------------------------------------
	mats[M_dead_scurge].name = NULL;		//dead scurge falls.
	mats[M_dead_scurge].color = 0xa00b0b;
	mats[M_dead_scurge].gravity = true;
//-------------------------------------------------------------------------------------------------------------------------------
	
	// find how many saturatable materials there are:
	numberOfSatableMats = 0; // set this to 0 by default. it will get incremented in the for loop and brought to the correct value.
	int i;
	for(i=0 ; i<MAX_NUMBER_OF_UNIQUE_MATERIALS ; i++){
		// if the saturationMaterial is an invalid choice (like M_no_saturation or M_no_change) skip and move on.
		if(mats[i].satEffect[0].satMat == M_no_saturation) continue;
		
		matSatOrder[numberOfSatableMats] = i; // put the material type into the array at the right point.
		
		printf("matSatOrder[%d] = %d\n", numberOfSatableMats, matSatOrder[numberOfSatableMats]);
		
		numberOfSatableMats++; // increment the number of materials that be saturated that we have.
	}
	printf("numberOfSatableMats = %d\n\n\n\n\n", numberOfSatableMats);
}



///this function resets all the cells to default state. Nothing
void reset_cells(void){
	int i; int j;

	for(i=0 ; i<GRID_WIDTH ; i++){
		for(j=0 ; j<GRID_HEIGHT ; j++){
			grid[i][j].mat = M_air;
			grid[i][j].sat  = M_no_saturation;
			grid[i][j].satLevel = 0;
			grid[i][j].matChange = M_no_change;
			grid[i][j].satChange = M_no_change;
			grid[i][j].satLevelChange = M_no_change;
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
	printf("printTime = %d\n\nSaturation:\n\n",printTime);
	for(j=0 ; j<GRID_HEIGHT ; j++){
		for(i=0 ; i<GRID_WIDTH ; i++){
			if(grid[i][j].sat >= 0)printf("%2d ",grid[i][j].sat);
			else printf(" . ");
		}
		printf("\n");
	}
	printf("\n\nSaturation Level:\n\n");
	for(j=0 ; j<GRID_HEIGHT ; j++){
		for(i=0 ; i<GRID_WIDTH ; i++){
			if(grid[i][j].satLevel > 0)printf("%2d ",grid[i][j].satLevel);
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

///this randomizes the materials and saturations in the grid.
///this basically randomizes cellMat[][] and cellSat[][].
void randomize_grid(){
	int i, j, temp;
	for(i=0 ; i<SCREEN_WIDTH ; i++){
		for(j=0 ; j<SCREEN_HEIGHT ; j++){
			if(get_rand(1,10) < 10){
				grid[i][j].mat = M_air;
				continue;
			}
			//get random material
			temp = M_valid_but_null_material;
			while(mats[temp].name == NULL){
				temp = get_rand(0, MAX_NUMBER_OF_UNIQUE_MATERIALS-1);
				//don't do scurge. that shit is cancer
				if(temp == M_scurge || temp == M_anti_scurge)
					temp = M_air;
			}
			grid[i][j].mat = temp;
		
		}
	}
}

void copy_affMat( struct affectMaterial *source, struct affectMaterial *destination){
	short c;
	for(c=0 ; c<8 ; c++)
		destination->chance[c] = source->chance[c];
	destination->changeOrigMat = source->changeOrigMat;
	destination->changeOrigSat = source->changeOrigSat;
	destination->changesPerEval = source->changesPerEval;
	destination->matAfter = source->matAfter;
	destination->matBefore = source->matBefore;
	destination->satAfter = source->satAfter;
	destination->satBefore = source->satBefore;
	destination->satGTE = source->satGTE;
	destination->satLTE = source->satLTE;
	destination->satNeeded = source->satNeeded;
}


