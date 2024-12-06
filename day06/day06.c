#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 130
#define MAXY 130
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
} TPoint;
int dir;

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

void printVis (char ***map, char **m) {
	int x,y;
	printf("\n");
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			int cnt=0;
			for(int i=0;i<8;i++) if(map[y][x][i]) cnt++;
			if(map[y][x]) printf("%d",cnt);
			else printf(" ");
		}
		printf("  ");
		for(x=0; x<MAXX; x++) {
			printf("%c", m[y][x]);
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

		//Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;
	}

	fclose(input);
	if (line)
	free(line);

//	printMap(map);

	return map;
}

TPoint move(TPoint guard, char **map, char ***visited) {
	TPoint ret=guard;

        if((guard.x<0) || (guard.x>=MAXX) ||
           (guard.y<0) || (guard.y>=MAXY)) {
		ret.x=-2;
		ret.y=-2;
		return ret;
	}

	if(visited[guard.y][guard.x][dir]) {
		ret.x=-1;
		ret.y=-1;
		return ret;
	}


	visited[guard.y][guard.x][dir]=1;
	if(mapnb(map,guard.y,guard.x,dir)!='#') {
		ret.x+=dx[dir];
		ret.y+=dy[dir]; }
	else {
		dir=(dir+2)%8;
	}
	return ret;
}

int main(int argc, char *argv[]) {

	char **map=readInput();
	int x,y,count=0;
	TPoint guard={ -1, -1};
	char ***visited=calloc(MAXY,sizeof(char**));
	for(int iter=0; iter<MAXY; iter++) {
		visited[iter]=calloc(MAXX,sizeof(char*));
		for(int iiter=0; iiter<MAXX; iiter++) {
			visited[iter][iiter]=calloc(8,sizeof(char));
		}
	}

	for(y=0; y<MAXY; y++)
		for(x=0; x<MAXX; x++)
			if(map[y][x]=='^') {
				guard.y=y;
				guard.x=x; 
				map[y][x]='.'; }

	TPoint orig=guard;

	TPoint obs;
	for(obs.y=0; obs.y<MAXY; obs.y++) {
		for(obs.x=0; obs.x<MAXX; obs.x++) {
			guard=orig; dir=1;
			if(map[obs.y][obs.x]=='#') continue;
			map[obs.y][obs.x]='#';
			while(1) {
				guard=move(guard,map,visited);
				if(guard.x<0) break;
			}
			if(guard.y==-1) {
				count++;
				//printVis(visited, map);
			}
			map[obs.y][obs.x]='.';
			for(int iter=0; iter<MAXY; iter++) {
				for(int iiter=0; iiter<MAXX; iiter++) {
					memset(visited[iter][iiter],0,8*sizeof(char));
				}
			}
		}
	}

	printf("Count: %d\n", count);

	return 0;
}
