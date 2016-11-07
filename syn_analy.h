#include "myhead.h"

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
#define INC 16
#define DEC 17
#define L_SMALL 24
#define R_SMALL 25
#define l_BIG 26
#define R_BIG 27
#define FENHAO 28
#define EQUAL 29
#define DOUHAO 30
#define YUNSUAN 66
/*********************/
/**    定义结构体   **/
struct  synStruct *note_loop[20];
	/*{
	int row;//行数
	int syn;//类型
	char string[20];//字符串
	int sum;//值
	struct  syn *l_child;
	struct  syn *r_child;
	struct  syn *e_child;
	struct  syn *parter;//相当于next
}*/
/***********************/
struct myStruct *note_code[50];

int note_i=0,note_code_i=0;
int note_loop_i=0;

int fenhao_state=0;
int fenhao_state_a=0;
int fenhao_state_if = 0;
/*****************************/
/**        分析程序         **/
/*****************************/


int int_analy(struct myStruct *code_h,struct  synStruct *syn_h){
	copy(code_h, syn_h);
	syn_h->r_child= (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
	syn_h = syn_h->r_child;
	code_h = code_h->next;
	while(code_h->syn!=FENHAO){
		copy(code_h,syn_h);
		syn_h->parter=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
		syn_h=syn_h->parter;
		code_h=code_h->next;
	}
	syn_h->syn=FENHAO;
	note_code[note_code_i++]=code_h;
	return 0;
}
//ok
int if_analy_f(struct myStruct *code_h,struct  synStruct *syn_h){
	copy(code_h, syn_h);
	syn_h->l_child = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
	syn_h = syn_h->l_child;
	while(code_h->syn!=L_SMALL){
		code_h=code_h->next;
	}
	code_h=code_h->next;
	while(code_h->syn!=R_SMALL){
		copy(code_h,syn_h);
		syn_h->parter=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
		syn_h=syn_h->parter;
		code_h=code_h->next;
	}
	syn_h->syn=FENHAO;
	note_code[note_code_i++]=code_h;
	return 0;
}

int if_analy_s(struct myStruct *code_h,struct  synStruct *syn_h){
	code_h=code_h->next;
	syn_h->r_child = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
	syn_h = syn_h->r_child;
	if (code_h->syn==l_BIG)
	{
		code_h=code_h->next;
		syn_analy(code_h,syn_h);
		code_h=note_code[--note_code_i];
	}else{
		fenhao_state=1;
		syn_analy(code_h,syn_h);
		code_h=note_code[--note_code_i];
		fenhao_state=0;
	}
	note_code[note_code_i++]=code_h;
	return 0;
}
//ok
int else_analy(struct myStruct *code_h,struct  synStruct *syn_h){
	struct synStruct *L=syn_h;
	copy(code_h,syn_h);
	code_h=code_h->next;
	syn_h->r_child=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
	syn_h=syn_h->r_child;
	if (code_h->syn==l_BIG)
	{
		code_h=code_h->next;
		syn_analy(code_h,syn_h);
		code_h=note_code[--note_code_i];
	}else if(code_h->syn!=IF){
		fenhao_state=1;
		syn_analy(code_h,syn_h);
		code_h=note_code[--note_code_i];
		fenhao_state=0;
	}
	else {
		L->sum = IF;
		fenhao_state_if = 1;
		syn_analy(code_h, syn_h);
		code_h = note_code[--note_code_i];
		fenhao_state_if = 0;
	}
	note_code[note_code_i++]=code_h;
	return 0;
}

int printf_analy(struct myStruct *code_h,struct  synStruct *syn_h){
	copy(code_h,syn_h);
	code_h=code_h->next->next;
	if (code_h->syn==DOUHAO)
	{
		code_h=code_h->next;
	}
	syn_h->r_child=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
	syn_h=syn_h->r_child;
	while(code_h->syn!=R_SMALL){
			copy(code_h,syn_h);
			syn_h->parter=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
			syn_h=syn_h->parter;
			code_h=code_h->next;
	}
	syn_h->syn=FENHAO;
	note_code[note_code_i++]=code_h;
	return 0;
}

int while_analy(struct myStruct *code_h,struct  synStruct *syn_h){
	copy(code_h,syn_h);
	code_h=code_h->next->next;
	syn_h->l_child=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
	syn_h=syn_h->l_child;
	while(code_h->syn!=R_SMALL){
			copy(code_h,syn_h);
			syn_h->parter=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
			syn_h=syn_h->parter;
			code_h=code_h->next;
	}
	syn_h->syn=FENHAO;
	note_code[note_code_i++]=code_h;
	return 0;
}

int while_analy_a(struct myStruct *code_h,struct  synStruct *syn_h){
	code_h=code_h->next;
	syn_h->r_child=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
	syn_h=syn_h->r_child;
	if (code_h->syn==l_BIG)
	{
		code_h=code_h->next;
		syn_analy(code_h,syn_h);
		code_h=note_code[--note_code_i];
	}else{
		fenhao_state=1;
		syn_analy(code_h,syn_h);
		code_h=note_code[--note_code_i];
		fenhao_state=0;
	}
	note_code[note_code_i++]=code_h;
	return 0;
}

int for_analy_e(struct myStruct *code_h,struct  synStruct *syn_h){
	syn_h->l_child=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
	syn_h=syn_h->l_child;
	while(code_h->syn!=FENHAO){
			copy(code_h,syn_h);
			syn_h->parter=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
			syn_h=syn_h->parter;
			code_h=code_h->next;
	}
	syn_h->syn=FENHAO;
	note_code[note_code_i++]=code_h;
	return 0;
}

int for_analy_a(struct myStruct *code_h,struct  synStruct *syn_h){
	if (code_h->syn==INT)
	{
		syn_h->syn = INT;
		code_h = code_h->next;
	}
	else {
		syn_h->syn = YUNSUAN;
	}
	syn_h->r_child=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
	syn_h=syn_h->r_child;
	while(code_h->syn!=FENHAO){
		copy(code_h, syn_h);
			syn_h->parter=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
			syn_h=syn_h->parter;
			code_h=code_h->next;
	}
	syn_h->syn=FENHAO;
	note_code[note_code_i++]=code_h;
	return 0;
}

int for_analy_c(struct myStruct *code_h,struct  synStruct *syn_h){
	syn_h->r_child=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
	syn_h=syn_h->r_child;
	while(code_h->syn!=R_SMALL){
			copy(code_h,syn_h);
			syn_h->parter=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
			syn_h=syn_h->parter;
			code_h=code_h->next;
	}
	syn_h->syn=FENHAO;
	note_code[note_code_i++]=code_h;
	return 0;
}

int for_analy_f(struct myStruct *code_h,struct  synStruct *syn_h){
	code_h=code_h->next;
	syn_h->e_child=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
	syn_h=syn_h->e_child;
	if (code_h->syn==l_BIG)
	{
		code_h=code_h->next;
		syn_analy(code_h,syn_h);
		code_h=note_code[--note_code_i];
	}else{
		fenhao_state=1;
		syn_analy(code_h,syn_h);
		code_h=note_code[--note_code_i];
		fenhao_state=0;
	}
	note_code[note_code_i++]=code_h;
	return 0;
}

int yusuan_analy(struct myStruct *code_h,struct  synStruct *syn_h){
	syn_h->syn=YUNSUAN;
	syn_h->r_child=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
	syn_h=syn_h->r_child;
	while(code_h->syn!=FENHAO){
			copy(code_h,syn_h);
			syn_h->parter=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
			syn_h=syn_h->parter;
			code_h=code_h->next;
	}
	syn_h->syn=FENHAO;
	if (fenhao_state)
	{
		fenhao_state_a=1;
	}
	note_code[note_code_i++]=code_h;
	return 0;
}

int copy(struct myStruct *code_h,struct  synStruct *syn_h){
	syn_h->row=code_h->row;
	syn_h->syn=code_h->syn;
	strcpy(syn_h->string,code_h->string);
	syn_h->sum=code_h->sum;
	return 0;
}




// if (syn_h->syn==EQUAL)
// 		{
// 			if (syn_h->row!=row_note_state)
// 			{
// 				row_note[row_note_i++]=syn_h->row;
// 				row_note_state=syn_h->row;
// 			}
// 		}
int syn_analy(struct myStruct *code_h, struct  synStruct * syn_h) {
	int do_state = 0;
	while (code_h->syn) {
		switch (code_h->syn) {
		case INT:
			int_analy(code_h, syn_h);
			code_h = note_code[--note_code_i];
			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			syn_h = syn_h->parter;
			break;
		case IF:
			if_analy_f(code_h, syn_h);
			code_h = note_code[--note_code_i];
			if_analy_s(code_h, syn_h);
			code_h = note_code[--note_code_i];
			note[note_i++] = syn_h;

			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			//syn_h->parter=(struct  synStruct *)calloc(1, sizeof (struct  synStruct));
			syn_h = syn_h->parter;
			if (fenhao_state_if==1&&code_h->next->syn!=ELSE)
			{
				note_code[note_code_i++] = code_h;
				return 0;
			}
			break;
		case ELSE:
			syn_h = note[--note_i];
			syn_h->e_child = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			//syn_h->parter = note[note_i]->parter;

			syn_h = syn_h->e_child;
			else_analy(code_h, syn_h);
		//	if (fenhao_state==IF)
		//{
		//		fenhao_state = 0;
		//		syn_h->sum = IF;
		//	}
			code_h = note_code[--note_code_i];
			if (fenhao_state_if == 1 && code_h->next->syn != IF)
			{
				note_code[note_code_i++] = code_h;
				return 0;
			}
			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			syn_h = syn_h->parter;
			break;
		case BREAK:
			copy(code_h, syn_h);
			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			syn_h = syn_h->parter;
			break;
		case PRINTF:
			printf_analy(code_h, syn_h);
			code_h = note_code[--note_code_i];
			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			syn_h = syn_h->parter;
			break;
		case DO:
			copy(code_h, syn_h);
			do_state = 1;
			syn_h->r_child = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			note_loop[note_loop_i++] = syn_h->r_child;
			syn_analy(code_h->next ->next, syn_h->r_child);
			code_h = note_code[--note_code_i];
			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			syn_h = syn_h->parter;
			break;
		case WHILE:
			while_analy(code_h, syn_h);
			code_h = note_code[--note_code_i];
			if (do_state)
			{
				syn_h->r_child = note_loop[--note_loop_i];
				do_state = 0;
			}
			else {
				while_analy_a(code_h, syn_h);
				code_h = note_code[--note_code_i];
			}
			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			syn_h = syn_h->parter;
			break;
		case FOR:
			if (code_h->next->next->syn != FENHAO)
			{
				for_analy_a(code_h->next->next, syn_h);
				syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
				syn_h = syn_h->parter;
				copy(code_h, syn_h);
				code_h = note_code[--note_code_i];
				for_analy_e(code_h->next, syn_h);
				code_h = note_code[--note_code_i];
			}
			else {
				copy(code_h, syn_h);
				for_analy_e(code_h->next->next->next, syn_h);
				code_h = note_code[--note_code_i];
			}
			for_analy_c(code_h->next, syn_h);
			code_h = note_code[--note_code_i];
			for_analy_f(code_h, syn_h);
			code_h = note_code[--note_code_i];

			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			syn_h = syn_h->parter;
			break;
		case l_BIG:
			code_h = code_h->next;
			syn_analy(code_h, syn_h);
			code_h = note_code[--note_code_i];

			syn_h = note[--note_i];
			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			syn_h = syn_h->parter;
			break;
		case R_BIG:
			note_code[note_code_i++] = code_h;
			return 0;
		default:
			yusuan_analy(code_h, syn_h);
			code_h = note_code[--note_code_i];
			if (fenhao_state_a == 1)
			{
				fenhao_state_a = 0;
				note_code[note_code_i++] = code_h;
				return 0;
			}
			syn_h->parter = (struct  synStruct *)calloc(1, sizeof(struct  synStruct));
			syn_h = syn_h->parter;
		}
//		if (code_h!=NULL)
//		{
			code_h = code_h->next;
//		}
//		else {
//			return 0;
//		}
		//free(code_h);//节省空间
	}
	syn_h->syn = 100;
	return 0;
}