///------------IMPORTANT TERMS----------------------------------------------
/// a CELL refers to a space in the GRID.
/// the GRID is the collection of cells in the application window.
/// a MATERIAL refers to what thing is occupying a certain cell.
/// for instance, you might fill a CELL with a MATERIAL.
/// you do NOT fill a METERIAL with a CELL. that just doesn't make any sense.
/// if you want to make a cell empty, that means you are filling it with AIR.


// this is how big each square cell is
int CELL_SIZE = 16;

/// this is how large the cell grid is INSIDE OF THE USER'S WINDOW
#define GRID_WIDTH (SCREEN_WIDTH/CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT/CELL_SIZE)

// these are the maximum dimensions inside of the user's window.
#define MAX_GRID_WIDTH  1920
#define MAX_GRID_HEIGHT 1080
// this is the max size the grid[][] material array can be. this should definitely be larger than MAX_GRID_WIDTH and MAX_GRID_HEIGHT.
#define GRID_WIDTH_ELEMENTS 1920
#define GRID_HEIGHT_ELEMENTS 1080

// these are to control the view of the world. the camera allows the user to pan.
// these are measured in CELLS! NOT PIXELS!
int camera_x = GRID_WIDTH_ELEMENTS/2;
int camera_y = GRID_HEIGHT_ELEMENTS/2;

//this is how many different types of materials there can be in the game.
#define MAX_NUMBER_OF_UNIQUE_MATERIALS 100 // the materials (i.e. m_earth, m_spring) can have values from 0-99

//this holds the number of unique materials that have the ability to be saturated by something.
// this value is detected and set in the set_default_material_attributes() function
short numberOfSatableMats;

//this is how many different interactions any given material can have with other materials in neighboring cells.
// number of possible affectMats that there can be.
#define MAX_NUMBER_OF_MATERIAL_INTERACTIONS 6
//this is how many different things a given material can be saturated with
#define MAX_NUMBER_OF_SATURATIONS 4

struct cellData{
	short mat; // the material in a cell. 	// default to m_air
	short sat; // the saturation of a cell. // default to m_no_saturation
	char satLevel;  // the saturation level of a cell. i.e. how saturated is it? (integer range 1-8) default to 1
	
	short matChange; // does the material need to change?
	short satChange; // does the saturation need to change?
	char satLevelChange; // does the saturationLevel need to change?
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
#define m_mud			2
#define m_rock			3
#define m_rubble		4
#define m_sand			5

#define m_spring		7
#define m_water			8
#define m_plant			9
#define m_plant_root	10
#define m_fire			11
#define m_torch			12

#define m_tree_base		13
#define m_pipe			14


#define m_scurge		19
#define m_anti_scurge	20
#define m_bottom_feeder	21

#define m_test			23
#define m_test2			24

#define m_quicksand		25

/// non-menu materials

//tree stuff
#define m_tree_trunk		80
#define m_tree_trunk_top	81
#define m_tree_branch_left	82
#define m_tree_branch_right 83
#define m_tree_branch_end	84
#define m_tree_leaves		85
#define m_tree_leaves_end	86
#define m_tree_fruit		87

#define m_dead_scurge	98

// This provides a valid material that has no name (name = NULL).
// this is useful for... something or other.
// I think this makes it easier to randomize the grid with materials.
// so, not CRUCIAL, but, useful. It will remain for now.
#define m_valid_but_null_material (MAX_NUMBER_OF_UNIQUE_MATERIALS-1)


/// material phase definitions

// solids
#define phase_solid		0
// fluids
#define phase_fluid		1



// this has to be a complete list of all the saturatable materials in the game.
// this is used by the grid evaluator to check and apply saturations of the cells in our grid.
// these are the materials that will be checked for saturation
/// this list is initialized in the init_material_attributes() function.
short matSatOrder[MAX_NUMBER_OF_UNIQUE_MATERIALS];

void set_chance(unsigned *, unsigned);
void set_chance_symmetrical(unsigned *chance, unsigned c1, unsigned c02, unsigned c34, unsigned c57, unsigned c6);


struct affectMaterial{
	
	//this is the type of saturation our material needs to have in order to carry out the desired affect.
	//default to m_dont_care
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
	// default to m_air
	short matBefore;
	
	// the type of saturation the material must have had before being changed
	//default to m_dont_care
	short satBefore;
	
	// this is material that the affected material will turn into.
	// default to m_air
	short matAfter;
	// this is the type of saturation the material will have after the affectMat
	// default to m_no_change
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
	// default set to m_no_change.
	short changeOrigMat;
	
	// what saturation will the original material have after and only after changing as a result of affecting the nearby material?
	// default set to m_no_change.
	short changeOrigSat;
	
};

void copy_affMat( struct affectMaterial *, struct affectMaterial *);

struct saturationEffect{
	
	//this is some material that is saturating our material.
	//default to m_no_saturation
	//if satMat == m_no_saturation for a given material, that means there is no saturation effect for that material.
	short satMat;
	
	// this tells us if the saturation has memory or not.
	// if satMem == true, the saturation will stay even if the saturating material is removed.
	// if satMem == false, the saturation will be in the material in our cell so long as the saturating material stays in proximity of our cell. 
	/// IMPORTANT NOTE: If a satEffect absorbs the saturating material (see the absorb element of this structure), satMem will automatically be set to true.
	bool satMem;
	
	// does the saturation material get absorbed (i.e. turn into air) when our material gets saturated by it? or is it not affected by saturating our material?
	// 1 = gets absorbed
	// 0 = doesn't get absorbed
	/// if absorb is set to true, satMem will automatically be set to true.
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
	//the minimum saturation level needed to decay. default 1 (GTE = greater than or equal to)
	char decaySatGTE;
	//the maximum saturation level that will allow decay. default 8 (LTE = less than or equal to)
	char decaySatLTE;
	// the thing that our saturated material may decay into now that it is saturated.
	short decayIntoMat;
	// what will the saturation be after the block decays?
	// set to m_no_saturation by default.
	short decayIntoSat;
};


///this is the data structure for materials:
struct material {
	
	//material name
	char *name;
	
	// the color of the material
	uint32_t color;
	
	// this is an array of affectMaterial structures.
	//Each element of the structure array describes one type of interaction this material can have with materials in neighboring cells.
	//start with affectMat[0] and put other effects into affectMat[1] and affectMat[2] and so on.
	struct affectMaterial affectMat[MAX_NUMBER_OF_MATERIAL_INTERACTIONS];
	
