#include "pretreat.h"

/**转化为预处理文件名**
*name:原来文件名
*pre:前置文件名头
*num:前置文件名开头
*/
char * name_2_pre(const char *name,char *pre,int num){
	char * prename= (char *)malloc(strlen(name)+num+6);//分配空间，+1即可
	if (prename==NULL)
	{
		exit(OVERFLOW);
	}
	strcpy(prename,pre);
	strcpy(prename+num,name);
	return prename;
}

/****预处理主程序******/
state pretreatment(const char * name){
	FILE * file;
	FILE * prefile;
	char *interFileName;
	char *preFileName;
	interFileName=name_2_pre(name,INTERNAM,INTERNUM);
	preFileName=name_2_pre(name,PRENAM,PRENUM);
	if (file=fopen(name,"r"))
	{
		prefile=fopen(interFileName,"w");
		includeHandle(file,prefile);
	}else{
		printf("%s文件名称有误，\n请检查该文件是否存在！\n",name);
		return ERROR;
	}
	fclose(file);
	fclose(prefile);
	if (file=fopen(interFileName,"r"))
	{
		prefile=fopen(preFileName,"w");
		defineHandle(file,prefile);
	}else{
		printf("%s文件名称有误，\n请检查该文件是否存在！\n",interFileName);
	}
	remove(interFileName);
	pretreatEnd(interFileName,preFileName);
	return OK;
}

/** pretreatstart用于初始化定义的变量**/
state pretreatStart(){
	int i;
	for (i = 0; i < 27; ++i)
	{
		defineMenu[i].next=NULL;
	}
	defineState=NO;
	return OK;
}

/** pretreatEnd 释放使用的空间 **/
state pretreatEnd(char * name1,char *name2){
	free(name1);
	free(name2);
	int i;
	DEFINES *defineState,*defSta_;
	DEFATOM *_token,*tok_;
	for (i = 0; i < 27; ++i)
	{
		defineState=defineMenu[i].next;
		while(defineState){
			if (defineState->type==DEF_FUN)
			{
				_token=defineState->value.next;
				while(_token){
					tok_=_token;
					_token=_token->next;
					free(tok_);
				}
			}
			defSta_=defineState;
			defineState=defineState->next;
		}
	}
}

/** includeHandle 处理include以及条件编译指令**/
state includeHandle(FILE *file,FILE *prefile){
	char character,c;
	char rowString[CHAR_NUMBER];
	int i=0;
	while((character=fgetc(file)) != EOF){
		if (character!='\n'){
			if (character!='/')
			{
				rowString[i++]=character;
			}else{//此处用来去注释
				c=fgetc(file);
				if (c=='*')
				{
					while((character=fgetc(file)) !='*'||(character=fgetc(file)) !='/');
				}else if (c=='/')
				{				
					rowString[i]='\0';
					i=0;
					string2file(rowString,prefile);
					while((character=fgetc(file)) !='\n');
				}else{
					rowString[i++]=character;
					rowString[i++]=c;//将c写入，自己认为不会有问题，待考证
				}
			}
		}else{
			rowString[i]='\0';
			i=0;
			string2file(rowString,prefile);
		}
	}
	if (i!=0)
	{
		rowString[i]='\0';
		string2file(rowString,prefile);
	}
	return OK;
}

/**string2file 为includeHandle的子程序，用来处理一行命令 **/
state string2file(char *rowString,FILE *prefile){
	int i=0;
	WHILE_T0(rowString);
	if (*rowString=='#')
	{
		if (strcmp_key(rowString,"include")==TRUE)
		{
			while(*rowString!='\"'&&*rowString!='<') rowString++;
			while(rowString[i]!='\"'&&rowString[i]!='>') i++;
			rowString[i]='\0';
			FILE *newFile;
			if (newFile=fopen(rowString,"r"))
			{
				includeHandle(newFile,prefile);
			}else{
				printf("%s文件未找到\n",rowString);
			}
		}else if (strcmp_key(rowString,"define"))
		{
			while(*rowString!=' '&&*rowString!='\t') rowString++;
			WHILE_T0(rowString);
			if (pushDefineOne(rowString)==ERROR)
			{
				while(rowString[i]!='\0') fputc(rowString[i++],prefile);
				fputc('\n',prefile);		
			}
		}else if (strcmp_key(rowString,"if"))
		{
			if (popDefine(rowString)!=TRUE)
			{
				defineState= IF;
			}else{
				defineState=NO;
			}
		}else if (strcmp_key(rowString,"ifdef"))
		{
			if (popDefine(rowString)!=TRUE)
			{
				defineState= IFDEF;
			}else{
				defineState=NO;
			}
		}else if (strcmp_key(rowString,"ifndef"))
		{
			if (popDefine(rowString)==TRUE)
			{
				defineState= IFNDEF;
			}else{
				defineState=NO;
			}
		}else if (strcmp_key(rowString,"elif"))
		{
			if (defineState!=NO)
			{
				if (popDefine(rowString)!=TRUE)
				{
					defineState= ELIF;
				}else{
					defineState=NO;
				}
			}
		}else if (strcmp_key(rowString,"endif"))
		{
			defineState=ENDIF;
		}else{
			fputc('#',prefile);
			while(rowString[i]!='\0') fputc(rowString[i++],prefile);
			fputc('\n',prefile);	
		}
	}else{
		if (defineState!=NO)
		{
			return NO;
		}
		while(rowString[i]!='\0') fputc(rowString[i++],prefile);
		fputc('\n',prefile);	
	}
	return NO;
}

