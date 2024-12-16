#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 141
#define MAXY 141
//#define INPUT "unit1.txt"
//#define MAXX 17
//#define MAXY 17

// Point structure definition
typedef struct {
	int x;
	int y;
} TPoint;

// Print a two-dimensional array
void printMap (char **map, int **visited) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(visited[y][x]) printf("%d",visited[y][x]%10);
			else printf("%c", map[y][x]);
		}
		printf("\n");
	}
}

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

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
	char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int step(char **map, int ***cost, int y, int x, int n, int sum, TPoint E) {

	if(cost[y][x][n]&&(sum > cost[y][x][n])) return 0; // Been here for better price before
	else cost[y][x][n]=sum;

	if((y==E.y)&&(x==E.x)) return 1; // Reached the goal

	if(map[y+dy[n]][x+dx[n]]!='#') // Forward
		step(map, cost, y+dy[n], x+dx[n], n, sum+1, E);

	int nn=n-2; if(nn<0) nn+=8; // Turn left
	if(map[y+dy[nn]][x+dx[nn]]!='#')
		step(map, cost, y+dy[nn], x+dx[nn], nn, sum+1001, E);
	
	nn=n+2; if(nn>=8) nn-=8; // Turn right
	if(map[y+dy[nn]][x+dx[nn]]!='#')
		step(map, cost, y+dy[nn], x+dx[nn], nn, sum+1001, E);

	return 1;
}

int hop(char **map, int ***cost, int **visited, TPoint *stack, int y, int x, int n, int sum, TPoint E, int depth, int max) {
	static int **revisit=NULL;

	if(!revisit) {
		revisit=calloc(MAXY,sizeof(int*));
		for(int i=0; i<MAXY; i++) revisit[i]=calloc(MAXX,sizeof(int));
	}

	if(revisit[y][x]) return 0;

	if(cost[y][x][n]&&(sum > cost[y][x][n])) return 0; // Been here for better price before
	if(sum>max) return 0; //
	stack[depth].x=x;
	stack[depth].y=y;

	revisit[y][x]=1;

	if((y==E.y)&&(x==E.x)) {
		visited[y][x]=1;
		for(int i=0; i<depth; i++) visited[stack[i].y][stack[i].x]++;
	}

	if(map[y+dy[n]][x+dx[n]]!='#') // Forward
		hop(map, cost, visited, stack, y+dy[n], x+dx[n], n, sum+1, E, depth+1, max);

	int nn=n-2; if(nn<0) nn+=8; // Turn left
	if(map[y+dy[nn]][x+dx[nn]]!='#')
		hop(map, cost, visited, stack, y+dy[nn], x+dx[nn], nn, sum+1001, E, depth+1, max);
	
	nn=n+2; if(nn>=8) nn-=8; // Turn right
	if(map[y+dy[nn]][x+dx[nn]]!='#')
		hop(map, cost, visited, stack, y+dy[nn], x+dx[nn], nn, sum+1001, E, depth+1, max);

	revisit[y][x]=0;

	return 1;
}

int main(int argc, char *argv[]) {

	char **map=readInput();
	int ***cost=calloc(MAXY,sizeof(int**));
	for(int i=0; i<MAXY; i++) {
		cost[i]=calloc(MAXX,sizeof(int*));
		for(int j=0;j<MAXX;j++) cost[i][j]=calloc(8,sizeof(int));
	}
	int **visited=calloc(MAXY,sizeof(int*));
	for(int i=0; i<MAXY; i++) visited[i]=calloc(MAXX,sizeof(int));
	TPoint *stack=calloc(19881,sizeof(TPoint));

	TPoint S,E; S.x=0; S.y=0; E.x=0; E.y=0;

	for(int y=0; y<MAXY; y++)
		for(int x=0; x<MAXX; x++) {
			if(map[y][x]=='S') { S.x=x; S.y=y; map[y][x]='.'; }
			if(map[y][x]=='E') { E.x=x; E.y=y; map[y][x]='.'; }
		}
	
	step(map, cost, S.y, S.x, 3, 0, E);

	int min=INT_MAX;
	for(int i=0;i<8;i++) {
		if((cost[E.y][E.x][i]>0)&&(cost[E.y][E.x][i]<min)) min=cost[E.y][E.x][i];
	}
	printf("Minimum cost: %d\n",min);
	
	hop(map, cost, visited, stack, S.y, S.x, 3, 0, E, 0, min);
	int cnt=0;
	for(int y=0; y<MAXY; y++)
		for(int x=0; x<MAXX; x++)
			if(visited[y][x]) cnt++;
	printf("Count: %d\n", cnt);

	return 0;
}