	// this is an array (list) of things that can saturate this material and how it affects it.
	// you must enter them in order starting at the 0th element. i.e. satEffect[0]. once the cell evalutator finds a default saturation effect, it breaks from the saturation effect checking loop.
	// basically, if you want to make a saturation effect for a material, you have to put it in the satEffect[0] spot. if you want to make another, put it in the satEffect[1] spot. and so on and so forth.
	// once the grid evaluator finds a saturation effect that is the default (does nothing)
	struct saturationEffect satEffect[MAX_NUMBER_OF_SATURATIONS];
	
	//this is the type of material that the current material may decay into.
	short decayIntoMat;
	
	//value between 0 and 100000 describing the likelihood of this material decaying on its own.
	// 467 would mean there is a 0.467% chance of decay on each evaluation cycle.
	unsigned decayChance;
	
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
	signed short gravity;
	
	// this describes how much the material weighs.
	// heavy fluids will sink below less heavy fluids
	// air has zero weight.
	// currently, the units of weight are meaningless.
	//They only have meaning relative to the weights of other materials.
	// currently, as a standard, water has a weight of 100.
	//That should give a decent baseline of relativity for other materials.
	int weight;
	
	// two possible material phases: phase_solid, phase_fluid.
	// heavy fluids fall down through light fluids.
	// heavy solids fall down through light fluids.
	// heavy fluids DO NOT fall down through light solids.
	char phase;
	
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
		mats[i].gravity = 0;			// is not affected by gravity
		mats[i].weight = 0;				// by default everything has 0 weight (air is said to have zero weight).
		mats[i].phase = phase_fluid;	// by default, materials are fluids.
		mats[i].color = 0x000000;		//default color is black
		mats[i].decayChance = 0;		// 0% chance to decay.
		mats[i].decayIntoMat = m_air;	 // decay into air (this is irrelevant because there is a 0% decayChance anyway)
	
		 // for every saturation effect, set it to the default of not being able to be saturated by anything.
		for(s=0 ; s<MAX_NUMBER_OF_SATURATIONS ; s++){
			mats[i].satEffect[s].satMat = m_no_saturation; // by default, nothing can be saturated with anything.
			mats[i].satEffect[s].satMem = false; // by default, there is no memory in saturation. If a satEffect absorbs the saturating material, satMem will auto matically be set to true.
			mats[i].satEffect[s].absorb = false; // does not absorb by default.
			mats[i].satEffect[s].decayChance = 0; // be default, nothing saturated will decay into anything.
			mats[i].satEffect[s].decayIntoMat =	m_air; // default decay into air. kind of irrelevant because the chance of decay is already 0. oh well. better safe than sorry.
			mats[i].satEffect[s].decayIntoSat =	m_no_saturation; // the default saturation the material will have after decaying is nothing.
			mats[i].satEffect[s].decaySatGTE = 1;
			mats[i].satEffect[s].decaySatLTE = 8;
			for(k=0 ; k<8 ; k++){ // set all the absorb chances to 0% be default
				mats[i].satEffect[s].chance[k] = 0; // 0%
			}
		}
	
		// for every affect that our material can have on other materials, set it to default (default = air changes to air with a 0% chance. nothing happens.)
		for(m=0 ; m<MAX_NUMBER_OF_MATERIAL_INTERACTIONS ; m++){
			mats[i].affectMat[m].matBefore = m_dont_care;// affects everything
			mats[i].affectMat[m].matAfter = m_no_change; // doesn't do anything to anything
			mats[i].affectMat[m].satBefore = m_dont_care; // by default, it doesn't matter what the affected material had for saturation before the incident.
			mats[i].affectMat[m].satAfter  = m_no_saturation; // by default, there is no saturation in the new material
			mats[i].affectMat[m].changesPerEval = 8; // by default, any material can affect the stuff around it all at once.
			mats[i].affectMat[m].satNeeded = m_dont_care; // by default, there is no required saturation.
			mats[i].affectMat[m].satGTE = 1;	// the Saturation can be Greater Than or Equal to 1.
			mats[i].affectMat[m].satLTE = 8;	// the Saturation can be Less Than or Equal to 8.
			mats[i].affectMat[m].changeOrigMat = m_no_change; // by default, doesn't change the original block type.
			mats[i].affectMat[m].changeOrigSat = m_no_change; // by default, the original materials saturation does not change.
			for(k=0 ; k<8 ; k++){
				mats[i].affectMat[m].chance[k] = 0; // no chance of affecting anything (so it never affects air. it never will actually go through the process of turning air into air)
			}
		}
	}
}

