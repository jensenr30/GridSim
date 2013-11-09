 
 void set_window_size(int w, int h){
	screen = SDL_SetVideoMode( w, h, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE );
	
	//If there was an error setting up the screen
	if(screen == NULL )
	{
		exit(111);
	}
}
 
 
 
 
SDL_Surface *load_image( char* filename ){
    //Temporary storage for the image that is loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image with either SDL_image or LoadBMP. comment-out the one you are not using
    loadedImage = IMG_Load( filename );
    //loadedImage = SDL_LoadBMP( filename );
    
    //If the image was loaded correctly
    if( loadedImage != NULL ){
        // Create an optimized image
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        //Free the old image
        SDL_FreeSurface( loadedImage );
    }
    
    //Return the optimized image
    return optimizedImage;
}

//this returns a pointer to an SDL_Surface
SDL_Surface *create_surface(int width, int height){
	return SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, 0xFF0000, 0x00FF00, 0x0000FF, 0x000000);
}

void apply_surface( int x, int y,  SDL_Surface* source, SDL_Surface* destination ){
    //make a temporary rectangle to hold offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;
    
    //Blit surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

int init(){
	//Initialize all subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ){
		return false;
	}
	
	//Set up the screen
	set_window_size(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//If there was an error setting up the screen
	if(screen == NULL ){
		return false;
	}
	
	//Set the window caption
	SDL_WM_SetCaption( "GridVenture 1.0", NULL );
	
	
	
	//If everything initialized fine
	return true;
}

int load_files(){
	/*
	//Load the image
	image = load_image( "box.png" );
	
	//if there was an error in loading the image
	if( image == NULL )
	{
		return false;
	}
	*/
	
	icon = SDL_LoadBMP("game icon.ico");
	SDL_WM_SetIcon(icon, NULL); // sets the icon of the windows and taskbar item
	
	//Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        MessageBox(NULL, "all your base are belong to us", "Hello Gentelmen", MB_OK);
        return false;
    }
    
    //open font file
    font = TTF_OpenFont( "FreeMonoBold.ttf", 22 );
    
    if (font == NULL)
    {
        MessageBox(NULL, "all your base are belong to us", "Hello Gentelmen", MB_OK);
    }
	
	//If everthing loaded fine
	return true;
}


void clean_up(){
	//free the image
	//SDL_FreeSurface(image);
	//SDL_FreeSurface(text);
	SDL_FreeSurface(gridSurface);
	SDL_FreeSurface(screen);
	
	//Quit SDL
	SDL_Quit();
}

/// returns a random number from lowBound to highBound.
// e.g. if you call:
//		get_rand(3,7);
// then you could get: 3, 4, 5, 6, 7 as a return value.
//#define get_rand(lowBound, highBound) (rand()%((highBound) - (lowBound) + 1) + (lowBound))

int get_rand(lowBound, highBound){
	
	// if the lowBound is higher than the highBound, then flip them around and return that.
	if(highBound < lowBound){
		return get_rand(highBound, lowBound);
	}
	
	return ( rand() % (highBound-lowBound+1) ) + lowBound;
}





/// when you call this function, it returns a 1 or a 0.
/// your chances of getting a '1' returned are chance/100,000
/// for example, if call:
/// 	roll_ht(5000);
/// then there is a 5000/100000 chance (5%) that roll_ht will return a 1.
/// "roll_ht" means, "roll hundred thousand"
int roll_ht(int chance){
	if(chance > get_rand(0,99999)) return 1; // return 1 if the number the user gave you is greater than a random number between 0 and 99999.
	return 0; // otherwise, return 0
}
