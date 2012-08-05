typedef int uint32;

typedef struct _mapSintaxe
{
	char *str;
	uint32 value;
	int type;
	
} mapSintaxe;


void getNextWord(char **buf,char *w);
int getSizeWord(char *bi);
int getNextInt(char **buf);
char * getNextStr(char **buf);
int getNextWordTrunc(char **buf,char *w,int trunc);

uint32 compWord(mapSintaxe *list,char *w,int *type);