void init_material_attributes(void){
	
	// I don't need to specify anything for air because air doesn't do anything.
	// all of the elements of the mats[m_air] structure are initialized in the set_default_material_attributes() function.
	mats[m_air].name = "Air";
	///DON'T YOU DARE CHANGE ANYTHING ABOUT AIR! you SACK of SHIT!
//-------------------------------------------------------------------------------------------------------------------------------
	/*
	mats[m_smoke].name = "Smoke";
	mats[m_smoke].color = 0xa3a3a3;
	
	mats[m_smoke].affectMat[0].matBefore = m_air;  /// smoke wafts upwards
	mats[m_smoke].affectMat[0].matAfter = m_smoke;
	mats[m_smoke].affectMat[0].changeOrigMat = m_air;
	mats[m_smoke].affectMat[0].chance[0] = 12500;
	mats[m_smoke].affectMat[0].chance[1] = 12500;
	mats[m_smoke].affectMat[0].chance[2] = 12500;
	mats[m_smoke].affectMat[0].chance[3] = 5000;
	mats[m_smoke].affectMat[0].chance[4] = 5000;
	mats[m_smoke].affectMat[0].changesPerEval = 1;
	mats[m_smoke].decayChance = 400;
	mats[m_smoke].decayIntoMat = m_water;
	*/
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_earth].name = "Earth";
	mats[m_earth].color = 0x8b672d;
	mats[m_earth].gravity = 1;
	mats[m_earth].weight = 120;
	
	
	mats[m_earth].satEffect[0].satMat = m_water;    /// earth turns into mud when soaked
	mats[m_earth].satEffect[0].absorb = 1;
	mats[m_earth].satEffect[0].chance[0] = 125;
	mats[m_earth].satEffect[0].chance[1] = 125;
	mats[m_earth].satEffect[0].chance[2] = 125;
	mats[m_earth].satEffect[0].chance[3] = 125;
	mats[m_earth].satEffect[0].chance[4] = 125;
	mats[m_earth].satEffect[0].chance[5] = 125;
	mats[m_earth].satEffect[0].chance[6] = 125;
	mats[m_earth].satEffect[0].chance[7] = 125;
	mats[m_earth].satEffect[0].decayIntoMat = m_mud;
	mats[m_earth].satEffect[0].decayChance = 100000;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_plant].name = "Plant";
    mats[m_plant].color = 0x008951;//0x20e112;
    
    
    /*
	mats[m_plant].satEffect[1].decaySatGTE = 7;		/// plant can be over crowded
	mats[m_plant].satEffect[1].satMat = m_plant;
	set_chance(mats[m_plant].satEffect[1].chance, 100000);
	mats[m_plant].satEffect[1].decayChance = 3500;
	mats[m_plant].satEffect[1].decayIntoMat = m_air;
	mats[m_plant].satEffect[1].decayIntoSat = m_no_saturation;
	*/
	/*
	mats[m_plant].satEffect[1].satMat = m_earth;
	set_chance(mats[m_plant].satEffect[1].chance, 100000);
	*/
	mats[m_plant].satEffect[0].satMat = m_water;		/// plant absorbs water. plant can die from too much water
	set_chance(mats[m_plant].satEffect[0].chance, 800);
	mats[m_plant].satEffect[0].absorb = true;
	mats[m_plant].satEffect[0].decayChance = 15000;
	mats[m_plant].satEffect[0].decayIntoMat = m_water;
	mats[m_plant].satEffect[0].decaySatGTE = 3;
	
	
	mats[m_plant].satEffect[3].satMat = m_fire;			/// plant starts to burn when it is next to fire
    mats[m_plant].satEffect[3].absorb = true;
    set_chance(mats[m_plant].satEffect[3].chance, 97000);
    mats[m_plant].satEffect[3].decayChance = 4000;
    mats[m_plant].satEffect[3].decayIntoMat = m_fire;
    
	mats[m_plant].affectMat[1].matBefore = m_mud; 		/// plant grows plant_roots into into mud
	mats[m_plant].affectMat[1].matAfter  = m_plant_root;
	mats[m_plant].affectMat[1].chance[0] = 100;
	mats[m_plant].affectMat[1].chance[1] = 100;
	mats[m_plant].affectMat[1].chance[2] = 100;
	mats[m_plant].affectMat[1].chance[3] = 300;
	mats[m_plant].affectMat[1].chance[4] = 300;
	mats[m_plant].affectMat[1].chance[5] = 450;
	mats[m_plant].affectMat[1].chance[6] = 450;
	mats[m_plant].affectMat[1].chance[7] = 450;
	
    mats[m_plant].affectMat[2].matBefore = m_air;		/// burning plant spreads fire
    mats[m_plant].affectMat[2].matAfter  = m_fire;
    set_chance_symmetrical(mats[m_plant].affectMat[2].chance, 6000,5000, 3500, 2000, 1000);
    mats[m_plant].affectMat[2].chance[0] = 100000;
    mats[m_plant].affectMat[2].satNeeded = m_fire;
    mats[m_plant].affectMat[2].changesPerEval = 1;
    
    
    mats[m_plant].affectMat[3].satNeeded = m_water;		/// wet plants may grow more plant at the cost of their water saturation
    mats[m_plant].affectMat[3].changeOrigSat = m_no_saturation;
    mats[m_plant].affectMat[3].matBefore = m_air;
    mats[m_plant].affectMat[3].matAfter = m_plant;
    mats[m_plant].affectMat[3].changesPerEval = 1;
    set_chance(mats[m_plant].affectMat[3].chance, 2500);
    
    mats[m_plant].affectMat[4].satNeeded = m_water;		/// wet plants may push their water saturation around
    mats[m_plant].affectMat[4].changeOrigSat = m_no_saturation;
    mats[m_plant].affectMat[4].matBefore = m_plant;
    mats[m_plant].affectMat[4].satBefore = m_no_saturation;
    mats[m_plant].affectMat[4].satAfter  = m_water;
    mats[m_plant].affectMat[4].changesPerEval = 1;
    //set_chance( mats[m_plant].affectMat[4].chance, 6000);
    mats[m_plant].affectMat[4].chance[0] = 6000;
    mats[m_plant].affectMat[4].chance[1] = 8000;
    mats[m_plant].affectMat[4].chance[2] = 6000;
    mats[m_plant].affectMat[4].chance[3] = 1000;
    mats[m_plant].affectMat[4].chance[4] = 1000;
    mats[m_plant].affectMat[4].chance[5] = 300;
    mats[m_plant].affectMat[4].chance[6] = 50;
    mats[m_plant].affectMat[4].chance[7] = 300;
	
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_water].name = "Water";
	mats[m_water].gravity = -200;
    mats[m_water].color = 0x52a9e0;
    mats[m_water].weight = 100;
    
    mats[m_water].affectMat[0].matBefore = m_fire;		/// water puts out fire
    mats[m_water].affectMat[0].matAfter  = m_air;
    set_chance(mats[m_water].affectMat[0].chance, 100000);
