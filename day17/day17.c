#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 76
//#define INPUT "unit1.txt"
//#define MAXX 10

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


// Read input file line by line (e.g., into an array)
int *readInput(int *reg) {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	int *inst=(int*)calloc(MAXX, sizeof(int));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<1) continue;
		if(line[0]=='R') {
			char rg;
			int val;
			sscanf(line,"Register %c: %d",
				&rg,
				&val);
			reg[rg-'A']=val;
		}
		else {
			char *lst=line+9;
			char *token;
			token = strtok(lst, ",");
			inst[count++]=atoi(token);
			
			while( 1 ) {
				if(!(token = strtok(NULL, ","))) break;
				inst[count+1]=-1;
				inst[count++]=atoi(token);
			}

		}
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
	int i=0;	
//	array = readInput();
	int *reg=(int*)calloc(3, sizeof(int));
	int *prog=readInput(reg);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; i<3; i++) printf("%d,", reg[i]);
	printf("\n");
	for(i=0; prog[i]>=0; i++) printf("%d,", prog[i]);
	printf("\n");

	return 0;
}
