/***
*字符串字面值的双引号未单独处理，参考资料上说
*字符串字面值不含双引号，但是我不知道把双引号放在哪里就加进去了
*字符的单引号我也没管啊
*在数字常量上 对于表达形式不同但是值相同的数字我也没计算
**/
#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "state.h"

#define FILE_NAME_LENGTH 30
#define PRENAME "pre"
#define LEX ".lex"
#define ROW_STRING_LENGTH 100//读取每行代码数量
#define STRING_LENGTH_MAX 100//貌似没用
#define CON_CHAR_MAX 8 
#define OTHER_TOKEN_MAX 3//目前发现最多就是两个的，sizeof划入关键字
#define KEY_TOKEN_MAX  10//关键字，8个，除非我数错了
#define IDENTIFIERS 0 //设定数组中标识符的位次
//#define CONSTANTS 1//设定数组中常量的位次
#define STRINGLITERALS 1//设定数组中字符串字面的位次
#define WORDSTRUC(c) c>='a'&&c<='z'||c>='A'&&c<='Z'||c=='_'
#define NUMBSTRUC(c) c>='0'&&c<='9'
#define TOKEN_IDE 1//以下为序号 1：标识符
#define TOKEN_KEY 2//2.关键字
#define TOKEN_CON 3//3.常量
#define TOKEN_STR 4//4.字符串
#define TOKEN_OPR 5//5.运算符
#define TOKEN_SEP 6//6.分隔符
#define OPRNUM 39
#define OPR_ opr[OPRNUM][OTHER_TOKEN_MAX]={"++","--","+","-","(",")","[","]","->",".","!",\
					"~","*","&","/","%","<<",">>","<","<=",">=",">",\
					"==","!=","^","|","&&","||","=","+=","-=","*=",\
					"/=","&=","^=","|=","<<=",">>=",",","?:"}//竟然查不到使用频度,自增在前面
//attention:没有区分正负号和加减，没有区分取地址&和按位与&，没有sizeof,?:
#define SEPNUM 5
#define SEP_ sep[SEPNUM][OTHER_TOKEN_MAX]={",",";",":","{","}"}
//SEP分隔符目前就这些吧
#define KEYNUM 32
#define KEY_ key[KEYNUM][KEY_TOKEN_MAX]={"auto","break","case","char","const","continue",\
					"double","default","do","else","enum","extern","float","for","goto",\
					"int","if","long","register","return","struct","switch","short","signed",\
					"sizeof","static","typedef","union","unsigned","void","volatile","while"} 

typedef struct word_key
{
	int num;
	char value[KEY_TOKEN_MAX];
	struct word_key *next; 
} SET_TOKEN_KEY;//关键字
typedef struct word_char
{
	int num;
	char value[CON_CHAR_MAX];
	struct word_char *next; 
} SET_TOKEN_CONCHAR;
typedef struct word_other
{
	int num;
	char value[OTHER_TOKEN_MAX];
	struct word_other *next; 
} SET_TOKEN_OTHER;//运算符和分隔符的
typedef struct word
{
	int num;
	char *value;
	struct word *next;
} SET_TOKEN;//标识符 字符串字面
/**
*SET_TOKEN tokens[5][27];
*其中，0表示标识符;
*1表示字符串字面值;
**/
SET_TOKEN tokens[2][27];//1~26表示字母a到z,0表示下划线
SET_TOKEN_KEY tokens_key[27];
SET_TOKEN tokens_conNum[10];//数字常量，哈希函数为%10
SET_TOKEN_OTHER tokens_opr,tokens_sep;
SET_TOKEN_CONCHAR tokens_conChar[10];//字符常量，哈希函数为%10
int rowNumber;//c文件行号
int rowNumFile;//目标文件中行号

state lexicalAnalysis(char *name);//词法分析
//此处使用过extern "C" 但是报错expected identifier or '(' before string constant
//由于没能解决，所以将.c文件后缀改为.cpp
//然后发现还不行，就只能把main中的 包含.h 改为包含.c

#endif