//-------------------------------------------------------------------------------------------------------------------------------
    mats[m_spring].name = "Spring";
	mats[m_spring].color = 0x97bcbb;
	
	mats[m_spring].affectMat[0].matBefore = m_air;  	/// spring generates in open cells
	mats[m_spring].affectMat[0].matAfter = m_water;
	set_chance(mats[m_spring].affectMat[0].chance, 100);
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_pipe].name = "Pipe";
	mats[m_pipe].color = /*0x778866; */ 0xbe7e22;
	mats[m_pipe].satEffect[0].absorb = true;			/// pipe absorbs water around it.
	mats[m_pipe].satEffect[0].satMat = m_water;
	mats[m_pipe].satEffect[0].chance[6] = 100000; // pipe only absorbs water from below it.
	
	mats[m_pipe].affectMat[0].matBefore = m_pipe;		/// pipe moves water up
	mats[m_pipe].affectMat[0].satBefore = m_no_saturation;
	mats[m_pipe].affectMat[0].satAfter = m_water;
	mats[m_pipe].affectMat[0].satNeeded = m_water;
	mats[m_pipe].affectMat[0].changeOrigSat = m_no_saturation;
	mats[m_pipe].affectMat[0].chance[1] = 100000;
	
	mats[m_pipe].affectMat[1].matBefore = m_air;
	mats[m_pipe].affectMat[1].matAfter = m_water;
	mats[m_pipe].affectMat[1].satNeeded = m_water;
	mats[m_pipe].affectMat[1].changeOrigSat = m_no_saturation;
	mats[m_pipe].affectMat[1].chance[1] = 100000;
	
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_fire].name = "Fire"; 
	mats[m_fire].color = 0xd83313;
	mats[m_fire].decayIntoMat = m_air;
	mats[m_fire].decayChance = 2500;
	mats[m_fire].weight = 
	
	mats[m_fire].affectMat[0].matBefore = m_air;		// fire creates more fire (flames) primarily above itself
	mats[m_fire].affectMat[0].matAfter  = m_fire;
	mats[m_fire].affectMat[0].chance[0] = 600;
	mats[m_fire].affectMat[0].chance[1] = 1000;
	mats[m_fire].affectMat[0].chance[2] = 600;
	mats[m_fire].affectMat[0].chance[3] = 250;
	mats[m_fire].affectMat[0].chance[4] = 250;
	mats[m_fire].affectMat[0].chance[5] = 100;
	mats[m_fire].affectMat[0].chance[6] = 75;
	mats[m_fire].affectMat[0].chance[7] = 100;
	//set_chance(mats[m_fire].affectMat[0].chance, 4000); 	// set fire to MAXIMUM OVERDRIVE!!
	
	/*
	mats[m_fire].affectMat[0].matBefore = m_air;		/// fire makes smoke
	mats[m_fire].affectMat[0].matAfter  = m_smoke;
	mats[m_fire].affectMat[0].chance[0] =  100;
	mats[m_fire].affectMat[0].chance[1] = 1000;
	mats[m_fire].affectMat[0].chance[2] =  100;
	*/
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_torch].name = "Torch";
	mats[m_torch].color = 0x8b5800;
	
	mats[m_torch].satEffect[0].chance[0] = 5000;		// torches can be saturated with fire
	mats[m_torch].satEffect[0].chance[1] = 33333;	
	mats[m_torch].satEffect[0].chance[2] = 5000;	
	mats[m_torch].satEffect[0].chance[3] = 7000;
	mats[m_torch].satEffect[0].chance[4] = 7000;
	mats[m_torch].satEffect[0].satMat = m_fire;
	mats[m_torch].satEffect[0].satMem = 1;
	
	mats[m_torch].affectMat[0].satNeeded = m_fire;		// torches can create flames when they are lit (saturated with fire)
	mats[m_torch].affectMat[0].matBefore = m_air;
	mats[m_torch].affectMat[0].matAfter  = m_fire;
	mats[m_torch].affectMat[0].chance[1] = 70000;
	
	mats[m_torch].affectMat[1].chance[1] = 5000;		// when water hits the top of a torch, it puts it out
	mats[m_torch].affectMat[1].chance[3] = 5000;
	mats[m_torch].affectMat[1].chance[4] = 5000;
	mats[m_torch].affectMat[1].chance[6] = 1000;
	mats[m_torch].affectMat[1].changeOrigSat = m_no_saturation;
	mats[m_torch].affectMat[1].matBefore = m_water;
	mats[m_torch].affectMat[1].matAfter = m_water;
	mats[m_torch].affectMat[1].satNeeded = m_fire;
	
	
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_test].name = "test"; // the material that jensen tests evaluate_grid() with
	mats[m_test].color = 0xCCFF00;
	mats[m_test].gravity = 1;
	mats[m_test].weight = 50;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_test2].name = "test2"; // the material that jensen tests evaluate_grid() with
	mats[m_test2].color = 0x00FFCC;
	mats[m_test2].gravity = 2;
	mats[m_test2].weight = 200;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_rock].name = "Rock";
	mats[m_rock].color = 0x5A5651;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_rubble].name = "Rubble";
	mats[m_rubble].color = 0x715A63;
	mats[m_rubble].gravity = 3;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_tree_base].name = "Tree";					/// this is the start of the tree. this is what you palce and watch a tree grow.
	mats[m_tree_base].color = 0x7b5126;
	mats[m_tree_base].affectMat[0].matBefore = m_air;
	mats[m_tree_base].affectMat[0].matAfter  = m_tree_trunk;
	mats[m_tree_base].affectMat[0].chance[1] = 850;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_sand].name = "Sand";
	mats[m_sand].gravity = -2;
	mats[m_sand].color = 0xcfc1aa;
	mats[m_sand].weight = 140;
	unsigned int quickSandChance = 10000;
	
	mats[m_sand].affectMat[0].matBefore = m_water;		// when water is on top of sand, the sand turns into quicksand.
	mats[m_sand].affectMat[0].matAfter  = m_air;
	mats[m_sand].affectMat[0].chance[0] = quickSandChance;
	mats[m_sand].affectMat[0].chance[1] = quickSandChance;
	mats[m_sand].affectMat[0].chance[2] = quickSandChance;
	mats[m_sand].affectMat[0].chance[3] = quickSandChance;
	mats[m_sand].affectMat[0].chance[4] = quickSandChance;
	mats[m_sand].affectMat[0].changeOrigMat = m_quicksand;
	
	mats[m_sand].affectMat[0].matBefore = m_water;		// when sand is on top of water, the water turns into quicksand.
	mats[m_sand].affectMat[0].matAfter  = m_quicksand;
	mats[m_sand].affectMat[0].chance[5] = quickSandChance;
	mats[m_sand].affectMat[0].chance[6] = quickSandChance;
	mats[m_sand].affectMat[0].chance[7] = quickSandChance;
	mats[m_sand].affectMat[0].changeOrigMat = m_air;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_quicksand].name = "Quicksand";
	mats[m_quicksand].gravity = -5;
	mats[m_quicksand].weight = mats[m_water].weight + mats[m_sand].weight;
	mats[m_quicksand].color = 0x9f917e;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_mud].name = "Mud";
	mats[m_mud].gravity = -3;
	mats[m_mud].weight = mats[m_water].weight + mats[m_earth].weight;
	mats[m_mud].color = 0x644310;
	mats[m_mud].satEffect[0].absorb = 1;
	mats[m_mud].satEffect[0].satMat = m_water;
	mats[m_mud].satEffect[0].satMem = true;
	mats[m_mud].satEffect[0].chance[0] = 1000; 			/// mud absorbs water
	mats[m_mud].satEffect[0].chance[1] = 1100;
	mats[m_mud].satEffect[0].chance[2] = 1000;
	mats[m_mud].satEffect[0].chance[3] = 500;
	mats[m_mud].satEffect[0].chance[4] = 500;
	mats[m_mud].satEffect[0].chance[5] = 150;
	mats[m_mud].satEffect[0].chance[6] = 200;
	mats[m_mud].satEffect[0].chance[7] = 150;
	
	mats[m_mud].affectMat[1].changesPerEval = 1; 		/// when mud is saturated with water, it will leak water into other mud that is NOT saturated with water.
	mats[m_mud].affectMat[1].changeOrigSat = m_no_saturation;
	mats[m_mud].affectMat[1].satNeeded = m_water;
	mats[m_mud].affectMat[1].matBefore = m_mud;
	mats[m_mud].affectMat[1].matAfter = m_mud;
	mats[m_mud].affectMat[1].satBefore = m_no_saturation;
	mats[m_mud].affectMat[1].satAfter = m_water;
	mats[m_mud].affectMat[1].chance[3] = 400;
	mats[m_mud].affectMat[1].chance[4] = 400;
	mats[m_mud].affectMat[1].chance[5] = 700;
	mats[m_mud].affectMat[1].chance[6] = 950;
	mats[m_mud].affectMat[1].chance[7] = 700;
	
	mats[m_mud].affectMat[2].changesPerEval = 1; 		/// mud leaks water into dry earth.
	mats[m_mud].affectMat[2].satNeeded = m_water;
	mats[m_mud].affectMat[2].changeOrigSat = m_no_saturation;
	mats[m_mud].affectMat[2].matBefore = m_earth;
	mats[m_mud].affectMat[2].matAfter = m_mud;
	mats[m_mud].affectMat[2].satBefore = m_no_saturation;
	mats[m_mud].affectMat[1].chance[0] = 45;
	mats[m_mud].affectMat[1].chance[1] = 70;
	mats[m_mud].affectMat[1].chance[2] = 45;
	mats[m_mud].affectMat[1].chance[3] = 150;
	mats[m_mud].affectMat[1].chance[4] = 150;
	mats[m_mud].affectMat[1].chance[5] = 325;
	mats[m_mud].affectMat[1].chance[6] = 650;
	mats[m_mud].affectMat[1].chance[7] = 325;
	
	mats[m_mud].affectMat[0].changeOrigMat = m_earth;	/// mud turns into dirt if it can make the dirt below it turn into earth.
	mats[m_mud].affectMat[0].changesPerEval = 1;
	mats[m_mud].affectMat[0].satBefore = m_no_saturation;
	mats[m_mud].affectMat[0].satAfter = m_no_saturation;
	mats[m_mud].affectMat[0].matBefore = m_earth;
	mats[m_mud].affectMat[0].matAfter = m_mud;
	mats[m_mud].affectMat[0].chance[0] = 20*3;
	mats[m_mud].affectMat[0].chance[1] = 15*3;
	mats[m_mud].affectMat[0].chance[2] = 20*3;
	mats[m_mud].affectMat[0].chance[3] = 55*3;
	mats[m_mud].affectMat[0].chance[4] = 55*3;
	mats[m_mud].affectMat[0].chance[5] = 100*3;
	mats[m_mud].affectMat[0].chance[6] = 175*3;
	mats[m_mud].affectMat[0].chance[7] = 100*3;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_plant_root].name = "Plant Root";
	mats[m_plant_root].color = 0xbfc69e;
	mats[m_plant_root].affectMat[0].matBefore = m_water;
	mats[m_plant_root].affectMat[0].matAfter  = m_plant;
	mats[m_plant_root].affectMat[0].chance[0] = 140;
	mats[m_plant_root].affectMat[0].chance[1] = 200; 
	mats[m_plant_root].affectMat[0].chance[2] = 140;
	mats[m_plant_root].affectMat[0].chance[3] = 500;
	mats[m_plant_root].affectMat[0].chance[4] = 500;
	mats[m_plant_root].affectMat[0].chance[5] = 800;
	mats[m_plant_root].affectMat[0].chance[6] = 800;
	mats[m_plant_root].affectMat[0].chance[7] = 800;
	mats[m_plant_root].affectMat[1].matBefore = m_mud;
	mats[m_plant_root].affectMat[1].matAfter  = m_plant_root;
	mats[m_plant_root].affectMat[1].chance[0] = 70;
	mats[m_plant_root].affectMat[1].chance[1] = 95; 
	mats[m_plant_root].affectMat[1].chance[2] = 70;
	mats[m_plant_root].affectMat[1].chance[3] = 250;
	mats[m_plant_root].affectMat[1].chance[4] = 250;
	mats[m_plant_root].affectMat[1].chance[5] = 400;
	mats[m_plant_root].affectMat[1].chance[6] = 400;
	mats[m_plant_root].affectMat[1].chance[7] = 400;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_scurge].name = "Scurge";
	mats[m_scurge].color = 0x561377;
	mats[m_scurge].affectMat[0].matBefore = m_air; /// scurge turns empty space into scurge.
	mats[m_scurge].affectMat[0].matAfter = m_scurge;
	mats[m_scurge].affectMat[0].changesPerEval = 1;
	set_chance( &mats[m_scurge].affectMat[0].chance[0], 100000 );
	mats[m_scurge].satEffect[0].satMat = m_scurge; /// scurge turns other scurge into empty space
	mats[m_scurge].satEffect[0].decayIntoMat = m_air;
	mats[m_scurge].satEffect[0].decayChance = 100000;
	set_chance( &mats[m_scurge].satEffect[0].chance[0], 8700);
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_anti_scurge].name = "Anti-Scurge";
	mats[m_anti_scurge].color = 0x0b94a0;
	mats[m_anti_scurge].affectMat[0].matBefore = m_air; /// anti scurge turns empty space into anti scurge.
	mats[m_anti_scurge].affectMat[0].matAfter = m_anti_scurge;
	mats[m_anti_scurge].affectMat[0].changesPerEval = 1;
	set_chance( &mats[m_anti_scurge].affectMat[0].chance[0], 100000 );
	
	mats[m_anti_scurge].satEffect[0].satMat = m_anti_scurge; /// anti scurge turns other anti scurge into empty space
	mats[m_anti_scurge].satEffect[0].decayIntoMat = m_air;
	mats[m_anti_scurge].satEffect[0].decayChance = 100000;
	set_chance( &mats[m_anti_scurge].satEffect[0].chance[0], mats[m_scurge].satEffect[0].chance[0]); // make the anti scuge survive just as well as the scurge
	
	mats[m_anti_scurge].affectMat[1].matBefore = m_scurge;		/// anti scurge will fight the scurge to the death
	mats[m_anti_scurge].affectMat[1].matAfter = m_dead_scurge;
	mats[m_anti_scurge].affectMat[1].changeOrigMat = m_dead_scurge;
	mats[m_anti_scurge].affectMat[1].changeOrigSat = m_no_saturation;
	mats[m_anti_scurge].affectMat[1].changesPerEval = 1;
	set_chance( &mats[m_anti_scurge].affectMat[1].chance[0], 100000);
