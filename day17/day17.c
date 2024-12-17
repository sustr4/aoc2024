#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary and input file definitions, set as required
//#define INPUT "input.txt"
#define MAXX 76
#define INPUT "unit1.txt"

// Read input file line by line (e.g., into an array)
long *readInput(long *reg) {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	long *inst=(long*)calloc(MAXX, sizeof(long));

	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		if(strlen(line)<1) continue;
		if(line[0]=='R') {
			char rg;
			long val;
			sscanf(line,"Register %c: %ld",
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

long ipow(long base, long exp) // Exponentiation by squaring. TODO: add to useful
{
	long result = 1;
	for (;;) {
		if (exp & 1) result *= base;
		exp >>= 1;
		if (!exp) break;
		base *= base;
	}
	return result;
}

long run(long *prog, long idx, long *reg, long *outseq) {

	long prefetch=0;
	if(prog[idx+1]<4) prefetch=prog[idx+1];
	else if(prog[idx+1]<7) prefetch=reg[prog[idx+1]-4];

	switch(prog[idx]) {
// The adv instruction (opcode 0) performs division. The numerator is the value in the A register. The denominator is found by raising 2 to the power of the instruction's combo operand. (So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would divide A by 2^B.) The result of the division operation is truncated to an integer and then written to the A register.
	case 0:
//		printf("%2d: adv %d/2^%d = ", idx, reg[0], prefetch);
		reg[0]=reg[0]/ipow(2,prefetch);
//		printf("%d\n", reg[0]);
		break;
// The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the instruction's literal operand, then stores the result in register B.
	case 1:
//		printf("%2d: bxl %d^%d = ", idx, reg[1], prog[idx+1]);
		reg[1]=reg[1]^prog[idx+1];
//		printf("%d\n", reg[1]);
		break;
// The bst instruction (opcode 2) calculates the value of its combo operand modulo 8 (thereby keeping only its lowest 3 bits), then writes that value to the B register.
	case 2:
//		printf("%2d: bst %d%%8 = ", idx, prefetch);
		reg[1]=prefetch%8;
//		printf("%d\n", reg[1]);
		break;
// The jnz instruction (opcode 3) does nothing if the A register is 0. However, if the A register is not zero, it jumps by setting the instruction pointer to the value of its literal operand; if this instruction jumps, the instruction pointer is not increased by 2 after this instruction.
	case 3:
//		printf("%2d: jnz to %d", idx, prog[idx+1]);
		if(!reg[0]) break;
		return prog[idx+1];
// The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C, then stores the result in register B. (For legacy reasons, this instruction reads an operand but ignores it.)
	case 4:
		reg[1]=reg[1]^reg[2];
		break;
// The out instruction (opcode 5) calculates the value of its combo operand modulo 8, then outputs that value. (If a program outputs multiple values, they are separated by commas.)
	case 5:
//		printf("%d: %d,\n", (*outseq)++, prefetch%8);
		if((prefetch%8)!=(prog[(*outseq)++])) return(-1);
		break;
// The bdv instruction (opcode 6) works exactly like the adv instruction except that the result is stored in the B register. (The numerator is still read from the A register.)
	case 6:
		reg[1]=reg[0]/ipow(2,prefetch);
		break;
// The cdv instruction (opcode 7) works exactly like the adv instruction except that the result is stored in the C register. (The numerator is still read from the A register.)
	case 7:
		reg[2]=reg[0]/ipow(2,prefetch);
		break;
	}

	return idx+2;
}

int main(int argc, char *argv[]) {

//	TPoint *array;
	long i=0;	
//	array = readInput();
	long *reg=(long*)calloc(3, sizeof(long));
	long *prog=readInput(reg);
	long len;
	long unknown;
	for(len=0; prog[len]>=0; len++)
		printf("%ld,",prog[len]);
	printf("\n");

//	#pragma omp parallel for private(<uniq-var>) shared(<shared-var>)
	for(unknown=0; unknown<INT_MAX; unknown++) {
		long outseq=0;
		reg[0]=unknown; reg[1]=0; reg[2]=0;
		i=0;
		while(1) {
	//		printf("%2i: %10d%10d%10d\n", i, reg[0], reg[1], reg[2]);
			i=run(prog,i,reg,&outseq);
			if(i<0) break;
			if(i>=len) break;
			if(outseq>len) break;
		}
		if(outseq==len) {
			printf("%ld matches\n", unknown);
			break;
		}
	}
	return 0;
}
