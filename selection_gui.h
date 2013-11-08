// all the code for displaying the gui that allows you to selection your material


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
            //cursorRectangle.x = x - x%CELL_SIZE - CELL_SIZE;
            //cursorRectangle.y = y - y%CELL_SIZE - CELL_SIZE;
            cursorRectangle.x = x - CELL_SIZE*1.5;
            cursorRectangle.y = y - CELL_SIZE*1.5;
            cursorRectangle.w = CELL_SIZE*3;
            cursorRectangle.h = CELL_SIZE*3;
            SDL_FillRect( screen, &cursorRectangle, mats[currentMat].color);
        }
}
