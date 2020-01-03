#pragma warning(disable:4996)
#define WORDNUM 600
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
void File_Generation()
{
	FILE* fp=fopen("testfile.txt","w+");
	srand((unsigned)time(NULL));
	for(int i=0;i<WORDNUM;i++)
	{
		if(rand()%27==13)
		{
			fprintf(fp," ");
		}
		else
		{
			fprintf(fp,"%c",(rand()%26+65));
		}
		//fprintf(fp,"%d",0);
	}
	fclose(fp);
	return;
}
