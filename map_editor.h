//map editor

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
}

void zoom_out()
{
    if(CELL_SIZE*0.8 < 1)
		CELL_SIZE -= 1;
	else CELL_SIZE *= 0.8;
    
    if(CELL_SIZE < 1)
		CELL_SIZE = 1;
}
