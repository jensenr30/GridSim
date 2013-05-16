/// this document will contain the necessary functions to evaluate and advance the simulation.
/// It will look at the materials in the cells and then update the cells based on the dynamics of the materials described in materials_and_cells.h


// this will evaluate the grid. It will run a simulation for the number of generations you tell it to.
void evaluate_cells(){






}



// this will print to the screen each material in each cell.
void print_cells(){

    cellData[0][0] = M_water;
    cellData[1][0] = M_fire;

    int a, b;
	SDL_Rect myRectangle;
	myRectangle.x = 0;
	myRectangle.y = 0;
	myRectangle.w = 10;
	myRectangle.h = 10;

    for(a = 0; a < GRID_HEIGHT; a += 1){
        for(b = 0; b < GRID_WIDTH; b += 1){
            SDL_FillRect( screen , &myRectangle , mats[cellData[b][a]].color);
            myRectangle.x += 10;
        }
        myRectangle.x = 0;
        myRectangle.y += 10;
    }
}
