#include "myhead.h"
extern int expValue(struct synStruct *exps);
extern struct formReturn *searchVariable(char name[20], int floor, int operate);

/*��ȡ���ʽ�����Զ��ŷֿ����Ҽ���*/
 int getEpression(struct  synStruct exps[])
{
	struct  synStruct exps2[40];
	int valueReturn = 0;
	int i = 0,j=0;
	while (exps[j].syn != FENHAO)
	{
		i = 0;
		int flag = exps[j].syn;/*falg��¼exps[j].syn��������*/
		while (flag != DOUHAO&&flag != FENHAO)
		{
			exps2[i++] = exps[j++];
			flag = exps[j].syn;
		}
		exps2[i].syn = FENHAO;
		if (exps[j].syn != FENHAO)
			j++;
		valueReturn=modeswitch(exps2);
	}
	return valueReturn;
}


/*�жϱ��ʽ�����ͣ�������������*/
int modeswitch(struct  synStruct exps[])
{
	
	int i = 0;
	int syn;
	int valueReturn = 0;
	int keySymboIndex = 0;/*�ؼ����ŵ�ָ��=��==��<=;<;>;>=;!=*/
	int key;
	while ((exps + i)->syn != FENHAO&&(exps + i)->syn != DOUHAO)
	{
		key = (exps + i)->syn;
		if (key == EQUAL || key == ISEQUAL || key == NOTHIGH || key == NOTLOW||key==LOW||key==HIGH||key==NOTEQUAL)
			break;
		i++;
	}
	if((exps+i)->syn!=FENHAO&&(exps+i)->syn!=DOUHAO)
	{
		key = (exps + i)->syn;
		keySymboIndex = i;
		exps[i].syn = FENHAO;
		switch (key)
		{
		case EQUAL:
			saveVariable(exps->string, exps->sum, expValue(exps +keySymboIndex+1));
			valueReturn = searchVariable(exps->string, exps->sum, (exps+1)->syn)->vaule;
			break;
		case ISEQUAL:
			if (expValue(exps)==expValue(exps+keySymboIndex+1))
				valueReturn = 1;
			else valueReturn = 0;
			break;
		case LOW:
			if (expValue(exps)<expValue(exps + keySymboIndex + 1))
				valueReturn = 1;
			else valueReturn = 0;
			break;
		case HIGH:
			if (expValue(exps)>expValue(exps + keySymboIndex + 1))
				valueReturn = 1;
			else valueReturn = 0;
			break;
		case NOTEQUAL:
			if (expValue(exps)!=expValue(exps + keySymboIndex + 1))
				valueReturn = 1;
			else valueReturn = 0;
			break;
		case NOTLOW:
			if (expValue(exps)>=expValue(exps + keySymboIndex + 1))
				valueReturn = 1;
			else valueReturn = 0;
			break;
		case NOTHIGH:
			if (expValue(exps)<=expValue(exps + keySymboIndex + 1))
				valueReturn = 1;
			else valueReturn = 0;
			break;
		case FENHAO:case DOUHAO:
			valueReturn = searchVariable(exps[i].string, exps[i].sum, 0)->vaule;
			break;
		default:
			valueReturn = expValue(exps);
			break;
		}
	}
	else if((exps + i)->syn ==FENHAO|| (exps + i)->syn==DOUHAO)
		valueReturn = expValue(exps);
	else valueReturn = 0;/*�����֮���*/
	popIncOrDec();
	return valueReturn;
}