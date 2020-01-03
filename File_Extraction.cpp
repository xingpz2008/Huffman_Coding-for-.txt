#pragma warning(disable:4996)
#pragma once
#include<ctype.h>
#include<string.h>
#include"huffman.h"
void PrintIndex();
Huffman_Index Index_R[ABNUM];
int _ABNUM=-1;
int ext_total = 0;
queue extqueue;

void File_Extraction()
{
	FILE* fp;
	
	printf("Input File Route:\n");
	char str[50];
	input:scanf("%s", str);
	if (!(fp = fopen(str, "rb")))
	{
		printf("[ERROR]File Error!\n");
		goto input;
	}
	if (!ReadIndex(fp))
	{
		printf("[ERROR]Only Standard .xpz File can be processed\n");
		return;
	}
	Extract(fp);
	fclose(fp);
	return;
}

int ReadIndex(FILE* fp)
{
	for (int i = 0; i < ABNUM; i++)			//initialization
	{
		for (int j = 0; j < ABNUM; j++)
		{
			Index_R[i].code[j] = '\0';
		}
	}
	char temp;
	char signstr[17];
	int status = fscanf(fp, "%s", signstr);
	if (!status||strcmp(signstr,BGN_STR))
	{
		printf("[ERROR]Syntax Error\n");
		return 0;
	}
	fscanf(fp, "%d", &ext_total);
	int flag = 1;
	int spaceflag = 1;
	do {
		fscanf(fp, "%c", &temp);
		switch (temp)
		{
			case '/':
			{
				break;
			}
			case '\n':
			{
				_ABNUM++;
				flag = 1;
				break;
			}
			case '0':
			{
				strcat(Index_R[_ABNUM].code, "0");
				break;
			}
			case '1':
			{
				strcat(Index_R[_ABNUM].code, "1");
				break;
			}
			default:
			{
				if (isalpha(temp))
				{
					Index_R[_ABNUM].c = temp;
					flag = 0;
				}
				if (spaceflag&&temp == ' ' && flag)
				{
					Index_R[_ABNUM].c = temp;
					spaceflag = 0;
				}
				break;
			}
		}
	} while (_ABNUM < ABNUM);
	//PrintIndex();
	//fscanf(fp, "%c", &temp);//absorb the next "/"
	printf("[INFO]Index Extracted from Original File.\n");
	return 1;
}

void Extract(FILE* fp_ori)
{
	printf("[INFO]Extraction in Progress.\n");
	FILE* fp_op = fopen(EXTSTR, "w+");
	int num = 0;
	char val;
	while (num < ext_total-1)
	{
		fscanf(fp_ori, "%c", &val);
		char str[9];
		for (int i = 0; i < 8; i++)
		{
			str[i] = '\0';
		}
		char* temp= c2bi((unsigned char)val);
		strcpy(str, temp);
		free(temp);
		for (int i = 0; i < 8; i++)
		{
			extqueue.push(str[i]);
			char res=0;
			do {
				res = extqueue.check('E',Index_R);
				if (res)
				{
					num++;
					//printf("%d...\n", num);
					fprintf(fp_op, "%c", res);
				}
			} while (res);
			//free(str);
		}
	}
	fclose(fp_op);
	printf("[INFO]Extraction Finalized.\n");
}

void PrintIndex()
{
	for (int i = 0; i < ABNUM; i++)
	{
		printf("%c %s\n", Index_R[i].c, Index_R[i].code);
	}
}

char* c2bi(unsigned char c)//free needed
{
	
	char* str = (char*)calloc(9, sizeof(char));
	for (int i = 0; i < 8; i++)
	{
		str[i] = '0';
	}
	str[8] = '\0';
	char* ptr = &str[7];
	while (c)
	{
		*ptr = c % 2+48;
		c = (c - (c % 2)) / 2;
		ptr--;
	}
	return str;
}