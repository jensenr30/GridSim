// let the user type in a string up to 80 characres long. (don't forget that strings have to be terminated by a null '\0' character)

// after you have input the string, print it back saying:
/**
<user input string>?
is that all you have to say? you BITCH!
**/

// try looking at "Programming in C" page 216. that is where "working with strings" starts =D  :D  :3  >:I 


#include <stdio.h>
void readLine (char buffer[]);
int main(){

	int i;
	char line[81];


	printf("Enter your SHIT here:\n");
	readLine(line);
}




void readLine (char buffer[]){
	char character;
	int i = 0;

	do{
	if( i > 80){
		break;
	}
	character = getchar();
	buffer[i] = character;
	++i;
	}while( character != '\n');
	buffer[ i-1 ] = '\0'; //terminates the string
	printf("\"%s\"?\n", buffer);
	printf("is that all you have to say? you BITCH!\n");
}

















































/*
#include <stdio.h>



int main(){

	printf("type in 5 characters to apply an algorithym to\n");
	
	char myChar1; // the original input variables
	char myChar2;
	char myChar3;
	char myChar4;
	char myChar5;

	//modified variables
	char c1[2], c2[2], c3[2], c4[2], c5[2]; // for in the loop (algorithym
	
	myChar1 = getchar(); // inputs one character from user.
	myChar2 = getchar();
	myChar3 = getchar();
	myChar4 = getchar();
	myChar5 = getchar();

	//transfer to the c variables.
	c1[0] = myChar1;
	c2[0] = myChar2;
	c3[0] = myChar3;
	c4[0] = myChar4;
	c5[0] = myChar5;
	
	
	int itterations = 1;
	while(1){
			
		//printing press
		printf("%c"  ,c5[0]);
		printf("%c"  ,c1[0]);
		printf("%c"  ,c4[0]);
		printf("%c"  ,c2[0]);
		printf("%c\n",c3[0]);

		// apply algorithym
		c1[1] = c5[0];
		c2[1] = c1[0];
		c3[1] = c4[0];
		c4[1] = c2[0];
		c5[1] = c3[0];

		// copy new variables into old variables.
		c1[0] = c1[1];
		c2[0] = c2[1];
		c3[0] = c3[1];
		c4[0] = c4[1];
		c5[0] = c5[1];


		if(c1[1] == myChar1){
			if(c2[1] == myChar2){
				if(c3[1] == myChar3){
					if(c4[1] == myChar4){
						if(c5[1] == myChar5){
							break;
						}
					}
				}
			}
		}
		
		itterations++;
	} // end while(1)

	
	printf("itterations taken: %d\n", itterations);
}

*/