//-------------------------------------------------------------------------------------------------------------------------------
	//mats[m_dead_scurge].name = NULL;		
	mats[m_dead_scurge].color = 0xa00b0b;
	mats[m_dead_scurge].gravity = 2;			///dead scurge falls. that's really it...
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_bottom_feeder].name = "Bottom Feeder";
	mats[m_bottom_feeder].color = 0xff6400;
	
	mats[m_bottom_feeder].affectMat[0].matBefore = m_scurge;			/// bottom feeders eat scurge
	mats[m_bottom_feeder].affectMat[0].matAfter  = m_bottom_feeder;
	mats[m_bottom_feeder].affectMat[0].satAfter  = m_no_saturation;
	set_chance( &mats[m_bottom_feeder].affectMat[0].chance[0], 100000 );
	mats[m_bottom_feeder].affectMat[0].changesPerEval = 8;
	
	mats[m_bottom_feeder].affectMat[1].matBefore = m_anti_scurge;		/// bottom feeders also eat anti-scurge
	mats[m_bottom_feeder].affectMat[1].matAfter  = m_bottom_feeder;
	mats[m_bottom_feeder].affectMat[1].satAfter  = m_no_saturation;
	set_chance( &mats[m_bottom_feeder].affectMat[1].chance[0], 100000 );
	mats[m_bottom_feeder].affectMat[1].changesPerEval = 8;
	
	mats[m_bottom_feeder].affectMat[2].matBefore = m_dead_scurge;		/// bottom feeders also eat dead scurge
	mats[m_bottom_feeder].affectMat[2].matAfter  = m_bottom_feeder;
	mats[m_bottom_feeder].affectMat[2].satAfter  = m_no_saturation;
	set_chance( &mats[m_bottom_feeder].affectMat[2].chance[0], 2000 );
	mats[m_bottom_feeder].affectMat[2].changesPerEval = 8;
	
	mats[m_bottom_feeder].affectMat[3].matBefore = m_air;				/// bottom feeders waltz around in search of food
	mats[m_bottom_feeder].affectMat[3].matAfter = m_bottom_feeder; 
	mats[m_bottom_feeder].affectMat[3].changesPerEval = 1;
	mats[m_bottom_feeder].affectMat[3].changeOrigMat = m_air;
	mats[m_bottom_feeder].affectMat[3].changeOrigSat = m_no_saturation;
	set_chance(mats[m_bottom_feeder].affectMat[3].chance, 100000);
	
	mats[m_bottom_feeder].satEffect[0].satMat = m_bottom_feeder;		/// bottom feeders cannot survive when they are packed together
	set_chance(mats[m_bottom_feeder].satEffect[0].chance, 100000);
	mats[m_bottom_feeder].satEffect[0].decaySatGTE = 6; // must be partially surrounded by air to decay
	mats[m_bottom_feeder].satEffect[0].decayIntoMat = m_air;
	mats[m_bottom_feeder].satEffect[0].decayChance = 4000;
	
	mats[m_bottom_feeder].satEffect[1].satMat = m_air;					/// if bottom feeders are completely surrounded by air, then they have a chance of dying
	set_chance(mats[m_bottom_feeder].satEffect[1].chance, 100000);
	mats[m_bottom_feeder].satEffect[1].decaySatGTE = 8; // must be completely surrounded by air to decay
	mats[m_bottom_feeder].satEffect[1].decayIntoMat = m_air;
	mats[m_bottom_feeder].satEffect[1].decayChance = 1000;
	
	
	/*
	mats[m_bottom_feeder].satEffect[1].satMat = m_bottom_feeder;
	set_chance(mats[m_bottom_feeder].satEffect[1].chance, 100000);
	mats[m_bottom_feeder].satEffect[1].decaySatGTE = 6; 
	mats[m_bottom_feeder].satEffect[1].decayChance = 33000;
	*/
	/*
	mats[m_bottom_feeder].affectMat[4].matBefore = m_bottom_feeder;
	mats[m_bottom_feeder].affectMat[4].matAfter  = m_air;
	mats[m_bottom_feeder].affectMat[4].changeOrigMat = m_air;
	*/
