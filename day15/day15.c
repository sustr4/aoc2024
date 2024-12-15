#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 50
#define MAXY 50
#define MAXM 20000
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int x;
	int y;
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
void printMap (char **map, TPoint rob) {
	int x,y;
	long long sum=0;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if((y==rob.y)&&(x==rob.x)) printf("\033[1;31m@\033[0m");
			else printf("%c", map[y][x]);
			if(map[y][x]=='O') sum+=y*100+x;
		}
		printf("\n");
	}
	printf("GPS sum: %lld\n", sum);
}
// Full block character for maps █ and border elements ┃━┗┛┏┓
// Color printf("\033[1;31mR \033[1;32mG \033[1;34mB \033[0moff\n");

// Retrieve nth neighbor from a map, diagonals are odd, side neighbors even
int dy[] = { -1, -1, -1, 0, 1, 1,  1,  0};
int dx[] = { -1,  0,  1, 1, 1, 0, -1, -1};
char* trans;
char mapnb(char **map, int y, int x, int n) {
	assert((n>=0) && (n<8));
	if((y+dy[n]<0) || (y+dy[n]>=MAXY) ||
	   (x+dx[n]<0) || (x+dx[n]>=MAXX)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
}

// Read input file line by line (e.g., into an array)
char **readInput(char *mov) {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0, m=0;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));
	trans=calloc(256, sizeof(char));
	trans['<']=7; trans['>']=3; trans['^']=1; trans['v']=5;

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<1) continue;

		if(line[0]=='#') { // Read into map
			for(x=0; x<MAXX; x++) map[y][x] = line[x];
			y++;
			continue;
		}

		for(int i=0; i<strlen(line); i++) mov[m++]=line[i];
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}


TPoint move(char **map, TPoint rob, int mov) {
	int y, x;

	//try move
	y=rob.y, x=rob.x;
	int isFree=0;
	while(map[y][x]!='#') {
		y+=dy[mov];
		x+=dx[mov];
		if(map[y][x]=='.') {
			isFree=1;
			break; } }
	if(!isFree) return rob;
	printf("Path towards %d is free\n", mov);

	y=rob.y+dy[mov], x=rob.x+dx[mov];
	int crates=0;
	while(map[y][x]!='.') {
		y+=dy[mov];
		x+=dx[mov]; 
		crates++;}
	printf("%d crates in the way\n", crates);

	while(crates) {
		map[y][x]=map[y-dy[mov]][x-dx[mov]];
		y-=dy[mov];
		x-=dx[mov]; 
		map[y][x]='.';
		crates--;
	}

	TPoint ret=rob;
	ret.x+=dx[mov];
	ret.y+=dy[mov];
	return ret;
}

int main(int argc, char *argv[]) {

	int i=0;	
	char *mov = (char*)calloc(MAXM,sizeof(char));
	char **map = readInput(mov);
	TPoint rob;

	for(rob.y=0; rob.y<MAXY; rob.y++)
		for(rob.x=0; rob.x<MAXX; rob.x++)
			if(map[rob.y][rob.x]=='@') goto foundrob;

foundrob:
	map[rob.y][rob.x]='.';		
			

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; i<MAXM; i++) {
		rob=move(map, rob, trans[(int)mov[i]]);
	}
	printMap(map, rob);

	return 0;
}
