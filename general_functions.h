 //prototype
 bool initGL();
 
 void set_window_size(int w, int h){
	screen = SDL_SetVideoMode( w, h, SCREEN_BPP, SDL_OPENGL | SDL_RESIZABLE | SDL_DOUBLEBUF );
	
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	if(screen == NULL ) exit(111);
}

//this acts as a replacement for GL_FillRect
void GL_FillRect(SDL_Surface *dst, SDL_Rect *rct, int color){
	//clear the color buffer
	//glClear( GL_COLOR_BUFFER_BIT );
	// quadrilateral
	glBegin(GL_QUADS);
	//process color
	glColor3ub(color/0x10000, (color/0x100)&0xff, color&0xff );
	//generate verticies
	glVertex2s(rct->x,          rct->y			);
	glVertex2s(rct->x + rct->w, rct->y			);
	glVertex2s(rct->x + rct->w, rct->y + rct->h	);
	glVertex2s(rct->x,          rct->y + rct->h	);
	//end the quad
	glEnd();
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

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	// this gives us the pixel grid that we want (the same layout as SDL with 0,0 in the upper left hand corner.
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    return true;
}

int init(){
	//Initialize all subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ){
		return false;
	}
	
	//Set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL | SDL_RESIZABLE );
	//If there was an error setting up the screen
	if(screen == NULL )
	{
		exit(111);
	}
	
	//If there was an error setting up the screen
	if(screen == NULL ){
		return false;
	}
	
	//Initialize OpenGL
    if( initGL() == false )
    {
        return false;    
    }
	
	
	//Set the window caption
	SDL_WM_SetCaption( "GridSim 0.1g - openGL", NULL );
	
	//SDL_Wm_SetIcon( icon ); // sets the icon of the windows and taskbar item
	
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
	//SDL_FreeSurface( image );
	//SDL_FreeSurface( text );
	//SDL_FreeSurface( screen );
	
	//Quit SDL
	SDL_Quit();
}

/// returns a random number from lowBound to highBound.
/// i.e. if you call:
///		get_rand(3,7);
/// then you could get: 3, 4, 5, 6, 7 as a return value.
#define get_rand(lowBound, highBound) (rand()%(highBound - lowBound + 1) + lowBound)
/*
int get_rand(lowBound, highBound){
	int diff = highBound - lowBound;
	return ( rand() % (diff+1) ) + lowBound;
}
*/




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
