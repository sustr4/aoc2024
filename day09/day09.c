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
#define MAXY 100000

// Point structure definition
typedef struct {
	int start;
	int length;
} TFile;

TFile *spaces;
TFile *files;

// Read input file line by line (e.g., into an array)
int *readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	input = fopen(INPUT, "r");
	if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	int *disk=(int*)calloc(MAXY, sizeof(int));

	int leng=0;
	int idx=0;
	int fid=1;
	while ((read = getline(&line, &len, input)) != -1) {
		line[strlen(line)-1] = 0; // Truncate the NL

		// Read into map
		int file=1;
		int space=0, flno=0;
		for(int x=0; x<strlen(line); x++) {
			if(file) {
				for(int i=0; i<line[x]-'0'; i++)
					disk[idx++]=fid;
				fid++;
				files[flno].start=leng;
				files[flno].length=line[x]-'0';
				flno++;
			}
			else {
				idx+=line[x]-'0';
				spaces[space].start=leng;
				spaces[space].length=line[x]-'0';
				space++;
			}
			file=!file;
			leng+=line[x]-'0';
		}

		count++;
	}
	fclose(input);
	if (line)
	free(line);

	return disk;
}

int main(int argc, char *argv[]) {

	spaces=calloc(10001,sizeof(TFile));
	files=calloc(10001,sizeof(TFile));
	int *disk=readInput();	

	int block;
	int fcount;

	for(fcount=1; files[fcount+1].start; fcount++) {
//		printf("File No. %d starts at %d and is %d long. ", fcount, files[fcount].start, files[fcount].length);
//		printf("Space No. %d starts at %d and is %d long.\n", fcount, spaces[fcount].start, spaces[fcount].length);
	}

	int f;
	int s;

	for(f=fcount; f>0; f--) {
		for(s=0; s<10000; s++) {
			if(spaces[s].length>=files[f].length) break;
		}
		if(!spaces[s].start) continue;
		if(spaces[s].start>files[f].start) continue;
		printf("Moving file %d (%d bytes, header \"%d\") from %d ",f ,files[f].length, disk[files[f].start], files[f].start);
		printf("to space %d (%d bytes) at %d\n",s ,spaces[s].length, spaces[s].start);	
		memcpy(&(disk[spaces[s].start]), &(disk[files[f].start]), files[f].length*sizeof(int));
		memset(&(disk[files[f].start]),0,files[f].length*sizeof(int));

		files[f].start=spaces[s].start;
		spaces[s].start+=files[f].length;
		spaces[s].length-=files[f].length;
	}

	long sum=0;
        for(block=0;block < MAXY;block++) {
		if(!disk[block]) continue;
                sum+=block*(disk[block]-1); 
	}
        printf("Sum %ld\n", sum);
	
	
	return 0;
}
