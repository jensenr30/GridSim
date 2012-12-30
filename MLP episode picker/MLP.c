//this program decides which episode of MLP you should watch.



#include <stdlib.h> // included for the rand() and srand() functions
#include <time.h>   // included for the time() function
#include <stdio.h>  // if you have to ask, leave now. xD


int main(){
	
	int season;
	int episode;

	srand(time(NULL)); // randomize the random number list.

	while(1){ // infinite loop		
		episode = rand() % (26 + 26 + 13) + 1;

		if(episode <= 26){
			season = 1;
		}
		else if(episode <= 26 + 26){
			season = 2;
			episode -= 26;
		}
		else{
			season = 3;
			episode -= 26 + 13;
		}
		
		
		if(season == 1 || season == 2){ // season = 1 or 2
		episode = rand() % 26 + 1; // episode lies between 1 and 26 
		}
		else{ // season = 3
			episode = rand() % 13 + 1; // episode lies between 1 and 13
		}
	
	
		//make it look pretty. "Episode %d"
		printf("Season %d\n",season);
		printf("Episode %d\n",episode);
		// at this point, you will have the season number and the episode number. write code to print out the episode number and season number.
		printf("Press a key to try pick another\n");
		system("pause>nul"); // wait for more user input.
	
	}
	return 0;
}