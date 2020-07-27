#include "lps.h"
#include "util.h"
#include "lcs.h"

#include <stdio.h>

void computeLps(int len, char sequence[], FILE *out) {
	//computes a lcs with the reverse of the given sequence and itself to find an lps

	if (len == 0) {
		fprintf(out, "# an LPS (length = 0) for the second sequence is:\n\n");
		return;
	}
	else if (len == 1) {
		fprintf(out, "# an LPS (length = 1) for the second sequence is:\n%c\n", sequence[0]);
		return;
	}

	//create a reversed copy of the given sequence
	char reverse[len + 1];
	reverse[len] = '\0';
	for (int i = 0; i < len; i++)
		reverse[i] = sequence[len - 1 - i];

	//perform the steps of an lcs
	tableType **lpsTable;
	allocateTable(len, len, &lpsTable);
	initializeCTable(len, len, lpsTable);
	computeCTableEntries(len, len, sequence, reverse, lpsTable);

	//palindrome is the reverse lps as the lps is determined backwards via the table
	char palindrome[len];
	int lpsLength = traceLcs(len, len, palindrome, sequence, lpsTable);

	
	for (int i = 0; i < lpsLength / 2; i++)
		palindrome[lpsLength - 1 - i] = palindrome[i];
	palindrome[lpsLength] = '\0';
	
	fprintf(out, "# an LPS (length = %d) for the first sequence is:\n", lpsLength);
	fprintf(out, "%s\n", palindrome);

	freeTable(len, &lpsTable);

	return;
}

void computeLcps(int len1, int len2, char seq1[], char seq2[], FILE *out) {
	//finds lcps by finding lcs across both sequences and their reverses

	if (len1 == 0 || len2 == 0) {
		fprintf(out, "# an LCPS (length = 0) is: \n\n");
		return;
	}

	//reverse sequences
	char reverse1[len1 + 1], reverse2[len2 + 1];
	reverse1[len1] = '\0';
	reverse2[len2] = '\0';

	for (int i = 0; i < len1; i++)
		reverse1[i] = seq1[len1 - 1 - i];
	for (int i = 0; i < len2; i++)
		reverse2[i] = seq2[len2 - 1 - i];

	//perform lcs across all four
	tableType ****lcpsQuad;
	allocateQuad(len1, len1, len2, len2, &lcpsQuad);
	initializeCQuad(len1, len1, len2, len2, lcpsQuad);
	computeCQuadEntries(len1, len1, len2, len2, seq1, reverse1, seq2, reverse2, lcpsQuad);

	//find length
	int max = len1 < len2 ? len1 : len2;
	char palindrome[max];
	int lcpsLength = trace4WayLCS(len1, len1, len2, len2, palindrome, seq1, lcpsQuad);

	for (int i = 0; i < lcpsLength / 2; i++)
		palindrome[lcpsLength - 1 - i] = palindrome[i];
	palindrome[lcpsLength] = '\0';

	fprintf(out, "# an LCPS (length = %d) is:\n%s\n", lcpsLength, palindrome);

	freeQuad(len1, len1, len2, &lcpsQuad);

	return;
}
