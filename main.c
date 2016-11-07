#include "myhead.h"
#include "run.h"

	int main()
	{
		
		FILE *fin,*fout;

		fin = fopen("input.txt", "r");
		index = 0;
		current_row = 1;
		while ((ch = fgetc(fin)) != EOF)
			progrem[index++] = ch;
		progrem[index] = '\0';
		code_head = (struct myStruct *) malloc(sizeof( myStruct));
		code_p = code_head;
		code_next= (struct myStruct *) malloc(sizeof(myStruct));

		code_head->next = code_p;
		
		index = 0;
		ch = progrem[index++];
		do
		{
			lexical_analysis();
			if (syn != -1) {
				code_p->next = code_next;
				code_p = code_next;
				code_p->row = current_row;
				code_p->syn = syn;
				if (syn == 11)
					code_p->sum = sum;
				else code_p->sum = currentFloor;
				strcpy(code_p->string, token);
				code_next = (struct myStruct *) malloc(sizeof(struct myStruct));
			}
		}while (syn != 0);
		code_p->next = NULL;
		code_p = code_head->next;
		
		
		
	/*	while (code_p != NULL)
		{
			printf("<%d %s %d %d>\n", code_p->syn, code_p->string, code_p->sum, code_p->row);
			code_p = code_p ->next;
		}
		*/
		
		code_p = code_head->next;
		struct  synStruct syn_h;
		struct  synStruct *syn_h2 = &syn_h;

		syn_analy(code_p, &syn_h);
		run_raw_analy(syn_h2);
		int raw_i = 0;
		fout = fopen("output.txt", "w+");
		while (raw_i<run_raw_i) {
			fprintf(fout,"%d ",run_raw[raw_i++]);
		}
		fclose(fin);
		fclose(fout);
		free(code_head);
		free(code_next);
		return 1;
	}

