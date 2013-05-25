//all the code for displaying the gui that allows you to selection your material

//material icon structure and delcarations
SDL_Rect matIcon[MAX_NUMBER_OF_UNIQUE_MATERIALS];

//main button controls
#define firstColumn 190
#define firstRow 80
#define widthButton 22
#define heightButton 22
#define rowSpacingMultiplier 1.45
#define columnSpacingMultiplier 1.618
#define selectionBoxSize 3

//main gui variables
#define xPos SCREEN_WIDTH - 200
#define yPos 0
#define wPos 200
#define hPos SCREEN_HEIGHT

//selection box positions
//compiler is GAY so you have to keep these as number values
short xSel = SCREEN_WIDTH - firstColumn - selectionBoxSize, ySel = firstRow - selectionBoxSize, wSel = widthButton + selectionBoxSize * 2, hSel = heightButton + selectionBoxSize * 2;

//displays gui
void selectionGUI(int x, int y, int mouse)
{
    //variables to step through array
    int i, j, k;
    
    //steps through the array and sets the icons of each material
    //varaibles for keeping track of were to put the icons
    j = xPos + widthButton/2;
    k = firstRow;
    for(i = 1; i < MAX_NUMBER_OF_UNIQUE_MATERIALS; i++){
        if(j + widthButton < SCREEN_WIDTH){
            //sets icons
            matIcon[i].x = j;
            matIcon[i].y = k;
            //updates value for the next icon
            j = j + (widthButton * rowSpacingMultiplier);
        }
        else{
            //reduces i by 1 so that it doesn't skip a material
            i--;
            //resets j
            j = xPos + widthButton/2;
            //increases k for the next line
            k = k + heightButton * columnSpacingMultiplier;
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
    apply_surface( xPos+10, 4, text, screen );
    SDL_FreeSurface( text );

    //selection box
    selectionBox.x = xSel;
	selectionBox.y = ySel;
	selectionBox.w = wSel;
	selectionBox.h = hSel;
    SDL_FillRect( screen , &selectionBox , 0xffffff);
    
    //prints a rectangle for each material icon
    for( i = M_earth; i < MAX_NUMBER_OF_UNIQUE_MATERIALS; i++ ){
        if(mats[i].name == NULL) {continue;}
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
                if(mats[i].name == NULL) {continue;}
                //changes material
                currentMat = i;
                //changes selection box to be under current material
                xSel = matIcon[i].x - selectionBoxSize, ySel = matIcon[i].y - selectionBoxSize, wSel = widthButton + selectionBoxSize * 2, hSel = heightButton + selectionBoxSize * 2;
            }
        }
    }
}
