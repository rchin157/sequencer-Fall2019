#include <stdio.h>

typedef short tableType;

void computeLts(int, char [], FILE *out);
int findLongestTandem(int, char [], int *);

void computeLcts(int len1, int len2, char seq1[], char seq2[], FILE *out);
int findLongestCommonTandem(int len1, int len2, char seq1[], char seq2[], int *maxmid1, int *maxmid2);

