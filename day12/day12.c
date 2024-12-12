#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
//#define INPUT "input.txt"
//#define MAXX 140
//#define MAXY 140
#define INPUT "unit1.txt"
#define MAXX 10
#define MAXY 10

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
void printMap (char **map, int **region) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("  ");
		for(x=0; x<MAXX; x++) {
			printf("%d", region[y][x]%10);
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
int regnb(int **region, int y, int x, int n) {
	assert((n>=0) && (n<8));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX)) return 0;
	return(region[y+dy[n]][x+dx[n]]);
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

int markRegion(char **map, int**region, int y, int x, int r) {
	int change=1;

	region[y][x]=r;
	while(change) {
		change=0;
		for(int ny=0;ny<MAXY;ny++) {
			for(int nx=0;nx<MAXY;nx++) {
				if(region[ny][nx]) continue;
				if(map[ny][nx]!=map[y][x]) continue;
				for(int n=1; n<8; n+=2) {
					if(regnb(region, ny, nx, n)==r) {
						region[ny][nx]=r;
						change=1;
					}
				}
			}
		}
	}
	return r+1;
}

void countRegions(int **region, int *size) {
	int x, y;
	for(y=0;y<MAXY;y++) {
		for(x=0;x<MAXX;x++) {
			size[region[y][x]]++;
		}
	}
}

int main(int argc, char *argv[]) {

	int discover=1,x,y;	
//	array = readInput();
	char **map = readInput();

	int **region=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) region[iter]=calloc(MAXX,sizeof(int));

	for(y=0;y<MAXY;y++) {
		for(x=0;x<MAXX;x++) {
			if(region[y][x]) continue;
//			printf("%d,%d\n", y, x);
			markRegion(map, region, y, x, discover++);
		}
	}

	int *size = calloc(discover+1, sizeof(int));
	countRegions(region, size);

	printMap(map,region);
	for(int i=1; size[i]; i++) printf("Region %3d, size %d\n", i, size[i]);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	return 0;
}
