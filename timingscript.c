#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/****************************************************************
 * A general script used for timing randomly generated sequences
 ****************************************************************/
int main(void) {
	char gencommand[] = "./sequence -g -o testfile.txt < lengths.txt > testgenout.txt";
	char command[] = "time -f \"\\t%E real\" ./sequence -ct -i testfile.txt > testout.txt";

	for (int i = 0; i < 2; i++) {
		system(gencommand);
		system(command);
	}

	return 0;
}
