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
#define MAXY 26
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
} TPoint;

char numkb[4][3] = {
	{ '7', '8', '9' },
	{ '4', '5', '6' },
	{ '1', '2', '3' },
	{ ' ', '0', 'A' }};

char arrkb[2][3] = {
	{ ' ', '^', 'A' },
	{ '<', 'v', '>' }};

TPoint **numKeyToKey;
TPoint **arrKeyToKey;

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
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	char **inst=(char**)calloc(MAXX, sizeof(char*));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Copy to string
		asprintf(&(inst[count]), "%s", line);	

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

void initKK() {

	numKeyToKey=calloc('A'+1, sizeof(TPoint*));
	arrKeyToKey=calloc('A'+1, sizeof(TPoint*));

	// Numeric
	for(int fy=0; fy<4; fy++) {
		for(int fx=0; fx<3; fx++) {
			char from=numkb[fy][fx];
			numKeyToKey[(int)from]=calloc('A'+1, sizeof(TPoint));
			for(int ty=0; ty<4; ty++) {
				for(int tx=0; tx<3; tx++) {
					char to=numkb[ty][tx];
					numKeyToKey[(int)from][(int)to].y = ty-fy;
					numKeyToKey[(int)from][(int)to].x = tx-fx;
//					printf("%c to %c: %d,%d\n", from, to, (numKeyToKey[(int)from][(int)to]).x,  numKeyToKey[(int)from][(int)to].y);
				} } } }
	// Directional arrows:
	for(int fy=0; fy<2; fy++) {
		for(int fx=0; fx<3; fx++) {
			char from=arrkb[fy][fx];
			arrKeyToKey[(int)from]=calloc('A'+1, sizeof(TPoint));
			for(int ty=0; ty<2; ty++) {
				for(int tx=0; tx<3; tx++) {
					char to=arrkb[ty][tx];
					arrKeyToKey[(int)from][(int)to].y = ty-fy;
					arrKeyToKey[(int)from][(int)to].x = tx-fx;
//					printf("%c to %c: %d,%d\n", from, to, (arrKeyToKey[(int)from][(int)to]).x,  arrKeyToKey[(int)from][(int)to].y);
				} } } }
}

int main(int argc, char *argv[]) {

//	TPoint *array;
	int i=0;	
//	array = readInput();
	char **numcode = readInput();
	initKK();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; numcode[i]; i++) {
		printf("%s\n", numcode[i]);
	}

	return 0;
}
