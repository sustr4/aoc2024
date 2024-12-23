#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
int MAXY;
//#define INPUT "unit1.txt"

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
		for(x=0; x<MAXY; x++) {
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
	   (x+dx[n]<0) || (x+dx[n]>=MAXY)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
}

int n2n(char *name) { // Name to number
	return (name[0]-'a')*('z'-'a'+1)+(name[1]-'a');
}

void printn(int n) {
	printf("%c",n/('z'-'a'+1)+'a');
	printf("%c",n%('z'-'a'+1)+'a');
}

// Read input file line by line (e.g., into an array)
int **readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	MAXY=n2n("zz");

	int **map=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXY,sizeof(int));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		int from=n2n(line);
		int to=n2n(line+3);

		map[from][to]=1;
		map[to][from]=1;

	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
//	int i=0;	
//	array = readInput();
	int **map=readInput();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	int sum=0;
	for(int i=0; i<MAXY-2; i++) {
		for(int j=i+1; j<MAXY-1; j++) {
			for(int k=j+1; k<MAXY; k++) {
				if(map[i][j] &&
				   map[j][k] &&
				   map[i][k]) {
					sum++;
					printn(i);
					printf("-");
					printn(j);
					printf("-");
					printn(k);
					printf("\n");
				}
			}
		}
	}

	printf("Sum: %d\n", sum);


	return 0;
}
