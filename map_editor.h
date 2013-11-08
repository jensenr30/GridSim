//map editor

#define MAX_CELL_SIZE 61
#define MIN_CELL_SIZE 1
#define CELL_STEP_UP 1.25f
#define CELL_SIZE_DOWN 0.80f


#define D_UP	0	// W
#define D_LEFT	1	// A
#define D_DOWN	2	// S
#define D_RIGHT	3	// D

// the "- CELL_SIZE" part of the if statement is to make sure that we are on the same size of the error in the integer division in the GRID_WIDTH term.
#define MAX_camera_x (int)(GRID_WIDTH_ELEMENTS -  GRID_WIDTH  - CELL_SIZE)
#define MAX_camera_y (int)(GRID_HEIGHT_ELEMENTS - GRID_HEIGHT - CELL_SIZE)


/// this makes sure that the size of the grid (in cells) is no larger than it's maximum (MAX_GRID_WIDTH and MAX_GRID_HEIGHT)
void verify_grid_and_cell_size(){
	//make sure the CELL_SIZE is not too big.
	if(CELL_SIZE > MAX_CELL_SIZE)
		CELL_SIZE = MAX_CELL_SIZE;
	// make sure the CELL_SIZE is not smaller than a pixel
    if(CELL_SIZE < MIN_CELL_SIZE)
		CELL_SIZE = MIN_CELL_SIZE;
	
	//limit the width of grid in terms of the number horizontal cells
	if(SCREEN_WIDTH/CELL_SIZE > MAX_GRID_WIDTH)
		CELL_SIZE = SCREEN_WIDTH/MAX_GRID_WIDTH + (1&&(SCREEN_WIDTH%MAX_GRID_WIDTH));
	//limit the height of grid in terms of the number verticle cells
	if(SCREEN_HEIGHT/CELL_SIZE > MAX_GRID_HEIGHT)
		CELL_SIZE = SCREEN_HEIGHT/MAX_GRID_HEIGHT + (1&&(SCREEN_HEIGHT%MAX_GRID_HEIGHT));
}

void verify_camera(){
	//make sure the camera values are not too large. they mustn't overstep their bounds.
	if(camera_x > MAX_camera_x){
		camera_x = MAX_camera_x;
	}
	if(camera_y > MAX_camera_y){
		camera_y = MAX_camera_y;
	}
	// make sure the camera is NOT NEGATIVE. there are no negative elements in an array  :P
	if(camera_x < 0) camera_x = 0;
	if(camera_y < 0) camera_y = 0;
}

void setcell(int mouse_x, int mouse_y, int data)
{
	//calculate the grid position of the user's mouse click.
	int cell_x = mouse_x/CELL_SIZE + camera_x;
	int cell_y = mouse_y/CELL_SIZE + camera_y;
	//make sure the user's cursor is inside the grid. (no negative values or values bigger than the grid size
	if(cell_x >= 0 && cell_x < GRID_WIDTH_ELEMENTS && cell_y >= 0 && cell_y < GRID_HEIGHT_ELEMENTS){
		grid[cell_x][cell_y].mat = data;
	}
}

void deletecell(int mouse_x, int mouse_y, int data)
{
	//calculate the grid position of the user's mouse click.
	int cell_x = mouse_x/CELL_SIZE + camera_x;
	int cell_y = mouse_y/CELL_SIZE + camera_y;
	//make sure the user's cursor is inside the grid. (no negative values or values bigger than the grid size
	if(cell_x >= 0 && cell_x < GRID_WIDTH_ELEMENTS && cell_y >= 0 && cell_y < GRID_HEIGHT_ELEMENTS){
		grid[cell_x][cell_y].mat = 0;
	}
}



void zoom_in(int mousex, int mousey)
{
	//store the old cell size
	short old_cell_size = CELL_SIZE;
	
	// modify cell size
    if(CELL_SIZE*CELL_STEP_UP < 1+CELL_SIZE)
		CELL_SIZE += 1;
	else CELL_SIZE *= CELL_STEP_UP;
	
	verify_grid_and_cell_size();
	
	//adjust camera position
	camera_x += (((SCREEN_WIDTH/old_cell_size) - (SCREEN_WIDTH/CELL_SIZE))*mousex)/SCREEN_WIDTH;
	camera_y += (((SCREEN_HEIGHT/old_cell_size) - (SCREEN_HEIGHT/CELL_SIZE))*mousey)/SCREEN_HEIGHT;
	
	verify_camera();
}

void zoom_out(int mousex, int mousey)
{
	//store the old cell size
	short old_cell_size = CELL_SIZE;
	
	// modify cell size
    if(CELL_SIZE*CELL_SIZE_DOWN < 1)
		CELL_SIZE -= 1;
	else CELL_SIZE *= CELL_SIZE_DOWN;
		
	verify_grid_and_cell_size();
	
	//adjust camera position
	camera_x -= (  ((SCREEN_WIDTH/CELL_SIZE)  - (SCREEN_WIDTH/old_cell_size))   * mousex) / SCREEN_WIDTH;
	camera_y -= (  ((SCREEN_HEIGHT/CELL_SIZE) - (SCREEN_HEIGHT/old_cell_size))  * mousey) / SCREEN_HEIGHT;
    
	verify_camera();
}



