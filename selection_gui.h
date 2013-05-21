//all the code for displaying the gui that allows you to selection your material

//material icon structure and delcarations
SDL_Rect matIcon[MAX_NUMBER_OF_UNIQUE_MATERIALS];

//main button controls
short firstColumn = 190;
short firstRow = 80;
short widthButton = 20;
short heightButton = 20;
double multiplier = 1.5;
double rowSpacingMultiplier = 2;
double columnSpacingMultiplier = 2;

//selection box positions
//compiler is GAY so you have to keep these as number values
int xSel = SCREEN_WIDTH - 195, ySel = 75, wSel = 30, hSel = 30;

//main gui varibles
short xPos = SCREEN_WIDTH - 200, yPos = 0, wPos = 200, hPos = SCREEN_HEIGHT;

//displays gui
void selectionGUI(int x, int y, int mouse)
{
    //materals start at 1 and go up from there
    //icon positions
    //first row
    matIcon[M_earth].x = SCREEN_WIDTH - firstColumn;
    matIcon[M_earth].y = firstRow;
    matIcon[M_grass].x = SCREEN_WIDTH - firstColumn + widthButton * rowSpacingMultiplier;
    matIcon[M_grass].y = firstRow;
    matIcon[M_water].x = SCREEN_WIDTH - firstColumn + (widthButton * 2) * rowSpacingMultiplier;
    matIcon[M_water].y = firstRow;
    matIcon[M_fire].x = SCREEN_WIDTH - firstColumn + (widthButton * 3) * rowSpacingMultiplier;
    matIcon[M_fire].y = firstRow;
    matIcon[M_gunpowder].x = SCREEN_WIDTH - firstColumn + (widthButton * 4) * rowSpacingMultiplier;
    matIcon[M_gunpowder].y = firstRow;
    //second row
    matIcon[M_cement].x = SCREEN_WIDTH - firstColumn;
    matIcon[M_cement].y = firstRow + heightButton * columnSpacingMultiplier;
    matIcon[M_concrete].x = SCREEN_WIDTH - firstColumn + widthButton * rowSpacingMultiplier;
    matIcon[M_concrete].y = firstRow + heightButton * columnSpacingMultiplier;
    matIcon[M_tree].x = SCREEN_WIDTH - firstColumn + (widthButton * 2) * rowSpacingMultiplier;
    matIcon[M_tree].y = firstRow + heightButton * columnSpacingMultiplier;
    matIcon[M_animal].x = SCREEN_WIDTH - firstColumn + (widthButton * 3) * rowSpacingMultiplier;
    matIcon[M_animal].y = firstRow + heightButton * columnSpacingMultiplier;
    matIcon[M_human].x = SCREEN_WIDTH - firstColumn + (widthButton * 4) * rowSpacingMultiplier;
    matIcon[M_human].y = firstRow + heightButton * columnSpacingMultiplier;
    //third row
    matIcon[M_rock].x = SCREEN_WIDTH - firstColumn;
    matIcon[M_rock].y = firstRow + (heightButton * 2) * columnSpacingMultiplier;
    matIcon[M_spring].x = SCREEN_WIDTH - firstColumn + widthButton * rowSpacingMultiplier;;
    matIcon[M_spring].y = firstRow + (heightButton * 2) * columnSpacingMultiplier;
    
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
    
    //material start
    //earth
    guiRectangle.x = matIcon[M_earth].x;
	guiRectangle.y = matIcon[M_earth].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_earth].color);
    
    //grass
    guiRectangle.x = matIcon[M_grass].x;
	guiRectangle.y = matIcon[M_grass].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_grass].color);
    
    //water
    guiRectangle.x = matIcon[M_water].x;
	guiRectangle.y = matIcon[M_water].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_water].color);

    //fire
    guiRectangle.x = matIcon[M_fire].x;
	guiRectangle.y = matIcon[M_fire].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_fire].color);

    //gunpowder
    guiRectangle.x = matIcon[M_gunpowder].x;
	guiRectangle.y = matIcon[M_gunpowder].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_gunpowder].color);
    
    //gunpowder
    guiRectangle.x = matIcon[M_cement].x;
	guiRectangle.y = matIcon[M_cement].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_cement].color);
    
    //concrete
    guiRectangle.x = matIcon[M_concrete].x;
	guiRectangle.y = matIcon[M_concrete].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_concrete].color);
    
    //tree
    guiRectangle.x = matIcon[M_tree].x;
	guiRectangle.y = matIcon[M_tree].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_tree].color);
    
    //animel
    guiRectangle.x = matIcon[M_animal].x;
	guiRectangle.y = matIcon[M_animal].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_animal].color);
    
    //human
    guiRectangle.x = matIcon[M_human].x;
	guiRectangle.y = matIcon[M_human].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_human].color);
    
    //rock
    guiRectangle.x = matIcon[M_rock].x;
	guiRectangle.y = matIcon[M_rock].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_rock].color);
    
    //spring
    guiRectangle.x = matIcon[M_spring].x;
	guiRectangle.y = matIcon[M_spring].y;
	guiRectangle.w = widthButton;
	guiRectangle.h = heightButton;
    SDL_FillRect( screen , &guiRectangle , mats[M_spring].color);
    
    //checking start
    //earth box
    if( ( x > matIcon[M_earth].x ) && ( x < matIcon[M_earth].x + widthButton ) && ( y > matIcon[M_earth].y ) && ( y < matIcon[M_earth].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_earth;
                xSel = matIcon[M_earth].x - 5, ySel = matIcon[M_earth].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
    
    //grass box
    else if( ( x > matIcon[M_grass].x ) && ( x < matIcon[M_grass].x + widthButton ) && ( y > matIcon[M_grass].y ) && ( y < matIcon[M_grass].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_grass;
                xSel = matIcon[M_grass].x - 5, ySel = matIcon[M_grass].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
        
    //water box
    else if( ( x > matIcon[M_water].x ) && ( x < matIcon[M_water].x + widthButton ) && ( y > matIcon[M_water].y ) && ( y < matIcon[M_water].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_water;
                xSel = matIcon[M_water].x - 5, ySel = matIcon[M_water].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
        
    //fire
    else if( ( x > matIcon[M_fire].x ) && ( x < matIcon[M_fire].x + widthButton ) && ( y > matIcon[M_fire].y ) && ( y < matIcon[M_fire].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_fire;
                xSel = matIcon[M_fire].x - 5, ySel = matIcon[M_fire].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
        
    //gunpowder	
    else if( ( x > matIcon[M_gunpowder].x ) && ( x < matIcon[M_gunpowder].x + widthButton ) && ( y > matIcon[M_gunpowder].y ) && ( y < matIcon[M_gunpowder].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_gunpowder;
                xSel = matIcon[M_gunpowder].x - 5, ySel = matIcon[M_gunpowder].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
        
    //cement
    else if( ( x > matIcon[M_cement].x ) && ( x < matIcon[M_cement].x + widthButton ) && ( y > matIcon[M_cement].y ) && ( y < matIcon[M_cement].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_cement;
                xSel = matIcon[M_cement].x - 5, ySel = matIcon[M_cement].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
        
    //concrete
    else if( ( x > matIcon[M_concrete].x ) && ( x < matIcon[M_concrete].x + widthButton ) && ( y > matIcon[M_concrete].y ) && ( y < matIcon[M_concrete].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_concrete;
                xSel = matIcon[M_concrete].x - 5, ySel = matIcon[M_concrete].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
    
    //tree
    else if( ( x > matIcon[M_tree].x ) && ( x < matIcon[M_tree].x + widthButton ) && ( y > matIcon[M_tree].y ) && ( y < matIcon[M_tree].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_tree;
                xSel = matIcon[M_tree].x - 5, ySel = matIcon[M_tree].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
        
    //animal
    else if( ( x > matIcon[M_animal].x ) && ( x < matIcon[M_animal].x + widthButton ) && ( y > matIcon[M_animal].y ) && ( y < matIcon[M_animal].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_animal;
                xSel = matIcon[M_animal].x - 5, ySel = matIcon[M_animal].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
    
    //human
    if( ( x > matIcon[M_human].x ) && ( x < matIcon[M_human].x + widthButton ) && ( y > matIcon[M_human].y ) && ( y < matIcon[M_human].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_human;
                xSel = matIcon[M_human].x - 5, ySel = matIcon[M_human].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
        
    //rock
    else if( ( x > matIcon[M_rock].x ) && ( x < matIcon[M_rock].x + widthButton ) && ( y > matIcon[M_rock].y ) && ( y < matIcon[M_rock].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_rock;
                xSel = matIcon[M_rock].x - 5, ySel = matIcon[M_rock].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
        
    //spring
    else if( ( x > matIcon[M_spring].x ) && ( x < matIcon[M_spring].x + widthButton ) && ( y > matIcon[M_spring].y ) && ( y < matIcon[M_spring].y + heightButton ) )
        {
            if(mouse == 1)
            {
                currentMat = M_spring;
                xSel = matIcon[M_spring].x - 5, ySel = matIcon[M_spring].y - 5, wSel = (widthButton * multiplier), hSel = (heightButton * multiplier);
            }
        }
}
