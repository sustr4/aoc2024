#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 75
//#define INPUT "unit1.txt"
#define MAXDEPTH 10
#define MAXHLEN 200000

// Point structure definition
typedef struct {
	int x;
	int y;
} TPoint;

typedef struct {
	char *str;
	int depth;
	long long val;
} THist;

THist *hist;

char numkb[4][3] = {
	{ '7', '8', '9' },
	{ '4', '5', '6' },
	{ '1', '2', '3' },
	{ ' ', '0', 'A' }};

char arrkb[2][3] = {
	{ ' ', '^', 'A' },
	{ '<', 'v', '>' }};

TPoint **numKeyToKey;
TPoint **arrKeyToKey;

TPoint *numAbs;
TPoint *arrAbs;

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
char **readInput() {
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
	char **inst=(char**)calloc(MAXX, sizeof(char*));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Copy to string
		asprintf(&(inst[count]), "%s", line);	

		count++;
	}

	fclose(input);
	if (line)
	free(line);

	return inst;
}

void initKK() {

	numKeyToKey=calloc('v'+1, sizeof(TPoint*));
	arrKeyToKey=calloc('v'+1, sizeof(TPoint*));

	numAbs=calloc('v'+1, sizeof(TPoint));
	arrAbs=calloc('v'+1, sizeof(TPoint));

	// Numeric
	for(int fy=0; fy<4; fy++) {
		for(int fx=0; fx<3; fx++) {
			char from=numkb[fy][fx];
			numAbs[(int)from].x=fx; numAbs[(int)from].y=fy;
			numKeyToKey[(int)from]=calloc('v'+1, sizeof(TPoint));
			for(int ty=0; ty<4; ty++) {
				for(int tx=0; tx<3; tx++) {
					char to=numkb[ty][tx];
					numKeyToKey[(int)from][(int)to].y = ty-fy;
					numKeyToKey[(int)from][(int)to].x = tx-fx;
//					printf("%c to %c: %d,%d\n", from, to, (numKeyToKey[(int)from][(int)to]).x,  numKeyToKey[(int)from][(int)to].y);
				} } } }

	// Directional arrows:
	for(int fy=0; fy<2; fy++) {
		for(int fx=0; fx<3; fx++) {
			char from=arrkb[fy][fx];
			arrAbs[(int)from].x=fx; arrAbs[(int)from].y=fy;
			arrKeyToKey[(int)from]=calloc('v'+1, sizeof(TPoint));
			for(int ty=0; ty<2; ty++) {
				for(int tx=0; tx<3; tx++) {
					char to=arrkb[ty][tx];
					arrKeyToKey[(int)from][(int)to].y = ty-fy;
					arrKeyToKey[(int)from][(int)to].x = tx-fx;
//					printf("%c to %c: %d,%d\n", from, to, (arrKeyToKey[(int)from][(int)to]).x,  arrKeyToKey[(int)from][(int)to].y);
				} } } }

}

long long arrCode(char *arr, int depth);

long long arrCodeWrap(char *arr, int depth) {
	long long ret=0;
	int i;

	for(i=0; hist[i].str; i++) {
		if(hist[i].depth!=depth) continue;
		if(!strcmp(arr, hist[i].str)) return hist[i].val;
	}

	if(i>=MAXHLEN) printf("Cache full\n");

	ret=arrCode(arr, depth);

	hist[i].str=strdup(arr);
	hist[i].depth=depth;
	hist[i].val=ret;
		
	return(ret);
}

long long arrCode(char *arr, int depth) {
	long long ret=0;
	if(depth==26) {
//		printf("%s\n", arr);
		return strlen(arr);
	}
	char prev='A';

	for(int i=0; i<strlen(arr); i++) {
		TPoint move=arrKeyToKey[(int)prev][(int)arr[i]];

		int dx=move.x<0 ? -1 : 1;
		int dy=move.y<0 ? -1 : 1;
		// left/right first
		long long optx=LLONG_MAX;
		if(!((arrAbs[(int)prev].y==0)&&(arrAbs[(int)arr[i]].x==0))) {
			char *numseq=calloc(abs(move.x)+abs(move.y)+2,sizeof(char));
			int n=0;
			for(int x=0; x!=move.x; x+=dx) numseq[n++]=dx>0 ? '>' : '<';
			for(int y=0; y!=move.y; y+=dy) numseq[n++]=dy>0 ? 'v' : '^';
			numseq[n++]='A';
			optx=arrCodeWrap(numseq, depth+1);
			free(numseq);
		}

		// up/down first
		long long opty=LLONG_MAX;
		if(!((arrAbs[(int)arr[i]].y==0)&&(arrAbs[(int)prev].x==0))) {
			char *numseq=calloc(abs(move.x)+abs(move.y)+2,sizeof(char));
			int n=0;
			for(int y=0; y!=move.y; y+=dy) numseq[n++]=dy>0 ? 'v' : '^';
			for(int x=0; x!=move.x; x+=dx) numseq[n++]=dx>0 ? '>' : '<';
			numseq[n++]='A';
			opty=arrCodeWrap(numseq, depth+1);
			free(numseq);
		}

		if(optx<opty) ret+=optx;
		else ret+=opty;
		prev=arr[i];
	}

	return ret;
}


long long numCode(char *code) {
	long long ret=0;
	char prev='A';

	for(int i=0; i<strlen(code); i++) {
		TPoint move=numKeyToKey[(int)prev][(int)code[i]];
//		printf("\n(from %c) num %c ([%d,%d]):\n", prev, code[i], numAbs[(int)code[i]].x, numAbs[(int)code[i]].y);
		int dx=move.x<0 ? -1 : 1;
		int dy=move.y<0 ? -1 : 1;


		// left/right first
		long long optx=LLONG_MAX;
		if(!((numAbs[(int)prev].y==3)&&(numAbs[(int)code[i]].x==0))) {
			char *numseq=calloc(abs(move.x)+abs(move.y)+2,sizeof(char));
			int n=0;
			for(int x=0; x!=move.x; x+=dx) numseq[n++] = dx>0 ? '>' : '<';
			for(int y=0; y!=move.y; y+=dy) numseq[n++] = dy>0 ? 'v' : '^';
			numseq[n++]='A';
			optx=arrCode(numseq, 1);
			free(numseq);
		}

		// up/down first
		long long opty=LLONG_MAX;
		if(!((numAbs[(int)prev].x==0)&&(numAbs[(int)code[i]].y==3))) {
			char *numseq=calloc(abs(move.x)+abs(move.y)+2,sizeof(char));
			int n=0;
			for(int y=0; y!=move.y; y+=dy) numseq[n++] = dy>0 ? 'v' : '^';
			for(int x=0; x!=move.x; x+=dx) numseq[n++] = dx>0 ? '>' : '<';
			numseq[n++]='A';
			opty=arrCode(numseq, 1);
//			printf("\n");
		}

		if(optx<opty) ret+=optx;
		else ret+=opty;
		prev=code[i];
	}

	return ret;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
	int i=0;	
//	array = readInput();
	char **numcode = readInput();
	initKK();

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	long long sum=0;

	hist=calloc(MAXHLEN, sizeof(THist));

	for(i=0; numcode[i]; i++) {
//	for(i=0; i<1; i++) {
		long long dura=numCode(numcode[i]);

		int nmp=atoi(numcode[i]);

		printf("%s: %lld (*%d)\n", numcode[i], dura, nmp);
		sum+=dura*nmp;
	}

	printf("%lld\n", sum);

	return 0;
}
