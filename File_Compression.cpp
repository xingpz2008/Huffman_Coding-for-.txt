#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"huffman.h"
//the function is currently designed for 26+1 alphabet file exclusively
Huffman_Index Index[ABNUM];
Huffman_Index* idxptr=Index;
stack idxstack;
queue qi;

void File_Compression()
{
	FILE* fp;
	FILE* fpop;
	char str[50];
	FrqNode frq_heap[ABNUM]; 
	for(int i=0;i<27;i++)	
	{
		frq_heap[i].ptrNode=(Tree*)malloc(sizeof(Tree));
		frq_heap[i].ptrNode->c=i+65;
		frq_heap[i].ptrNode->left=NULL;
		frq_heap[i].ptrNode->right=NULL;
		frq_heap[i].ptrNode->frq=0;
	}
	frq_heap[26].ptrNode->c=' ';
	printf("Input File Route:\n");
	input:scanf("%s",str);
	if (!(fp = fopen(str, "r")))
	{
		printf("[ERROR]File Error!\n");
		goto input;
	}
	char temp;
	printf("[INFO]File Compression in Progress.\n");
	while(!feof(fp))
	{
		fscanf(fp,"%c",&temp);
		if(temp==' ')
		{
			frq_heap[26].ptrNode->frq++;
		}
		else
		{
			frq_heap[temp-65].ptrNode->frq++;
		}
	}
	fclose(fp);
	FrqListSort(frq_heap,27);
	Tree* ptr = CreateTree(frq_heap);
	if (ptr)
	{
		printf("[INFO]Huffman Code Index Generated.\n");
	}
	else
	{
		printf("[ERROR]Huffman Code Index Generation Failed.\n");
		return;
	}
	for(int i=0;i<ABNUM;i++)			//initialization
	{
		for(int j=0;j<ABNUM;j++)
		{
			Index[i].code[j]='\0';
		}
	}
	GetIndex(ptr);
	//PrintIndex();
	fp = fopen(str, "r");
	if (!(fpop = fopen(OPSTR, "wb")))
	{
		printf("[ERROR]Conpression Error: Cannot create file.\n");
		return;
	}
	IndexInjection(fpop,ptr->frq);
	while (!feof(fp))
	{
		fscanf(fp, "%c", &temp);
		int id = 0;
		do {
			if (Index[id].c == temp)
			{
				break;
			}
			id++;
		} while (1);
		int len = strlen(Index[id].code);
		for (int i = 0; i < len; i++)
		{
			qi.push(Index[id].code[i]);
			qi.check(fpop, 'W');
		}
	}
	qi.check(fpop, 'C');				//clear all the queue
	fclose(fpop);
	printf("[INFO]File Compression Finalized.\n");
	return;
}

void FrqListSort(FrqNode frq_heap[],int size)
{
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size-1-i;j++)
		{
			if(frq_heap[j].ptrNode->frq>frq_heap[j+1].ptrNode->frq)
			{
				FrqNode temp=frq_heap[j];
				frq_heap[j]=frq_heap[j+1];
				frq_heap[j+1]=temp;
			}
		}
	}
	/*for(int i=0;i<27;i++)
	{
		printf("%c %d\n",frq_heap[i].ptrNode->c,frq_heap[i].ptrNode->frq);
	}*/
	return;
}

//this function assumes that the heap is a min_heap already
Tree* HeapPop(FrqNode frq_heap[])
{
	Tree* temp=frq_heap[0].ptrNode;
	FrqNode* ptr=&frq_heap[1];
	while(ptr<=&frq_heap[26]&&ptr->ptrNode)
	{
		*(ptr-1)=*ptr;
		ptr++;
	}
	ptr--;
	ptr->ptrNode=NULL;
	return temp;
}

void HeapInsert(FrqNode Node,FrqNode frq_heap[])
{
	FrqNode* ptr=frq_heap;
	while(ptr->ptrNode&&Node.ptrNode->frq>ptr->ptrNode->frq)
	{
		ptr++;
	}
	FrqNode temp=Node;
	do{
		Node=temp;
		temp=*ptr;
		*ptr=Node;
		ptr++;
	}while(ptr->ptrNode);
	*ptr=temp;
	return;
}

Tree* CreateTree(FrqNode frq_heap[])
{
	Tree* ptr=NULL;
	for(int i=1;i<ABNUM;i++)
	{
		ptr=(Tree*)malloc(sizeof(Tree));
		ptr->left=HeapPop(frq_heap);
		ptr->right=HeapPop(frq_heap);
		ptr->frq=(ptr->left->frq)+(ptr->right->frq);
		ptr->c='\0';					//invalid val
		FrqNode Node;
		Node.ptrNode=ptr;
		HeapInsert(Node,frq_heap);
	}
	ptr=HeapPop(frq_heap);
	return ptr;
}

void GetIndex(Tree* ptr)
{
	if(ptr)
	{
		if(ptr->c!='\0')
		{
			idxptr->c=ptr->c;
			strcat(idxptr->code,idxstack.codebck());
			idxptr++;
		}
		idxstack.push('0');
		GetIndex(ptr->left);
		idxstack.pop();
		idxstack.push('1');
		GetIndex(ptr->right);
		idxstack.pop();
	}

}



void IndexInjection(FILE* fp,int Total)
{
	fprintf(fp, "i0sojKxqGomw0KoM\n%d\n",Total);
	for (int i = 0; i < ABNUM; i++)
	{
		fprintf(fp,"%c %s\n", Index[i].c, Index[i].code);
	}
	//fprintf(fp, "//");
	return;
}
