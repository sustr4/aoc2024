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
#define MAXY 321
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	long x;
	long y;
} TPoint;

typedef struct {
	TPoint button[2];
	int cost[2];
	TPoint prize;
} TMachine;


// Read input file line by line (e.g., into an array)
TMachine *readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	TMachine *mach=(TMachine*)calloc(MAXY, sizeof(TMachine));
	for(int iter=0; iter<MAXY; iter++) {
		mach[iter].cost[0]=3;
		mach[iter].cost[1]=1;
	}

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) {
			count++;
			continue; }

		int b=1;
		switch(line[7]) {
		case 'A':
			b=0;
		case 'B':
			char var;
			sscanf(line,"Button %c: X+%ld, Y+%ld", &var,
				&(mach[count].button[b].x),
				&(mach[count].button[b].y));
			break;
		case 'X':
			sscanf(line,"Prize: X=%ld, Y=%ld",
				&(mach[count].prize.x),
				&(mach[count].prize.y));
			mach[count].prize.x+=10000000000000;
			mach[count].prize.y+=10000000000000;
			break;
		}

	}

	fclose(input);
	if (line)
	free(line);

//	printMap(map);

	return mach;
}

int main(int argc, char *argv[]) {

	TMachine *array;
	int i=0;	
//	array = readInput();
	array=readInput();

        for(i=0; array[i].prize.x; i++) {
               printf("Button A: X+%ld, Y+%ld\n", array[i].button[0].x, array[i].button[0].y);
              printf("Button B: X+%ld, Y+%ld\n", array[i].button[1].x, array[i].button[1].y);
              printf("Prize: X=%ld, Y=%ld\n\n", array[i].prize.x, array[i].prize.y);
	}


	long long sum=0;
	#pragma omp parallel for shared(sum)
	for(i=0; i<MAXY; i++) {
		long long min=LLONG_MAX;
		for(long a=0; a<array[i].prize.x/array[i].button[0].x; a++) {
//			printf("b=(%d-%d*%d)/%d\n",array[i].prize.x,a,array[i].button[0].x,array[i].button[1].x);
			long b=(array[i].prize.x-a*array[i].button[0].x)/array[i].button[1].x;
			
			if((a*array[i].button[0].x+b*array[i].button[1].x==array[i].prize.x) &&
			   (a*array[i].button[0].y+b*array[i].button[1].y==array[i].prize.y)) {
				long cost= array[i].cost[0] * a;
				cost+= array[i].cost[1] * b;
				if(min>cost) min=cost;
				printf("%d Hit %ld+%ld = %ld, min = %lld\n", i, a, b, cost, min );
			}
		}
		if(min<INT_MAX) sum+=min;
	}

	printf("Sum: %lld\n", sum);

	return 0;
}
