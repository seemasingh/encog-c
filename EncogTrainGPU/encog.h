#ifndef __ENCOG_H
#define __ENCOG_H

#include <SDKDDKVer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 256

#define ACTIVATION_LINEAR 0
#define ACTIVATION_TANH 1
#define ACTIVATION_SIGMOID 2

typedef struct {
	char ident[8];
	double input;
	double ideal;
} EGB_HEADER;

typedef struct {
	int idealCount;
	int inputCount;
	int recordCount;
	int recordSize;
	int totalRecords;
	double *data;
} EGB_FILE;

typedef struct {
	int beginTraining;
	double connectionLimit;
	int *contextTargetOffset;
	int *contextTargetSize;
	int endTraining;
	int hasContext;
	int inputCount;
	int *layerCounts;
	int *layerFeedCounts;
	int *layerContextCount;
	int *layerIndex;
	double *output;
	int outputCount;
	int *weightIndex;
	double *weights;
	double *biasActivation;
	int layerCount;
	int weightCount;
	int neuronCount;
	int *activationFunction;
} ENCOG_NETWORK;

EGB_FILE *EncogEGBLoad(char *f);
void EncogEGBFree(EGB_FILE *f);
ENCOG_NETWORK *EncogNetworkLoad(char *name);
void EncogNetworkFree(ENCOG_NETWORK *network);

int PopLine(char *line, char *arg, int start, int len);
int IsWhiteSpace(char ch);
void Trim(char *line);
void StripQuotes(char *line);
void ParseNV(char *line, char *name, char *value, size_t len);
void StripCRLF(char *str);
int CountValues(char *line);
int *ParseIntList(char *line);
int ParseBoolean(char *line);
double *ParseDoubleList(char *line);
void EncogNetworkSave(char *name, ENCOG_NETWORK *network);

void WriteValueInt(FILE *fp, char *name, int value);
void WriteValueBoolean(FILE *fp, char *name, int value);
void WriteValueIntArray(FILE *fp, char *name, int *a, int count);
void WriteValueDouble(FILE *fp, char *name, double value);
void WriteValueDoubleArray(FILE *fp, char *name, double *a, int count);

void EncogNetworkCompute(ENCOG_NETWORK *network, double *inputData, double *outputData );

#endif;