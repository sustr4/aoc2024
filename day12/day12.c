#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 140
#define MAXY 140
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Print a two-dimensional array
void printMap (char **map, int **region, int ***fence) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("  ");
		for(x=0; x<MAXX; x++) {
			printf("%d", region[y][x]%10);
		}
		printf("  ");
		for(x=0; x<MAXX; x++) {
			if(fence[y][x][7]) printf("┃");
			else printf(" ");
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

void countFences(char **map, int ***fence) {
	int x, y;
	for(y=0;y<MAXY;y++) {
		for(x=0;x<MAXX;x++) {
			for(int n=1; n<8; n+=2) {
				if(mapnb(map,y,x,n)!=map[y][x]) fence[y][x][n]=1;
			}
		}
	}
}

void reduceFences(char **map, int ***fence) {
	int x, y;
	// Left to right
	for(y=0;y<MAXY;y++) {
		for(x=MAXX-1;x>0;x--) {
			if((map[y][x]==map[y][x-1]) &&
			    fence[y][x-1][1]) fence[y][x][1]=0;
			if((map[y][x]==map[y][x-1]) &&
			    fence[y][x-1][5]) fence[y][x][5]=0;
		}
	}

	// Top to bottom
	for(x=0;x<MAXX;x++) {
		for(y=MAXY-1;y>0;y--) {
			if((map[y][x]==map[y-1][x]) &&
			    fence[y-1][x][3]) fence[y][x][3]=0;
			if((map[y][x]==map[y-1][x]) &&
			    fence[y-1][x][7]) fence[y][x][7]=0;
		}
	}
	
}

int worth(int *size, int **region, int ***fence) {
	int sum=0;
	int x, y;
	for(y=0;y<MAXY;y++)
		for(x=0;x<MAXX;x++)
			for(int n=1; n<8; n+=2) 
				sum+=size[region[y][x]]*fence[y][x][n];
	return sum;
}

int sides(int r, int **region, int ***fence) {
	int sum=0;
	int x, y;
	for(y=0;y<MAXY;y++)
		for(x=0;x<MAXX;x++) {
			if(region[y][x]!=r) continue;
			for(int n=1; n<8; n+=2) 
				sum+=fence[y][x][n];
		}
	return sum;
}

int main(int argc, char *argv[]) {

	int discover=1,x,y;	
//	array = readInput();
	char **map = readInput();

	int **region=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) region[iter]=calloc(MAXX,sizeof(int));

	int ***fence=calloc(MAXY,sizeof(int**));
	for(int iter=0; iter<MAXY; iter++) {
		fence[iter]=calloc(MAXX,sizeof(int*));
		for(int iter2=0; iter2<MAXX; iter2++) fence[iter][iter2]=calloc(8,sizeof(int*));
	}

	for(y=0;y<MAXY;y++) {
		for(x=0;x<MAXX;x++) {
			if(region[y][x]) continue;
//			printf("%d,%d\n", y, x);
			markRegion(map, region, y, x, discover++);
		}
	}

	int *size = calloc(discover+1, sizeof(int));
	countRegions(region, size);

	countFences(map, fence);

//	printMap(map,region, fence);
//	for(int i=1; size[i]; i++) printf("Region %3d, size %d\n", i, size[i]);

	printf("Total price  %d\n", worth(size, region, fence));

	reduceFences(map, fence);
//	printMap(map,region, fence);
//	for(int i=1; size[i]; i++) printf("Region %3d  %d * %d = %d\n", i, size[i], sides(i, region, fence), sides(i, region, fence) * size[i]);
	printf("Reduced price %d\n", worth(size, region, fence));

	return 0;
}
