#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "util.h"

#define MAX_LENGTH 10000

void get2Input(int *lenStr1, int *lenStr2, char sequence1[], char sequence2[]) {
	//prompts user repeatedly until valid input is given for both sequences

	while(1) {
		//read sequences from standard input
		printf("Enter the first sequence: ");
		if ((*lenStr1 = readValidateInput(sequence1, NULL)) == -1) continue;
		printf("Enter the second sequence: ");
		if ((*lenStr2 = readValidateInput(sequence2, NULL)) == -1) continue;
		break;
	}
	return;
}

void get1Input(int *lenStr1, char sequence1[]) {
	//prompts user repeatedly until valid input is given for one sequence

	while(1) {
		//read sequences from standard input
		printf("Enter a sequence: ");
		if ((*lenStr1 = readValidateInput(sequence1, NULL)) == -1) continue;
		break;
	}
	return;
}

int getInputFromFile2(int *len1, int *len2, char seq1[], char seq2[], char *filename) {
	//return -1 on fail, 0 on success

	FILE *input;
	if ((input = fopen(filename, "r")) == NULL) {
		return -1;
	}

	//read and assign length
	*len1 = readValidateInput(seq1, input);
	*len2 = readValidateInput(seq2, input);
	if (*len1 == -3 || *len2 == -3)
		return 0;
	if (*len1 == -1 || *len2 == -1) {
		return -1;
	}

	//only close if not stdin
	if (filename != NULL)
		fclose(input);

	return 0;
}

int getInputFromFile1(int *len, char seq[], char *filename) {
	//return -1 on fail, 0 on success

	FILE *input;
	if ((input = fopen(filename, "r")) == NULL) {
		return -1;
	}
	*len = readValidateInput(seq, input);
	if (*len == -1) {
		return -1;
	}
	if (filename != NULL)
		fclose(input);

	return 0;
}

int readValidateInput(char sequence[], FILE *inputFile){
        //read sequences from standard input or file and validate
        //returns length of sequence
        //returns -1 on failed validation
	//returns -2 on no file
	//returns 0 on no newline

	FILE *input = stdin;
	if (inputFile != NULL)
		input = inputFile;

	//read from input stream
        int length = 0;
        if (fgets(sequence, MAX_LENGTH  + 2, input) == NULL) {
		sequence[0] = '\0';
		return 0;
	}

	//get length
        length = strlen(sequence);
	//replace newline with null
	if (sequence[length - 1] != '\n') {
		sequence[0] = '\0';
		return 0;
	}
        sequence[--length] = '\0'; //removes newline character
        //check for valid input
        for (int i = 0; sequence[i] != '\0'; i++) {
                if (!isdigit(sequence[i])) {
                        printf("Error, non-digit character detected!\n");
                        length = -1;
                        break;
                }
        }

        return length;
}

void allocateTable(int len1, int len2, tableType ***table) {
	//takes in a ** pointer and allocates space for a table len1+1 x len2+1

	*table = (tableType **)malloc(sizeof(tableType *) * (len1 + 1));
	for (int i = 0; i < (len1 + 1); i++)
		(*table)[i] = (tableType *)malloc(sizeof(tableType) * (len2 + 1));

	return;
}

void freeTable(int len, tableType ***table) {
	//frees all memory allocated via malloc used for the tables

	for (int i = 0; i <= len; i++)
		free((*table)[i]); //frees all the blocks inside the first array
	free(*table); //frees the main array

	return;
}

void printSequence(int n, char seq[], FILE *out) {
	//prints sequence in reverse
	//n is the length of the given string

	for (int i = n - 1; i >= 0; i--)
		fputc(seq[i], out);
	fputc('\n', out);

	return;
}

void reportBadArguments() {
	//prints out all the options and valid combinations

	printf("Invalid arguments, usage as follows: \n");
	printf("-g:	to generate an instance consisting of two sequences over the digit alphabet. Use -o to specify output file name.\n\
-c:	to compute an LCS for the two input sequences. Use -i and -o to specify input and output file names.\n\
-t:	to compute an LTS for the input sequence. Use -i and -o to specify input and output file names.\n\
-p:	to compute an LPS for the input sequence. Use -i and -o to specify input and output file names.\n\
-ct:	to compute an LCTS for the two input sequences. Use -i and -o to specify input and output file names.\n\
-cp:	to compute an LCPS for the two input sequences. Use -i and -o to specify input and output file names.\n\
-i inputfilename:	to read in sequence(s) from file \"inputfilename\"\n\
-o outputfilename:	to write all the results into the file \"outputfilename\"\n");
	printf("The valid argument/option combinations are (\"a|b\" means \"a\" and/or \"b\", and \"[a]\" means the argument \"a\" is or is not there):\n\
-g [-o outputfilename]\n\
-t|-p [-i inputfilename] [-o outputfilename]\n\
-c|-ct|-cp [-t|-p] [-i inputfilename] [-o outputfilename]\n");

	return;
}

void generateSequences(char *filename) {
	//generates 2 sequences

	//seed random
	srand(time(0));

	int len1, len2;

	printf("Enter the lengths of the two sequences: ");
	if (scanf("%d %d", &len1, &len2) != 2) {
		printf("Invalid input\n");
		return;
	}

	//put specified number of random ascii digits into stream
	if (filename != NULL) {
		FILE *output;

		output = fopen(filename, "w");

		//pick random digit and put ascii value into file
		for (int i = 0; i < len1; i++)
			fputc((rand() % 10) + 48, output);
		fputc('\n', output);

		//create second sequence
		for (int i = 0; i < len2; i++)
			fputc((rand() % 10) + 48, output);
		fputc('\n', output);

		fclose(output);
	}
	else {
		for (int i = 0; i < len1; i++)
			putchar((rand() % 10) + 48);
		putchar('\n');

		for (int i = 0; i < len2; i++)
			putchar((rand() % 10) + 48);
		putchar('\n');
	}

	return;
}

void allocateQuad(int len1, int len2, int len3, int len4, tableType *****quad) {
	//allocates space for a 4 way lcs

	*quad = (tableType ****)malloc((len1 + 1) * sizeof(tableType ***));

	for (int i = 0; i < (len1 + 1); i++) {
		(*quad)[i] = (tableType ***)malloc((len2 + 1) * sizeof(tableType **));
		for (int j = 0; j < (len2 + 1); j++) {
			(*quad)[i][j] = (tableType **)malloc((len3 + 1) * sizeof(tableType *));
			for (int k = 0; k < (len3 + 1); k++) {
				(*quad)[i][j][k] = (tableType *)malloc((len4 + 1) * sizeof(tableType));
			}
		}
	}

	return;
}

void freeQuad(int len1, int len2, int len3, tableType *****quad) {
	//frees a given quad's memory

	for (int i = 0; i <= len1; i++) {
		for (int j = 0; j <= len2; j++) {
			for (int k = 0; k <= len3; k++)
				free((*quad)[i][j][k]);
			free((*quad)[i][j]);
		}
		free((*quad)[i]);
	}
	free(*quad);

	return;
}
