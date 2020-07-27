#include "util.h"
#include "lcs.h"
#include <stdio.h>

void initializeCTable(int len1, int len2, tableType **lcsTable) {
	//sets first row, column zero as prep for lcs table calculations

	for (int i = 0; i <= len1; i++)
		lcsTable[i][0] = 0;
	for (int j = 0; j <= len2; j++)
		lcsTable[0][j] = 0;

	return;
}

void computeCTableEntries(int len1, int len2, char sequence1[], char sequence2[], tableType **lcsTable) {
	//fill out table entries with lcs lengths

	for (int i = 1; i <= len1; i++) { //skip zero column and row
		for (int j = 1; j <= len2; j++) {
			//if the sequence element corresponding to the table index is the same
			//then the lcs length is increased. Like the matches being removed and
			//concatenated as discussed in class
			if (sequence1[i-1] == sequence2[j-1])
				lcsTable[i][j] = lcsTable[i-1][j-1] + 1;
			//otherwise the current position is set to the greatest of DP(n-1,m), DP(n,m-1)
			else lcsTable[i][j] = (lcsTable[i-1][j] > lcsTable[i][j-1]) ? lcsTable[i-1][j] : lcsTable[i][j-1];
		}
	}
	return;
}

int traceLcs(int len1, int len2, char rLcs[], char sequence[], tableType **lcsTable) {
	//from the computed table put the lcs into the given rLcs array
	//returns the length

	int rLcsPos = 0;

	//using the table work from bottom right following
	//the longest previous adjacent lengths, where a decrease indicates
	//the current position is an addition of an lcs element
	while (len1 > 0 && len2 > 0) {
		if (lcsTable[len1][len2] == lcsTable[len1][len2 - 1]) --len2;
		else if (lcsTable[len1][len2] == lcsTable[len1 - 1][len2]) --len1;
		else {
			rLcs[rLcsPos] = sequence[len1-1]; //when finding elements add to rLcs
			rLcsPos++;
			--len1;
			--len2;
		}
	}
	return rLcsPos;
}

int computeLcs(int len1, int len2, char sequence1[], char sequence2[], bool printResult, FILE *out) {
	//performs all steps to compute and print an lcs
	//takes a boolean so other functions can utilise it without printing
	//returns the length of the computed lcs

	//declare lcs table array with space for the empty string at the begining
	tableType **lcsTable;
	allocateTable(len1, len2, &lcsTable);

	//initialize first row and col as zeroes (lcs of empty and anything is zero)
	initializeCTable(len1, len2, lcsTable);

	//fill table entries
	computeCTableEntries(len1, len2, sequence1, sequence2, lcsTable);

	//rLcs stands for reverse lcs as the lcs is determined backwards via the table
	//maxPossibleLen ensures rLcs is large enough to hold the longest possible lcs
	int maxPossibleLen = len1 > len2 ? len1 : len2;
	char rLcs[maxPossibleLen];

	//traceLcs fills out rLcs
	int lcsLength = traceLcs(len1, len2, rLcs, sequence1, lcsTable);

	if (printResult) {
		fprintf(out, "# an LCS (length = %d) for the two sequences is:\n", lcsLength);
		printSequence(lcsLength, rLcs, out);
	}
	
	//free allocated table
	freeTable(len1, &lcsTable);

	return lcsLength;
}

