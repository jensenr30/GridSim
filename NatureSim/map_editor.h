//map editor

void setcell(int x, int y, int data)
{
    cellData[x/CELL_SIZE][y/CELL_SIZE] = data;
}

void deletecell(int x, int y, int data)
{
    cellData[x/CELL_SIZE][y/CELL_SIZE] = 0;
}

void zoomplus()
{
    CELL_SIZE *= 2;
    CELL_SIZE *= 2;
    SCREEN_WIDTH *= 2;
    SCREEN_HEIGHT *= 2;
}

void zoomminus()
{
    CELL_SIZE /= 2;
    CELL_SIZE /= 2;
    SCREEN_WIDTH /= 2;
    SCREEN_HEIGHT /= 2;
}
