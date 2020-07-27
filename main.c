//Takes in two numerical sequences and can compute a
//longest common subsequence,
//as well as tandem and palindrome.
//longest common and tandem can also be performed
//output can be specified by command line arguments
//Name: assgn2.c
//Author: Rylan Chin
//Date: November 3, 2019

//Timing Results (rounded to nearest hundredth of a second)
/**********************************************************************
* Length * 50	100	200	500	1000	2000	5000	10000 *
***********************************************************************
* -c	 *0.00  0.00   0.00     0.00    0.00    0.02    0.12     0.49 *
***********************************************************************
* -t	 *0.00  0.00   0.01     0.08    0.62    4.93   86.63    704.15*
***********************************************************************
* -p	 *0.00  0.00   0.00     0.00    0.00    0.02    0.12     0.49 *
***********************************************************************
* -ct	 *5.54  336.88  -        -        -      -        -        -  *
***********************************************************************
* -cp	 *0.08  1.32  20.41      -        -      -        -        -  *
***********************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "util.h"
#include "lcs.h"
#include "lts.h"
#include "lps.h"

int main(int argc, char *argv[]) {

	if (argc == 1) {
		printf("No arguments given\n");
		return 0;
	}

	//setup arrays to track argument values
	bool flags[NONFILEARGS] = {false};
	char *ioFileNames[2] = {NULL};

	//read args and set trackers
	if (processArguments(argc, argv, flags, ioFileNames) == -1) {
		reportBadArguments();
		return 0;
	}

	//-g is a stand alone operation
	if (flags[0]) {
		generateSequences(ioFileNames[1]);
		return 0;
	}

	//get sequences
	char sequence1[MAX_LENGTH + 1], sequence2[MAX_LENGTH + 1];
	int len1, len2;

	//get two sequences either from stdin or from file
	if (flags[1] || flags[4] || flags[5]) {
		if (ioFileNames[0] == NULL)
			get2Input(&len1, &len2, sequence1, sequence2);
		else if (getInputFromFile2(&len1, &len2, sequence1, sequence2, ioFileNames[0]) == -1)
			return 0;
	}
	//get one sequence either from stdin or file
	else {
		if (ioFileNames[0] == NULL)
			get1Input(&len1, sequence1);
		else if (getInputFromFile1(&len1, sequence1, ioFileNames[0]) == -1)
			return 0;
	}

	//setup output stream
	FILE *output = stdout;
	if(ioFileNames[1] != NULL)
		output = fopen(ioFileNames[1], "a+");

	//run all specified arguments
	for (int i = 1; i < NONFILEARGS; i++) {
		//skip -g flag
		if (flags[i]) {
			switch (i) {
				case 1: //-c | compute longest common subsequence
					computeLcs(len1, len2, sequence1, sequence2, true, output);
					break;
				case 2: //-t | compute longest tandem sequence
					computeLts(len1, sequence1, output);
					break;
				case 3: //-p | compute longest palindrome sequence
					computeLps(len1, sequence1, output);
					break;
				case 4: //-ct | compute longest common tandem sequence
					computeLcts(len1, len2, sequence1, sequence2, output);
					break;
				case 5: //-cp | compute longest common palindrome sequence
					computeLcps(len1, len2, sequence1, sequence2, output);
					break;
			}
		}
	}
	fclose(output);

	return 0;
}

/***********************************************************************
 * processes the arguments given to the program and records the results
 * argc: the number of arguments passed
 * argv: the array of arguments
 * flags: a bool array for tracking which arguments were passed
 * ioFileNames: an array for storing any passed file names
 **********************************************************************/
int processArguments(int argc, char *argv[], bool flags[], char *ioFileNames[]) {
	//fills out flags and file names according to arguments
	//returns 0 on valid args, -1 on invalid, no duplicates allowed

	//the bools in flags match the order of the options in options
	char *options[8] = {"-g", "-c", "-t", "-p", "-ct", "-cp", "-i", "-o"};

	for (int i = 1; i < argc; i++) {
		int j = 0;
		for (;j < 8;) {
			if (strcmp(argv[i], options[j]) == 0)
				break;
			j++;
		}

		//the value of j falls through from the loop
		switch (j) {
			//no match in options, must be filename or invalid
			case 8:
				//preceeding argument must be the appropriate file specifier
				if (strcmp(argv[i - 1], options[6]) == 0 && ioFileNames[0] == NULL) //ioFileNames 0 is input, 1 is output
					ioFileNames[0] = argv[i];
				else if (strcmp(argv[i - 1], options[7]) == 0 && ioFileNames[1] == NULL)
					ioFileNames[1] = argv[i];
				else
					return -1;
				break;
			//use file options, check next arg is possible
			case 6: case 7:
				if (i + 1 >= argc)
					return -1;
				break;
			//other flags
			default:
				if (flags[j])
					return -1;
				flags[j] = true;
		}
	}

	//check validity of combination
	//-g must be alone or with an output file
	if (flags[0]) {
		for (int k = 1; k < 6; k++) {
			if (flags[k])
				return -1;
		}
		if (ioFileNames[0] != NULL)
			return -1;
		return 0;
	}
	//-c, -t, -p, -ct, -cp can be together with input and output files
	return 0;
}
