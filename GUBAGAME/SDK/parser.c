#include "parser.h"


char carLimit[]={'&','{','}',' ','=',';',',',10,13,9,0};

uint32 compWord(mapSintaxe *list,char *w,int *type)
{uint32 j=-1;


	*type=-1;
	while(list->str!=0)
	{
		if(strcmp(list->str,w)==0)
		{	j=list->value;
			*type=list->type;
			break;
		}
		list++;
	}

	return j;
}

int getNextInt(char **buf)
{char word[50];
 int i;
  
	getNextWord(buf,word);
	i=(uint32)strtoul(word,0,10);	 // nao .. entao le o valor corrente

	return i;
}

char * getNextStr(char **buf)
{char word[100];
 char *str;

	getNextWord(buf,word);
	str=mymalloc(strlen(word)+1);
	strcpy(str,word);

	return str;
}


void getNextWord(char **buf,char *w)
{
	getNextWordTrunc(buf,w,99999999);
}

int getSizeWord(char *bi)
{char **buf=&bi;
	return getNextWordTrunc(buf,0,99999999);
}

int getNextWordTrunc(char **buf,char *w,int trunc)
{char *bi=*buf;
 int size=0;
 int str=0;
 int i=0;
  

 	if(w) *w=0;
	while(*bi!=0)
	{
		if(strrchr( &carLimit[i], *bi))
		{

			while(strrchr( &carLimit[i], *bi))
			{	if(*bi==0) break;
				bi++;
			}
	
			if(size>0) break;
		}
		else
		{
		
			if(*bi=='"') 
			{
				bi++;
				while ((*bi!='"') && (*bi!=0)) 
				{	if(size<trunc)
						if(w) *w++=*bi; 
					bi++;
					size++;
					if(w) *w=0;
				}
				bi++;
				*buf=bi; 
				break;
			}
			else
			{	if(size<trunc)
					if(w) *w++=*bi; 
				bi++;
				size++;
				if(w) *w=0;
			}
		}
	}

	*buf=bi; 

	return size;
	
}