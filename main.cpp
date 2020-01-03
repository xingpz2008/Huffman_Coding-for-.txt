#pragma warning(disable:4996)
#include<stdio.h>
#include"huffman.h"
void checkinput(int);

int main(void)
{
	int input;
	do{
		printf("1.Generate Test File\n2.File Compression\n3.File Extraction\nInput request:\n");
		scanf("%d",&input);
		checkinput(input);
	}while(1);
	return 0;
}

void checkinput(int input)
{
	switch(input)
	{
		case 1:
			{
				File_Generation();
				break;
			}
		case 2:
			{
				File_Compression();
				break;
			}
		case 3:
			{
				File_Extraction();
				break;
			}
		default:
			{
				
			}
	}
	return;
}
