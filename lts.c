#include "util.h"
#include "lcs.h"
#include "lts.h"
#include <string.h>
#include <stdio.h>

void computeLts(int len, char sequence[], FILE *out) {
	//finds an lts by splitting sequence into two parts for all possible pivot points and finding lcs

	int maxmid = 0;

	//find longest tandem length of the given sequence
	int bestLength = findLongestTandem(len, sequence, &maxmid);

	//using the determined best index to split on, compute corresponding lcs
	int leftLen = maxmid;
	int rightLen = len - maxmid;
	char subseq1[leftLen];
	char subseq2[rightLen];
	memcpy(subseq1, sequence, sizeof(subseq1)); //use memcpy to create a 2 temp substrings
	memcpy(subseq2, &sequence[maxmid], sizeof(subseq2));

	//perform the steps of an lcs
	tableType **ltsTable;
	allocateTable(leftLen, rightLen, &ltsTable);
	initializeCTable(leftLen, rightLen, ltsTable);
	computeCTableEntries(leftLen, rightLen, subseq1, subseq2, ltsTable);
	char tandem[bestLength * 2]; //the tandem sequence is the lcs repeated so double the length is needed
	traceLcs(leftLen, rightLen, tandem, subseq1, ltsTable);

	//duplicate lcs to form tandem sequence
	for (int i = 0; i < bestLength; i++)
		tandem[bestLength + i] = tandem[i];

	fprintf(out, "# an LTS (length = %d) for the first sequence is:\n", bestLength * 2);
	printSequence(bestLength * 2, tandem, out);

	freeTable(leftLen, &ltsTable);

	return;
}

int findLongestTandem(int len, char sequence[], int *maxmid) {
	//with the given len and sequence many lcs are found to determine the lts
	//the index associated with the lts is put into the provided int pointer location
	//returns the longest encountered common subsequence -> the longest tandem

	int longest = 0;

	//the split position moves down the sequence as long as a longer tandem is possible
	for (int i = 1; i < len && (len - i) > longest; i++) {
		int leftLen = i;
		int rightLen = len - i;
		char subseq1[leftLen];
		char subseq2[rightLen];
		memcpy(subseq1, sequence, sizeof(subseq1));
		memcpy(subseq2, &sequence[i], sizeof(subseq2));
		int tandemLength = computeLcs(leftLen, rightLen, subseq1, subseq2, false, NULL);
		if (tandemLength > longest) {
			longest = tandemLength;
			*maxmid = i;
		}
	}

	return longest;
}

void computeLcts(int len1, int len2, char seq1[], char seq2[], FILE *out) {
	//finds lcts by finding longest lcs across the two part decomp of both input sequences

	int maxmid1 = 0, maxmid2 = 0;

	//find longest tandem length between the given sequences
	int bestLength = findLongestCommonTandem(len1, len2, seq1, seq2, &maxmid1, &maxmid2);

	//using the determined best index to split on, compute corresponding lcs
	int leftLen1 = maxmid1;
	int rightLen1 = len1 - maxmid1;
	int leftLen2 = maxmid2;
	int rightLen2 = len2 - maxmid2;
	char subseq11[leftLen1];
	char subseq12[rightLen1];
	char subseq21[leftLen2];
	char subseq22[rightLen2];
	 //use memcpy to create 4 temp substrings
	memcpy(subseq11, seq1, sizeof(subseq11));
	memcpy(subseq12, &seq1[maxmid1], sizeof(subseq12));
	memcpy(subseq21, seq2, sizeof(subseq21));
	memcpy(subseq22, &seq2[maxmid2], sizeof(subseq22));

	//perform the steps of an lcs
	tableType ****lctsQuad;
	allocateQuad(leftLen1, rightLen1, leftLen2, rightLen2, &lctsQuad);
	initializeCQuad(leftLen1, rightLen1, leftLen2, rightLen2, lctsQuad);
	computeCQuadEntries(leftLen1, rightLen1, leftLen2, rightLen2, subseq11, subseq12, subseq21, subseq22, lctsQuad);
	//the tandem sequence is the lcs repeated so double the length is needed
	char tandem[bestLength * 2]; 
	trace4WayLCS(leftLen1, rightLen1, leftLen2, rightLen2, tandem, subseq11, lctsQuad);

	//duplicate lcs to form tandem sequence
	for (int i = 0; i < bestLength; i++)
		tandem[bestLength + i] = tandem[i];

	fprintf(out, "# an LCTS (length = %d) for the two sequences is:\n", bestLength * 2);
	printSequence(bestLength * 2, tandem, out);

	freeQuad(leftLen1, rightLen1, leftLen2, &lctsQuad);

	return;
}

int findLongestCommonTandem(int len1, int len2, char seq1[], char seq2[], int *maxmid1, int *maxmid2) {
	//with the given len and sequence many lcs are found to determine the lts
	//the index associated with the lts is put into the provided int pointer location
	//returns the longest encountered common subsequence -> the longest tandem

	int longest = 0;

	//the split position moves down the sequence as long as a longer tandem is possible
	for (int i = 1; i < len1 && (len1 - i) > longest; i++) {
		//break seq1 into two parts
		int leftLen1 = i;
		int rightLen1 = len1 - i;
		char subseq11[leftLen1];
		char subseq12[rightLen1];
		memcpy(subseq11, seq1, sizeof(subseq11));
		memcpy(subseq12, &seq1[i], sizeof(subseq12));
		for (int j = 1; j < len2 && (len2 - j) > longest; j++) {
			//break seq2 into 2 parts
			int leftLen2 = j;
			int rightLen2 = len2 - j;
			char subseq21[leftLen2];
			char subseq22[rightLen2];
			memcpy(subseq21, seq2, sizeof(subseq21));
			memcpy(subseq22, &seq2[j], sizeof(subseq22));
			//find length of corresponding lcs
			int tandemLength = compute4WayLCS(leftLen1, rightLen1, leftLen2, rightLen2, subseq11, subseq12, subseq21, subseq22);
			if (tandemLength > longest) {
				longest = tandemLength;
				*maxmid1 = i;
				*maxmid2 = j;
			}
		}
	}

	return longest;
}
