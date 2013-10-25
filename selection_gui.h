//all the code for displaying the gui that allows you to selection your material

//material icon structure and delcarations
SDL_Rect matIcon[MAX_NUMBER_OF_UNIQUE_MATERIALS];

//main button controls
#define XPosForGUISelectionStart 190
#define YPosFromGUISide 80
#define widthButton 20
#define heightButton 20
#define rowSpacingMultiplier 1.45 
#define columnSpacingMultiplier 1.618
#define selectionBoxSize 3

//main gui variables
#define GUI_X SCREEN_WIDTH - 200
#define GUI_Y 0
#define GUI_W 200
#define GUI_H SCREEN_HEIGHT

//selection box positions
//compiler is GAY so you have to keep these as number values
short xSel = SCREEN_WIDTH - XPosForGUISelectionStart - selectionBoxSize, ySel = YPosFromGUISide - selectionBoxSize, wSel = widthButton + selectionBoxSize * 2, hSel = heightButton + selectionBoxSize * 2;

//displays gui
void selectionGUI(int x, int y, int mouse)
{
    //variables to step through array
    int i, j, k;
    
    //steps through the array and sets the icons of each material
    //varaibles for keeping track of were to put the icons
    j = GUI_X + widthButton/2;
    k = YPosFromGUISide;
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
            j = GUI_X + widthButton/2;
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
	guiRectangle.x = GUI_X;
	guiRectangle.y = GUI_Y;
	guiRectangle.w = GUI_W;
	guiRectangle.h = GUI_H;
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
    for( i = m_earth; i < MAX_NUMBER_OF_UNIQUE_MATERIALS; i++ ){
        if(mats[i].name == NULL) {continue;}
        guiRectangle.x = matIcon[i].x;
        guiRectangle.y = matIcon[i].y;
        guiRectangle.w = widthButton;
        guiRectangle.h = heightButton;
        SDL_FillRect( screen , &guiRectangle , mats[i].color);
    }
    
    //checks for mouse clicks over material icons
    for ( i = m_earth; i < MAX_NUMBER_OF_UNIQUE_MATERIALS; i++ ){

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

#define MAX_BRUSHES 7
#define brushSpacingMultiplier 2
#define brushButtonWidth 20
#define brushButtonHeight 20
#define xPosFromEdge 180
#define yPosFromEdge 15

int oldx = 0;
int oldy = 0;
int BrushSize = 1;

//brushes structure
struct Brushes
{
    char *name;
    int x;
    int y;
}Brushes[MAX_BRUSHES];

//brushs and speed control
void brushesGUI(int x, int y, int mouse)
{
    //variables to step through array
    int i, j;
        
    //define rectangle for cursor
    SDL_Rect brushesRectangle;
 
    //default setting
    j = xPosFromEdge + brushButtonWidth/2;
    //steps through the array and fill it with data for each button
    for(i = 0; i < MAX_BRUSHES; i++){
        Brushes[i].x = j;
        Brushes[i].y = yPosFromEdge;
        j = j + (brushButtonWidth * brushSpacingMultiplier);
    }
    
    //main window
	brushesRectangle.x = 0;
	brushesRectangle.y = 0;
	brushesRectangle.w = GUI_X;
	brushesRectangle.h = 50;
    SDL_FillRect( screen , &brushesRectangle , 0x181818);
    
    //assigns text to brush names
    Brushes[0].name = "Standard";
    Brushes[1].name = "Basic";
    Brushes[2].name = "L Basic";
    Brushes[3].name = "L Standard";
    Brushes[4].name = "Horizontal";
    Brushes[5].name = "Vertical";
    Brushes[6].name = "Line Tool";
    
    //prints text
    text = TTF_RenderText_Blended( font, Brushes[mouseModifier].name , textColor );
    
    //apply text to screen
    apply_surface( 10, 0, text, screen );
    SDL_FreeSurface( text );
    
    //prints a rectangle for each material icon
    for( i = 0; i < MAX_BRUSHES; i++ ){
        brushesRectangle.x = Brushes[i].x;
        brushesRectangle.y = Brushes[i].y;
        brushesRectangle.w = brushButtonWidth;
        brushesRectangle.h = brushButtonHeight;
        SDL_FillRect( screen , &brushesRectangle , 0xffffff);
    }

    //checks for mouse clicks over brush icons
    for ( i = 0; i < MAX_BRUSHES; i++ ){

        if( ( x > Brushes[i].x ) && ( x < Brushes[i].x + brushButtonWidth ) && ( y > Brushes[i].y ) && ( y < Brushes[i].y + brushButtonHeight ) )
        {
            if(mouse == 1)
            {
                mouseModifier = i;
            }
        }
    }
       
    //brushes
    if(mouse == 1)
    {
        switch(mouseModifier){
            //basic brush
            case 1:
                setcell(x,y,currentMat);
                setcell(x+CELL_SIZE,y,currentMat);
                setcell(x,y+CELL_SIZE,currentMat);
                setcell(x-CELL_SIZE,y,currentMat);
                setcell(x,y-CELL_SIZE,currentMat);
                break;
            //larger basic brush
            case 2:
                setcell(x,y,currentMat);
                setcell(x+CELL_SIZE,y,currentMat);
                setcell(x,y+CELL_SIZE,currentMat);
                setcell(x-CELL_SIZE,y,currentMat);
                setcell(x,y-CELL_SIZE,currentMat);
                setcell(x+CELL_SIZE*2,y,currentMat);
                setcell(x,y+CELL_SIZE*2,currentMat);
                setcell(x-CELL_SIZE*2,y,currentMat);
                setcell(x,y-CELL_SIZE*2,currentMat);
                setcell(x+CELL_SIZE,y-CELL_SIZE,currentMat);
                setcell(x-CELL_SIZE,y+CELL_SIZE,currentMat);
                setcell(x+CELL_SIZE,y+CELL_SIZE,currentMat);
                setcell(x-CELL_SIZE,y-CELL_SIZE,currentMat);
                break;
            //bigger square
            case 3:
                setcell(x,y,currentMat);
                setcell(x+CELL_SIZE,y,currentMat);
                setcell(x,y+CELL_SIZE,currentMat);
                setcell(x-CELL_SIZE,y,currentMat);
                setcell(x,y-CELL_SIZE,currentMat);
                setcell(x+CELL_SIZE,y+CELL_SIZE,currentMat);
                setcell(x-CELL_SIZE,y-CELL_SIZE,currentMat);
                setcell(x+CELL_SIZE,y-CELL_SIZE,currentMat);
                setcell(x-CELL_SIZE,y+CELL_SIZE,currentMat);
                break;
            case 4:
                setcell(x,y,currentMat);
                //setcell(x+CELL_SIZE,y,currentMat);
                //setcell(x,y+CELL_SIZE,currentMat);
                //setcell(x-CELL_SIZE,y,currentMat);
                //setcell(x,y-CELL_SIZE,currentMat);
                setcell(x-(BrushSize/CELL_SIZE),y,currentMat);
                setcell(x,y-(BrushSize/CELL_SIZE),currentMat);
                setcell(x-CELL_SIZE+(BrushSize/CELL_SIZE),y,currentMat);
                setcell(x,y-CELL_SIZE+(BrushSize/CELL_SIZE),currentMat);
                //setcell(x+CELL_SIZE,y-CELL_SIZE,currentMat);
                //setcell(x-CELL_SIZE,y+CELL_SIZE,currentMat);
                //setcell(x+CELL_SIZE,y+CELL_SIZE,currentMat);
                //setcell(x-CELL_SIZE,y-CELL_SIZE,currentMat);
                break;
            case 5:
                BrushSize++;
                mouseModifier = 4;
                break;
                /*
            //vertical line
            case 4:
                setcell(x,y,currentMat);
                setcell(x+CELL_SIZE,y,currentMat);
                setcell(x-CELL_SIZE,y,currentMat);
                setcell(x+CELL_SIZE*2,y,currentMat);
                setcell(x-CELL_SIZE*2,y,currentMat);
                break;
            //horizontal line
            case 5:
                setcell(x,y,currentMat);
                setcell(x,y+CELL_SIZE,currentMat);
                setcell(x,y-CELL_SIZE,currentMat);
                setcell(x,y+CELL_SIZE*2,currentMat);
                setcell(x,y-CELL_SIZE*2,currentMat);
                break;
                */
            //line tool
            case 6:
                if(x == 1){
                    setcell(x, y, currentMat);
                }
                oldx = x;
                oldy = y;
                setcell((oldx + x),(oldy + y),currentMat);
                break;
            default:
                setcell(x,y,currentMat);
                break;
        }
    }
        
        /* if statement way
        
        //basic brush
        if(mouseModifier == 1)
        {
            setcell(x,y,currentMat);
            setcell(x+CELL_SIZE,y,currentMat);
            setcell(x,y+CELL_SIZE,currentMat);
            setcell(x-CELL_SIZE,y,currentMat);
            setcell(x,y-CELL_SIZE,currentMat);
        }
        
        //larger basic brush
        if(mouseModifier == 2)
        {
            setcell(x,y,currentMat);
            setcell(x+CELL_SIZE,y,currentMat);
            setcell(x,y+CELL_SIZE,currentMat);
            setcell(x-CELL_SIZE,y,currentMat);
            setcell(x,y-CELL_SIZE,currentMat);
            setcell(x+CELL_SIZE*2,y,currentMat);
            setcell(x,y+CELL_SIZE*2,currentMat);
            setcell(x-CELL_SIZE*2,y,currentMat);
            setcell(x,y-CELL_SIZE*2,currentMat);
            setcell(x+CELL_SIZE,y-CELL_SIZE,currentMat);
            setcell(x-CELL_SIZE,y+CELL_SIZE,currentMat);
            setcell(x+CELL_SIZE,y+CELL_SIZE,currentMat);
            setcell(x-CELL_SIZE,y-CELL_SIZE,currentMat);
        }
        
        //bigger square
        if(mouseModifier == 3)
        {
            setcell(x,y,currentMat);
            setcell(x+CELL_SIZE,y,currentMat);
            setcell(x,y+CELL_SIZE,currentMat);
            setcell(x-CELL_SIZE,y,currentMat);
            setcell(x,y-CELL_SIZE,currentMat);
            setcell(x+CELL_SIZE,y+CELL_SIZE,currentMat);
            setcell(x-CELL_SIZE,y-CELL_SIZE,currentMat);
            setcell(x+CELL_SIZE,y-CELL_SIZE,currentMat);
            setcell(x-CELL_SIZE,y+CELL_SIZE,currentMat);
        }
        
        //vertical line
        if(mouseModifier == 4)
        {
            setcell(x,y,currentMat);
            setcell(x+CELL_SIZE,y,currentMat);
            setcell(x-CELL_SIZE,y,currentMat);
            setcell(x+CELL_SIZE*2,y,currentMat);
            setcell(x-CELL_SIZE*2,y,currentMat);
        }
        
        //horizontal line
        if(mouseModifier == 5)
        {
            setcell(x,y,currentMat);
            setcell(x,y+CELL_SIZE,currentMat);
            setcell(x,y-CELL_SIZE,currentMat);
            setcell(x,y+CELL_SIZE*2,currentMat);
            setcell(x,y-CELL_SIZE*2,currentMat);
        }
        
        //line tool
        if(mouseModifier == 6)
        {
            oldx = x;
            oldy = y;
            setcell((oldx + x),(oldy + y),currentMat);
        }
        
    }*/
}

void cursorDisplay(x, y)
{
        //cursor rectangle
        SDL_Rect cursorRectangle;
        cursorRectangle.x = 0;
        cursorRectangle.y = 0;
        cursorRectangle.w = CELL_SIZE;
        cursorRectangle.h = CELL_SIZE;
        
        //print recagle for base cursor
        cursorRectangle.x = x - CELL_SIZE/2;
        cursorRectangle.y = y - CELL_SIZE/2;
        cursorRectangle.w = cursorRectangle.h = CELL_SIZE;
        SDL_FillRect( screen , &cursorRectangle , mats[currentMat].color);
        
        //updates cursor to what it the cursor is about to print
        
        //basic brush
        if(mouseModifier == 1)
        {
            //top
            cursorRectangle.x = x - CELL_SIZE/2;
            cursorRectangle.y = y - CELL_SIZE*1.5;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
            
            //left
            cursorRectangle.x = x - CELL_SIZE*1.5;
            cursorRectangle.y = y - CELL_SIZE/2;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
            
            //bottom
            cursorRectangle.x = x - CELL_SIZE/2;
            cursorRectangle.y = y + CELL_SIZE/2;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
            
            //right
            cursorRectangle.x = x + CELL_SIZE/2;
            cursorRectangle.y = y - CELL_SIZE/2;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
           
        }
        
        //bigger basic brush
        if(mouseModifier == 2)
        {
            
            cursorRectangle.x = x - CELL_SIZE*1.5;
            cursorRectangle.y = y - CELL_SIZE*1.5;
            cursorRectangle.w = CELL_SIZE*3;
            cursorRectangle.h = CELL_SIZE*3;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
            
            //top
            cursorRectangle.x = x - CELL_SIZE/2;
            cursorRectangle.y = y - CELL_SIZE*2.5;
            cursorRectangle.w = CELL_SIZE;
            cursorRectangle.h = CELL_SIZE;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
            
            //left
            cursorRectangle.x = x - CELL_SIZE*2.5;
            cursorRectangle.y = y - CELL_SIZE/2;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
            
            //bottom
            cursorRectangle.x = x - CELL_SIZE/2;
            cursorRectangle.y = y + CELL_SIZE*1.5;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
            
            //right
            cursorRectangle.x = x + CELL_SIZE*1.5;
            cursorRectangle.y = y - CELL_SIZE/2;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
            
        }
        
        //bigger square
        if(mouseModifier == 3)
        {
            cursorRectangle.x = x - CELL_SIZE*1.5;
            cursorRectangle.y = y - CELL_SIZE*1.5;
            cursorRectangle.w = CELL_SIZE*3;
            cursorRectangle.h = CELL_SIZE*3;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
        }
}
