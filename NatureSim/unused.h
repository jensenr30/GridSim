//--------------------------------------------------------------------------------------------------------------------------------------------------------------

			//if a key was pressed
			if( event.type == SDL_KEYDOWN ){
				//nothing
			}
			//if a key was released
			if( event.type == SDL_KEYUP ){
				//nothing
			}
			
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

		//keeps track of how long you have been pressing the button.
		if(keyup == 1) countup++;
		if(keydown == 1) countdown++;
		if(keyleft == 1) countleft++;
		if(keyright == 1) countright++;

		if(countup >= MAX_COUNT) { y-=20; countup = 0; }
		if(countdown >= MAX_COUNT) { y+=20; countdown = 0; }
		if(countleft >= MAX_COUNT) { x-=20; countleft = 0; }
		if(countright >= MAX_COUNT) { x+=20; countright = 0; }

		//apply white background
		SDL_Rect my_rect;
		my_rect.x = 0;
		my_rect.y = 0;
		my_rect.w = 640;
		my_rect.h = 480;
		SDL_FillRect( screen, &my_rect, 0xffffff );
		SDL_FreeSurface( screen );

		//apply images
		apply_surface(x, y, image, screen ); //apply image
		apply_surface(x + 20, y, image, screen ); //apply image
		apply_surface(x - 20, y, image, screen ); //apply image
		apply_surface(x, y + 20, image, screen ); //apply image
		apply_surface(x, y - 20, image, screen ); //apply image
		// flip screen
		SDL_Flip( screen ); // update screen
		//SDL_FreeSurface( screen ); // saves memory
		
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

