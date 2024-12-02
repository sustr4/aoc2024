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
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

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
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");

// Retrieve nth neighbor from a map
int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1};
int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1};
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<8));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
}

// Read input file line by line (e.g., into an array)
int **readInput() {
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

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	int **map=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(int));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

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

//	printMap(map);

	return map;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
	int y=0,x;
	int** array;
	array = readInput();
	int count = 0;
	int dir;
	int safe;

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(y=0; array[y][0]; y++) {
		safe = 1;
		dir = array[y][1] - array[y][0];
			printf(" (%d)", dir);
		for(x=0; array[y][x+1]; x++) {
			printf("%d ", array[y][x]);
			int curdir = array[y][x+1] - array[y][x];
			printf(" (%d)", curdir);
			if((curdir>0)&&(dir<0)) safe = 0;
			if((curdir<0)&&(dir>0)) safe = 0;
			if((abs(array[y][x+1]-array[y][x])>3) ||
			   (abs(array[y][x+1]-array[y][x])<1)) safe=0;
		}
		if(safe) {
			printf("*");
			count++;
		}
		printf("\n");
	}

	printf("%d\n", count);
	return 0;
}
