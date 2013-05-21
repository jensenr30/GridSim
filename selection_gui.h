//all the code for displaying the gui that allows you to selection your material

//material icon structure and delcarations
SDL_Rect matIcon[MAX_NUMBER_OF_UNIQUE_MATERIALS];

//main button controls
short firstColumn = 190;
short firstRow = 80;
short widthButton = 20;
short heightButton = 20;
double selectionBoxSizeMultiplier = 1.5;
double rowSpacingMultiplier = 2;
double columnSpacingMultiplier = 2;

//selection box positions
//compiler is GAY so you have to keep these as number values
short xSel = SCREEN_WIDTH - 195, ySel = 75, wSel = 30, hSel = 30;

//main gui varibles
short xPos = SCREEN_WIDTH - 200, yPos = 0, wPos = 200, hPos = SCREEN_HEIGHT;

//displays gui
void selectionGUI(int x, int y, int mouse)
{
    //variables to step through array
    int i;
    
    //materals start at 1 and go up from there
    //icon positions
    for(i = 1; i < MAX_NUMBER_OF_UNIQUE_MATERIALS; i++){
        if(i <= 5){
            matIcon[i].x = SCREEN_WIDTH - firstColumn + widthButton * (i - 1) * rowSpacingMultiplier;  
            matIcon[i].y = firstRow;
        }
        else if(i <= 10){
            matIcon[i].x = SCREEN_WIDTH - firstColumn + widthButton * (i - 6) * rowSpacingMultiplier;
            matIcon[i].y = firstRow + heightButton * columnSpacingMultiplier;
        }
        else if(i <= 15){
            matIcon[i].x = SCREEN_WIDTH - firstColumn + widthButton * (i - 11) * rowSpacingMultiplier;
            matIcon[i].y = firstRow + heightButton * 2 * columnSpacingMultiplier;
        }
        else if(i <= 20){
            matIcon[i].x = SCREEN_WIDTH - firstColumn + widthButton * (i - 16) * rowSpacingMultiplier;
            matIcon[i].y = firstRow + heightButton * 3 * columnSpacingMultiplier;
        }
    }
    
    //define rectangles
    SDL_Rect guiRectangle; 
    SDL_Rect selectionBox;
     
    //prints names of material
    text = TTF_RenderText_Blended( font, mats[currentMat].name , textColor );

    //main window
	guiRectangle.x = xPos;
	guiRectangle.y = yPos;
	guiRectangle.w = wPos; 
	guiRectangle.h = hPos;
    SDL_FillRect( screen , &guiRectangle , 0x181818);
    
    //box under text color
    guiRectangle.x = SCREEN_WIDTH - 200;
	guiRectangle.y = 0;
	guiRectangle.w = 200;
	guiRectangle.h = 50;
    SDL_FillRect( screen , &guiRectangle , mats[currentMat].color);
    
    //apply text to screen
    apply_surface( SCREEN_WIDTH - 170, 0, text, screen );
    SDL_FreeSurface( text );

    //selection box
    selectionBox.x = xSel;
	selectionBox.y = ySel;
	selectionBox.w = wSel;
	selectionBox.h = hSel;
    SDL_FillRect( screen , &selectionBox , 0xffffff);
    
    //prints a rectangle for each material icon
    for( i = M_earth; i < MAX_NUMBER_OF_UNIQUE_MATERIALS; i++ ){
        guiRectangle.x = matIcon[i].x;
        guiRectangle.y = matIcon[i].y;
        guiRectangle.w = widthButton;
        guiRectangle.h = heightButton;
        SDL_FillRect( screen , &guiRectangle , mats[i].color);
    }
    
    //checks for mouse clicks over material icons
    for ( i = M_earth; i < MAX_NUMBER_OF_UNIQUE_MATERIALS; i++ ){

        if( ( x > matIcon[i].x ) && ( x < matIcon[i].x + widthButton ) && ( y > matIcon[i].y ) && ( y < matIcon[i].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = i;
                xSel = matIcon[i].x - 5, ySel = matIcon[i].y - 5, wSel = (widthButton * selectionBoxSizeMultiplier), hSel = (heightButton * selectionBoxSizeMultiplier);
            }
        }
    }
}
