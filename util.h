#include <stdio.h>

typedef short tableType;

//handle input
void get2Input(int *, int *, char [], char []);
void get1Input(int *, char []);
int getInputFromFile2(int *len1, int *len2, char seq1[], char seq2[], char *in);
int getInputFromFile1(int *len, char seq[], char *in);
int readValidateInput(char [], FILE *in);

//dynamic allocation for tables
void allocateTable(int len1, int len2, tableType ***table);
void freeTable(int len, tableType ***table);
//dynamic allocation for quads
void allocateQuad(int len1, int len2, int len3, int len4, tableType *****quad);
void freeQuad(int len1, int len2, int len3, tableType *****quad);

//function for printing sequences
void printSequence(int n, char seq[], FILE *);

//exit message for bad command line arguments
void reportBadArguments(void);

//generate sequences
void generateSequences(char *filename);
