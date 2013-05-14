

void thrust(int up, int left, int down, int right){
	
	if(up) // up
		speedy -= thrusterAccel;
	
	if(down) // left
		speedy += thrusterAccel;
	
	if(left) // down
		speedx -= thrusterAccel;
	
	if(right) // right
		speedx += thrusterAccel;
	
	//make sure the thrusters aren't negative
	if(uThruster < 0) uThruster = 0;
	if(dThruster < 0) dThruster = 0;
	if(lThruster < 0) lThruster = 0;
	if(rThruster < 0) rThruster = 0;
	
	
	return;
}


void move_glider(){
	
	posx += speedx;
	posy +=speedy;
	
	
}

void check_glider(){
	
	// x bounce
	if(posx < 24){
		posx = 24;
		if(speedx < -reflecSpeedCutOff) speedx *= -reflecSpeedMod;
		else speedx = 0;
	}
	else if(posx > SCREEN_WIDTH - 24){
		posx = SCREEN_WIDTH - 24;
		if(speedx > reflecSpeedCutOff) speedx *= -reflecSpeedMod;
		else speedx = 0;
	}
	
	// y bounce
	if(posy < 24){
		posy = 24;
		if(speedy < -reflecSpeedCutOff) speedy *= -reflecSpeedMod;
		else speedy = 0;
	}
	else if(posy > SCREEN_HEIGHT - 24){
		posy = SCREEN_HEIGHT - 24;
		if(speedy > reflecSpeedCutOff) speedy *= -reflecSpeedMod;
		else speedy = 0;
	}
}


