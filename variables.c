#include "myhead.h"
/*���Һ���������ֵΪ�ṹָ��*/


/*oprete:next token.syn
default,read only;
16,read and inc;
17,read and dec*/
int IncOrDec[10][3];/*0:floor,1 index,2 operate*/
int IncOrDecIndex = 0;
extern struct formReturn *searchVariable(char name[20], int floor, int operate);
struct formReturn *searchVariable(char name[20],int floor, int operate)
{
	destoryVariable(floor);
	struct formReturn *form=&form1;/*return vaule*/
	int i, j;
	int flag = 1;
	for (j = floor; j >= 0 && flag; j--)
	{
		for (i = 0; i < 50 && flag; i++)
		{
			if (!strcmp(name, variable[j][i].name))
			{
				flag = 0;
			}
		}
	}
	j++, i--;
	form->vaule = variable[j][i].vaule;
	form->floor = j;
	form->index = i;
	switch (operate)
	{
	case 16:pushIncOrDec(j, i,operate); return form; break;
	case 17:pushIncOrDec(j, i, operate); return form; break;
	default:return form; break;
	}
}
void pushIncOrDec(int floor, int i, int operate)/*���������Լ�ѹջ*//*floor ����������i ָ��, operate ++ or --*/
{
	IncOrDec[IncOrDecIndex][0] = floor;
	IncOrDec[IncOrDecIndex][1] = i;
	IncOrDec[IncOrDecIndex][2] = operate;
	IncOrDecIndex++;
}
void popIncOrDec(void)/*���������Լ�������*/
{
	IncOrDecIndex--;
	while (IncOrDecIndex >= 0)
	{

		switch (IncOrDec[IncOrDecIndex][2])
		{
		case INC:
			variable[IncOrDec[IncOrDecIndex][0]][IncOrDec[IncOrDecIndex][1]].vaule++;
			break;
		case DEC:
			variable[IncOrDec[IncOrDecIndex][0]][IncOrDec[IncOrDecIndex][1]].vaule--;
			break;
		}
		IncOrDecIndex--;
	}
	IncOrDecIndex = 0;
}
/*������������ʼ��Ϊ0*/
void create(struct  synStruct exps[])
{
	struct  synStruct exps2[40];
	int valueReturn = 0;
	int i = 0, j = 0;
	while (exps[j].syn != FENHAO)
	{
		i = 0;
		int flag = exps[j].syn;/*falg��¼exps[j].syn,��������*/
		while (flag != DOUHAO&&flag != FENHAO)
		{
			exps2[i++] = exps[j++];
			flag = exps[j].syn;
		}
		exps2[i].syn = FENHAO;
		if (exps[j].syn != FENHAO)
			j++;
		if ((exps2 + 1)->syn == EQUAL)
		createVariable(exps2->string, exps2->sum,getEpression(exps2+2));
		else 
			createVariable(exps2->string, exps2->sum, 0);
	}
}
void createVariable(char name[],int floor,int value)
{
	int i = 0;
	while (variable[floor][i].isStore)
		i++;
	strcpy(variable[floor][i].name, name);
	variable[floor][i].vaule = value;
	variable[floor][i].isStore = 1;
}

/*�������*/
void saveVariable(char name[],int floor,int value)
{
	struct formReturn mark;
	mark = *searchVariable(name, floor, 0);
	variable[mark.floor][mark.index].vaule = value;
}

/*���뿪ĳ��֮������ĳ��ı���*/
void destoryVariable(int startfloor)/*��ǰ���ڲ���*/
{
	int floor,index;
	if(startfloor<5)
	for (floor = startfloor+1; floor <= MAX_FLOOR; floor++)
	{
		for (index = 0; index < MAX_VARY_NUM; index++)
		{
			variable[floor][index].isStore=0;
			variable[floor][index].name[0] = '\0';
			variable[floor][index].vaule = 0;
		}
	}
}