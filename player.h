
#define DEFAULT_PLAYER_WIDTH	1.0f
#define DEFAULT_PLAYER_HEIGHT	3.0f

struct playerData{
	
	// x and y POSITION		in the grid (using the SDL coordinate system)
	float x_pos;
	float y_pos;
	
	// x and y VELOCITY		in the grid (using the SDL coordinate system)
	float x_vel;
	float y_vel;
	
	// x and y ACCELERATION	in the grid (using the SDL coordinate system)
	float x_accel;
	float y_accel;
	
	// this is the size of the user's avatar
	float height;
	float width;
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
	
}

