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
		grid[x/CELL_SIZE][y/CELL_SIZE].mat = data;
		grid[x/CELL_SIZE][y/CELL_SIZE].sat = m_no_saturation;
		grid[x/CELL_SIZE][y/CELL_SIZE].satLevel = 0;
	}
}

void deletecell(int x, int y, int data)
{
	//make sure the user's cursor is inside the grid. (no negative values or values bigger than the grid size
	if(x>=0 && x/CELL_SIZE<GRID_WIDTH && y>=0 && y/CELL_SIZE<GRID_HEIGHT && x<=SCREEN_WIDTH - 200 && y<= SCREEN_WIDTH){
		grid[x/CELL_SIZE][y/CELL_SIZE].mat = 0;
		grid[x/CELL_SIZE][y/CELL_SIZE].sat = m_no_saturation;
		grid[x/CELL_SIZE][y/CELL_SIZE].satLevel = 0;
	}
}

void zoom_in()
{
    if(CELL_SIZE*1.25 < 1+CELL_SIZE)
		CELL_SIZE += 1;
	else CELL_SIZE *= 1.25;
	
    if(CELL_SIZE > 62)
		CELL_SIZE = 62;
	verify_grid_and_cell_size();
}

void zoom_out()
{
    if(CELL_SIZE*0.8 < 1)
		CELL_SIZE -= 1;
	else CELL_SIZE *= 0.8;
    
    if(CELL_SIZE < 1)
		CELL_SIZE = 1;
	verify_grid_and_cell_size();
}



