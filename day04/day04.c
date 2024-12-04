#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 140
#define MAXY 140
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10
//#define INPUT "unit2.txt"
//#define MAXX 6
//#define MAXY 5

// Print a two-dimensional array
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}

// Retrieve nth neighbor from a map
int dy[] = { -1, -1, -1, 0, 1, 1, 1, 0};
int dx[] = { -1, 0, 1, 1, 1, 0, -1, -1};
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<8));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
}

// Read input file line by line (e.g., into an array)
char **readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		//Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int main(int argc, char *argv[]) {

	char **map=readInput();

	int count=0;
	int x,y;
	for(y=1; y<MAXY-1; y++) {
		for(x=1; x<MAXX-1; x++) {
			if(map[y][x]=='A') {
				int nb;
				for(nb=0;nb<8;nb+=2) {
					if((mapnb(map,y,x,nb)=='M') && (mapnb(map,y,x,(nb+4)%8)=='S') &&
					   (mapnb(map,y,x,(nb+2)%8)=='M') && (mapnb(map,y,x,(nb+6)%8)=='S')) count++;
					if((mapnb(map,y,x,nb)=='M') && (mapnb(map,y,x,(nb+4)%8)=='S') &&
					   (mapnb(map,y,x,(nb+2)%8)=='S') && (mapnb(map,y,x,(nb+6)%8)=='M')) count++;
				}
			}
		}
	}

	printf("%d\n",count/2); // The search does a full circle, hence every X is counted twice

	return 0;
}
