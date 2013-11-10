
#define DEFAULT_PLAYER_WIDTH	1.0f
#define DEFAULT_PLAYER_HEIGHT	3.0f

// jump velocity in cells/millisecond (positive by SDL coordinate convention)
#define GRAVITY_ACCEL 0.00018f
// the fastest you can ever travel in cells/millsecond (positive by SDL coordinate convention)
#define TERMINAL_VELOCITY 0.33f


struct playerData{
	
	// x and y POSITION		in the grid (using the SDL coordinate system)
	int x_pos; // on the left side of the avater
	int y_pos; // position of feet (lowest point on avatar)
	
	// x and y VELOCITY		in the grid (using the SDL coordinate system)
	float x_vel;
	float y_vel;
	
	// x and y ACCELERATION	in the grid (using the SDL coordinate system)
	float x_accel;
	float y_accel;
	
	// this is the size of the user's avatar
	float height;
	float width;
	
	//the base color of the player
	int color;
	
	// the speed in cells/millisecond at which the player ascends upwards when jumping. (negative by SDL coordinate convention)
	float jumpVelocity;
	float walkSpeed;
	
	// this keeps track of whether or not the player is on a collision material
	bool onCollMat;
};

// this is the player.
struct playerData player;


// this sets up a blank character
void init_player_attributes(struct playerData *datplayer){
	
	// set all position, velocity, and acceleration values to 0's.
	datplayer->x_pos = datplayer->x_vel = datplayer->x_accel = datplayer->y_pos = datplayer->y_vel = datplayer->y_accel = 0;
	
	// set default player dimensions.
	datplayer->width  = DEFAULT_PLAYER_WIDTH;
	datplayer->height = DEFAULT_PLAYER_HEIGHT;
	
	//set default jump velocity. this is negative (following the SDL coordinate convention)
	datplayer->jumpVelocity = -0.110;
	
	// set default walking speed. this is used for walking left and right. so just enter a positive value. evaluate_player_movement() will do the rest.
	datplayer->walkSpeed = 0.045;
	
	// set default state of standing on a collision materia
	datplayer->onCollMat = false;
	
	datplayer->color = 0x987a5c;
}

/// tests to see if the position is valid for the player to be
bool player_valid_position(struct playerData *datplayer, int x, int y){
	int i,j;
	// index through the places that the player occupies
	for(i=0; i<datplayer->width; i++){
		for(j=0; j<datplayer->height; j++){
			if(mats[grid[x+i][y-j-1].mat].collision) return false; // if you encounter any material inside the player's area, return false. the player is not in a valid position
		}
	}
	return true; // if there are no collision type materials found inside the player, then return true!  :D
}


#define MILLISECONDS_PER_CELL 20
/// evaluate the physics of the player's motion.
void evaluate_player_movement(struct playerData *datplayer, int keyup, int keyleft, int keydown, int keyright){
	//----------------------------------------------------
	// time stuff
	//----------------------------------------------------
	static ticksSinceMotion = 0;
	static int previousTicks = 0;				// the previous cycle's ticks (milliseconds since SDL library initialized) default to 0
	int currentTicks = SDL_GetTicks();			// the current cycle's ticks (milliseconds since SDL library initialized)
	int millis = currentTicks - previousTicks;	// the difference in ticks (the time that has passed since the last cycle in milliseconds)
	if(millis <= 0) return; 					// don't check player movement when time stops or runs backwards.
	previousTicks = currentTicks;				// store current ticks in the previous ticks variable. it will be used next time.
	ticksSinceMotion += millis;					// add to the cumulative number of milliseconds that have passed since last movement
	//----------------------------------------------------
	// standing on collision material checking
	//----------------------------------------------------
	// check to see if the player is standing on a collision type material
	if(mats[grid[datplayer->x_pos][datplayer->y_pos].mat].collision){
		datplayer->onCollMat = true;			// set the player's "standing on collision material" flag to true
		datplayer->y_accel = 0;					// set accelteration to 0. (as of 2013-11-10 this doesn't do anything)
	}
	else{
		datplayer->onCollMat = false;			// set the player's "standing on collision material" flag to false
		datplayer->y_accel = GRAVITY_ACCEL;		// set the player's y acceleration to acceleration due to gravity
	}
	
	///---------------------------------------------------
	/// this section of the code is only temporary.
	/// The movement of the character is still under development.
	/// this was put here so that I don't have to focus on development of the player's motion right now.
	///---------------------------------------------------
	// these are used to see where the user is trying to get to.
	int moveHoriz=0, moveVert=0;
	//check to see if it is time to move
	while(ticksSinceMotion >= MILLISECONDS_PER_CELL){
		// decrement ticksSinceMotion because we are about to move.
		ticksSinceMotion -= MILLISECONDS_PER_CELL;
		if(keyright){
			moveHoriz += 1;	// move right
		}
		if(keyleft){
			moveHoriz -= 1;	// move left
		}
		if(keyup){
			moveVert  -= 1;	// move up
		}
		if(keydown){
			moveVert  += 1;	// move down
		}
		//try to move (only if the destination is valid)
		if( player_valid_position(datplayer, datplayer->x_pos+moveHoriz, datplayer->y_pos+moveVert) ){
			datplayer->x_pos += moveHoriz;	// apply motion
			datplayer->y_pos += moveVert;	// apply motion
		}
	}
	//----------------------------------------------------
	// position boundaries checking and enforcement
	//----------------------------------------------------
	//check to see if the outside the bounds of the game screen.
	if(datplayer->x_pos < 0)
		datplayer->x_pos = 0;						// lower bound enforcement on x
	if(datplayer->x_pos >= GRID_WIDTH_ELEMENTS)
		datplayer->x_pos = GRID_WIDTH_ELEMENTS-1;	// upper bound enforcement on x
	if(datplayer->y_pos < 0)
		datplayer->y_pos = 0;						// lower bound enforcement on y
	if(datplayer->y_pos >= GRID_HEIGHT_ELEMENTS)
		datplayer->y_pos = GRID_HEIGHT_ELEMENTS-1;	// upper bound enforcement on y
}
