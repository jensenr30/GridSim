//map editor

void setcell(int x, int y, int data)
{
    cellData[x/10][y/10] = data;
    print_cells();
}

void deletecell(int x, int y, int data)
{
    cellData[x/10][y/10] = 0;
    print_cells();
}