//below are the same lcs functions but repurposed to accommodate 4 sequences
int compute4WayLCS(int len1, int len2, int len3, int len4, char seq1[], char seq2[], char seq3[], char seq4[]) {
	//performs all steps to compute an lcs with 4 sequences
	//returns the length of the computed lcs

	//declare lcs quad array
	tableType ****lcsQuad;
	allocateQuad(len1, len2, len3, len4, &lcsQuad);

	//initialize first row of each dim as zeroes (lcs of empty and anything is zero)
	initializeCQuad(len1, len2, len3, len4, lcsQuad);

	//fill quad entries
	computeCQuadEntries(len1, len2, len3, len4, seq1, seq2, seq3, seq4, lcsQuad);

	//rLcs stands for reverse lcs as the lcs is determined backwards via the table
	//maxPossibleLen ensures rLcs is large enough to hold the longest possible lcs
	int maxPossibleLen = len1 < len2 ? len2 : len1;
	if (len3 > maxPossibleLen)
		maxPossibleLen = len3;
	if (len4 > maxPossibleLen)
		maxPossibleLen = len4;
	char rLcs[maxPossibleLen];

	//traceLcs fills out rLcs
	int lcsLength = trace4WayLCS(len1, len2, len3, len4, rLcs, seq1, lcsQuad);

	//free allocated table
	freeQuad(len1, len2, len3, &lcsQuad);

	return lcsLength;
}

void initializeCQuad(int len1, int len2, int len3, int len4, tableType ****quad) {

	for (int i = 0; i <= len1; i++)
		for (int j = 0; j <= len2; j++)
			for (int k = 0; k <= len3; k++)
				for (int l = 0; l <= len4; l++)
					quad[i][j][k][l] = 0;

	return;
}

void computeCQuadEntries(int len1, int len2, int len3, int len4, char seq1[], char seq2[], char seq3[], char seq4[], tableType ****lcsQuad) {
	//fill out table entries with lcs lengths

	for (int i = 1; i <= len1; i++) { //skip zero column and row
		for (int j = 1; j <= len2; j++) {
			for (int k = 1; k <= len3; k++) {
				for (int l = 1; l <= len4; l++) {
					//if the sequence element corresponding to the table index is the same
					//then the lcs length is increased. Like the matches being removed and
					//concatenated as discussed in class
					if (seq1[i-1] == seq2[j-1] && seq1[i-1] == seq3[k-1] && seq1[i-1] == seq4[l-1])
						lcsQuad[i][j][k][l] = lcsQuad[i-1][j-1][k-1][l-1] + 1;
					//otherwise the current position is set to the greatest of DP(n-1,m), DP(n,m-1)
					else {
						int max1, max2;
						max1 = (lcsQuad[i-1][j][k][l] > lcsQuad[i][j-1][k][l]) ? lcsQuad[i-1][j][k][l] : lcsQuad[i][j-1][k][l];
						max2 = (lcsQuad[i][j][k-1][l] > lcsQuad[i][j][k][l-1]) ? lcsQuad[i][j][k-1][l] : lcsQuad[i][j][k][l-1];
						lcsQuad[i][j][k][l] = (max1 > max2) ? max1 : max2;
					}
				}
			}
		}
	}
	return;
}

int trace4WayLCS(int len1, int len2, int len3, int len4, char rLcs[], char seq[], tableType ****lcsQuad) {
	//from the computed table put the lcs into the given rLcs array
	//returns the length

	int rLcsPos = 0;

	//using the table work from bottom right following
	//the longest previous adjacent lengths, where a decrease indicates
	//the current position is an addition of an lcs element
	while (len1 > 0 && len2 > 0 && len3 > 0 && len4 > 0) {
		if (lcsQuad[len1][len2][len3][len4] == lcsQuad[len1][len2][len3][len4 - 1]) --len4;
		else if (lcsQuad[len1][len2][len3][len4] == lcsQuad[len1][len2][len3 - 1][len4]) --len3;
		else if (lcsQuad[len1][len2][len3][len4] == lcsQuad[len1][len2 - 1][len3][len4]) --len2;
		else if (lcsQuad[len1][len2][len3][len4] == lcsQuad[len1 - 1][len2][len3][len4]) --len1;
		else {
			rLcs[rLcsPos] = seq[len1-1]; //when finding elements add to rLcs
			rLcsPos++;
			--len1;
			--len2;
			--len3;
			--len4;
		}
	}
	return rLcsPos;
}