/**str cmp判断字符串前几位是否相同**/
state strcmp_key(char *aimstr,char *conststr){
	int i=0;
	WHILE_T0(aimstr);
	while(conststr[i]!='\0'){
		if (aimstr[i]!=conststr[i])
		{
			return FALSE;
		}
	}
	if ((aimstr[i]>='a'&&aimstr[i]<='z')||
		(aimstr[i]>='A'&&aimstr[i]<='Z')||
		aimstr[i]=='_'||(aimstr[i]>='0'&&aimstr[i]<='9'))
	{
		return FALSE;
	}
	return TRUE;
}

/** push define 元素入define**/
state pushDefineOne(char *aimstr){
	int i=0,n;
	DEFINES *atoms;
	if (*aimstr=='_')
	{
		n=0;
	}else if (*aimstr>='a'&&*aimstr<='z')
	{
		n=*aimstr-'a';	
	}else{
		n=*aimstr-'A';
	}
	atoms=(DEFINES *)malloc(sizeof(DEFINES));
	atoms->next=defineMenu[n].next;
	defineMenu[n].next=atoms;
	while(*aimstr!=' '&&*aimstr!='\t'&&*aimstr!='\n'){
		i=0;
		atoms->name[i++]=*aimstr;
		aimstr++;	
	}
	atoms->name[i]='\0';
	WHILE_T0(aimstr);
	if (*aimstr=='\n')
	{
		atoms->type=DEF_ONE;
		return OK;
	}
	free(atoms);
	defineMenu[n].next=NULL;
	return FALSE;
}

/**POP DEFINE 查找define内容**/
state popDefine(char *aimstr){
	WHILE_T0(aimstr);
	int i=0,n;
	DEFINES *state;
	if (*aimstr=='_')
	{
		n=0;
	}else if (*aimstr>='a'&&*aimstr<='z')
	{
		n=*aimstr-'a';	
	}else{
		n=*aimstr-'A';
	}
	state=defineMenu[n].next;
	while(state){
		if (strcmp_key(aimstr,state->name)==TRUE)
		{
			return TRUE;
		}
	}
	return FALSE;
}


/**define Handle 处理define **/
state defineHandle(FILE *file,FILE *prefile){
	char character,c;
	char rowString[CHAR_NUMBER];
	char keyWord[varDefNam];
	int i=0;
	defineState=NO;
	while((character=fgetc(file)) != EOF){
		if (character!='\n')
		{
			rowString[i++]=character;
		}else{
			rowString[i]='\0';
			rowStringDeal(rowString,file,prefile);
			i=0;
		}
	}
	rowString[i]='\0';
	rowStringDeal(rowString,file,prefile);
	return OK;		
}

/** hash   求hash中匹配的n**/
int hashChar(char c){
	int n;
	if (c=='_')
	{
		n=0;
	}else if (c>='a'&&c<='z')
	{
		n=c-'a';	
	}else{
		n=c-'A';
	}
	return n;
}

