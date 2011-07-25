#include <SDKDDKVer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "encog.h"

int PopLine(char *line, char *arg, int start, int len) 
{
	char *argPtr,*linePtr;
	int stop;

	argPtr = arg;
	linePtr = line + start;
	stop = len-1;

	while( *linePtr && (argPtr-arg)<stop )
	{
		if( *linePtr==',' ) 
		{
			break;
		} 
		else 
		{
			*(argPtr++) = *(linePtr++);
		}		
	}

	*argPtr = 0;

	return (linePtr - line)+1;
}

int IsWhiteSpace(char ch)
{
	return ( ch==' ' || ch=='\t' || ch=='\n' || ch=='\r' );
}

void Trim(char *line)
{
	char *ptr;
	/* first trim the end */

	ptr = line + strlen(line)-1;
	while( IsWhiteSpace(*ptr) )
	{
		*ptr = 0;
		ptr--;
	}

	/* now trim the beginning */
	ptr = line;
	while( IsWhiteSpace(*ptr) )
	{
		ptr++;
	}

	strncpy(ptr,line,strlen(line));
}

void StripQuotes(char *line)
{
	char quoteChar = 0;

	if( *line=='\"' || *line=='\'' )
	{
		quoteChar = *line;
		strncpy(line,line+1,strlen(line));		
	}

	if( quoteChar && line[strlen(line)-1]==quoteChar )
	{
		line[strlen(line)-1] = 0;
	}
}

void ParseNV(char *line, char *name, char *value, size_t len)
{
	char *p = strchr(line,'=');
	
	if( p==NULL )
	{
		*name = *value = 0;
		return;
	}

	*p = 0;

	strncpy(name,line,len);
	strncpy(value,p+1,len);

	Trim(name);
	Trim(value);

	/* Restore */
	*p = '=';
}


void StripCRLF(char *str) 
{
	while( str[strlen(str)-1]=='\n' || str[strlen(str)-1]=='\r' )
	{
		str[strlen(str)-1] = 0;
	}
}

int CountValues(char *line)
{
	int result;
	char *ptr;

	/* count and see how many numbers */
	result = 1;
	ptr = line;
	while(*ptr)
	{
		if( *ptr==',' )
		{			
			result++;
		}
		ptr++;
	}

	return result;
}

int *ParseIntList(char *line)
{
	int size, index, i;
	int *result;
	char arg[MAX_LINE];

	size = CountValues(line);	
	result = (int*)malloc(size*sizeof(int));

	index = 0;
	for(i = 0; i<size; i++ )
	{
		index = PopLine(line, arg, index, sizeof(arg));
		result[i] = atoi(arg);
	}
	return result;
}

int ParseBoolean(char *line) 
{
	return( toupper(*line)=='T' );
}

double *ParseDoubleList(char *line)
{
	int size, index, i;
	double *result;
	char arg[MAX_LINE];

	size = CountValues(line);		
	result = (double*)malloc(size*sizeof(double));

	index = 0;
	for(i = 0; i<size; i++ )
	{
		index = PopLine(line, arg, index, sizeof(arg));
		result[i] = atof(arg);
	}
	return result;
}

