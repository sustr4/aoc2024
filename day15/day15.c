#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
//#define INPUT "unit1.txt"
#define MAXX 100
#define MAXY 50
#define MAXM 20000

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


// Print a two-dimensional array
void printMap (char **map, TPoint rob, char **needsMove) {
	int x,y;
	long long sum=0;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if((y==rob.y)&&(x==rob.x)) {
				printf("\033[1;31m@\033[0m");
				continue;
			}
			if(needsMove && needsMove[y][x]) printf("\033[1;34m");
			printf("%c", map[y][x]);
			if(map[y][x]=='[') sum+=y*100+x;
			if(needsMove && needsMove[y][x]) printf("\033[0m");
		}
		printf("\n");
	}
	printf("GPS sum: %lld\n", sum);
}

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
	trans['>']=3; trans['<']=7;

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<1) continue;

		if(line[0]=='#') { // Read into map
			for(x=0; x<MAXX; x++) {
				map[y][x*2] = line[x];
				map[y][x*2+1] = line[x];
				if(line[x]=='O') {
					map[y][x*2] = '[';
					map[y][x*2+1] = ']';
				}
				if(line[x]=='@') map[y][x*2+1] = '.';
			}
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

int findNeed(char **map, int y, int x, int mov, char **needsMove) {
	if((map[y][x]!='[')&&(map[y][x]!=']')) return 0;
	if(needsMove[y][x]) return 0;

	needsMove[y][x]=map[y][x];
	printf("[%d,%d] needs move\n", y, x);
	findNeed(map,y+dy[mov],x+dx[mov],mov,needsMove);

	if(map[y][x]==']') findNeed(map, y, x+dx[mov]-1, mov, needsMove);
	if(map[y][x]=='[') findNeed(map, y, x+dx[mov]+1, mov, needsMove);

	return 0;
}

int isBlocked(char **map, int mov, char **needsMove) {
	for(int y=0; y<MAXY; y++) {
		for(int x=0; x<MAXX; x++) {
			if(needsMove[y][x]&&(mapnb(map,y,x,mov)=='#')) {
				printf("Blocked by wall at [%d,%d]\n", y, x);
				return 1;
			}
		}
	}

	return 0;
}

int shift(char **map, int mov, char **needsMove) {
	for(int y=0; y<MAXY; y++)
		for(int x=0; x<MAXX; x++)
			if(needsMove[y][x]) map[y][x]='.';
	for(int y=0; y<MAXY; y++) {
		for(int x=0; x<MAXX; x++) {
			if(needsMove[y][x]) {
				printf("moving [%d,%d] (%c) to [%d,%d]\n", y, x, needsMove[y][x],
					y+dy[mov], x+dx[mov]);
				map[y+dy[mov]][x+dx[mov]]=needsMove[y][x];
			}
		}
	}
	return 0;
}

TPoint move(char **map, TPoint rob, int mov) {
	TPoint ret=rob;
	char **needsMove=NULL;
	//try move
	assert((mov==1)||(mov==3)||(mov==5)||(mov==7));
	if(mapnb(map,rob.y,rob.x,mov)=='#') goto cleanup; // Don't hit wall

	needsMove=calloc(MAXY,sizeof(char*));
	for(int i=0;i<MAXY;i++) needsMove[i]=calloc(MAXX,sizeof(char));

	findNeed(map, rob.y+dy[mov], rob.x+dx[mov], mov, needsMove);

	if(isBlocked(map, mov, needsMove)) goto cleanup;
	printf("Path towards %d is free\n", mov);
//	printMap(map, rob, needsMove);

	shift(map, mov, needsMove);

	ret.y+=dy[mov];
	ret.x+=dx[mov];
cleanup:
	for(int i=0;i<MAXY;i++) if(needsMove) free(needsMove[i]);
	if(needsMove) free(needsMove);

	printf("Afterwards the robot is at [%d,%d]\n", ret.y, ret.x);
	
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
	printf("Robot starts at [%d,%d]\n", rob.y, rob.x);
			
	trans['>']=3; trans['<']=7;
	for(i=0; i<MAXM; i++) {
		rob=move(map, rob, trans[(int)mov[i]]);
	}
//	printMap(map, rob);
//	printf("Directions test: %d %d %d %d\n", trans['^'], trans['>'], trans['v'], trans['<']);

	printMap(map, rob, NULL);

	return 0;
}
