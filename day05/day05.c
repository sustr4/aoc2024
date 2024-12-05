#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 100
#define MAXY 1200
//#define INPUT "unit.txt"
//#define MAXX 10
//#define MAXY 10

// Rule structure definition
typedef struct {
	int need;
	int want;
} TRule;

int **prints;
TRule *rule;
int **needs;

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
int readInput() {
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
	//char **rule=(char**)calloc(MAXX, sizeof(char*));
	rule=(TRule*)calloc(MAXY, sizeof(TRule));

	// Allocate a two-dimensional arrray of ints
	prints=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) prints[iter]=calloc(MAXX,sizeof(int));

	int rules=1;

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		if(strlen(line)<1) {
			count=0;
			rules=0;
			continue;
		}

		if(rules) {
			// Read into array
			sscanf(line,"%d|%d",
				&(rule[count].need),
				&(rule[count].want));
		}
		else {
			// Read tokens from single line
			char *token;
			token = strtok(line, ",");
			prints[count][0]=atoi(token);
			int i=0;
			while( 1 ) {
				if(!(token = strtok(NULL, ","))) break;
				prints[count][++i]=atoi(token);
			}
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

//	printMap(map);

	return 0;
//	return rule;
//	return map;
}

void printNeed(int *nd, int *isinline) {
	for(int i=0; i<MAXX;i++) {
		if(nd[i] && isinline[i]) printf("%d,",i);
	}
	printf("\n");
}

int checkLine(int *line,int count) {
	int i,j;

	int isinline[100];

	memset(isinline,0,100*sizeof(int));

	for(i=0;i<count;i++) isinline[line[i]]=1;

	for(i=0; i<count; i++) {
		int *nd=malloc(MAXX*sizeof(int));
		memcpy(nd,needs[line[i]],MAXX*sizeof(int));
		printf("%d needs:  ",line[i]); printNeed(nd,isinline);
		for(j=0; j<i; j++) {
			nd[line[j]]=0;
		}
		printf("%d remain: ",line[i]); printNeed(nd,isinline);
		for(j=0; j<100; j++) 
			if(isinline[j] && nd[j]) {
				free(nd);
				printf("unsat\n");
				return 0;
			}
		free(nd);
	}
	printf("(%d,%d) \n",count,count/2);
	return line[count/2];
}

int main(int argc, char *argv[]) {

//	TRule *array;
	int i=0,j;
	int count[MAXY];
//	array = readInput();
	readInput();

	needs=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) needs[iter]=calloc(MAXX,sizeof(int));

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; rule[i].want; i++) {
//		printf("%d <- %d\n", rule[i].need, rule[i].want);
		needs[rule[i].want][rule[i].need]=1;
	}

	for(i=0; prints[i][0]; i++) {
		for(j=0; prints[i][j]; j++) {
//			printf("%d,", prints[i][j]);
		}
		count[i]=j;
//		printf(" (%d)\n", count[i]);
	}

	int sum=0;
	int res;
	for(i=0; prints[i][0]; i++) {
		for(j=0; prints[i][j]; j++);
//			printf("%d,", prints[i][j]);
//		}
		count[i]=j;
		res=checkLine(prints[i],count[i]);
		printf(" (%d)\n", res);
		sum+=res;
	}


	printf("%d\n", sum);

	return 0;
}