//-------------------------------------------------------------------------------------------------------------------------------
	//mats[m_bedrock].color = 0x2f2614;
	//mats[m_bedrock].name  = "Bedrock";
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_tree_trunk].color = mats[m_tree_base].color;
	
	mats[m_tree_trunk].satEffect[0].satMat = m_tree_trunk; /// tree_trunk can tell when there is tree_trunk around it.
	mats[m_tree_trunk].satEffect[0].chance[1] = 100000;
	
	mats[m_tree_trunk].affectMat[0].matBefore = m_air;  /// turns air into leaves only when there isn't a tree_trunk above it.
	mats[m_tree_trunk].affectMat[0].matAfter  = m_tree_leaves_end;
	mats[m_tree_trunk].affectMat[0].chance[0] = 350;
	mats[m_tree_trunk].affectMat[0].chance[1] = 1000;
	mats[m_tree_trunk].affectMat[0].chance[2] = 350;
	mats[m_tree_trunk].affectMat[0].chance[3] = 500;
	mats[m_tree_trunk].affectMat[0].chance[4] = 500;
	mats[m_tree_trunk].affectMat[0].satNeeded = m_no_saturation;
	
	//copy_affMat(&mats[m_tree_trunk].affectMat[0], &mats[m_tree_trunk].affectMat[1]);
	
	mats[m_tree_trunk].affectMat[2].matBefore = m_tree_leaves_end;		/// tree_trunk grows upwards into tree_leaves_end
	mats[m_tree_trunk].affectMat[2].matAfter  = m_tree_trunk;
	mats[m_tree_trunk].affectMat[2].chance[1] = 500;
	
	mats[m_tree_trunk].affectMat[3].matBefore = m_tree_leaves_end;		/// tree trunk grows tree_trunk_end into tree_leaves_end
	mats[m_tree_trunk].affectMat[3].matAfter  = m_tree_trunk_top;
	mats[m_tree_trunk].affectMat[3].chance[1] = 200;
	
	mats[m_tree_trunk].affectMat[4].matBefore = m_tree_leaves_end; /// once tree has grown, it sheds the leaves lower on it's trunk.
	mats[m_tree_trunk].affectMat[4].matAfter  = m_air;
	//mats[m_tree_trunk].affectMat[4].satBefore = m_tree_trunk;		// tree_trunk can only remove leaves that are saturated with tree_trunk.
	mats[m_tree_trunk].affectMat[4].chance[3] = 450;				// if end_leaves are saturated with tree_trunk_top, they are not removed.
	mats[m_tree_trunk].affectMat[4].chance[4] = 450;
	mats[m_tree_trunk].affectMat[4].satNeeded = m_tree_trunk;
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_tree_trunk_top].color = mats[m_tree_base].color; /// this is the top of the tree. the trunk will stop growing when this material is spawned.
	
	mats[m_tree_trunk_top].affectMat[0].matBefore = m_air;		///turns air into leaves_end
	mats[m_tree_trunk_top].affectMat[0].matAfter  = m_tree_leaves_end;
	mats[m_tree_trunk_top].affectMat[0].chance[0] = 500;
	mats[m_tree_trunk_top].affectMat[0].chance[1] = 750;
	mats[m_tree_trunk_top].affectMat[0].chance[2] = 500;
	
	mats[m_tree_trunk_top].affectMat[1].matBefore = m_tree_leaves_end;
	mats[m_tree_trunk_top].affectMat[1].matAfter  = m_tree_branch_right;
	mats[m_tree_trunk_top].affectMat[1].chance[2] = 250;		/// spawns tree_branch_right on its rght side.
	
	mats[m_tree_trunk_top].affectMat[2].matBefore = m_tree_leaves_end;
	mats[m_tree_trunk_top].affectMat[2].matAfter  = m_tree_branch_left;
	mats[m_tree_trunk_top].affectMat[2].chance[0] = 250;		/// spawns tree_branch_right on its rght side.
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_tree_leaves].color = 0x708D23;
	
	mats[m_tree_leaves].satEffect[0].satMat = m_tree_branch_left; /// tree_leaves saturated by tree_branches_right
	set_chance(mats[m_tree_leaves].satEffect[0].chance, 100000);
	
	mats[m_tree_leaves].satEffect[1].satMat = m_tree_branch_right;/// and tree_leaves saturated by tree_branches_right
	set_chance(mats[m_tree_leaves].satEffect[1].chance, 100000);
	
	mats[m_tree_leaves].affectMat[0].matBefore = m_air;
	mats[m_tree_leaves].affectMat[0].matAfter  = m_tree_leaves;
	mats[m_tree_leaves].affectMat[0].satNeeded = m_any_of_my_sats;
	mats[m_tree_leaves].affectMat[0].chance[1] = 100;
	mats[m_tree_leaves].affectMat[0].chance[0] = 
	mats[m_tree_leaves].affectMat[0].chance[2] = 54;
	mats[m_tree_leaves].affectMat[0].chance[3] = 
	mats[m_tree_leaves].affectMat[0].chance[4] = 40;
	mats[m_tree_leaves].affectMat[0].chance[5] = 
	mats[m_tree_leaves].affectMat[0].chance[7] = 35;
	mats[m_tree_leaves].affectMat[0].chance[6] = 20;
	
	
	mats[m_tree_leaves].affectMat[1].matBefore = m_air;
	mats[m_tree_leaves].affectMat[1].matAfter  = m_tree_leaves_end;
	mats[m_tree_leaves].affectMat[1].chance[1] = 350;
	mats[m_tree_leaves].affectMat[1].chance[0] = 
	mats[m_tree_leaves].affectMat[1].chance[2] = 250;
	mats[m_tree_leaves].affectMat[1].chance[3] = 
	mats[m_tree_leaves].affectMat[1].chance[4] = 200;
	mats[m_tree_leaves].affectMat[1].chance[5] = 
	mats[m_tree_leaves].affectMat[1].chance[7] = 165;
	mats[m_tree_leaves].affectMat[1].chance[6] = 100;
	
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_tree_leaves_end].color = 0x708D23;
	
	mats[m_tree_leaves_end].satEffect[0].satMat = m_tree_trunk;		/// tree_leaves_end can be saturated by both tree_trunk 
	set_chance(mats[m_tree_leaves_end].satEffect[0].chance, 100000);
	
	mats[m_tree_leaves_end].satEffect[1].satMat = m_tree_trunk_top; /// and tree_trunk_top.
	set_chance(mats[m_tree_leaves_end].satEffect[1].chance, 100000);
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_tree_branch_right].color = mats[m_tree_base].color;
	
	mats[m_tree_branch_right].satEffect[0].satMat = m_tree_branch_right; /// can be saturated by tree_branch_right
	mats[m_tree_branch_right].satEffect[0].chance[1] = 100000;
	mats[m_tree_branch_right].satEffect[0].chance[2] = 100000;
	mats[m_tree_branch_right].satEffect[0].chance[4] = 100000;
	
	mats[m_tree_branch_right].satEffect[0].satMat = m_tree_branch_end;  /// can be saturated by tree_branch_end
	mats[m_tree_branch_right].satEffect[0].chance[1] = 100000;
	mats[m_tree_branch_right].satEffect[0].chance[2] = 100000;
	mats[m_tree_branch_right].satEffect[0].chance[4] = 100000;
	
	mats[m_tree_branch_right].affectMat[0].matBefore = m_air;			/// tree_branch_right makes leaves around it.
	mats[m_tree_branch_right].affectMat[0].matAfter  = m_tree_leaves;
	set_chance( &mats[m_tree_branch_right].affectMat[0].chance[0], 433);
	//mats[m_tree_branch_right].affectMat[0].satNeeded = m_no_saturation;
	
	mats[m_tree_branch_right].affectMat[1].matBefore = m_air;			/// tree_branch_right makes leaves_end around it.
	mats[m_tree_branch_right].affectMat[1].matAfter  = m_tree_leaves_end;
	set_chance( &mats[m_tree_branch_right].affectMat[1].chance[0], 150);
	//mats[m_tree_branch_right].affectMat[1].satNeeded = m_no_saturation;
	
	mats[m_tree_branch_right].affectMat[2].matBefore = m_tree_leaves;		/// tree_branch_right spawns tree_branch end in these locations.
	mats[m_tree_branch_right].affectMat[2].matAfter  = m_tree_branch_end;
	mats[m_tree_branch_right].affectMat[2].chance[7] = 80;
	mats[m_tree_branch_right].affectMat[2].chance[2] = 180;
	mats[m_tree_branch_right].affectMat[2].chance[0] = 75;
	mats[m_tree_branch_right].affectMat[2].satNeeded = m_no_saturation;
	mats[m_tree_branch_right].affectMat[2].changesPerEval = 1; // only one branch per evaluation
	
	mats[m_tree_branch_right].affectMat[3].matBefore = m_tree_leaves;		/// tree_branch_right grows more right/up
	mats[m_tree_branch_right].affectMat[3].matAfter  = m_tree_branch_right;
	mats[m_tree_branch_right].affectMat[3].chance[1] = 170;
	mats[m_tree_branch_right].affectMat[3].chance[2] = 400;
	mats[m_tree_branch_right].affectMat[3].chance[4] = 70;
	mats[m_tree_branch_right].affectMat[3].satNeeded = m_no_saturation;
	mats[m_tree_branch_right].affectMat[3].changesPerEval = 1; // only one branch per evaluation

