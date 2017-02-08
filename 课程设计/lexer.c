#include "lexer.h"

/** 词法分析 **/
/**
*有点问题，由于中间使用的文件名是pre 
*本质应该只用一个文件名就可以
*不应该存在 得到pre文件名
**/
state lexicalAnalysisEnter(char *name){
	FILE * file;
	FILE * aimfile;
	char fileName[FILE_NAME_LENGTH];
	char aimFileName[FILE_NAME_LENGTH];
	int i=0;

	strcat(fileName,PRENAME);
	strcat(fileName,name);//得到pre文件名
	strcat(aimFileName,name);
	while(aimFileName[i++]!='.');
	aimFileName[i]='\0';
	strcat(aimFileName,LEX);//得得目标存储文件名
	lexerStart();
	if (file=fopen(fileName,"r"))
	{
		aimfile=fopen(aimFileName,"w");
		readFile(file,aimfile);
	}else{
		printf("%s文件名称有误，\n请检查该文件是否存在！\n",fileName);
		return ERROR;
	}
	lexerEnd();
	return OK;
}

/** hash   求hash中匹配的n**/
int hashChar_lexer(char c){
	int n=0;
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

/** lexerStart**/
/**
*设置关键字等内容
*存在头结点，会有一点浪费空间，但是开心就好
**/
state lexerStart(void){
	int i,j;
	char OPR_;
	char SEP_;
	char KEY_;
	SET_TOKEN_OTHER *atom_other;
	SET_TOKEN_KEY *atom_key;
	//第一步标识符，字符串
	for (i = 0; i < 2; ++i)
	{
		for (j = 0; j < 27; ++j)
		{
			tokens[i][j].next=NULL;
		}
	}
	//第二步，运算符
	atom_other=&tokens_opr;
	for (i = 0; i < OPRNUM; ++i)
	{
		atom_other->next=(SET_TOKEN_OTHER *)malloc(sizeof(SET_TOKEN_OTHER));
		atom_other=atom_other->next;
		atom_other->num=0;
		strcpy(atom_other->value,opr[i]);
	}
	atom_other->next=NULL;
	//第三步，分隔符
	atom_other=&tokens_sep;
	for (i = 0; i < SEPNUM; ++i)
	{
		atom_other->next=(SET_TOKEN_OTHER *)malloc(sizeof(SET_TOKEN_OTHER));
		atom_other=atom_other->next;
		atom_other->num=0;
		strcpy(atom_other->value,sep[i]);
	}
	atom_other->next=NULL;
	//第四步，关键字
	for (i = 0,j = 0; i < 27; ++i)
	{
		atom_key=&tokens_key[i];
		for (; j < KEYNUM; ++j)
		{
			if (hashChar_lexer(key[j][0])!=i)
			{
				break;
			}else{
				atom_key->next=(SET_TOKEN_KEY *)malloc(sizeof(SET_TOKEN_KEY));
				atom_key=atom_key->next;
				atom_key->num=0;
				strcpy(atom_key->value,key[j]);
			}
		}
		atom_key->next=NULL;
	}
	//第五步,2017/2/6 10:14目前没用到
	rowNumber=1;//行号归0；
	rowNumFile=1;
	//第六步，常量，经修改，常量与标识符不同
	//字符常量
	for (i = 0; i < 10; ++i)
	{
		tokens_conChar[i].next=NULL;
	}
	//数字常量
	for (i = 0; i < 10; ++i)
	{
		tokens_conNum[i].next=NULL;
	}
	return OK;
}

/**lexerEnd **/
state lexerEnd(){
	SET_TOKEN *token_134,*token_134_a;
	SET_TOKEN_CONCHAR *token_3char,*token_3char_a;
	int i,j;
	//释放 标识符和字符串字面
	for (i = 0; i <2; ++i)
	{
		for (j = 0; j < 27; ++j)
		{
			token_134=tokens[i][j].next;
			tokens[i][j].next=NULL;
			while(token_134){
				free(token_134->value);
				token_134_a=token_134;
				token_134=token_134->next;
				free(token_134_a);
			}
		}
	}
	//释放数字常量
	for (i = 0; i < 10; ++i)
	{
		token_134=tokens_conNum[i].next;
		tokens_conNum[i].next=NULL;
		while(token_134){
			free(token_134->value);
			token_134_a=token_134;
			token_134=token_134->next;
			free(token_134_a);
		}
	}
	//释放字符常量
	for (i = 0; i < 10; ++i)
	{
		token_3char=tokens_conChar[i].next;
		tokens_conChar[i].next=NULL;
		while(token_3char){
			token_3char_a=token_3char;
			token_3char=token_3char->next;
			free(token_3char);
		}
	}
	return OK;
}

/** readFile 文件中内容**/
state readFile(FILE *file,FILE *aimfile){
	char rowString[ROW_STRING_LENGTH];
	char character;
	int i=0;
	while((character=fgetc(file)) != EOF){
		if (character!='\n')
		{
			rowString[i++]=character;
		}else{
			rowString[i]="\0";
			i=0;
			string2file_lexer(rowString,aimfile);
			rowNumber++;
		}
	}
	if (i!=0)
	{
		rowString[i]="\0";
		string2file_lexer(rowString,aimfile);
	}
	return OK;
}
/** sepTell 判断是否是分隔符,运算符，返回指针**/
SET_TOKEN_OTHER *sepOprTell(SET_TOKEN_OTHER *local,char *string){
	int i;
	local=local->next;
	while(local){
		for (i = 0;local->value[i]!='\0'; ++i)
		{
			if (string[i]!=local->value[i])
			{
				break;
			}	
		}
		if (local->value[i]=='\0')
		{
			return local;
		}
		local=local->next;
	}
	return NULL;
}

/** keyTell 判断是否是关键字**/
SET_TOKEN_KEY *keyTell(char *string){
	int i;
	SET_TOKEN_KEY *local=tokens_key[hashChar_lexer(*string)].next;
	while(local){
		for (i = 0;local->value[i]!='\0'; ++i)
		{
			if (string[i]!=local->value[i])
			{
				break;
			}	
		}
		if (local->value[i]=='\0'&&!(WORDSTRUC(string[i])||NUMBSTRUC(string[i])))
		{
			if (strcmp(local->value,"?:")==0)
			{
				while(*string!=":") string++;
				*string=' ';
			}//处理三目运算符？：使其：为空格不处理
			return local;
		}
		local=local->next;	
	}
	return NULL;
}

/** idenTell 标识符判断 或添加**/
SET_TOKEN *idenTell(char *string){
	int i,n;
	n=hashChar_lexer(*string);
	SET_TOKEN *local=tokens[IDENTIFIERS][n].next;
	while(local){
		for (i=0;local->value[i]!='\0';++i)
		{
			if (string[i]!=local->value[i])
			{
				break;
			}
		}
		if (local->value[i]=='\0'&&!(WORDSTRUC(string[i])||NUMBSTRUC(string[i])))
		{
			return local;
		}
		local=local->next;	
	}
	local=(SET_TOKEN *)malloc(sizeof(SET_TOKEN));
	local->next=tokens[IDENTIFIERS][n].next;
	tokens[IDENTIFIERS][n].next=local;
	local->num=0;
	i=0;
	while(WORDSTRUC(string[i])||NUMBSTRUC(string[i])) i++;
	local->value=(char *)malloc(i*sizeof(char)+4);
	i=0;
	while(WORDSTRUC(string[i])||NUMBSTRUC(string[i])){
		local->value[i]=string[i];
		i++;
	}
	local->value[i]='\0';
	return local;
}

/**strTELl 判断字符串的 **/
SET_TOKEN *strTell(char *string){
	int i=1,n;
	n=hashChar_lexer(string[1]);
	SET_TOKEN *local=tokens[IDENTIFIERS][n].next;
	while(local){
		for (i=1;local->value[i]!='\0';++i)
		{
			if (string[i]!=local->value[i])
			{
				break;
			}
		}
		if (local->value[i]=='\0')
		{
			return local;
		}
		local=local->next;	
	}//检查是否有，没有下面制作
	local=(SET_TOKEN *)malloc(sizeof(SET_TOKEN));
	local->next=tokens[IDENTIFIERS][n].next;
	tokens[IDENTIFIERS][n].next=local;
	local->num=0;
	i=1;
	while(string[i]!='"'||string[i-1]=='\\') i++;
	local->value=(char *)malloc(i*sizeof(char)+4);
	i=0;
	while(string[i]!='"'||string[i-1]=='\\'){
		local->value[i]=string[i];
		i++;
	}
	local->value[i]=string[i];
	local->value[i+1]='\0';
	return local;
}

/** ConCharTell 字符常量**/
SET_TOKEN_CONCHAR *ConCharTell(char *string){
	int i,n;
	SET_TOKEN_CONCHAR *local;
	n=(int)string[1]%10;
	local=tokens_conChar[n].next;
	while(local){
		for (i=1;local->value[i]!='\0';++i)
		{
			if (string[i]!=local->value[i])
			{
				break;
			}
		}
		if (local->value[i]=='\0')
		{
			return local;
		}
		local=local->next;	
	}//检查是否有，没有下面制作
	local=(SET_TOKEN_CONCHAR *)malloc(sizeof(SET_TOKEN_CONCHAR));
	local->next=tokens_conChar[n].next;
	tokens_conChar[n].next=local;
	local->num=0;
	i=0;
	while(string[i]!='\''||string[i-1]=='\\'){
		local->value[i]=string[i];
		i++;
	}
	local->value[i]=string[i];
	local->value[i+1]='\0';
	return local;
}

/** string2file_lexer**/
/**
*处理一行代码并写入
*常量部分，枚举未考虑
**/
state string2file_lexer(char *rowString,FILE *aimfile){
	int lineNumber=0,tokenNumber,number;//依次为c文件中列号，token编号，出现次数
	char *tokenStr;//token内容
	int length;
	int i;
	SET_TOKEN *token_134;
	SET_TOKEN_KEY *token_2;
	SET_TOKEN_OTHER *token_56;
	SET_TOKEN_CONCHAR *token_3char;
	while(rowString[lineNumber]!='\0'){
		if (rowString[lineNumber]==' '||"\t")
		{
			length=1;
		}else{//以下顺序经过考虑的
			if (rowString[lineNumber]=='"')
			{//字符串  
				token_134=strTell(&rowString[lineNumber]);
				length=strlen(token_134->value);
				tokenNumber=TOKEN_IDE;
				tokenStr=token_134->value;
				number=++(token_134->num);
			}else if (rowString[lineNumber]=='\'')
			{//字符常量
				token_3char=ConCharTell(&rowString[lineNumber]);
				length=strlen(token_3char->value);
				tokenNumber=TOKEN_CON;
				tokenStr=token_3char->value;
				number=++(token_3char->num);
			}else if (rowString[lineNumber]>='0'&&rowString[lineNumber]<='9')
			{//数字常量
				token_134=ConNumTell(&rowString[lineNumber]);
				length=strlen(token_134->value);
				tokenNumber=TOKEN_CON;
				tokenStr=token_134->value;
				number=++(token_134->num);
			}else if (rowString[lineNumber]>='a'&&rowString[lineNumber]<='z'||
				rowString[lineNumber]>='A'&&rowString[lineNumber]<='Z'||
				rowString[lineNumber]=='_')//标识符 关键字
			{
				if (token_2=keyTell(&rowString[lineNumber]))
				{//关键字
					length=strlen(token_2->value);
					tokenNumber=TOKEN_KEY;
					tokenStr=token_2->value;
					number=++(token_2->num);
				}else{//标识符
					token_134=idenTell(&rowString[lineNumber]);
					length=strlen(token_134->value);
					tokenNumber=TOKEN_IDE;
					tokenStr=token_134->value;
					number=++(token_134->num);
				}
			}else {
				if(token_56=sepOprTell(&tokens_sep,&rowString[lineNumber]))
				{//分隔符
					length=strlen(token_56->value);
					tokenNumber=TOKEN_SEP;
					tokenStr=token_56->value;
					number=++(token_56->num);
				}else if (token_56=sepOprTell(&tokens_opr,&rowString[lineNumber]))
				{//运算符
					length=strlen(token_56->value);
					tokenNumber=TOKEN_OPR;
					tokenStr=token_56->value;
					number=++(token_56->num);
				}else{
					printf("%s你是啥东西!\n",rowString);
				}
			}
			fprintf(aimfile, "%d %d %s %d %d\n",rowNumFile++,tokenNumber,tokenStr,rowNumber,lineNumber+1,number);
		}
		lineNumber+=length;
	}
	return OK;
}