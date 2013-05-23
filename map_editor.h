//map editor

void setcell(int x, int y, int data)
{
	//make sure the user's cursor is inside the grid. (no negative values or values bigger than the grid size
	if(x>=0 && x/CELL_SIZE<GRID_WIDTH && y>=0 && y/CELL_SIZE<GRID_HEIGHT && x<=SCREEN_WIDTH - 200 && y<= SCREEN_WIDTH){
		cellMat[x/CELL_SIZE][y/CELL_SIZE] = data;
		cellSat[x/CELL_SIZE][y/CELL_SIZE] = M_no_saturation;
	}
}

void deletecell(int x, int y, int data)
{
	//make sure the user's cursor is inside the grid. (no negative values or values bigger than the grid size
	if(x>=0 && x/CELL_SIZE<GRID_WIDTH && y>=0 && y/CELL_SIZE<GRID_HEIGHT && x<=SCREEN_WIDTH - 200 && y<= SCREEN_WIDTH){
		cellMat[x/CELL_SIZE][y/CELL_SIZE] = 0;
		cellSat[x/CELL_SIZE][y/CELL_SIZE] = M_no_saturation;
	}
}

void zoomplus()
{
    CELL_SIZE *= 2;
    if(CELL_SIZE > 64)
		CELL_SIZE = 64;
}

void zoomminus()
{
    CELL_SIZE /= 2;
    if(CELL_SIZE < 1)
		CELL_SIZE = 1;
}
