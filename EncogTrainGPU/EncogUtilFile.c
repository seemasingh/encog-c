#include "encog.h"

void WriteValueInt(FILE *fp, char *name, int value)
{
	char buffer[MAX_LINE];
	itoa(value,buffer,10);
	fputs(name,fp);
	fputc('=',fp);
	fputs(buffer,fp);
	fputs("\n",fp);
}

void WriteValueBoolean(FILE *fp, char *name, int value)
{
	fputs(name,fp);
	fputc('=',fp);
	fputc(value?'T':'F',fp);
	fputs("\n",fp);
}

void WriteValueIntArray(FILE *fp, char *name, int *a, int count)
{
	char buffer[MAX_LINE];
	int i;

	fputs(name,fp);
	fputc('=',fp);
	for(i=0;i<count;i++)
	{
		if( i>0 )
		{
			fputc(',',fp);
		}
		itoa(a[i],buffer,10);
		fputs(buffer,fp);		
	}
	fputs("\n",fp);
}

void WriteValueDouble(FILE *fp, char *name, double value)
{
	fputs(name,fp);
	fputc('=',fp);
	fprintf(fp,"%.20g",value);	
	fputs("\n",fp);
}

void WriteValueDoubleArray(FILE *fp, char *name, double *a, int count)
{
	int i;
	fputs(name,fp);
	fputc('=',fp);
	for(i=0;i<count;i++)
	{
		if( i>0 )
		{
			fputc(',',fp);
		}
		fprintf(fp,"%.20g",a[i]);	
	}
	fputs("\n",fp);
}
