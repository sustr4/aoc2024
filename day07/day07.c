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
#define MAXY 1000

int *check;

// Print a two-dimensional array
void printArray (long long **array) {
	int x,y;
	for(y=0; array[y][0]; y++) {
		for(x=0; array[y][x]; x++) {
			printf("%lld", array[y][x]);
			if(!x) printf(": ");
			else if(array[y][x+1]) printf(" ");
		}
		printf("\n");
	}
}

// Read input file line by line (e.g., into an array)
long long **readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0;
	long long **map=calloc(MAXY,sizeof(long long*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(long long));
	check=calloc(MAXY,sizeof(long long));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read tokens from single line
		char *token;
		token = strtok(line, " ");
		token[strlen(token)]=0;
		map[count][0]=atoll(token);
		x=1;
		while( 1 ) {
			if(!(token = strtok(NULL, " "))) break;
			map[count][x++]=atoll(token);
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return map;
}

int try(long long **formula, int line, int depth, long long sum) {
	if(!formula[line][depth]) { // The calculation should be finished
		if(formula[line][0]==sum) {
			check[line]++;
			return 1;
		}
		return 0;
	}
	if(sum>formula[line][0]) return 0; // Grew too big
	if(try(formula, line, depth + 1, sum + formula[line][depth])) return 1;
	if(try(formula, line, depth + 1, sum * formula[line][depth])) return 1;
	long long fac=1;
	while(fac<=formula[line][depth]) fac*=(long long)10;
//	printf("%lld * %lld + %lld = %lld == %lld%lld ?\n", sum, fac, formula[line][depth], fac * sum + formula[line][depth], sum, formula[line][depth]);
	if(try(formula, line, depth + 1, fac * sum + formula[line][depth])) return 1;/**/

/*	char *conc;
	asprintf(&conc,"%lld%lld",sum,formula[line][depth]);
	long long next = atoll(conc);
	free(conc);
	if(try(formula, line, depth + 1, next)) return 1;*/
	return 0;
}

int main(int argc, char *argv[]) {

	int i=0;	
	long long **array = readInput();

	long long sum=0;
	i=680;
	for(i=0; array[i][0]; i++) {
		try(array, i, 2, array[i][1]);
//		printf("%d: %d", i, check[i]);
		if(check[i]) {
			sum+=array[i][0];
//			printf("(%lld)", array[i][0]);
		}
//		printf("\n");
	}
	printf("Sum %lld\n", sum);

	return 0;
}
