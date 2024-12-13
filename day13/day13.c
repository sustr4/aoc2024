#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>
#include<math.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXY 320

// Point structure definition
typedef struct {
	long long x;
	long long y;
} TPoint;

typedef struct {
	TPoint button[2];
	long long cost[2];
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
			sscanf(line,"Button %c: X+%lld, Y+%lld", &var,
				&(mach[count].button[b].x),
				&(mach[count].button[b].y));
			break;
		case 'X':
			sscanf(line,"Prize: X=%lld, Y=%lld",
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

	return mach;
}

int main(int argc, char *argv[]) {

	TMachine *array;
	int i=0;	
	array=readInput();
/*
        for(i=0; array[i].prize.x; i++) {
		printf("Button A: X+%lld, Y+%lld\n", array[i].button[0].x, array[i].button[0].y);
		printf("Button B: X+%lld, Y+%lld\n", array[i].button[1].x, array[i].button[1].y);
		printf("Prize: X=%lld, Y=%lld\n\n", array[i].prize.x, array[i].prize.y);
	} */

	long long sum=0;
	#pragma omp parallel for shared(sum)
	for(i=0; i<MAXY; i++) {

		long long x1 = array[i].button[0].x; long long y1 = array[i].button[0].y;
		long long x2 = array[i].button[1].x; long long y2 = array[i].button[1].y;
		long long x3 = array[i].prize.x; long long y3 = array[i].prize.y;

		long long b=(long long)round(((long double)y3-(long double)x3/(long double)x1*(long double)y1)/((long double)y2-(long double)x2*(long double)y1/(long double)x1));
		long long a=(long long)round(((long double)x3-(long double)b*(long double)x2)/(long double)x1);
		if((a*x1 + b*x2 == x3) && (a*y1 + b*y2 == y3)) {
			long long cost=a*array[i].cost[0]+b*array[i].cost[1];
			printf("%i Hit %lld*%lld + %lld*%lld = %lld\n", i, a, array[i].cost[0], b, array[i].cost[1], cost);
			sum+=cost;
		}
	}

	printf("Sum: %lld\n", sum);

	return 0;
}
