#include "encog.h"

#include <SDKDDKVer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void EncogEGBFree(EGB_FILE *f)
{
	free(f->data);
	free(f);
}

EGB_FILE *EncogEGBLoad(char *f)
{
	EGB_HEADER header;
	EGB_FILE *result;
	FILE *fp;
	long s;

	fp = fopen(f,"rb");
	fseek(fp,0,SEEK_END);
	s = ftell(fp);
	fseek(fp,0,SEEK_SET);
	fread(&header,sizeof(EGB_HEADER),1,fp);
	if( memcmp("ENCOG-00",header.ident,8) )
	{
		printf("Invalid training file.\n");
		fclose(fp);
		return NULL;
	}

	result = (EGB_FILE*)malloc(sizeof(EGB_FILE));
	result->idealCount = (int)header.input;
	result->inputCount = (int)header.ideal;
	result->recordCount = result->idealCount + result->inputCount + 1;
	result->recordSize = result->recordCount * sizeof(double);
	result->totalRecords = (s-24)/result->recordSize;

	result->data = (double*)malloc(result->totalRecords*result->recordSize);
	fread(result->data,1,result->totalRecords*result->recordSize,fp);

	fclose(fp);
	return result;
}