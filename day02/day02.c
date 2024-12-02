#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
//#define INPUT "unit.txt"
#define MAXX 101
#define MAXY 1001

// Read input file line by line (e.g., into an array)
int **readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	int x=0, y=0;
	int **map=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(int));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		map[y][x++]=atoi(token);
		while( 1 ) {
			if(!(token = strtok(NULL, " "))) break;
			map[y][x++]=atoi(token);
		}

		y++; x=0;
		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int isSafe(int *array) {
	int x=0,dir,safe=1;
	dir = array[1] - array[0];
	for(x=0; array[x+1]; x++) {
		int curdir = array[x+1] - array[x];
		if((curdir>0)&&(dir<0)) safe = 0;
		if((curdir<0)&&(dir>0)) safe = 0;
		if((abs(array[x+1]-array[x])>3) ||
		   (abs(array[x+1]-array[x])<1)) safe=0;
	}
	if(safe) return(1);

	return(0);
}

int main(int argc, char *argv[]) {

	int y=0;
	int** array;
	array = readInput();
	int count = 0;

	for(y=0; array[y][0]; y++) {

		if(isSafe(array[y])) {
			count++;
			continue;
		}
		int rem;
		int wasSafe=0;
		for(rem=0; array[y][rem]; rem++) {
			int *ln=calloc(MAXX,sizeof(int));
			int x, pt=0;
			for(x=0; array[y][x]; x++) {
				if(x!=rem) ln[pt++]=array[y][x];
			}
			if (isSafe(ln)) {
				wasSafe=1;
				free(ln);
				break;
			}
			free(ln);
		}
		if(wasSafe) count++;
	}

	printf("%d\n", count);
	return 0;
}
