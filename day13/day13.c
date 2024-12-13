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
#define MAXY 320
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

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
			sscanf(line,"Button %c: X+%lld, Y+%lld", &var,
				&(mach[count].button[b].x),
				&(mach[count].button[b].y));
			break;
		case 'X':
			sscanf(line,"Prize: X=%lld, Y=%lld",
				&(mach[count].prize.x),
				&(mach[count].prize.y));
			break;
		}

		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%lld,%lld",
		//	&(inst[count].x),
		//	&(inst[count].y));

		// Read tokens from single line
		//char *token;
		//token = strtok(line, ",");
		//while( 1 ) {
		//	if(!(token = strtok(NULL, ","))) break;
		//}

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
               printf("Button A: X+%lld, Y+%lld\n", array[i].button[0].x, array[i].button[0].y);
              printf("Button B: X+%lld, Y+%lld\n", array[i].button[1].x, array[i].button[1].y);
              printf("Prize: X=%lld, Y=%lld\n\n", array[i].prize.x, array[i].prize.y);
	}


	long long sum=0;
	#pragma omp parallel for shared(sum)
	for(i=0; i<MAXY; i++) {
		long long min=INT_MAX;
		long long y1=-array[i].button[0].y;
		for(long long x1=0; x1<=array[i].prize.x; x1+=array[i].button[0].x) {
			y1+=array[i].button[0].y;
//			printf("b=(%lld-%lld*%lld)/%lld\n",array[i].prize.x,a,array[i].button[0].x,array[i].button[1].x);
//			long long b=(array[i].prize.x-a*array[i].button[0].x)/array[i].button[1].x;
			long long b=(array[i].prize.x-x1)/array[i].button[1].x;
			
			if(x1+b*array[i].button[1].x!=array[i].prize.x) continue;
			if(y1+b*array[i].button[1].y!=array[i].prize.y) continue;

			long long a=x1/array[i].button[0].x;
			long long cost= array[i].cost[0] * a;
			cost+= array[i].cost[1] * b;
			if(min>cost) min=cost;
			printf("%i Hit %lld+%lld = %lld, min = %lld\n", i, a, b, cost, min );
			
		}
		if(min<INT_MAX) sum+=min;
	}

	printf("Sum: %lld\n", sum);

	return 0;
}
