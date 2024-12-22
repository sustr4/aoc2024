#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 76
#define MAXY 2050
//#define INPUT "unit3.txt"

// Read input file line by line (e.g., into an array)
long long *readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	long long *inst=(long long*)calloc(MAXY, sizeof(long long));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		inst[count++] = atoll(line);
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

long long next(long long val) {
	
//    Calculate the result of multiplying the secret number by 64. Then, mix this result into the secret number. Finally, prune the secret number.

	long long ret = val*64;
	ret = (val^ret)&16777215;
	
//    Calculate the result of dividing the secret number by 32. Round the result down to the nearest integer. Then, mix this result into the secret number. Finally, prune the secret number.

	long long ret2 = ret/32;
	ret = (ret2^ret)&16777215;

//    Calculate the result of multiplying the secret number by 2048. Then, mix this result into the secret number. Finally, prune the secret number.
	
	ret2 = ret*2048;
	ret = (ret2^ret)&16777215;

//    To mix a value into the secret number, calculate the bitwise XOR of the given value and the secret number. Then, the secret number becomes the result of that operation. (If the secret number is 42 and you were to mix 15 into the secret number, the secret number would become 37.)
//    To prune the secret number, calculate the value of the secret number modulo 16777216. Then, the secret number becomes the result of that operation. (If the secret number is 100000000 and you were to prune the secret number, the secret number would become 16113920.)
	
	return(ret);
}

int main(int argc, char *argv[]) {

//	TPoint *array;
	int i=0;	
	long long *buyer = readInput();

	long long max = 0;

	long long *chng=calloc(4,sizeof(long long));
	long long *diff=calloc(4,sizeof(long long));


	long long *****price=malloc(19*sizeof(long long****));;

	for(chng[0]=-9; chng[0]<10; chng[0]++) {
	  price[chng[0]+9]=malloc(19*sizeof(long long***));
	  for(chng[1]=-9; chng[1]<10; chng[1]++) {
	    price[chng[0]+9][chng[1]+9]=malloc(19*sizeof(long long**));
	    for(chng[2]=-9; chng[2]<10; chng[2]++) {
	      price[chng[0]+9][chng[1]+9][chng[2]+9]=malloc(19*sizeof(long long*));
	      for(chng[3]=-9; chng[3]<10; chng[3]++) {
	        price[chng[0]+9][chng[1]+9][chng[2]+9][chng[3]+9]=malloc(MAXY*sizeof(long long));
		for(i=0; buyer[i]; i++) {
		price[chng[0]+9][chng[1]+9][chng[2]+9][chng[3]+9][i]=-1;
	}}}}}



	for(i=0; buyer[i]; i++) {
		long long r=buyer[i];
		for(int y=0; y<2000; y++) {
			for(int j=0; j<3; j++) diff[j]=diff[j+1]; // shift history
			long long new = next(r);
			diff[3]=(new%10) - (r%10);
			r=new;
			if(y<=3) continue;
			if(price[diff[0]+9][diff[1]+9][diff[2]+9][diff[3]+9][i]<0) price[diff[0]+9][diff[1]+9][diff[2]+9][diff[3]+9][i]=r%10;
		}
	}



	max=0;
	for(chng[0]=-9; chng[0]<10; chng[0]++) {
	  for(chng[1]=-9; chng[1]<10; chng[1]++) {
	    for(chng[2]=-9; chng[2]<10; chng[2]++) {
	      for(chng[3]=-9; chng[3]<10; chng[3]++) {
		int sum=0;
		for(i=0; buyer[i]; i++) {
			if(price[chng[0]+9][chng[1]+9][chng[2]+9][chng[3]+9][i]>0)
				sum+=price[chng[0]+9][chng[1]+9][chng[2]+9][chng[3]+9][i];
		}
		if(sum>max) max=sum;
	}}}}


	printf ("Max: %lld\n", max);

	return 0;
}
