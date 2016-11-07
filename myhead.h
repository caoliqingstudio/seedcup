#pragma once

/*ͷ�ļ�*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define STACK_INIT_SIZE 20
#define STACKINCREMENT 10

typedef struct
{
	int *base;
	int *top;
	int stacksize;
} NumStack;

typedef struct
{
	int *base;
	int *top;
	int stacksize;
} OpeStack;
/*�����Ķ���*/
#define MAX_ROW 300
#define MAX_COLUMN 255
#define MAX_KEYWORLD 8

/*�����ṹ�Ĳ��������*/
#define MAX_FLOOR 5
#define MAX_VARY_NUM 50

/*�ṹ*/
char progrem[MAX_ROW*MAX_COLUMN];/*���������뵽������*/

struct myStruct
{
	int syn;/*�����ֱ���*/
	char string[20];/*�ַ���*/
	int sum;/*����ֵ*//*�����ǵ�ǰ����*/
	int row;/*�к�*/
	struct	myStruct * next;/**/
}myStruct, *code_head, *code_p, *code_next;/*ͷָ�������ָ��*/



										   /*��ű����Ľṹ��*/
struct vary
{
	int isStore;/*�Ƿ����˱���*/
	char name[20];
	int vaule;
}variable[MAX_FLOOR][MAX_VARY_NUM];/*��ű�����5�㣬ÿ�����50������*/




struct formReturn
{
	int vaule;/*����ֵ*/
	int floor;/*����*/
	int index;/*��ǰ���ָ��*/
}form1;

struct  synStruct
{
	int row;//����
	int syn;//����
	char string[20];//�ַ���
	int sum;//ֵ
	struct  synStruct *l_child;
	struct  synStruct *r_child;
	struct  synStruct *e_child;
	struct  synStruct *parter;
}*note[1000];


/*����������*/
int syn, sum;/*��ʱ�����������ֵ*/
char token[20];/*��ʱ���ڴ洢�����ַ���*/

int	current_row;/*��¼��ǰ��������*/
long index;/*����Դ����ʱ��ָ��*/
int currentFloor;/*��ǰ����*/


/*�ؼ���*/

char ch;/*�洢������ַ�*/


/*����������*/
int lexical_analysis(void);/*�ʷ�����*/

/*oprete:next token.syn
default,read only;
16,read and inc;
17,read and dec*/


/*������������ʼ��Ϊ0*/
void createVariable(char name[], int floor, int value);

/*�������*/
void saveVariable(char name[], int floor, int value);

/*���뿪ĳ��֮������ĳ��ı���*/
void destoryVariable(int startfloor);/*��ǰ���ڲ���*/

void pushIncOrDec(int floor, int i, int operate);/*���������Լ�ѹջ*//*floor ����������i ָ��, operate ++ or --*/
void popIncOrDec(void);							/*���������Լ�������*/


									 /*��ȡ���ʽ�����Զ��ŷֿ����Ҽ���*/
int getEpression(struct  synStruct exps[]);
int modeswitch(struct  synStruct exps[]);






#define ADD 12
#define SUB 13
#define MULTI 14
#define DIV 15
#define INC 16
#define DEC 17
#define HIGH 18
#define LOW 19
#define NOTEQUAL 21
#define NOTLOW 22
#define NOTHIGH 23
#define LBRACKET 24
#define RBRACKET 25


#define INT 1
#define PRINTF 2
#define IF 3
#define ELSE 4
#define DO 5
#define WHILE 6
#define BREAK 7
#define FOR 8
#define LETTER 10
#define DIGIT 11
#define ISEQUAL 20
#define L_SMALL 24
#define R_SMALL 25
#define l_BIG 26
#define R_BIG 27
#define FENHAO 28
#define EQUAL 29
#define DOUHAO 30

		/*
		int 1
		printf 2
		if 3
		else 4
		do 5
		while 6
		break 7
		for 8

		������ 10
		��ֵ 11

		+	12
		-
		*
		/	15
		++  16
		--17


		> 18
		< 19
		== 20
		!= 21
		>= 22
		<= 23
		( 24
		) 25
		{ 26
		} 27
		; 28
		= 29
		, 30
		" 31 printf(",s1);
		EOF 0;
		deafult -1,reload;


		*/










