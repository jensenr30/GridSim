//map editor

/// this makes sure that the size of the grid (in cells) is no larger than it's maximum (MAX_GRID_WIDTH and MAX_GRID_HEIGHT)
void verify_grid_and_cell_size(){
	//limit the width of grid in terms of the number horizontal cells
	if(SCREEN_WIDTH/CELL_SIZE > MAX_GRID_WIDTH)
		CELL_SIZE = SCREEN_WIDTH/MAX_GRID_WIDTH + (1||(SCREEN_WIDTH%MAX_GRID_WIDTH));
	//limit the height of grid in terms of the number verticle cells
	if(SCREEN_HEIGHT/CELL_SIZE > MAX_GRID_HEIGHT)
		CELL_SIZE = SCREEN_HEIGHT/MAX_GRID_HEIGHT + (1||(SCREEN_HEIGHT%MAX_GRID_HEIGHT));
}

void setcell(int x, int y, int data)
{
	//make sure the user's cursor is inside the grid. (no negative values or values bigger than the grid size
	if(x>=0 && x/CELL_SIZE<GRID_WIDTH && y>=0 && y/CELL_SIZE<GRID_HEIGHT && y<= SCREEN_WIDTH){
		grid[x/CELL_SIZE + camera_x][y/CELL_SIZE + camera_y].mat = data;
		grid[x/CELL_SIZE + camera_x][y/CELL_SIZE + camera_y].sat = m_no_saturation;
		grid[x/CELL_SIZE + camera_x][y/CELL_SIZE + camera_y].satLevel = 0;
	}
}

void deletecell(int x, int y, int data)
{
	//make sure the user's cursor is inside the grid. (no negative values or values bigger than the grid size
	if(x>=0 && x/CELL_SIZE<GRID_WIDTH && y>=0 && y/CELL_SIZE<GRID_HEIGHT && x<=SCREEN_WIDTH - 200 && y<= SCREEN_WIDTH){
		grid[x/CELL_SIZE + camera_x][y/CELL_SIZE + camera_y].mat = 0;
		grid[x/CELL_SIZE + camera_x][y/CELL_SIZE + camera_y].sat = m_no_saturation;
		grid[x/CELL_SIZE + camera_x][y/CELL_SIZE + camera_y].satLevel = 0;
	}
}

#define MAX_CELL_SIZE 61
#define MIN_CELL_SIZE 1

void zoom_in()
{
	// modify cell size
    if(CELL_SIZE*1.25 < 1+CELL_SIZE)
		CELL_SIZE += 1;
	else CELL_SIZE *= 1.25;
	
	// make sure the cell_size is valid
    if(CELL_SIZE > MAX_CELL_SIZE)
		CELL_SIZE = MAX_CELL_SIZE;
	verify_grid_and_cell_size();
	verify_camera();
}

void zoom_out()
{
	// modify cell size
    if(CELL_SIZE*0.8 < 1)
		CELL_SIZE -= 1;
	else CELL_SIZE *= 0.8;
    
    // make sure the cell_size is valid
    if(CELL_SIZE < MIN_CELL_SIZE)
		CELL_SIZE = MIN_CELL_SIZE;
		
	verify_grid_and_cell_size();
	verify_camera();
}

#define D_UP	0	// W
#define D_LEFT	1	// A
#define D_DOWN	2	// S
#define D_RIGHT	3	// D

// the "- CELL_SIZE" part of the if statement is to make sure that we are on the same size of the error in the integer division in the GRID_WIDTH term.
#define MAX_camera_x (MAX_GRID_WIDTH*3 - GRID_WIDTH - CELL_SIZE)
#define MAX_camera_y (MAX_GRID_HEIGHT*3 - GRID_HEIGHT - CELL_SIZE)

void verify_camera(){
	// make sure the camera is in a valid place
	if(camera_x < 0) camera_x = 0;
	if(camera_y < 0) camera_y = 0;
	
	//make sure the camera values are not too large. they mustn't overstep their bounds.
	if(camera_x > MAX_camera_x)
		camera_x = MAX_camera_x;
	if(camera_y > MAX_camera_y)
		camera_y = MAX_camera_y;
}

void pan(int direction){
	switch(direction){
	case D_UP:
		camera_y -= MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	case D_LEFT:
		camera_x -= MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	case D_DOWN:
		camera_y += MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	case D_RIGHT:
		camera_x += MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	}
	verify_camera();
}



