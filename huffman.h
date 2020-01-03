#pragma once
#define ABNUM 27
extern int _ABNUM;
#define LEN_QUEUE 17
#define OPSTR "output.xpz"
#define EXTSTR "extracted_file.txt"
#define BGN_STR "i0sojKxqGomw0KoM"
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#define LOG "log.dat"


void File_Generation();
void File_Compression();
void File_Extraction();

typedef struct Tree{
	char c;
	int frq;
	Tree* left;
	Tree* right;
}Tree;

typedef struct FrqNode{
	Tree* ptrNode;
}FrqNode;

typedef struct Huffman_Index{
	char c;
	char code[ABNUM];
}Huffman_Index;

class stack{
	private:
		char data[ABNUM];
		char* iptr;
		int num;
	public:
		stack()
		{
			for(int i=0;i<ABNUM;i++)
			{
				data[i]='\0';
			}
			iptr=&data[0];
			num=0;
		}
		void push(char c)
		{
			*iptr=c;
			//printf("%c was pushed into %d\n",c,num);
			iptr++;
			num++;
		}
		char pop()
		{
			iptr--;
			char temp=*iptr;
			*iptr='\0';
			num--;
			return temp;
		}
		char* codebck()
		{
			return data;
		}
};

class queue {
	private:
		char data[LEN_QUEUE];
		int num;
		char* ptr_in;
		int fullflag;
	public:
		queue()
		{
			for (int i = 0; i < LEN_QUEUE; i++)
			{
				data[i] = '\0';
			}
			num = 0;
			ptr_in = data;
			fullflag = 0;
		}
		char pop()
		{
			char res = data[0];
			char* ptr_f = data;
			char* ptr_r = ptr_f + 1;
			while (ptr_f!=&data[LEN_QUEUE-1]&&*ptr_r!='\0')
			{
				*ptr_f = *ptr_r;
				ptr_f++;
				ptr_r++;
			}
			*ptr_f = '\0';
			num--;
			//printf("queue with %d data...\n", num);
			if (!fullflag)
			{
				ptr_in--;
			}
			else
			{
				fullflag = 0;
			}
			return res;
		}
		void push(char c)
		{
			if (!fullflag)
			{
				*ptr_in = c;
				num++;
				if (ptr_in == &data[LEN_QUEUE - 1])
				{
					fullflag = 1;
				}
				else
				{
					ptr_in++;
				}
				//printf("queue with %d data...\n", num);
				return;
			}
			//printf("queue full\n");
			//exit(2);
		}
		void check(FILE* fp,const char mode)
		{
			if (mode == 'W')
			{
				unsigned char res = 0;
				if (num >= 8)
				{
					for (int i = 0; i < 8; i++)
					{
						res += (data[0] - 48) * pow(2,7-i);
						pop();
					}
					fprintf(fp, "%c", res);
				}
				
			}
			if (mode == 'C'&&num)
			{
				unsigned char res = 0;
				for (int i = 0; i < num; i++)
					{
						res += (data[0] - 48) * pow(2, 7 - i);
						pop();
					}
				fprintf(fp, "%c", res);
			}
		}
		char check(const char mode, Huffman_Index Index[])
		{
			if ('E' == mode) //extract
			{
				char str[LEN_QUEUE];
				for (int i = 0; i < LEN_QUEUE - 1; i++)
				{
					str[i] = '\0';
				}
				for (int i = 0; i < num; i++)
				{
					str[i] = data[i];
					for (int j = 0; j < _ABNUM; j++)
					{
						if (!strcmp(str, Index[j].code))
						{
							int len = strlen(str);
							//FILE* fp = fopen(LOG, "w");
							//fprintf(fp,"%s %c\n", str, Index[j].c);
							//fclose(fp);
							for (int k = 0; k < len; k++)
							{
								pop();
							}
							return Index[j].c;
						}
					}
				}
				/*if (num >= strlen(Index[_ABNUM-1].code))
				{
					printf("No result occurred. Data Stream:");
					for (int i = 0; i < num; i++)
					{
						printf("%c", data[i]);
					}
					printf("\n");
				}*/
			}
			return 0;
		}
		~queue()
		{
			ptr_in = NULL;
		}
};


Tree* CreateTree(FrqNode frq_heap[]);
void FrqListSort(FrqNode frq_heap[],int size);
Tree* HeapPop(FrqNode frq_heap[]);
void HeapInsert(FrqNode Node,FrqNode frq_heap[]);
void GetIndex(Tree* ptr);
void PrintIndex();
void IndexInjection(FILE* fp,int Total);
int ReadIndex(FILE* fp);
void Extract(FILE* fp_ori);
char* c2bi(unsigned char c);


