#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXDEPTH 75
#define MAXLEN 10000
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	long long val;
	int num;
	void *next;
} TStone;

typedef struct {
	long long val;
	long long res;
} TAcc;

long long *fac;
TAcc **accel;
long long hit=0;
long long csize=0;

int numPlaces (long long n) { // Count numerical places
    if (n < 0) return numPlaces ((n == LLONG_MIN) ? LLONG_MAX: -n);
    if (n < 10) return 1;
    return 1 + numPlaces (n / 10);
}

TStone *addStone(long long v) {
	TStone *ret=calloc(1,sizeof(TStone));
	ret->val=v;
	ret->num=numPlaces(v);
	return ret;
}

// Read input file line by line (e.g., into an array)
TStone *readInput() {
//int readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	TStone *start=NULL;

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		char *token;
		token = strtok(line, " ");
		start=addStone(atoll(token));
		TStone *current=start;
		while( 1 ) {
			if(!(token = strtok(NULL, " "))) break;
			current->next=addStone(atoll(token));
			current=current->next;
		}

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return start;
}

long long getAcc(long long val, int depth) {
	for(int i=0; accel[depth][i].val!=-1; i++) {
		if(val==accel[depth][i].val) {
			hit++;
			return accel[depth][i].res;
		}
	}
	return -1;
}

void pushAcc(long long val, long long res, int depth) {
	int i;
	for(i=0; accel[depth][i].val!=-1; i++);
	accel[depth][i].val=val;
	accel[depth][i].res=res;
	csize++;
}

long long process(long long val, int depth) {
	if(depth==75) {
//		printf("%lld ", val);
		return 1;
	}

	long long a=getAcc(val, depth);
	if(a!=-1) return a;
//    If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
	if(val==0) {
		long long ret=process(1, depth+1);
		pushAcc(val, ret, depth);
		return ret;
	}
//    If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
	int n=numPlaces(val);
	if(!(n%2)) {
		int hsize=n/2;
//		printf("spit %d(%d), two halves of %d\n", s->val, s->num, hsize);
		long long sm=process(val/fac[hsize], depth+1);
		sm+=process(val%fac[hsize], depth+1);
		pushAcc(val,sm,depth);
		return sm;
	}
//    If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
//	printf("multiply %d\n", s->val);
	long long ret = process(val*2024, depth+1);
	pushAcc(val,ret,depth);
	return ret;
}

int main(int argc, char *argv[]) {

	TStone *start=readInput();
	fac=calloc(100,sizeof(int));

	int i=0, j=1;
	while(1) { // Fill in the fractions table
		fac[i++]=j;
		if(j>=INT_MAX/10) break;
		j*=10;
	}

	accel=calloc(MAXDEPTH,sizeof(TAcc*));
	for(int j=0; j<MAXDEPTH; j++) {
		accel[j]=malloc(MAXLEN * sizeof(TAcc));
		for(int y=0; y<MAXLEN; y++) accel[j][y].val=-1;
	}

	long long sum=0;

	TStone *s=start;
	while(s) {
		sum+=process(s->val, 0);
		s=s->next;
	}

	printf("There are %lld stones. %lld cache hits. %lld values stored.\n", sum, hit, csize);

	return 0;
}
