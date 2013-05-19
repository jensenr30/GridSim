//all the code for displaying the gui that allows you to selection your material



void selectionGUI()
{
    //define rectangle
    SDL_Rect guiRectangle;

    //main window
	guiRectangle.x = SCREEN_WIDTH - 200;
	guiRectangle.y = 0;
	guiRectangle.w = 200;
	guiRectangle.h = SCREEN_HEIGHT;
    SDL_FillRect( screen , &guiRectangle , 0x0000ff);

    //selection box

    //water
    guiRectangle.x = SCREEN_WIDTH - 150;
	guiRectangle.y = 100;
	guiRectangle.w = 50;
	guiRectangle.h = 50;
    SDL_FillRect( screen , &guiRectangle , mats[M_water].color);

    //earth
    guiRectangle.x = SCREEN_WIDTH - 150;
	guiRectangle.y = 200;
	guiRectangle.w = 50;
	guiRectangle.h = 50;
    SDL_FillRect( screen , &guiRectangle , mats[M_earth].color);

    //fire
    guiRectangle.x = SCREEN_WIDTH - 150;
	guiRectangle.y = 300;
	guiRectangle.w = 50;
	guiRectangle.h = 50;
    SDL_FillRect( screen , &guiRectangle , mats[M_fire].color);

    //air
    //then the fire nation attacked!!!
    guiRectangle.x = SCREEN_WIDTH - 150;
	guiRectangle.y = 400;
	guiRectangle.w = 50;
	guiRectangle.h = 50;
    SDL_FillRect( screen , &guiRectangle , 0x00ffff);

}
