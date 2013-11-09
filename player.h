
#define DEFAULT_PLAYER_WIDTH	1.0f
#define DEFAULT_PLAYER_HEIGHT	3.0f

// jump velocity in cells/millisecond (positive by SDL coordinate convention)
#define GRAVITY_ACCEL 0.00022f
// the fastest you can ever travel in cells/millsecond (positive by SDL coordinate convention)
#define TERMINAL_VELOCITY 0.33f


struct playerData{
	
	// x and y POSITION		in the grid (using the SDL coordinate system)
	float x_pos; // on the left side of the avater
	float y_pos; // position of feet (lowest point on avatar)
	
	// x and y VELOCITY		in the grid (using the SDL coordinate system)
	float x_vel;
	float y_vel;
	
	// x and y ACCELERATION	in the grid (using the SDL coordinate system)
	float x_accel;
	float y_accel;
	
	// this is the size of the user's avatar
	float height;
	float width;
	
	// the speed in cells/millisecond at which the player ascends upwards when jumping. (negative by SDL coordinate convention)
	float jumpVelocity;
	float walkSpeed;
	
	// this keeps track of whether or not the player is on solid ground.
	bool onTheGround;
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
	datplayer->jumpVelocity = -0.070;
	
	// set default walking speed. this is used for walking left and right. so just enter a positive value. evaluate_player_movement() will do the rest.
	datplayer->walkSpeed = 0.026;
	
	// set default standing on ground state
	datplayer->onTheGround = true;
}


// x is the left-most point of the rectangle
// y is the lowest point of the rectangle
// width extends to the right
// height extends upwards
bool is_mat_in_rect(float x, float y, float width, float height){
	int i,j;
	
	int startj = 0;
	if(x - (int)x != 0) width++;			// if the float x value is not exactly on an integer value, then evaluate one more cell to the right.
	if(y - (int)y != 0){height++;startj=1;}	// if the float y value is not exactly on an integer value, then evaluate one more cell upward.
	
	for(i=0; i<width; i++){
		for(j=startj; j<height; j++){
			if(mats[grid[(int)x+i][(int)y-j].mat].collision) return true;
		}
	}
	return false;
}


/*
#define MIN_COLLISION_STEPS 5
/// this checks to see if there is a collision with a block between starting point (x1,y1) and ending point (x2,y2).
// if there is a collision, the x and y values are set to the last valid cell.
// returns true if the player makes it alright
// returns false if the player enocounters a collision
// the algorithm takes a specified number of steps in calculating if there are any obstructions in the trajectory.

// (x1,y1) is the lower left beginning point of the target.
// width extends the target rectangle to the right
// height extends the target rectangle upwards. (these widths are not the conventional SDL rectangle widths.
bool is_collision(float x1, float y1, float width, float height, float x2, float y2, float *x_collision, float *y_collision){
	
	// this is the number of step we will take along the line from (x1,y1) to (x2,y2)
	// the square root term is used as a scaling factor. the farther the distance is to travel, the more intermediate steps should be taken.
	int steps = MIN_COLLISION_STEPS + sqrtf( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1) );
	
	//calculate the absolute value of the difference in the x's and y's
	float absval_diffx = x2-x1;
	if(absval_diffx < 0) absval_diffx*= -1.0f;
	float absval_diffy = y2-y1;
	if(absval_diffy < 0) absval_diffy*= -1.0f;
	
	int i,j; // these are for indexing
	float x,y; // these are the intermediate values when checking for obstacles on the line between the two points
	
	// if there is a greater change in y than there is in x
	if(absval_diffy >= absval_diffx){				/// index through y
		for(  j=1,y=y1;  j<=steps&&y<=y2;  j++,y=y1+(y2-y1)*(j/(steps))  ){
			x = x1 + (x2-x1)*(j/steps); // calculate x index
			if(is_mat_in_rect(x,y,width,height) == true){
				*x_collision = (x1 + (x2-x1)*((j-1)/steps));		// report last valid x value
				*y_collision = (int)(y1 + (y2-y1)*((j-1)/steps));		// report valid y value
				return true;										// collision detected
			}
		}
	}
	// if there is a greater change in x than there is in y
	else{											/// index through x
		for(  i=1,x=x1;  i<=steps;  i++,x=x1+(x2-x1)*(i/(steps))  ){
			y = y1 + (y2-y1)*(i/steps); // calculate x index
			if(is_mat_in_rect(x,y,width,height) == true){
				*x_collision = (int)(x1 + (x2-x1)*((i-1)/steps));// report collision x value
				if(x2 > x1) *x_collision += 0.9999;
				*y_collision = (y1 + (y2-y1)*((i-1)/steps));	// report collision y value
				return true;									// collision detected
			}
		}
	}
	
	
	return false; // no collision
}
*/