//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_tree_branch_left].color = mats[m_tree_base].color;
	
	/// tree_branch_left makes leaves around it as well
	copy_affMat(&mats[m_tree_branch_right].affectMat[0], &mats[m_tree_branch_left].affectMat[0]);
//-------------------------------------------------------------------------------------------------------------------------------
	mats[m_tree_branch_end].color = mats[m_tree_base].color;
	
	mats[m_tree_branch_end].affectMat[0].matBefore = m_tree_leaves;
	mats[m_tree_branch_end].affectMat[0].matAfter  = m_tree_leaves_end;
	set_chance(mats[m_tree_branch_end].affectMat[0].chance, 100000); // instantly changes into end leaves.
	
	mats[m_tree_branch_end].affectMat[1].matBefore = m_air;
	mats[m_tree_branch_end].affectMat[1].matAfter  = m_tree_leaves_end;
	set_chance(mats[m_tree_branch_end].affectMat[1].chance, 150); // instantly changes into end leaves.
	
	mats[m_tree_branch_end].affectMat[2].matBefore = m_air;
	mats[m_tree_branch_end].affectMat[2].matAfter  = m_tree_leaves;
	set_chance(mats[m_tree_branch_end].affectMat[2].chance, 30); // instantly changes into end leaves.
//-------------------------------------------------------------------------------------------------------------------------------
	