/** rowStringDeal 用来处理define中的 **/
/** 将 define去掉 **/
state rowStringDeal(char *rowString,FILE *file,FILE *prefile){
	int i=0;
	WHILE_T0(rowString);
	DEFINES *local;
	DEFATOM *local_token;
	if (strcmp_key(rowString,"#define")==TRUE)//把undef 落了
	{
		while(*rowString!=' '&&*rowString!='\t') rowString++;
		definePush(rowString);
	}else{
		if (defineState==NO)//非define语句
		{
			while(*rowString!='\0'){
				if (WORDSTRUC(*rowString))
				{
					local=defineMenu[hashChar(*rowString)].next;
					while(local)
					{
						if (strcmp_key(rowString,local->name)==TRUE)
						{
							rowString+=strlen(local->name);
							if (local->type==DEF_VAR)
							{
								fprintf(prefile, "%s\n",local->value.infor);
							}else if (local->type==DEF_FUN)
							{
								WHILE_T0(rowString);
								rowString++;
								defineName[0][0]=1;
								while(*rowString!=')'){
									if(*rowString==',')
									{
										defineName[defineName[0][0]][i]='\0';
										i=0;
										defineName[0][0]++;
									}else{
										defineName[defineName[0][0]][i++]=*rowString;
									}
									rowString++;
								}
								defineName[defineName[0][0]][i]='\0';//参数处理完毕
								local_token=local->value.next->next;
								while(local_token->type!=END){
									switch(local_token->type){
										case VAR:
											fprintf(prefile, "%s",defineName[(int)local_token->c]);
											break;
										case NO:
											fputc(local_token->c,prefile);
											break;
										case _NEXT:
											fputc('\n',prefile);
											break;
										default:
											printf("你有毒！\n"); 
									}
									local_token=local_token->next;
								}
							}else{
								printf("ERROR!\n");
							}
							break;
						}
						local=local->next;
					}
					if (local==NULL)
					{
						while(WORDSTRUC(*rowString)||*rowString>='0'&&*rowString<='9'){
							fputc(*rowString,prefile);
							rowString++;
						}
					}//处理尾子串
				}
				fputc(*rowString,prefile);
				if (*rowString!='\0')
				 {
				 	rowString++;
				 } 
			}
			fputc('\n',prefile);
		}else{//当define有多行的时候
			if (defineState==DEF_FUN)
			{
				define_help(rowString);
			}else{
				printf("不带参数，你还写多行啊；\n");//lazy to finish it
			}
		}
	}
}

/** definePush 将define 加入 **/ 
state definePush(char *aimstr){
	int i=0,n;
	char j;//计数
	char c;
	state type;
	DEFINES *atoms;
	if (*aimstr=='_')
	{
		n=0;
	}else if (*aimstr>='a'&&*aimstr<='z')
	{
		n=*aimstr-'a';	
	}else{
		n=*aimstr-'A';
	}
	atoms=(DEFINES *)malloc(sizeof(DEFINES));
	atoms->next=defineMenu[n].next;
	defineMenu[n].next=atoms;
	while(*aimstr!=' '&&*aimstr!='\t'&&*aimstr!='('){
		i=0;
		atoms->name[i++]=*aimstr;
		aimstr++;	
	}
	atoms->name[i]='\0';
	i=0;
	switch(*aimstr){
		case ' ':
		case '\t':
			WHILE_T0(aimstr);
			while(*aimstr!='\0'&&*aimstr!='\\') atoms->value.infor[i++]=*(aimstr++);
			atoms->value.infor[i]='\0';
			if (*aimstr=='\\')
			{
				defineState=DEF_VAR;
			}
			break;
		case '(':
			atoms->type=DEF_FUN;
			defineName[0][0]=1;
			while(*aimstr!=')'){
				if(*aimstr==',')
				{
					defineName[defineName[0][0]][i]='\0';
					i=0;
					defineName[0][0]++;
				}else{
					defineName[defineName[0][0]][i++]=*aimstr;
				}
				aimstr++;
			}
			defineName[j][i]='\0';//参数处理完毕
			WHILE_T0(aimstr);
			atoms->next=(DEFATOM *)malloc(sizeof(DEFATOM));
			token=atoms->next;
			define_help(aimstr);
			break;
		default:
			printf("%s ERROR!\n",aimstr);
			return ERROR;
	}
	
}

/** define _help 用来辅助插入define操作 **/
state define_help(char *aimstr){
	char c;
	char j;
	state type;
	token->next=(DEFATOM *)malloc(sizeof(DEFATOM));
	token=token->next;
	while(*aimstr!='\0'&&*aimstr!='\\'){
		c=*aimstr;
		type=NO;
		if (WORDSTRUC(*aimstr))
		{
			for (j = 1; j <= defineName[0][0]; ++j)
			{
				if (strcmp_key(aimstr,defineName[j])==TRUE)
				{
					c=j;
					type=VAR;
					aimstr+=strlen(defineName[j]);
					break;
				}
			}
			if (j>defineName[0][0])
			{
				while(WORDSTRUC(*aimstr)||*aimstr>='0'&&*aimstr<='9'){
					token->c=*(aimstr++);
					token->type=NO;
					token->next=(DEFATOM *)malloc(sizeof(DEFATOM));
					token=token->next;	
				}//防止尾子串
			}
		}
		token->c=c;
		token->type=type;
		token->next=(DEFATOM *)malloc(sizeof(DEFATOM));
		token=token->next;
	}
	token->type=END;
	if (*aimstr=='\\')
	{
		defineState=DEF_FUN;
		token->type=_NEXT;
	}else{
		defineState=NO;
	}
	token->next=NULL;
	return OK;	
}