void blit_glider(){
	apply_surface(posx - 24, posy - 24, glider, screen);
}


void blit_background(void){
	int i, j;

	for(i = 0; i < SCREEN_WIDTH; i+=500){
		for(j = 0; j < SCREEN_HEIGHT; j+=500){
			apply_surface(i,j,background, screen);
		}
	}
	
}
