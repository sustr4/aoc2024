#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 71
#define MAXY 71
#define TASK1 1024
#define MAXB 3450
//#define INPUT "unit1.txt"
//#define MAXX 7
//#define MAXY 7
//#define TASK1 12

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TByte;

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
TByte *readInput(char **map) {
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
	TByte *inst=(TByte*)calloc(MAXB, sizeof(TByte));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into array
		sscanf(line,"%d,%d",
			&(inst[count].x),
			&(inst[count].y));
		if(count<TASK1) map[inst[count].y][inst[count].x]='#';

		count++;
	}

	fclose(input);
	if (line)
	free(line);


	return inst;
}

int main(int argc, char *argv[]) {

	TByte *array;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));
//	int i=0;	
	int y, x, n;
	array = readInput(map);
	for(y=0; y<MAXY; y++)
		for(x=0; x<MAXX; x++) if(!map[y][x]) map[y][x]=' ';

	int **dist=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) dist[iter]=calloc(MAXX,sizeof(int));
	dist[0][0]=1;

	int step=1;
	while(1) {
		int change=0;
		for(y=0; y<MAXY; y++) {
			for(x=0; x<MAXX; x++) {
				if(dist[y][x]!=step) continue;
				for(n=1; n<8; n+=2) {
					if((mapnb(map, y, x, n)==' ') &&
					  (dist[y+dy[n]][x+dx[n]])==0) {
						printf("Stepping to [%d,%d] in step %d, direction %d\n", y+dy[n], x+dx[n], step+1, n);
						dist[y+dy[n]][x+dx[n]]=step+1;
						change++;
					}
				}
			}
		}
		if(!change) break;
		printf("%d changes at step %d\n", change, step);
		step++;
	}
	printMap(map,dist);
	printf("Distance :%d\n", dist[MAXY-1][MAXX-1]-1);
//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	return 0;
}
