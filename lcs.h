#include <stdbool.h>
#include <stdio.h>

typedef short tableType;

void initializeCTable(int len1, int len2, tableType **lcsTable);
void computeCTableEntries(int len1, int len2, char sequence1[], char sequence2[], tableType **);
int traceLcs(int len1, int len2, char rLcs[], char sequence[], tableType **);
int computeLcs(int len1, int len2, char sequence1[], char sequence2[], bool printResult, FILE *out);

void initializeCQuad(int len1, int len2, int len3, int len4, tableType ****);
void computeCQuadEntries(int len1, int len2, int len3, int len4, char seq1[], char seq2[], char seq3[], char seq4[], tableType ****);
int trace4WayLCS(int len1, int len2, int len3, int len4, char rLcs[], char seq1[], tableType ****);
int compute4WayLCS(int len1, int len2, int len3, int len4, char seq1[], char seq2[], char seq3[], char seq4[]);
