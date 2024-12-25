#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 5
#define MAXY 500

void readInput(int **key, int **lock) {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }


	int inlock=0;
	int inkey=0;

	int l=0;
	int k=0;
	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) {
			if(inlock) {
				assert(lock[l][0]);
				l++;
			}
			if(inkey) {
				assert(key[k][0]);
				k++;
			}

			inlock=0;
			inkey=0;

			continue;
		}

		// Read into map
		if((inkey==0) && (inlock==0)) {
			if(line[0]=='#') inkey=1;
			if(line[0]=='.') inlock=1;
		}

		for(int x=0; x<MAXX; x++) {
			if(inkey && line[x]=='#') key[k][x]++;
			if(inlock && line[x]=='.') lock[l][x]++;
		}
			
	}

	printf("Read %d keys and %d locks.\n", k, l);

	fclose(input);
	if (line)
	free(line);

}

void print5(int *five) {
	for(int n=0; n<MAXX; n++) {
		printf("%d", five[n]);
		if(n!=MAXX-1) printf(",");
	}
}

int main(int argc, char *argv[]) {

	int **lock=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) lock[iter]=calloc(MAXX,sizeof(int));

	int **key=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) key[iter]=calloc(MAXX,sizeof(int));

	readInput(key, lock);

	
//	for(int k=0; key[k][0]; k++) {
//		print5(key[k]);
//		printf("\n");
//	}

	int sum=0;
	for(int l=0; lock[l][0]; l++) {
		for(int k=0; key[k][0]; k++) {
			int fit=1;
			for(int n=0; n<MAXX; n++) {
				if(key[k][n]>lock[l][n]) {
					fit=0;
					break;
				}
			}
			if(fit) sum++;
		}
	}

	printf("Sum: %d\n", sum);
	return 0;
}
