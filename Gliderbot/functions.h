// 0 = shitty physics_settings.txt
// 1 = awesome. physics_settings.txt read and data loaded.
// 2 = no physics_settings.txt
int load_physics_settings(){
	
	FILE *physics_settings_txt = fopen("physics_settings.txt", "r");
	
	if(physics_settings_txt == NULL) return 2;
	
	char buff;
	
	//find the first § character
	do {   buff = fgetc(physics_settings_txt);   } while(buff != '§');
	
	// go the the line after the first '§' character
	while (buff != '\n')  buff = fgetc(physics_settings_txt);
	
	
	//input SCREEN_WIDTH
	fscanf(physics_settings_txt, "SCREEN_WIDTH: %d\n", &SCREEN_WIDTH);
	//input SCREEN_HEIGHT
	fscanf(physics_settings_txt, "SCREEN_HEIGHT: %d\n", &SCREEN_HEIGHT);
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );//reset screen size
	//input the reflection speed modifier
	fscanf(physics_settings_txt, "reflecSpeedMod: %f\n", &reflecSpeedMod);
	//input cut off speed
	fscanf(physics_settings_txt, "reflecSpeedCutOff: %f\n", &reflecSpeedCutOff);
	//input the thruster force
	fscanf(physics_settings_txt, "thrusterAccel: %f\n", &thrusterAccel);
	
	// close text document
	fclose(physics_settings_txt);
	return 1; // SUCESS!
}

SDL_Surface *load_image( char *filename )
{
    //Temporary storage for the image that is loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image with either SDL_image or LoadBMP. comment-out the one you are not using
    loadedImage = IMG_Load( filename );
    //loadedImage = SDL_LoadBMP( filename );
    
    //If the image was loaded correctly
    if( loadedImage != NULL )
    {
        // Create an optimized image
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        //Free the old image
        SDL_FreeSurface( loadedImage );
    }
    else{
		char message[256];
		strcpy(message, filename);
		strcat(message, " is missing");
		MessageBox(NULL, message, "Error", MB_OK);
		return NULL;
	}
    
    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y,  SDL_Surface* source, SDL_Surface* destination )
{
    //make a temporary rectangle to hold offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;
    
    //Blit surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

bool init()
{
	//Initialize all subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		exit(109);
	}
	
	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
		exit(110);
	}
	
	font = TTF_OpenFont("ttf\\FreeMonoBold.ttf", font_size);
	if(font == NULL) return false;
	
	//Set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	
	//If there was an error setting up the screen
	if(screen == NULL )
	{
		exit(111);
	}
	
	//Set the window caption
	SDL_WM_SetCaption( "Gliderbot - Jensen - 2013", NULL );
	
	//If everything initialized fine
	return true;
}

bool load_files()
{
	//Load the image
	glider = load_image( "images\\glider.png" );
	background = load_image( "images\\background.png" );
	
	
	//If everthing loaded fine
	return true;
}


void clean_up()
{
	//free the image
	SDL_FreeSurface( screen );
	SDL_FreeSurface( text );
	SDL_FreeSurface( glider );
	SDL_FreeSurface(background);
	//Quit SDL
	SDL_Quit();
}



void white_screen()
{
	int a, b;
	int i, j;
	//it is necessary to know which of the two (screen_height or screen_width) is bigger.
	//that will determine the way in which we make the background.
	if(SCREEN_HEIGHT > SCREEN_WIDTH)
	{
		a = SCREEN_HEIGHT;
		b = SCREEN_WIDTH;
	}
	else
	{
		a = SCREEN_WIDTH;
		b = SCREEN_HEIGHT;
	}
	//blit the entire background
	for(i = 0; i < a; i += 50)
	{
		for(j = 0; j < b; j += 50)
			apply_surface(i, j, background, screen);
	}
}
