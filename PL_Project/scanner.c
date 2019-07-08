#include <string.h>
#include "struct.h"
extern struct Input code[500000];
int scanner(char *input)
{
	int index =0;
	/** type 
	  word	: 0
	  (	: 1
	  )	: 2
	  ;	: 3
	  ,	: 4
	  int	: 5
	  char	: 6
	  void	: 7
	  {	: 8
	  }	: 9
	  IF	: 10
	  THEN	: 11
	  ELSE	: 12
	  WHILE : 13
	  =	: 14
	  RETURN: 15
	  >	: 16
	  ==	: 17
	  +	: 18
	  *	: 19
	  num	: 20
	  $	: 21
	 **/
	while(*input)
	{
        while(1)
        {
    		while(*input==' ' || *input =='\t' || *input == '\n')	input++;
            if(*input=='#')
            {
                while(*input!=0 && *input!='\n')
                    input++;
            }
            else break;
        }
		if(*input==0) break;
		struct Input tmp;
		tmp.str = NULL;
		if(*input=='(')
		{
			tmp.type = 1;
			input++;
		}
		else if(*input==')')
		{
			tmp.type = 2;
			input++;
		}
		else if(*input==';')
		{
			tmp.type = 3;
			input++;
		}
		else if(*input==',')
		{
			tmp.type = 4;
			input++;
		}
		else if(strncmp(input,"int ",4)==0 || strncmp(input,"int\t",4)==0 || strncmp(input,"int\n",4)==0)
		{
			tmp.type = 5;
			input+=4;
		}
		else if(strncmp(input,"char ",5)==0 || strncmp(input,"char\t",5)==0 || strncmp(input,"char\n",5)==0)
		{
			tmp.type = 6;
			input+=5;
		}
		else if(strncmp(input,"void ",5)==0 || strncmp(input,"void\t",5)==0 || strncmp(input,"void\n",5)==0)
		{
			tmp.type = 7;
			input+=5;
		}
		else if(*input=='{')
		{
			tmp.type = 8;
			input++;
		}
		else if(*input=='}')
		{
			tmp.type = 9;
			input++;
		}
		else if(strncmp(input,"IF",2)==0)
		{
			tmp.type = 10;
			input+=2;
		}
		else if(strncmp(input,"THEN ",5)==0 || strncmp(input,"THEN\t",5)==0 || strncmp(input,"THEN\n",5)==0)
		{
			tmp.type = 11;
			input+=5;
		}
		else if(strncmp(input,"ELSE ",5)==0 || strncmp(input,"ELSE\t",5)==0 || strncmp(input,"ELSE\n",5)==0)
		{
			tmp.type = 12;
			input+=5;
		}
		else if(strncmp(input,"WHILE ",6)==0 || strncmp(input,"WHILE\t",6)==0 || strncmp(input,"WHILE\n",6)==0)
		{
			tmp.type = 13;
			input+=6;
		}
		else if(*input=='=')
		{
			tmp.type = 14;
			input++;
		}
		else if(strncmp(input,"RETURN ",7)==0 || strncmp(input,"RETURN\t",7)==0 || strncmp(input,"RETURN\n",7)==0)
		{
			tmp.type = 15;
			input+=7;
		}
		else if(*input=='>')
		{
			tmp.type = 16;
			input++;
		}
		else if(strncmp(input,"==",2)==0)
		{
			tmp.type = 17;
			input+=2;
		}
		else if(*input=='+')
		{
			tmp.type = 18;
			input++;
		}
		else if(*input=='*')
		{
			tmp.type = 19;
			input++;
		}
		else
		{
			char *p = input;
			while(('a'<=*p&&*p<='z')||('A'<=*p&&*p<='Z')||('0'<=*p&&*p<='9'))	p++;
			int token_length = (int)(p-input);
			int num_len =0;
			int word_len=0;

			for(int i=0;i<token_length;i++)
			{
				if('a'<=*(input+i)&&*(input+i)<='z' || 'A'<=*(input+i)&&*(input+i)<='Z')	word_len++;
				else if('0'<=*(input+i)&&*(input+i)<='9')	num_len++;
			}
			if(num_len == token_length)	tmp.type = 20;
			else if(word_len == token_length)	tmp.type = 0;
			else
			{
				printf("Error on scanning process : Invalid input... Exit.\n");
				exit(0);
			}
			if(tmp.type>=0)
			{
				char *token_str = (char *)malloc(sizeof(char)*token_length);
				strncpy(token_str,input,token_length);
				if(tmp.type == 0)	tmp.str = token_str;
				if(tmp.type == 20)	tmp.value = atoi(token_str);
				input+=token_length;
			}
		}
		code[index] = tmp;
		/* Type check & what it is in

		printf("type : %d",tmp.type);
		if(tmp.type==0)
		{
			printf(", string : %s",tmp.str);
		}
		if(tmp.type==20)
		{
			printf(", value : %d",tmp.value);
		}
		printf("\n");
		*/
		index++;
	}
	code[index].type = 21;
    printf("scanner finish\n");
    fflush(stdout);
	return 0;
}
