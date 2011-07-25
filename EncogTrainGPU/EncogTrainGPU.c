// EncogTrainGPU.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encog.h"


int main(int argc, char* argv[])
{
	if( argc<3 )
	{
		puts("EncogTrainGPU [Training File *.egb] [Network *.eg]\n");
	}
	else
	{
		char *trainingFile = argv[1];
		char *networkFile = argv[2];
		EGB_FILE *file = EncogEGBLoad(trainingFile);
		ENCOG_NETWORK *net = EncogNetworkLoad(networkFile);

		printf("Training records: %d\n", file->totalRecords);		
		EncogNetworkSave("d:\\data\\test.eg",net);

		EncogEGBFree(file);

		
	}
}

