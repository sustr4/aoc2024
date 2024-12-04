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

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		//Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

		// Read tokens from single line
		//char *token;
		//token = strtok(line, ",");
		//while( 1 ) {
		//	if(!(token = strtok(NULL, ","))) break;
		//}

		count++;
	}
	printMap(map);
	printf("\n");

	fclose(input);
	if (line)
	free(line);

	return map;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
//	int i=0;	
//	array = readInput();
	char **map=readInput();

	printMap(map);
	printf("\n");

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	int count=0;
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			int oldc=count;
			if((x<MAXX-3) && (map[y][x]=='X') && (map[y][x+1]=='M') && (map[y][x+2]=='A') && (map[y][x+3]=='S')) count++;
			if((x>2) && (map[y][x]=='X') && (map[y][x-1]=='M') && (map[y][x-2]=='A') && (map[y][x-3]=='S')) count++;
			if((y<MAXY-3) && (map[y][x]=='X') && (map[y+1][x]=='M') && (map[y+2][x]=='A') && (map[y+3][x]=='S')) count++;
			if((y>2) && (map[y][x]=='X') && (map[y-1][x]=='M') && (map[y-2][x]=='A') && (map[y-3][x]=='S')) count++;

			if(((x<MAXX-3)&&(y<MAXY-3)) && (map[y][x]=='X') && (map[y+1][x+1]=='M') && (map[y+2][x+2]=='A') && (map[y+3][x+3]=='S')) count++;
			if(((x>2)&&(y>2)) && (map[y][x]=='X') && (map[y-1][x-1]=='M') && (map[y-2][x-2]=='A') && (map[y-3][x-3]=='S')) count++;
			if(((x<MAXX-3)&&(y>2)) && (map[y][x]=='X') && (map[y-1][x+1]=='M') && (map[y-2][x+2]=='A') && (map[y-3][x+3]=='S')) count++;
			if(((x>2)&&(y<MAXY-3)) && (map[y][x]=='X') && (map[y+1][x-1]=='M') && (map[y+2][x-2]=='A') && (map[y+3][x-3]=='S')) count++;

			if(oldc==count) printf("\033[0m");
			else printf("\033[1;31m");
			printf("%c",map[y][x]);
		}
		printf("\n");
	}

	printf("%d\n",count);
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	return 0;
}
