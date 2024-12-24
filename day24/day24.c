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

	TComp *comp=(TComp*)calloc(MAXY, sizeof(TComp));

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
			rule[count].out=cid(token, comp);

			count++;

		}
	}

	fclose(input);
	if (line)
	free(line);

	return comp;
}

void printRule(TRule r, TComp *comp) {
	printf("%d ", r.in[0]);
	switch(r.type) {
	case 2:
		printf("AND ");
		break;
	case 3:
		printf("OR ");
		break;
	case 4:
		printf("XOR ");
		break;
	default:
		printf("??? ");
		break;
	}

	printf("%d -> %d", r.in[1], r.out);

	printf(" | %s ", comp[r.in[0]].name);
	switch(r.type) {
	case 2:
		printf("AND ");
		break;
	case 3:
		printf("OR ");
		break;
	case 4:
		printf("XOR ");
		break;
	default:
		printf("??? ");
		break;
	}

	printf("%s -> %s", comp[r.in[1]].name, comp[r.out].name);

}

int adder(int no, TComp *comp, TRule *rule, int *carry) {

	int retval=1;

	printf("Adder %d:\n", no);

	int a,b;
	for(a=0; comp[a].name; a++) if((comp[a].name[0]=='x')&&(atoi(comp[a].name+1)==no)) break;
	for(b=0; comp[b].name; b++) if((comp[b].name[0]=='y')&&(atoi(comp[b].name+1)==no)) break;

	printf("  Wire A %d (%s)\n", a, comp[a].name);
	printf("  Wire B %d (%s)\n", b, comp[b].name);

	int x1;
	for(x1=0; rule[x1].type; x1++) {
		if(rule[x1].type!=4) continue;
		if(((rule[x1].in[0]==a)&&(rule[x1].in[1]==b)) ||
		   ((rule[x1].in[0]==b)&&(rule[x1].in[1]==a))) break;
	}
	if(rule[x1].type!=4) {
		retval=0;
		printf("\033[1;31mInput %d not XORed\033[0m\n", no);
	}

	printf("    go into XOR %d (", x1);
	printRule(rule[x1], comp);
	printf(")\n");

	int Ci1=-1;
	int Ci2=-1;
	int a1=0;

	if(no!=0) {
		int x2;
		for(x2=0; rule[x2].type; x2++) {
			if(rule[x2].type!=4) continue;
			if((rule[x2].in[0]==rule[x1].out)||(rule[x2].in[1]==rule[x1].out)) break;
		}
		if(rule[x2].type!=4) {
			retval=0;
			printf("\033[1;31m%d (%s) does not go to another XOR\033[0m\n", x2, comp[rule[x2].out].name);
		}
		printf("      that goes into XOR %d (", x2);
		printRule(rule[x2], comp);
		printf(")\n");
		Ci1 = rule[x2].in[0] == rule[x1].out ? rule[x2].in[1] : rule[x2].in[0];
		printf("        together with Carry in %d (%s)\n", Ci1, comp[Ci1].name);

		for(a1=0; rule[a1].type; a1++) {
			if(rule[a1].type!=2) continue;
			if((rule[a1].in[0]==rule[x1].out)||(rule[a1].in[1]==rule[x1].out)) break;
		}
		if(rule[a1].type!=2) {
			retval=0;
			printf("\033[1;31mInput %d not ANDed\033[0m\n", no);
		}
		printf("      that goes into AND %d (", a1);
		printRule(rule[a1], comp);
		printf(")\n");
		
		Ci2 = rule[a1].in[0] == rule[x1].out ? rule[a1].in[1] : rule[a1].in[0];
		printf("        together with Carry in %d (%s)\n", Ci2, comp[Ci2].name);

		if((Ci1>=0)&&(Ci1!=Ci2)) {
			retval=0;
			printf("\033[1;31mCarry inputs do not match at %d\033[0m\n", no);
		}

	}
	
	int a2;
	for(a2=0; rule[a2].type; a2++) {
		if(rule[a2].type!=2) continue;
		if(((rule[a2].in[0]==a)&&(rule[a2].in[1]==b)) ||
		   ((rule[a2].in[0]==b)&&(rule[a2].in[1]==a))) break;
	}
	if(rule[a2].type!=2) {
		retval=0;
		printf("\033[1;31mDiff with carry at %d not ANDed\033[0m\n", no);
	}
	printf("    go into AND %d (", a2);
	printRule(rule[a2], comp);
	printf(")\n");

	if(no>0) {
		int o2;
		for(o2=0; rule[o2].type; o2++) {
			if(rule[o2].type!=3) continue;
			if(((rule[o2].in[0]==rule[a1].out)&&(rule[o2].in[1]==rule[a2].out)) ||
			   ((rule[o2].in[0]==rule[a2].out)&&(rule[o2].in[1]==rule[a1].out))) break;
		}
		if(rule[o2].type!=3) {
			retval=0;
			printf("\033[1;31m sum (%s) and carry (%s) at %d not ORed\033[0m\n", comp[rule[a2].out].name, comp[rule[a1].out].name, no);
		}

		printf("      that goes to OR %d (", o2);
		printRule(rule[o2], comp);
		printf(")\n");


		int Co = rule[o2].out;
		carry[no]=Co;
		printf("        and produces Carry out %d (%s)\n", Co, comp[Co].name);
		

		if((no>0)&&(Ci2!=carry[no-1])) {
			retval=0;
			printf("\033[1;31m Wrong carry at %d. This stage expects carry at %d, stage %d outputs at %d \033[0m\n", no, Ci2, no-1, carry[no-1]);
		}
	}
	else {
		carry[no]=rule[a2].out;
		printf("        and produces Carry out %d (%s)\n", carry[no], comp[carry[no]].name);
	}

	return retval;	
}

int main(int argc, char *argv[]) {

	TRule *rule=(TRule*)calloc(MAXY, sizeof(TRule));
	TComp *comp = readInput(rule);

	int *carry=(int*)calloc(MAXX, sizeof(int));

	for(int q=0; q<45; q++) {
		if(!adder(q, comp, rule, carry)) break;
		printf("\n");
	}

	return 0;
}
