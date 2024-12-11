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
#define MAXY 26
//#define INPUT "unit2.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	long long val;
	int num;
	void *next;
} TStone;

long long *fac;

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

int numPlaces (long long n) {
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

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

		// Read tokens from single line
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

TStone *process(TStone *s) {
	if(!s) return NULL;
	
//    If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
	if(s->val==0) {
//		printf("set 1 for %d\n", s->val);
		s->val=1;
		s->num=1;
		return s->next;
	}
//    If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
	if(!(s->num%2)) {
		int hsize=s->num/2;
//		printf("spit %d(%d), two halves of %d\n", s->val, s->num, hsize);
		TStone *new=addStone(s->val%fac[hsize]);
		s->val=s->val/fac[hsize];
		s->num=hsize;
		new->next=s->next;
		s->next=new;
		return new->next;
	}
//    If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
//	printf("multiply %d\n", s->val);
	s->val*=2024;
	s->num=numPlaces(s->val);
	return s->next;
}

void printStones(TStone *s) {
	while(s) {
		printf("%lld ", s->val);
//		printf("%d(%d) ", s->val, s->num);
		s=s->next;
	}
	printf("\n");
}

int countStones(TStone *s) {
	int ret=0;
	while(s) {
		ret++;
		s=s->next;
	}
	return ret;
}

int main(int argc, char *argv[]) {

//	int i=0;	
//	array = readInput();
	TStone *start=readInput();
	fac=calloc(100,sizeof(int));

	int i=0, j=1;
	while(1) {
		fac[i++]=j;
		printf("%d ", j);
		if(j>=INT_MAX/10) break;
		j*=10;
	}
	printf("\n");


	printStones(start);
	for(int cyc=0; cyc<25; cyc++) {
		printf("%d\t%d \n", cyc, countStones(start));
		TStone *s=start;
		while(s) s=process(s);

//		printStones(start);

	}
	
//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	printf("There are %d stones.\n", countStones(start));

	return 0;
}
