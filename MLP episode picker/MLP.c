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

		//makes it look pretty.
		printf("\n");
		printf("Season %d\n",season);
		printf("Episode %d\n",episode);

		//episode names
        if(episode == 1 && season == 1){
        printf("Friendship Is Magic, part 1\n");
        }
        else if(episode == 2 && season == 1){
        printf("Friendship Is Magic, part 2\n");
        }
        else if(episode == 3 && season == 1){
        printf("The Ticket Master\n");
        }
        else if(episode == 4 && season == 1){
        printf("Applebuck Season\n");
        }
        else if(episode == 5 && season == 1){
        printf("Giffon the Brush-Off\n");
        }
        else if(episode == 6 && season == 1){
        printf("Boast Busters\n");
        }
        else if(episode == 7 && season == 1){
        printf("Dragonshy\n");
        }
        else if(episode == 8 && season == 1){
        printf("Look Before You Sleep\n");
        }
        else if(episode == 9 && season == 1){
        printf("Bridle Gossip\n");
        }
        else if(episode == 10 && season == 1){
        printf("Swarm of the Century\n");
        }
        else if(episode == 11 && season == 1){
        printf("Winter Wrap Up\n");
        }
        else if(episode == 12 && season == 1){
        printf("Call of the Cutie\n");
        }
        else if(episode == 13 && season == 1){
        printf("Fall Weather Friends\n");
        }
        else if(episode == 14 && season == 1){
        printf("Suited for Success\n");
        }
        else if(episode == 15 && season == 1){
        printf("Feeling Pinkie Keen\n");
        }
        else if(episode == 16 && season == 1){
        printf("Sonic Rainboom\n");
        }
        else if(episode == 17 && season == 1){
        printf("Stare Master\n");
        }
        else if(episode == 18 && season == 1){
        printf("The Show Stoppers\n");
        }
        else if(episode == 19 && season == 1){
        printf("A Dog and Pony Show\n");
        }
        else if(episode == 20 && season == 1){
        printf("Green Isn't Your Color\n");
        }
        else if(episode == 21 && season == 1){
        printf("Over a Barrel\n");
        }
        else if(episode == 22 && season == 1){
        printf("A Bird in the Hoof\n");
        }
        else if(episode == 23 && season == 1){
        printf("The Cutie Mark Chronicles\n");
        }
        else if(episode == 24 && season == 1){
        printf("Owl's Well That Ends Well\n");
        }
        else if(episode == 25 && season == 1){ // Pinkie Pie
        printf("Party of One\n");
        }
        else if(episode == 26 && season == 1){
        printf("The Best Night Ever\n");
        }
        else if(episode == 1 && season == 2){
        printf("The Return of Harmony, Part 1\n");
        }
        else if(episode == 2 && season == 2){
        printf("The Return of Harmony, Part 2\n");
        }
        else if(episode == 3 && season == 2){ // twilight goes CRAZY!
        printf("Lesson Zero\n");
        }
        else if(episode == 4 && season == 2){
        printf("Luna Eclipsed\n");
        }
        else if(episode == 5 && season == 2){
        printf("Sisterhooves Social\n");
        }
        else if(episode == 6 && season == 2){
        printf("The Cutie Pox\n");
        }
        else if(episode == 7 && season == 2){
        printf("May the Best Pet Win!\n");
        }
        else if(episode == 8 && season == 2){
        printf("The Mysterious Mare Do Well\n");
        }
        else if(episode == 9 && season == 2){
        printf("Sweet and Elite\n");
        }
        else if(episode == 10 && season == 2){
        printf("Secret of My Excess\n");
        }
        else if(episode == 11 && season == 2){
        printf("Family Appreciation Day\n");
        }
        else if(episode == 12 && season == 2){
        printf("Baby Cakes\n");
        }
        else if(episode == 13 && season == 2){
        printf("Hearth's Warming Eve\n");
        }
        else if(episode == 14 && season == 2){
        printf("The Last Roundup\n");
        }
        else if(episode == 15 && season == 2){
        printf("The Super Speedy Cider Squeezy 6000\n");
        }
        else if(episode == 16 && season == 2){
        printf("Read It and Weep\n");
        }
        else if(episode == 17 && season == 2){
        printf("Hearts and Hooves Day\n");
        }
        else if(episode == 18 && season == 2){
        printf("A Friend in Deed\n");
        }
        else if(episode == 19 && season == 2){
        printf("Putting Your Hoof Down\n");
        }
        else if(episode == 20 && season == 2){
        printf("It's About Time\n");
        }
        else if(episode == 21 && season == 2){
        printf("Dragon Quest\n");
        }
        else if(episode == 22 && season == 2){
        printf("Hurricane Fluttershy\n");
        }
        else if(episode == 23 && season == 2){
        printf("Ponyville Confidential\n");
        }
        else if(episode == 24 && season == 2){
        printf("MMMystery on the Friendship Express\n");
        }
        else if(episode == 25 && season == 2){
        printf("A Canterlot Wedding, Part 1\n");
        }
        else if(episode == 26 && season == 2){
        printf("A Canterlot Wedding, Part 2\n");
        }
        else if(episode == 1 && season == 3){
        printf("The Crystal Empire, Part 1\n");
        }
        else if(episode == 2 && season == 3){
        printf("The Crystal Empire, Part 2\n");
        }
        else if(episode == 3 && season == 3){
        printf("Too Many Pinkie Pies\n");
        }
        else if(episode == 4 && season == 3){
        printf("One Bad Apple\n");
        }
        else if(episode == 5 && season == 3){
        printf("Magic Duel\n");
        }
        else if(episode == 6 && season == 3){
        printf("Sleepless in Ponyville\n");
        }
        else if(episode == 7 && season == 3){
        printf("Wonderbolts Academy\n");
        }
        else if(episode == 8 && season == 3){
        printf("Apple Family Reunion\n");
        }
        else if(episode == 9 && season == 3){
        printf("Spike at Your Service\n");
        }
        else if(episode == 10 && season == 3){
        printf("Keep Calm and Flutter On\n");
        }
        else if(episode == 11 && season == 3){
        printf("Just For Sidekicks\n");
        }
        else if(episode == 12 && season == 3){
        printf("TBA\n");
        }
        else if(episode == 13 && season == 3){
        printf("TBA\n");
        }

		// at this point, you will have the season number and the episode number. write code to print out the episode number and season number.
		printf("Press a key to try pick another\n");
		system("pause>nul"); // wait for more user input.

	}
	return 0;
}
