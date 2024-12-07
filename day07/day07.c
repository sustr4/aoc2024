#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
//#define INPUT "unit2.txt"
#define MAXX 100
#define MAXY 1000
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

int *check;

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
void printArray (long long **array) {
	int x,y;
	for(y=0; array[y][0]; y++) {
		for(x=0; array[y][x]; x++) {
			printf("%lld", array[y][x]);
			if(!x) printf(": ");
			else if(array[y][x+1]) printf(" ");
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
long long **readInput() {
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
	int x=0;
	long long **map=calloc(MAXY,sizeof(long long*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(long long));
	check=calloc(MAXY,sizeof(long long));

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
		token[strlen(token)]=0;
		map[count][0]=atoll(token);
		x=1;
		while( 1 ) {
			if(!(token = strtok(NULL, " "))) break;
			map[count][x++]=atoll(token);
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

//	printMap(map);

//	return inst;
	return map;
}

int try(long long **formula, int line, int depth, long long sum) {
	if(!formula[line][depth]) { // The calculation should be finished
//		printf("%lld == %lld?\n", formula[line][0], sum);
		if(formula[line][0]==sum) {
			check[line]++;
			return 1;
		}
		return 0;
	}
	if(sum>formula[line][0]) return 0; // Grew too big
	if(try(formula, line, depth + 1, sum + formula[line][depth])) return 1;
	if(try(formula, line, depth + 1, sum * formula[line][depth])) return 1;
	return 0;
}

int main(int argc, char *argv[]) {

	int i=0;	
	long long **array = readInput();

//	printArray(array);

	long long sum=0;
	for(i=0; array[i][0]; i++) {
		try(array, i, 2, array[i][1]);
		printf("%d: %d", i, check[i]);
		if(check[i]) {
			sum+=array[i][0];
			printf("(%lld)", array[i][0]);
		}
		printf("\n");
	}
	printf("Sum %lld\n", sum);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	return 0;
}
