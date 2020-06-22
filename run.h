#include "syn_analy.h"
#include "myhead.h"
//#include "variables.h"
  
//test

struct  synStruct note_run_yunsuan[40];
/***********************/
/*获取表达式并且以逗号分开并且计算*/
int do_note=0;
/*********************/
int run_raw[2000];
int run_raw_i=0;
int run_raw_state=0;

int state_break = 0;
/******************************/


int int_run(struct  synStruct * syn_h){
	int i=0;
	while(syn_h->syn!=FENHAO){
		if (syn_h->syn==EQUAL)
		{
			if (syn_h->row!=run_raw_state)
			{
				run_raw[run_raw_i++]=syn_h->row;
				run_raw_state=syn_h->row;
			}
		}
		copy_run(syn_h,&note_run_yunsuan[i++]);
		syn_h = syn_h->parter;
	}
	note_run_yunsuan[i++].syn=FENHAO;
	return 0;
}


int yusuan_run(struct  synStruct * syn_h){
	int i=0;
	while(syn_h->syn!=FENHAO){
			if (syn_h->row!=run_raw_state)
			{
				run_raw[run_raw_i++]=syn_h->row;
				run_raw_state=syn_h->row;
			}
			note_run_yunsuan[i].syn = syn_h->syn;
			strcpy(note_run_yunsuan[i].string, syn_h->string);
			note_run_yunsuan[i++].sum = syn_h->sum;
			syn_h = syn_h->parter;
			//copy_run(syn_h,&note_run_yunsuan[i++]);
	}
	note_run_yunsuan[i++].syn=FENHAO;
	return getEpression(note_run_yunsuan);
}

int copy_run(struct  synStruct * syn_h,struct  synStruct *run_h){
	//syn_h->row=syn_h->row;
	run_h->syn=syn_h->syn;
	strcpy(run_h->string,syn_h->string);
	run_h->sum=syn_h->sum;
	return 0;
}



int run_raw_analy(struct  synStruct * syn_h) {

	while (syn_h->syn != 100) {
		switch (syn_h->syn) {
		case INT:
			int_run(syn_h->r_child);
			create(note_run_yunsuan);
			break;
		case IF:
			if (syn_h->row != run_raw_state)
			{
				run_raw[run_raw_i++] = syn_h->row;
				run_raw_state = syn_h->row;
			}
			if (yusuan_run(syn_h->l_child))
			{
				run_raw_analy(syn_h->r_child);
				if (state_break) {
					return 0;
				}
			}
			else if (syn_h->e_child != NULL)
			{
				run_raw_analy(syn_h->e_child->r_child);
				if (state_break) {
					return 0;
				}
			}

	//		while(syn_h->e_child != NULL) {
			if (syn_h->e_child != NULL) 
			syn_h = syn_h->e_child;
		//		if (syn_h->sum==IF)
		//		{
		//			syn_h = syn_h->r_child;
		//		}
	//		}
			break;
		case PRINTF:
			if (syn_h->row != run_raw_state)
			{
				run_raw[run_raw_i++] = syn_h->row;
				run_raw_state = syn_h->row;
			}
			yusuan_run(syn_h->r_child);
			break;
		case DO:
			run_raw_analy(syn_h->r_child);
			//do_note = 1;
			/*if (syn_h->row != run_raw_state)
			{
				if (do_note == 0)
				{
					/*
					run_raw[run_raw_i++]=syn_h->row;
					run_raw_state=syn_h->row;
				
				}
			}*/
			break;
		case WHILE:
			while (yusuan_run(syn_h->l_child)) {
				run_raw_analy(syn_h->r_child);
				do_note = 0;
				if (state_break)
				{
					state_break = 0;
					break;
				}
			}
			break;
		case BREAK:
			if (syn_h->row != run_raw_state)
			{
				run_raw[run_raw_i++] = syn_h->row;
				run_raw_state = syn_h->row;
			}
			state_break = 1;
			return 0;
		case FOR:
			while (yusuan_run(syn_h->l_child)) {
				run_raw_analy(syn_h->e_child);
				yusuan_run(syn_h->r_child);
			}
			state_break = 0;
			break;
		case YUNSUAN:
			yusuan_run(syn_h->r_child);
			break;
		case 0:
			return 0;
		default:
			yusuan_run(syn_h);
			break;
		}
		syn_h = syn_h->parter;
		if (syn_h == NULL)
		{

			return 0;
		}
	}
	return 0;
}
