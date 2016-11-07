#include "myhead.h"


/*�ж��Ƿ���for��*/
int forFlag = 0;
char * keywords[MAX_KEYWORLD] = { "int","printf","if","else","do","while","break","for" };
int lexical_analysis(void)/*�ʷ�����*/
{
	/*
	����Ϊ�Ĵ�飬�ֱ��ǿո�ע�͡���ʾ�������֡����ţ���Ӧ����� if   else if  �� else
	*/
	int n = 0, m = 0;
	sum = 0;
	for (n = 0; n < 20; n++)
		token[n] = '\0';
	while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\"')
	{
		while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
		{
			if (ch == '\n')
				current_row++;
			ch = progrem[index++];
		}
		if (ch == '\"')
		{
			ch = progrem[index++];
			while (ch != '\"')
				ch = progrem[index++];
			ch = progrem[index++];
		}
	}
	if (ch == '/')/*�ж��Ƿ�Ϊע��*/
	{
		ch = progrem[index++];
		if (ch == '*')
		{
			ch = progrem[index++];
			while ((ch != '*') && (progrem[index] != '/'))
			{
				if (ch == '\n')
					current_row++;
				ch = progrem[index++];
			}
			index += 2;
			ch = progrem[index - 1];
			syn = -1;
			return 0;
		}
		else if (ch == '/')
		{
			ch = progrem[index++];
			while (ch != '\n'&&ch != EOF)
			{
				ch = progrem[index++];
			}
			if (ch == '\n')
				current_row++;
			ch = progrem[index++];
		}
		else
		{
			index--,ch = progrem[index - 1];
		}
	}
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || ch == '_')  //�����Ǳ�ʾ�����߱����� 
	{
		while ((ch >= '0'&&ch <= '9') || (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || ch == '_')
		{
			token[m++] = ch;
			ch = progrem[index++];
		}
		token[m++] = '\0';
		syn = 10;
		for (n = 0; n<MAX_KEYWORLD; n++)  //��ʶ��������ַ����Ѷ���ı�ʾ�����Ƚϣ� 
			if (strcmp(token, keywords[n]) == 0)
			{
				syn = n + 1;
				break;
			}
		if (strcmp(token, "for") == 0)
			forFlag = 1,currentFloor++;
	}
	else if ((ch >= '0'&&ch <= '9'))  //���� 
	{
		sum = 0;
		while ((ch >= '0'&&ch <= '9'))
		{
			sum = sum * 10 + ch - '0';
			ch = progrem[index++];
		}
		syn = 11;
	}

	else switch (ch)   //�����ַ� 
	{
	case'<':m = 0; token[m++] = ch;
		ch = progrem[index++];
		if (ch == '=')
		{
			syn = 23;
			token[m++] = ch;
			ch = progrem[index++];
		}
		else
		{
			syn = 19;
		}
		break;
	case'>':m = 0; token[m++] = ch;
		ch = progrem[index++];
		if (ch == '=')
		{
			syn = 22;
			token[m++] = ch;
			ch = progrem[index++];
		}
		else
		{
			syn = 18;
		}
		break;
	case'!':m = 0; token[m++] = ch;
		ch = progrem[index++];
		if (ch == '=')
		{
			syn = 21;
			token[m++] = ch;
			ch = progrem[index++];
		}
		else
		{
			syn = 17;

		}
		break;
	case'+':m = 0; token[m++] = ch;
		ch = progrem[index++];
		if (ch == '+')
		{
			syn = 16;
			token[m++] = ch;
			ch = progrem[index++];
		}
		else
		{
			syn = 12;

		}
		break;
	case'-':m = 0; token[m++] = ch;
		ch = progrem[index++];
		if (ch == '-')
		{
			syn = 17;
			token[m++] = ch;
			ch = progrem[index++];
		}
		else
		{
			syn = 13;
		}
		break;
	case '=':m = 0; token[m++] = ch;
		ch = progrem[index++];
		if (ch == '=')
		{
			syn = 20;
			token[m++] = ch;
			ch = progrem[index++];
		}
		else
		{
			syn = 29;
		}
		break;
	case'*':syn = 14; token[0] = ch; ch = progrem[index++]; break;
	case'/':syn = 15; token[0] = ch; ch = progrem[index++]; break;
	case'(':syn = 24; token[0] = ch; ch = progrem[index++]; break;
	case')':syn = 25; token[0] = ch; ch = progrem[index++];
		if (forFlag)
			forFlag = 0,currentFloor--;
		break;
	case'{':syn = 26; token[0] = ch; ch = progrem[index++]; currentFloor++; break;
	case'}':syn = 27; token[0] = ch; ch = progrem[index++]; currentFloor--; break;
	case';':syn = 28; token[0] = ch; ch = progrem[index++]; break;

	case',':syn = 30; token[0] = ch; ch = progrem[index++]; break;
	case'\0':syn = 0; token[0] = ch; ch = progrem[index++]; break;
	case'\n':syn = -1; ch = progrem[index++]; current_row++; break;
	default: syn = -1; break;
	}
	return 0;
}