//-------------------------------------------------------------------------------------------------------------------------------
	
//-------------------------------------------------------------------------------------------------------------------------------
	
	
	//make it so that any materials that absorb other materials have memory of absorbing them. (satMem = true)
	unsigned short m,s;
	for(m=0 ; m<MAX_NUMBER_OF_UNIQUE_MATERIALS ; m++){
		for(s=0 ; s<MAX_NUMBER_OF_SATURATIONS ; s++){
			if(mats[m].satEffect[s].absorb == true)
				mats[m].satEffect[s].satMem = true;
		}
	}
	
	/// find how many saturatable materials there are and stick them into a nice organized array.
	numberOfSatableMats = 0; // set this to 0 by default. it will get incremented in the for loop and brought to the correct value.
	int i,j,validMatSat;
	for(i=0 ; i<MAX_NUMBER_OF_UNIQUE_MATERIALS ; i++){
		validMatSat = 0;
		// if the saturationMaterial is an invalid choice (like m_no_saturation or m_no_change) skip and move on.
		for(j=0; j<MAX_NUMBER_OF_SATURATIONS; j++){
			if(mats[i].satEffect[j].satMat != m_no_saturation){
				validMatSat = 1;
			}
		}
		if(validMatSat){
			// put the material type into the array at the right point.
			matSatOrder[numberOfSatableMats] = i;
			#if(DEBUG_GRIDSIM)
				printf("matSatOrder[%d] = %d\n", numberOfSatableMats, matSatOrder[numberOfSatableMats]); // print to the output debug file
			#endif
			// increment the number of materials that be saturated that we have.
			numberOfSatableMats++;
		}
	}
	#if(DEBUG_GRIDSIM)
		printf("numberOfSatableMats = %d\n\n\n\n\n", numberOfSatableMats);
	#endif
}



///this function resets all the cells to default state. Nothing
void reset_cells(void){
	int i; int j;
	
	for(i=0 ; i<GRID_WIDTH_ELEMENTS ; i++){
		for(j=0 ; j<GRID_HEIGHT_ELEMENTS ; j++){
			grid[i][j].mat = m_air;
			grid[i][j].sat  = m_no_saturation;
			grid[i][j].satLevel = 0;
			grid[i][j].matChange = m_no_change;
			grid[i][j].satChange = m_no_change;
			grid[i][j].satLevelChange = m_no_change;
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
	printf("printTime = %d\n\nMaterials:\n\n",printTime);
	for(j=0 ; j<GRID_HEIGHT ; j++){
		for(i=0 ; i<GRID_WIDTH ; i++){
			if(grid[i+camera_x][j+camera_y].mat > 0)printf("%2d ",grid[i+camera_x][j+camera_y].mat);
			else printf(" . ");
		}
		printf("\n");
	}
	printf("Saturation:\n\n");
	for(j=0 ; j<GRID_HEIGHT ; j++){
		for(i=0 ; i<GRID_WIDTH ; i++){
			if(grid[i+camera_x][j+camera_y].sat >= 0)printf("%2d ",grid[i+camera_x][j+camera_y].sat);
			else printf(" . ");
		}
		printf("\n");
	}
	printf("\n\nSaturation Level:\n\n");
	for(j=0 ; j<GRID_HEIGHT ; j++){
		for(i=0 ; i<GRID_WIDTH ; i++){
			if(grid[i+camera_x][j+camera_y].satLevel > 0)printf("%2d ",grid[i+camera_x][j+camera_y].satLevel);
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
				grid[i+camera_x][j+camera_y].mat = m_air;
				continue;
			}
			//get random material
			temp = m_valid_but_null_material;
			while(mats[temp].name == NULL){
				temp = get_rand(0, MAX_NUMBER_OF_UNIQUE_MATERIALS-1);
				//don't do scurge. that shit is cancer
				if(temp == m_scurge || temp == m_anti_scurge)
					temp = m_air;
			}
			grid[i+camera_x][j+camera_y].mat = temp;
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

///this will set a chance array (8 elements) symmetrically.
// diagram of the arrangement of chance elements.
// 0 1 2
// 3 m 4
// 5 6 7
//-----arguments--------------------------------------
// chance	pointer to beginning of the chance array
// c1		chance[1]					// top center
// c02		chance[0] and chance[2]		// top sides
// c34		chance[3] and chance[4]		// middle sides
// c57		chance[5] and chance[7]		//bottom sides
// 6		chance[6]					//bottom center
void set_chance_symmetrical(unsigned *chance, unsigned c1, unsigned c02, unsigned c34, unsigned c57, unsigned c6){
	chance[1] = c1;
	chance[0] = chance[2] = c02;
	chance[3] = chance[4] = c34;
	chance[5] = chance[7] = c57;
	chance[6] = c6;
}
