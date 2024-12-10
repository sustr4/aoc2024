#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 45
#define MAXY 45
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

int altsum=0;
int hx[10], hy[10];

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
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
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

	fclose(input);
	if (line)
	free(line);

//	printMap(map);

	return map;
}

int step(char** map, int y, int x, int* count, char **visited) {
	hx[map[y][x]-'0']=x;
	hy[map[y][x]-'0']=y;
	if(map[y][x]=='9') {
		*count=*count+1;
		visited[y][x]=1;
//		for(int i=0; i<=9; i++) printf("[%d,%d] ", hy[i], hx[i]);
//		printf("\n");
		return 1;
	}

	int n;
	for(n=1; n<8; n+=2) {
		if(mapnb(map, y, x, n)==map[y][x]+1) {
//			printf("Stepping from [%d,%d] (%c) to [%d,%d].\n",
//				y, x, map[y][x],
				
			step(map, y+dy[n], x+dx[n], count, visited);
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
//	int i=0;	
//	array = readInput();
	char **map=readInput();

	int x,y,sum=0;


	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]=='0') {
				char **visited=calloc(MAXY,sizeof(char*));
				for(int iter=0; iter<MAXY; iter++) visited[iter]=calloc(MAXX,sizeof(char));
				int count=0,altcount=0;
				step(map, y, x, &count, visited);
				for(int i=0; i<MAXY; i++)
					for(int j=0; j<MAXX; j++)
						if(visited[i][j]) altcount++;
				altsum+=altcount;
				printf("[%d,%d]: %d (%d)\n", y, x, count, altcount);
				for(int iter=0; iter<MAXY; iter++) free(visited[iter]);
free(visited);
			}
		}
	}
	

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	printf("Sum: %d (%d)\n", sum, altsum);
	return 0;
}
