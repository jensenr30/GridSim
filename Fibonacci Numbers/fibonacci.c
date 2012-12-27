#include <stdio.h>

#define max_n 300 // the max number of fib numbers that can be calculated


int main(){
	
	int n; // the number of fibonacci numbers to be calculated
	
	unsigned long long int fibonacci[max_n];
	
	
	printf("How many Fibonacci numbers do you want to calculate?\nn = ");
	scanf("%d", &n);
	
	// don't allow n to be greater than max_n
	if( n > max_n) n = max_n;
	// don't allow n to be less than 1
	else if(n <1) n = 1;
	
	int i;
	for(i = 0; i < n; i++){
		if(i == 0 || i == 1)
			fibonacci[i] = 1;
		else
			fibonacci[i] = fibonacci[i-1] + fibonacci[i-2];
		
		
		printf("%.2d:\t%lu\n", i, fibonacci[i]);
	}
	
	
	
	
	
	
	return 0;
}
