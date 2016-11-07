
#include "myhead.h"


extern int expValue(struct synStruct *exps);
extern struct formReturn *searchVariable(char name[20], int floor, int operate);

void InitStackOpe(OpeStack *s)
{
	s->base = (int *)malloc(STACK_INIT_SIZE * sizeof(int));
	s->top = s->base;
	s->stacksize = STACK_INIT_SIZE;
}

void InitStackNum(NumStack *s)
{
	s->base = (int *)malloc(STACK_INIT_SIZE * sizeof(int));
	s->top = s->base;
	s->stacksize = STACK_INIT_SIZE;
}
int GetTopNum(NumStack *s, int e)
{
	//if (s->top == s->base)
	//printf("error\n");
	e = *(s->top - 1);
	return e;
}//GetTopNum
int GetTopOpe(OpeStack *s, int e)
{
	//if (s->top == s->base)
	//printf("error\n");
	e = *(s->top - 1);
	return e;


}//GetTopOpe
void PushNum(NumStack *s, int e)
{
	if (s->top - s->base >= s->stacksize)
	{
		s->base = (int *)realloc(s->base, (s->stacksize + STACKINCREMENT) * sizeof(int));
		//if (!s->base)
		//printf("realloc error!\n");
		s->top = s->base + s->stacksize;
		s->stacksize += STACKINCREMENT;

	}
	*(s->top++) = e;
}//PushNum

void PushOpe(OpeStack *s, int e)
{
	if (s->top - s->base >= s->stacksize)
	{
		s->base = (int *)realloc(s->base, (s->stacksize + STACKINCREMENT * sizeof(int)));
		//if (!s->base)
		//printf("realloc error!\n");
		s->top = s->base + s->stacksize;
		s->stacksize += STACKINCREMENT;
	}
	*s->top++ = e;
}//PushNum

void PopNum(NumStack *s, int *e)
{
	//if (s->top == s->base)
	//printf("error!");
	*e = *--s->top;
}//PopNum

void PopOpe(OpeStack *s, int *e)
{
	//if (s->top == s->base)
	//printf("error!");
	*e = *--s->top;
}//PopOpe

int Operate(int a, int theta, int b)
{
	switch (theta) {
	case ADD:return a + b; break;
	case SUB:return a - b; break;
	case MULTI:return a*b; break;
	case DEC:return a / b; break;
	default:return 0;
	}
}//Operate

int opetrans(int a)
{
	switch (a) {
	case ADD:return 1;
	case SUB:return 2;
	case MULTI:return 3;
	case DIV:return 4;
	case LBRACKET:return 5;
	case RBRACKET:return 6;
	case FENHAO:return 7;
	default:return 7;
	}

}
char Precede(int a, int c)
{
	char array[7][7] = {
		/* 1   2   3    4   5  6   7  */
		{ '>','>','<','<','<','>','>' },//1
		{ '>','>','<','<','<','>','>' },//2
		{ '>','>','>','>','<','>','>' },//3
		{ '>','>','>','>','<','>','>' },//4
		{ '<','<','<','<','<','=',' ' },//5
		{ '>','>','>','>',' ','>','>' },//6
		{ '<','<','<','<','<',' ','=' },//7
	};
	int m, n;
	m = opetrans(a);
	n = opetrans(c);
	return array[m - 1][n - 1];
}


extern int expValue(struct synStruct *exps)
{
	OpeStack oprt1;
	NumStack opnd2;
	OpeStack *OPRT = &oprt1;
	NumStack *OPND = &opnd2;
	struct  synStruct c;
	c = *(exps++);
	InitStackNum(OPND);
	InitStackOpe(OPRT);
	PushOpe(OPRT, FENHAO);
	int e = 0, theta;
	int moreSynFlag = 0;
	if (c.syn == SUB || c.syn == ADD)
	{
		if (c.syn == SUB&&exps->syn == DIGIT)
		{
			(exps)->sum *= -1;
			c =*(exps++);
		}
		else if (c.syn == ADD&&exps->syn == DIGIT)
			c=*(exps++);
	}
	while ((c.syn) != FENHAO || GetTopOpe(OPRT, e) != FENHAO)
	{
		if ((c.syn) == INC || (c.syn) == DEC)
			c = *(exps++);
		else if ((c.syn) == LETTER)
		{
			PushNum(OPND, (searchVariable(c.string, c.sum, exps->syn)->vaule));
			c = *(exps++);
			if(c.syn==INC||c.syn==DEC)
				c = *(exps++);
		}
		else if ((c.syn) == DIGIT)
		{
			PushNum(OPND, c.sum);
			c = *(exps++);
		}
		else switch (Precede(GetTopOpe(OPRT, e), (c.syn)))
			{
			case '<':
				PushOpe(OPRT, (c.syn));
				c = *(exps++);
				break;
			case '=':
				PopOpe(OPRT, &(c.syn));
				c = *(exps++);
				break;
			case '>':
				PopOpe(OPRT, &theta);
				int a, b;
				PopNum(OPND, &b);
				PopNum(OPND, &a);
				PushNum(OPND, Operate(a, theta, b));
				break;
			}
	}
	return GetTopNum(OPND, e);
}

