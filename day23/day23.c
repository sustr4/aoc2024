#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
int MAXY;
//#define INPUT "unit1.txt"

// Point structure definition
typedef struct {
	int no;
	int cnt;
} TComp;

// Comparator function example
int comparator(const void *a, const void *b)
{
	const TComp *cA = (const TComp *)a;
	const TComp *cB = (const TComp *)b;

	return cB->cnt - cA->cnt;

}

int n2n(char *name) { // Name to number
	return (name[0]-'a')*('z'-'a'+1)+(name[1]-'a');
}

void printn(int n) {
	printf("%c",n/('z'-'a'+1)+'a');
	printf("%c",n%('z'-'a'+1)+'a');
}

// Print a two-dimensional array
void printMesh (int **mesh, int *label, int maxm) {
	int x,y;
	for(y=0; y<maxm; y++) {
		printn(label[y]); printf(" (%3d): ", label[y]);
		for(x=0; x<maxm; x++) {
			if(mesh[y][x]) printf("%3d", mesh[y][x]);
			else printf("\033[1;31m%3d\033[0m", mesh[y][x]);
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
	   (x+dx[n]<0) || (x+dx[n]>=MAXY)) return 0;
	return(map[y+dy[n]][x+dx[n]]);
}

// Read input file line by line (e.g., into an array)
int **readInput(TComp *comp) {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }


	int **map=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXY,sizeof(int));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		int from=n2n(line);
		int to=n2n(line+3);

		map[from][to]=1;
		map[to][from]=1;

		comp[from].no=from;
		comp[from].cnt++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int next(int *elem, int *from, int *to, int size) {

	elem[size-1]++;
	for(int j=size-1; j>0; j--)
		if(elem[j]>to[j]) {
			elem[j-1]++;
			elem[j]=from[j-1];
		}
	if(elem[0]>to[0]) return 0;

	for(int j=0; j<size-1; j++)
		if(elem[j+1]<=elem[j]) elem[j+1]=elem[j]+1;

	return 1;
}

int meshConnections(int e, int *elem, int size, int **map) {
	int sum=0;
	for(int i=0; i<size; i++) 
		if(map[elem[e]][elem[i]]) sum++;

//	printf("Connections %d: %d\n", e, sum);
	return sum;
}

int main(int argc, char *argv[]) {

//	TComp *array;
//	int i=0;	
//	array = readInput();
	MAXY=n2n("zz");
	TComp *comp=calloc(MAXY,sizeof(TComp));
	int **map=readInput(comp);
//	int *from, *to, *elem;

	// Try starting from the computer with the most connections.
	qsort(comp,MAXY,sizeof(TComp),comparator);

	printf("Most connected "); printn(comp[0].no); printf(" (%d): %d\n", comp[0].no, comp[0].cnt);


	int fromtop=24;
	int with=comp[fromtop].no;

	int maxm=comp[0].cnt+2;
	int **mesh=calloc(maxm,sizeof(int*));
	for(int iter=0; iter<maxm; iter++) mesh[iter]=calloc(maxm,sizeof(int));

	int *label=calloc(maxm, sizeof(int));

	int k=0;
	for(int j=0; j<MAXY; j++) {
		if((map[j][with])||(j==with)) label[k++]=j;
	}
	for(int i=0; i<maxm; i++) {
		mesh[i][i]=1;
		for(int j=i+1; j<maxm; j++) {
			if(map[label[i]][label[j]]) {
				mesh[i][j]=1;
				mesh[j][i]=1;
			}
		}
	}

	
	
	printMesh(mesh, label, maxm);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)

/*
	for(int size=3; size<MAXY; size++) {
		printf("Trying size %d\n",size);
		from=calloc(size,sizeof(int));
		to=calloc(size,sizeof(int));
		elem=calloc(size,sizeof(int));

		for(int i=0; i<size; i++) {
			from[i]=i;
			to[i]=MAXY-size+i+1;
			elem[i]=from[i];
		}

		while(1) {

			int line=1;
			for(int j=0; j<size-1; j++)
				if(!map[elem[j]][elem[j+1]]) {
					line = 0;
					break;
				}

			if(line) { // Don't even try otherwise

				int mesh=1;

				for(int j=0; j<size; j++) {
					if(meshConnections(j, elem, size, map)!=size-1) {
						mesh=0;
						break;
					}
				}

				if(mesh) {
					for(int i=0; i<size; i++) {
						printn(elem[i]);
						if(i!=size-1) printf(",");
					}
					printf("\n");
				}

			}

			if(!next(elem, from, to, size)) break;
		}
		free(from);
		free(to);
		free(elem);
	}
*/
	return 0;
}
