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
//#define INPUT "unit1.txt"
//#define MAXX 15
//#define MAXY 15

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

// Comparator function example
int comp(const void *a, const void *b)
{
	const int *da = (const int *) a;
	const int *db = (const int *) b;
	return (*da > *db) - (*da < *db);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);


// Print a two-dimensional array
void printMap (char **map, int **dist) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(dist && dist[y][x]) printf("%d", dist[y][x]%10);
			else printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");

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

int count(char **map, TPoint S, TPoint E) {
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

	int ret=dist[E.y][E.x]-1;
	for(int iter=0; iter<MAXY; iter++) free(dist[iter]);
	free(dist);

	return(ret);	
}

int main(int argc, char *argv[]) {

	TPoint S,E; S.x=0; S.y=0; E.x=0; E.y=0;
	int x,y;
//	int i=0;	
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

	int full = count(map, S, E);
	printf("Full duration: %d\n", full);

	int sum=0;
	for(y=1; y<MAXY-1; y++) {
		printf("starting row %d\n", y);
		for(x=1; x<MAXX-1; x++) {
			if(map[y][x]!='#') continue;
			map[y][x]='.';
			int c=count(map, S, E);
//			printf("Shortcut saves %d.\n", full-c);
			if(full-c>=100) sum++;
			map[y][x]='#';
		}
	}

	printf("%d cheats save over 100 ps\n",sum);
//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	return 0;
}
