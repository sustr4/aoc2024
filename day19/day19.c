#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 450
#define MAXY 401
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	int from;
	int to;
} THash;

// Comparator function example
int comp(const void *a, const void *b)
{
	char *const *da = a;
	char *const *db = b;
	return strcmp(*da,*db);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);

// Read input file line by line (e.g., into an array)
char **readInput(char **req) {
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
	char **twl=(char**)calloc(MAXX, sizeof(char*));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	// char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		if(count>1)
			asprintf(&(req[count-2]), "%s", line);	

		int t=0;
		if(count==0) {// Read tokens from single line
			char *token;
			token = strtok(line, ", ");
			asprintf(&(twl[t++]), "%s", token);	
			while( 1 ) {
				if(!(token = strtok(NULL, ", "))) break;
				asprintf(&(twl[t++]), "%s", token);
			}
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return twl;
}

long long check(char *string, char **twl, THash *hash, int depth, long long *history);

long long wrapcheck(char *string, char **twl, THash *hash, int depth, long long *history) {
	if(history[depth]>=0) return history[depth];
	
	long long ret=check(string, twl, hash, depth, history);
	history[depth]=ret;
	return(ret);
}

long long check(char *string, char **twl, THash *hash, int depth, long long *history) {
	long long ret=0;
	if(string[0]==0) return 1;

	if(hash[(int)string[0]].from<0) return 0;
	for(int i=hash[(int)string[0]].from; i<=hash[(int)string[0]].to; i++) {
//		printf("Trying %s on %s (%ld char)", twl[i], string, strlen(twl[i]));
		if(strncmp(string,twl[i],strlen(twl[i]))) {
//			printf("\n");
			continue; }
	
//		printf(" FIT\n");

		ret+=wrapcheck(string+strlen(twl[i]), twl, hash, depth+strlen(twl[i]), history);
	}
	return ret;
}
THash *makehash(char **twl) {

	THash *hash=calloc('z'+1,sizeof(THash));
	for(int i=0; i<='z'; i++) hash[i].from=-1;

	for(int i=0; twl[i]; i++) {
		if(hash[(int)twl[i][0]].from<0) hash[(int)twl[i][0]].from=i;
		hash[(int)twl[i][0]].to=i;
	}
	return hash;
}

int main(int argc, char *argv[]) {

	int i=0, twlcount=0;;	
	char **req=(char**)calloc(MAXY, sizeof(char*));
	char **twl=readInput(req);
	while(twl[twlcount]) twlcount++;

	qsort(twl,twlcount,sizeof(char*),comp);
	THash *hash=makehash(twl);

	for(char c=0; c<='z'; c++)
		if(hash[(int)c].from>=0) printf("%c: %d--%d\n", c,
			hash[(int)c].from, hash[(int)c].to);

//	for(i=0; twl[i]; i++) printf("%s\n", twl[i]);

	long long sum=0;
	#pragma omp parallel for shared(sum)
	for(i=0; i<MAXY; i++) {
		if(!req[i]) continue;
		long long *history=malloc((strlen(req[i])+1) * sizeof(long long));
		for(int j=0; j<=strlen(req[i]); j++) history[j]=-1;
		sum+=check(req[i], twl, hash, 0, history);
		free(history);
		printf("%lld %s\n", sum, req[i]);
	}
	printf("%lld\n",sum);


	return 0;
}