/// move the player from point a to point b
void move_player(struct playerData *datplayer, int millis){
	// generate current and next point coordinates.
	float x1 = datplayer->x_pos;
	float y1 = datplayer->y_pos;
	float x2 = x1 + datplayer->x_vel*millis;
	float y2 = y1 + datplayer->y_vel*millis;
	// these are for getting where there is a collision
	float x,y;
	
	// if there is a collision, don't move
	
	if( is_collision(x1,y1,datplayer->width,datplayer->height, x2,y2, &x,&y) ){ 
		datplayer->x_pos = x;
		datplayer->y_pos = y;
		datplayer->onTheGround = true;
		datplayer->y_accel = 0;
		datplayer->y_vel = 0;
	}
	
	// otherwise, if there is no collision, move!
	else{
		datplayer->x_pos = x2;
		datplayer->y_pos = y2;
	}
}



/// evaluate the physics of the player's motion.
void evaluate_player_movement(struct playerData *datplayer, int keyup, int keyleft, int keydown, int keyright){
	//----------------------------------------------------
	// time stuff
	//----------------------------------------------------
	static int previousTicks = 0;				// the previous cycle's ticks (milliseconds since SDL library initialized) default to 0
	int currentTicks = SDL_GetTicks();			// the current cycle's ticks (milliseconds since SDL library initialized)
	int millis = currentTicks - previousTicks;	// the difference in ticks (the time that has passed since the last cycle in milliseconds)
	if(millis <= 0) return; 					// don't check player movement when time stops or runs backwards.
	previousTicks = currentTicks;				// store current ticks in the previous ticks variable. it will be used next time.
	//----------------------------------------------------
	// checking key states
	//----------------------------------------------------
	// check if the player is on the ground.
	if(mats[grid[(int)datplayer->x_pos][(int)datplayer->y_pos+1].mat].collision == false && mats[grid[(int)datplayer->x_pos+1][(int)datplayer->y_pos+1].mat].collision == false){
		datplayer->onTheGround = false;
		datplayer->y_accel = GRAVITY_ACCEL;
	}
	// jump
	if(keyup && datplayer->onTheGround){
		datplayer->y_vel = datplayer->jumpVelocity;
		datplayer->y_accel = GRAVITY_ACCEL;
		datplayer->onTheGround = false; // the player is no longer on the ground
	}
	// if the left and right movement keys are either both being pressed or both off, set acceleration to 0.
	if(!(keyright^keyleft)){
		datplayer->x_vel = 0;
	}
	// if the right-movement key is pressed down
	else if(keyright){
		datplayer->x_vel = datplayer->walkSpeed;	// move right
	}
	// if the left-movement key is pressed down
	else if(keyleft){
		datplayer->x_vel = -datplayer->walkSpeed;	// move left
	}
	
	//move the player according to the player's velocity.
	move_player(datplayer, millis);
	
	// add accelerations*time to the velocities
	datplayer->y_vel += datplayer->y_accel*millis;
	datplayer->y_vel += datplayer->y_accel*millis;
	
	// limit the velocity of the player to the terminal velocity
	if(datplayer->y_vel > TERMINAL_VELOCITY) datplayer->y_vel = TERMINAL_VELOCITY;
	if(datplayer->y_vel < -TERMINAL_VELOCITY) datplayer->y_vel = -TERMINAL_VELOCITY;
}
