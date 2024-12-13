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
#define MAXY 321
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
} TPoint;

typedef struct {
	TPoint button[2];
	int cost[2];
	TPoint prize;
} TMachine;


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
TMachine *readInput() {
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
	TMachine *mach=(TMachine*)calloc(MAXY, sizeof(TMachine));
	for(int iter=0; iter<MAXY; iter++) {
		mach[iter].cost[0]=3;
		mach[iter].cost[1]=1;
	}

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) {
			count++;
			continue; }

		switch(line[7]) {
		case 'A':
		case 'B':
			char b;
			sscanf(line,"Button %c: X+%d, Y+%d", &b,
				&(mach[count].button[b-'A'].x),
				&(mach[count].button[b-'A'].y));
			break;
		case 'X':
			sscanf(line,"Prize: X=%d, Y=%d",
				&(mach[count].prize.x),
				&(mach[count].prize.y));
			break;
		}

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

	}

	fclose(input);
	if (line)
	free(line);

//	printMap(map);

	return mach;
}

int main(int argc, char *argv[]) {

	TMachine *array;
	int i=0;	
//	array = readInput();
	array=readInput();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; array[i].prize.x; i++) {
		printf("Button A: X+%d, Y+%d\n", array[i].button[0].x, array[i].button[0].y);
		printf("Button B: X+%d, Y+%d\n", array[i].button[1].x, array[i].button[1].y);
		printf("Prize: X=%d, Y=%d\n\n", array[i].prize.x, array[i].prize.y);


	}

	return 0;
}
