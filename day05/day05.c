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

int **prints;
int **needs;

// Comparator function
int comp(const void *a, const void *b)
{
	const int *da = (const int *) a;
	const int *db = (const int *) b;
	if(needs[*da][*db]) return 1;
	if(needs[*db][*da]) return -1;
	return 0;
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

	// Allocate a two-dimensional arrray of ints
	prints=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) prints[iter]=calloc(MAXX,sizeof(int));
	needs=calloc(100,sizeof(int*));
	for(int iter=0; iter<100; iter++) needs[iter]=calloc(100,sizeof(int));

	int rules=1;

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<1) { // Empty line in the middle
			count=0;
			rules=0;
			continue;
		}

		if(rules) { // First part of input file
			// Read into array
			int want,need;
			sscanf(line,"%d|%d",
				&(need),
				&(want));
			needs[want][need]=1;
		}
		else { // Second part of input file
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

	return 0;
}

int checkLine(int *line,int count) {
	int i,j;
	int isinline[100];

	memset(isinline,0,100*sizeof(int));
	for(i=0;i<count;i++) isinline[line[i]]=1;

	for(i=0; i<count; i++) {
		int *nd=malloc(100*sizeof(int));
		memcpy(nd,needs[line[i]],100*sizeof(int));
		for(j=0; j<i; j++) nd[line[j]]=0;
		for(j=0; j<100; j++) 
			if(isinline[j] && nd[j]) {
				free(nd);
				return 0;
			}
		free(nd);
	}
	return line[count/2];
}

int main(int argc, char *argv[]) {

	int i=0;
	int count[MAXY];
	readInput();

	int sum=0;
	int sum2=0;
	int res;
	for(i=0; prints[i][0]; i++) {
		for(count[i]=0; prints[i][count[i]]; count[i]++);
		res=checkLine(prints[i],count[i]);
		sum+=res;
		if(!res) {
			qsort(prints[i],count[i],sizeof(int),comp);
			sum2+=prints[i][count[i]/2];
		}

	}

	printf("Already sorted: %d\nNeeded sorting: %d\n", sum,sum2);
	return 0;
}
