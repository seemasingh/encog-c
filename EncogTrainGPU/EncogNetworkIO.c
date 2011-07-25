#include <SDKDDKVer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "encog.h"


int CheckNetwork(FILE *fp)
{
	int index, v;
	char line[MAX_LINE];
	char arg[MAX_LINE];

	fgets(line,sizeof(line),fp);

	index = 0;

	/* Make sure this is an Encog file */
	index = PopLine(line,arg,index,sizeof(arg));
	if( strcmp(arg,"encog") )
	{
		return -1;
	}

	/* Make sure this is a BasicNetwork */
	index = PopLine(line,arg,index,sizeof(arg));
	if( strcmp(arg,"BasicNetwork") )
	{
		return -1;
	}

	/* Encog platform */
	index = PopLine(line,arg,index,sizeof(arg));

	/* Encog version */
	index = PopLine(line,arg,index,sizeof(arg));

	/* File version */
	index = PopLine(line,arg,index,sizeof(arg));
	v = atoi(arg);

	if( v>1 ) 
	{
		return -1;
	}

	return 0;
}


void LoadBasic(char *line, ENCOG_NETWORK *network)
{
	char name[MAX_LINE],value[MAX_LINE];

	StripCRLF(line);
	ParseNV(line,name,value,MAX_LINE);

	if(!strcmp(name,"beginTraining") )
	{
		network->beginTraining = atoi(value);
	}
	else if(!strcmp(name,"connectionLimit") )
	{
		network->connectionLimit = atof(value);
	}
	else if(!strcmp(name,"contextTargetOffset") )
	{
		network->contextTargetOffset = ParseIntList(value);
	}
	else if(!strcmp(name,"contextTargetSize") )
	{
		network->contextTargetSize = ParseIntList(value);
	}
	else if(!strcmp(name,"endTraining") )
	{
		network->endTraining = atoi(value);
	}
	else if(!strcmp(name,"hasContext") )
	{
		network->hasContext = ParseBoolean(value);
	}
	else if(!strcmp(name,"inputCount") )
	{
		network->inputCount = atoi(value);
	}
	else if(!strcmp(name,"layerCounts") )
	{
		network->layerCount = CountValues(value);
		network->layerCounts = ParseIntList(value);
		network->activationFunction = (int*)malloc(sizeof(int)*network->layerCount);
	}
	else if(!strcmp(name,"layerFeedCounts") )
	{
		network->layerFeedCounts = ParseIntList(value);
	}
	else if(!strcmp(name,"layerContextCount") )
	{
		network->layerContextCount = ParseIntList(value);
	}
	else if(!strcmp(name,"layerIndex") )
	{
		network->layerIndex = ParseIntList(value);
	}
	else if(!strcmp(name,"output") )
	{
		network->neuronCount = CountValues(value);
		network->output = ParseDoubleList(value);
	}
	else if(!strcmp(name,"outputCount") )
	{
		network->outputCount = atoi(value);
	}
	else if(!strcmp(name,"weightIndex") )
	{
		network->weightIndex = ParseIntList(value);
	}
	else if(!strcmp(name,"weights") )
	{
		network->weightCount = CountValues(value);
		network->weights = ParseDoubleList(value);
	}
	else if(!strcmp(name,"biasActivation") )
	{
		network->biasActivation = ParseDoubleList(value);
	}	
}

void LoadActivation(char *line, ENCOG_NETWORK *network, int currentActivation)
{
	StripQuotes(line);
	if( !strcmp(line,"ActivationLinear") )
	{
		network->activationFunction[currentActivation] = ACTIVATION_LINEAR;
	}
	else if( !strcmp(line,"ActivationSigmoid") )
	{
		network->activationFunction[currentActivation] = ACTIVATION_SIGMOID;
	}
	else if( !strcmp(line,"ActivationTANH") )
	{
		network->activationFunction[currentActivation] = ACTIVATION_TANH;
	}
}

ENCOG_NETWORK *EncogNetworkLoad(char *name)
{
	char line[MAX_LINE];
	ENCOG_NETWORK *result;
	int mode, currentActivation;

	FILE *fp = fopen(name,"r");

	if( CheckNetwork(fp) == -1 )
	{
		return NULL;
	}

	result = (ENCOG_NETWORK *)malloc(sizeof(ENCOG_NETWORK));
	mode = 0;
	currentActivation = 0;

	while( fgets(line,sizeof(line),fp) ) 
	{
		StripCRLF(line);

		if(!strcmp(line,"[BASIC:NETWORK]") )
		{
			mode = 1;
		}
		else if(!strcmp(line,"[BASIC:ACTIVATION]") )
		{
			mode = 2;
		}
		else 
		{
			switch(mode)
			{
				case 1:
					LoadBasic(line,result);
					break;
				case 2:
					LoadActivation(line,result,currentActivation++);
					break;
			}
		}		
	}

	fclose(fp);
	return result;
}

void EncogNetworkSave(char *name, ENCOG_NETWORK *network)
{
	char line[MAX_LINE];
	int i;
	FILE *fp;
	time_t t;

	/* Write the header line */
	fp = fopen(name,"w");
	*line=0;
	strcat(line,"encog");
	strcat(line,",");
	strcat(line,"BasicNetwork");
	strcat(line,",");
	strcat(line,"c++");
	strcat(line,",");
	strcat(line,"3.0");
	strcat(line,",");
	strcat(line,"1");
	strcat(line,",");
	
	time(&t);
	ltoa((long)t,line+strlen(line),10);
	fputs(line,fp);
	fputs("\n[BASIC]\n",fp);
	fputs("[BASIC:PARAMS]\n",fp);
	fputs("[BASIC:NETWORK]\n",fp);
	WriteValueInt(fp,"beginTraining",network->beginTraining);
	WriteValueDouble(fp,"connectionLimit",network->connectionLimit);
	WriteValueIntArray(fp,"contextTargetOffset",network->contextTargetOffset,network->layerCount);
	WriteValueIntArray(fp,"contextTargetSize",network->contextTargetSize,network->layerCount);
	WriteValueInt(fp,"endTraining",network->endTraining);
	WriteValueBoolean(fp,"hasContext",network->hasContext);
	WriteValueInt(fp,"inputCount",network->inputCount);
	WriteValueIntArray(fp,"layerCounts",network->layerCounts,network->layerCount);
	WriteValueIntArray(fp,"layerFeedCounts",network->layerFeedCounts,network->layerCount);
	WriteValueIntArray(fp,"layerContextCount",network->layerContextCount,network->layerCount);
	WriteValueIntArray(fp,"layerIndex",network->layerIndex,network->layerCount);
	WriteValueDoubleArray(fp,"output",network->output,network->neuronCount);
	WriteValueInt(fp,"outputCount",network->outputCount);
	WriteValueIntArray(fp,"weightIndex",network->weightIndex,network->layerCount);
	WriteValueDoubleArray(fp,"weights",network->weights,network->weightCount);
	WriteValueDoubleArray(fp,"biasActivation",network->biasActivation,network->layerCount);
	fputs("[BASIC:ACTIVATION]\n",fp);
	for(i=0;i<network->layerCount;i++)
	{
		fputc('\"',fp);
		switch(network->activationFunction[i])
		{
			case ACTIVATION_LINEAR:
				fputs("ActivationLinear",fp);
				break;
			case ACTIVATION_TANH:
				fputs("ActivationTANH",fp);
				break;
			case ACTIVATION_SIGMOID:
				fputs("ActivationSigmoid",fp);
				break;
		}
		fputs("\"\n",fp);
	}

	/* Write the basic info */

	/* Write the activation functions */

	fclose(fp);
}