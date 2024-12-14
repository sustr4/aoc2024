#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 101
#define MAXY 103
#define MAXR 500
//#define INPUT "unit1.txt"
//#define MAXX 11
//#define MAXY  7
//#define MAXR 12
//#define INPUT "unit2.txt"
//#define MAXX 11
//#define MAXY  7
//#define MAXR  1

// Point structure definition
typedef struct {
	int x;
	int y;
	int dx;
	int dy;
} TRobot;

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
TRobot *readInput() {
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
	TRobot *inst=(TRobot*)calloc(MAXR, sizeof(TRobot));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into array
		sscanf(line,"p=%d,%d v=%d,%d",
			&(inst[count].x),
			&(inst[count].y),
			&(inst[count].dx),
			&(inst[count].dy));

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

int count(TRobot* robot) {
	int i;
	int sum[] = {0,0,0,0};
	for(i=0; i<MAXR; i++) {
		printf("%d, %d\n", robot[i].x, robot[i].y);
		if((robot[i].x<MAXX/2)&&(robot[i].y<MAXY/2)) sum[0]++;
		else if((robot[i].x>MAXX/2)&&(robot[i].y<MAXY/2)) sum[1]++;
		else if((robot[i].x<MAXX/2)&&(robot[i].y>MAXY/2)) sum[2]++;
		else if((robot[i].x>MAXX/2)&&(robot[i].y>MAXY/2)) sum[3]++;
	}
	return sum[0]*sum[1]*sum[2]*sum[3];
}

int main(int argc, char *argv[]) {

	TRobot *robot;
	int i=0;	
	robot = readInput();

//	#pragma omp parallel
	for(i=0; i<MAXR; i++) {
		printf("%d, %d\n", robot[i].x, robot[i].y);
		for(int sec=0;sec<100;sec++) {
			robot[i].x=(robot[i].x+robot[i].dx);
			while(robot[i].x>=MAXX) robot[i].x-=MAXX;
			while(robot[i].x<0) robot[i].x+=MAXX;
			robot[i].y=(robot[i].y+robot[i].dy);
			while(robot[i].y>=MAXY) robot[i].y-=MAXY;
			while(robot[i].y<0) robot[i].y+=MAXY;
			printf("sec %d: %d, %d\n", sec, robot[i].x, robot[i].y);
		}
	}

	printf("Safety factor: %d\n", count(robot));
	return 0;
}
