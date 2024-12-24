#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
#define INPUT "input.txt"
#define MAXX 50
#define MAXY 4000
int MAXW;
//#define INPUT "unit1.txt"
//#define MAXX 10
//#define MAXY 10

// Point structure definition
typedef struct {
	char *name;
	int type;
	int out;
} TComp;

typedef struct {
	int in[2];
	int type;
	int out;
} TRule;


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

int cid (char *nm, TComp *comp) { // Component ID
	int id;

	for(id=0; comp[id].name; id++)
		if(!strcmp(comp[id].name, nm)) return id;

	comp[id].name=strdup(nm);
	return id;
}

// Read input file line by line (e.g., into an array)
TComp *readInput(TRule *rule) {
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
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	TComp *comp=(TComp*)calloc(MAXY, sizeof(TComp));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
	char **wire=calloc(2,sizeof(char*));
	for(int iter=0; iter<2; iter++) wire[iter]=calloc(MAXX,sizeof(char));

	int wires=1;
	MAXW=0;
	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<2) {
			wires=0;
			continue;
		}

		if(wires) {
			char c;
			int n;
			int val;
			sscanf(line,"%c%d: %d", &c, &n, &val);
			wire[c-'x'][n]=val;
			line[3]=0;
			comp[cid(line, comp)].out = val;
			comp[cid(line, comp)].type = 1;
		}
		else { // Read by words;
			char *token;

			int id = cid(line, comp);

			comp[id].name=strdup(line);

			token = strtok(line, " "); // First input
			rule[count].in[0]=cid(token, comp);
			
			token = strtok(NULL, " "); // Type
			if(token[0]=='A') {
				rule[count].type = 2; // AND
				comp[id].type =2; }
			if(token[0]=='O') {
				rule[count].type = 3; // OR
				comp[id].type =3; }
			if(token[0]=='X') {
				rule[count].type = 4; // XOR
				comp[id].type =4; }

			comp[id].out = -1;

			token = strtok(NULL, " "); // Second input
			rule[count].in[1]=cid(token, comp);

			token = strtok(NULL, " "); // Arrow

			token = strtok(NULL, " "); // Output
			rule[count].in[1]=cid(token, comp);

			count++;

		}
	}

	fclose(input);
	if (line)
	free(line);

	return comp;
}

int main(int argc, char *argv[]) {

	int i=0;	
//	array = readInput();
	TRule *rule=(TRule*)calloc(MAXY, sizeof(TRule));
	TComp *comp = readInput(rule);

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(i=0; rule[i].type; i++) {
		printf("%d(%s) %d %d\n", rule[i].in[0], comp[rule[i].in[0]].name, rule[i].type, rule[i].in[1]);
	}

	return 0;
}
