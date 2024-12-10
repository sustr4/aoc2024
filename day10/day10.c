#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 45
#define MAXY 45
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

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

// Retrieve nth neighbor from a map, diagonals are odd, side neighbors even
int dy[] = { -1, -1, -1, 0, 1, 1,  1,  0};
int dx[] = { -1,  0,  1, 1, 1, 0, -1, -1};
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<8));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
}

// Read input file line by line (e.g., into an array)
char **readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

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

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int step(char** map, int y, int x, int* count, char **visited) {
	if(map[y][x]=='9') {
		*count=*count+1;
		visited[y][x]=1;
		return 1;
	}

	int n;
	for(n=1; n<8; n+=2)
		if(mapnb(map, y, x, n)==map[y][x]+1)
			step(map, y+dy[n], x+dx[n], count, visited);
	return 0;
}

int main(int argc, char *argv[]) {
	char **map=readInput();
	int x,y,sum=0, altsum=0;

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]=='0') {
				char **visited=calloc(MAXY,sizeof(char*));
				for(int iter=0; iter<MAXY; iter++) visited[iter]=calloc(MAXX,sizeof(char));
				int count=0,altcount=0;
				step(map, y, x, &count, visited);
				for(int i=0; i<MAXY; i++)
					for(int j=0; j<MAXX; j++)
						if(visited[i][j]) altcount++;
				sum+=count;
				altsum+=altcount;
				for(int iter=0; iter<MAXY; iter++) free(visited[iter]);
				free(visited);
			}
		}
	}

	printf("Sum Task 1: %d\nSum Task 2: %d\n", altsum, sum);
	return 0;
}
