#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 450
#define MAXY 401
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
char **readInput(char **req) {
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
	char **twl=(char**)calloc(MAXX, sizeof(char*));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	// char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		if(count>1)
			asprintf(&(req[count-2]), "%s", line);	

		int t=0;
		if(count==0) {// Read tokens from single line
			char *token;
			token = strtok(line, ", ");
			asprintf(&(twl[t++]), "%s", token);	
			while( 1 ) {
				if(!(token = strtok(NULL, ", "))) break;
				asprintf(&(twl[t++]), "%s", token);
			}
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return twl;
}

int check(char *string, char **twl) {
	if(string[0]==0) return 1;

	for(int i=0; twl[i]; i++) {
//		printf("Trying %s on %s (%ld char)", twl[i], string, strlen(twl[i]));
		if(strncmp(string,twl[i],strlen(twl[i]))) {
//			printf("\n");
			continue; }
	
//		printf(" FIT\n");
		
		if(check(string+strlen(twl[i]), twl)) return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
	int i=0;	
//	array = readInput();
	char **req=(char**)calloc(MAXY, sizeof(char*));
	char **twl=readInput(req);

//	for(i=0; twl[i]; i++) printf("%s\n", twl[i]);

	int sum=0;
//	#pragma omp parallel for shared(sum)
	for(i=0; i<MAXY; i++) {
		if(!req[i]) continue;
		sum+=check(req[i], twl);
		printf("%3d %s\n", sum, req[i]);
	}
	printf("%d\n",sum);


	return 0;
}
