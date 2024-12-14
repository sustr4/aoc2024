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

// Point structure definition
typedef struct {
	int x;
	int y;
	int dx;
	int dy;
} TRobot;

// Print a two-dimensional array
int countMap (TRobot *robot) {
	int sum=0;
	int x,y,r;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			for(r=0;r<MAXR;r++) {
				if((y==robot[r].y)&&(x==robot[r].x)) {
					sum++;
					break;
				}
			}
		}
	}
	return sum;
}
// Print a two-dimensional array
int printMap (TRobot *robot) {
	int sum=0;
	int x,y,r;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			int dr=0;
			for(r=0;r<MAXR;r++) {
				if((y==robot[r].y)&&(x==robot[r].x)) {
					sum++;
					dr=1;
					break;
				}
			}
			if(dr) printf("█");
			else printf(" ");
		}
		printf("\n");
	}
	return sum;
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

int main(int argc, char *argv[]) {

	TRobot *robot;
	int i=0;	
	robot = readInput();

	int sec=0;
	int r=0;
	int fut=0;
	for(sec=0; fut<1; sec++) {
		for(i=0; i<MAXR; i++) {
			robot[i].x=(robot[i].x+robot[i].dx);
			while(robot[i].x>=MAXX) robot[i].x-=MAXX;
			while(robot[i].x<0) robot[i].x+=MAXX;
			robot[i].y=(robot[i].y+robot[i].dy);
			while(robot[i].y>=MAXY) robot[i].y-=MAXY;
			while(robot[i].y<0) robot[i].y+=MAXY;
		}
		r=countMap(robot);
		if(r==500) {
			printMap(robot);
			printf("Robots: %d, Seconds: %d\n", r, sec+1);
			fut++;
		}
	}

	return 0;
}
