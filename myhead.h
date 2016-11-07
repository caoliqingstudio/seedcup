#pragma once

/*头文件*/
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
/*常量的定义*/
#define MAX_ROW 300
#define MAX_COLUMN 255
#define MAX_KEYWORLD 8

/*变量结构的层数与个数*/
#define MAX_FLOOR 5
#define MAX_VARY_NUM 50

/*结构*/
char progrem[MAX_ROW*MAX_COLUMN];/*最初读入代码到数组中*/

struct myStruct
{
	int syn;/*单词种别码*/
	char string[20];/*字符串*/
	int sum;/*数字值*//*或者是当前层数*/
	int row;/*行号*/
	struct	myStruct * next;/**/
}myStruct, *code_head, *code_p, *code_next;/*头指针与遍历指针*/



										   /*存放变量的结构体*/
struct vary
{
	int isStore;/*是否存放了变量*/
	char name[20];
	int vaule;
}variable[MAX_FLOOR][MAX_VARY_NUM];/*存放变量，5层，每层最多50个变量*/




struct formReturn
{
	int vaule;/*返回值*/
	int floor;/*层数*/
	int index;/*当前层的指向*/
}form1;

struct  synStruct
{
	int row;//行数
	int syn;//类型
	char string[20];//字符串
	int sum;//值
	struct  synStruct *l_child;
	struct  synStruct *r_child;
	struct  synStruct *e_child;
	struct  synStruct *parter;
}*note[1000];


/*变量的声明*/
int syn, sum;/*临时储存符号与数值*/
char token[20];/*临时用于存储单词字符串*/

int	current_row;/*记录当前代码行数*/
long index;/*访问源代码时的指向*/
int currentFloor;/*当前层数*/


/*关键字*/

char ch;/*存储读入的字符*/


/*函数的声明*/
int lexical_analysis(void);/*词法分析*/

/*oprete:next token.syn
default,read only;
16,read and inc;
17,read and dec*/


/*创建变量，初始化为0*/
void createVariable(char name[], int floor, int value);

/*保存变量*/
void saveVariable(char name[], int floor, int value);

/*在离开某层之后，销毁某层的变量*/
void destoryVariable(int startfloor);/*当前所在层数*/

void pushIncOrDec(int floor, int i, int operate);/*变量自增自减压栈*//*floor 变量层数，i 指向, operate ++ or --*/
void popIncOrDec(void);							/*变量自增自减的运算*/


									 /*获取表达式并且以逗号分开并且计算*/
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

		变量名 10
		数值 11

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










