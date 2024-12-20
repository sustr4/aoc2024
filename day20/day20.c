#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 141
#define MAXY 141
#define MINSAVE 100
//#define INPUT "unit1.txt"
//#define MAXX 15
//#define MAXY 15
//#define MINSAVE 50

// Point structure definition
typedef struct {
	int x;
	int y;
} TPoint;

// Print a two-dimensional array
void printMap (char **map, int **dist) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(dist && dist[y][x]) printf("%2d", dist[y][x]%100);
			else printf("%c%c", map[y][x],map[y][x]);
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

int **populateDist(char **map, TPoint S, TPoint E) {
	int x,y;
	int **dist=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) dist[iter]=calloc(MAXX,sizeof(int));
	dist[S.y][S.x]=1;

	int step=1;
	while(1) {
		int change=0;
		for(y=1; y<MAXY-1; y++) {
			for(x=1; x<MAXX-1; x++) {
				if(dist[y][x]!=step) continue;
				for(int n=1; n<8; n+=2) {
					if((mapnb(map,y,x,n)=='.') &&
					   (dist[y+dy[n]][x+dx[n]]==0)) {
						dist[y+dy[n]][x+dx[n]]=step+1;
						change=1;	
					}
				}
			}
		}

		if(!change) break;
		step++;
//		printMap(map, dist);
	}

	return(dist);	
}

int save(char **map, int **dist, int sy, int sx) {

	int fy=sy-20 < 1? 1 : sy-20;
	int fx=sx-20 < 1? 1 : sx-20;
	int ty=sy+20 >= MAXY ? MAXY-1 : sy+20;
	int tx=sx+20 >= MAXX ? MAXX-1 : sx+20;

	int ret=0;
	for(int y=fy; y<=ty; y++) {
		for(int x=fx; x<=tx; x++) {
			int chdist=abs(sx-x)+abs(sy-y);
			if(chdist > 20) continue;
			if(map[y][x]!='.') continue;
			if(dist[y][x]-dist[sy][sx]-chdist >= MINSAVE) ret++;
		}
	}

	return ret;
}

int main(int argc, char *argv[]) {

	TPoint S,E; S.x=0; S.y=0; E.x=0; E.y=0;
	int x,y;
	char **map = readInput();

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]=='S') {
				S.x=x;
				S.y=y;
				map[y][x]='.';
			}
			if(map[y][x]=='E') {
				E.x=x;
				E.y=y;
				map[y][x]='.';
			}
		}
	}

	int **dist = populateDist(map, S, E);
	printf("Full duration: %d\n", dist[E.y][E.x]-1);

	int sum=0;
//	#pragma omp parallel for shared(sum)
	for(int y=1; y<MAXY-1; y++) {
		for(int x=1; x<MAXX-1; x++) {
			if(map[y][x]!='.') continue;
			sum+=save(map, dist, y, x);
		}
	}

	printf("%d cheats save at least %d ps.\n", sum, MINSAVE);

	return 0;
}
