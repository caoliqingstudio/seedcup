#include <stdio.h>
#include "pretreat.c"
#include "lexer.c"
//#include "state.h"

int main(int argc, char const *argv[])
{
    int order=1;
	while(argc--)//允许循环进行
	{
		printf("编译预处理正在进行！\n");
		if(pretreatment(argv[order])==OK){
			printf("%s预处理完毕！\n",argv[order]);
		}else{
			printf("%s处理失败！\n",argv[order]);
		}
		if (lexicalAnalysisEnter(argv[order])==OK)
		{
			printf("pre%s词法分析完毕！\n",argv[order]);
		}else{
			printf("pre%s词法分析处理失败！\n",argv[order]);
		}
		order++;
	};
	printf("可以运行");
	return 0;
}
