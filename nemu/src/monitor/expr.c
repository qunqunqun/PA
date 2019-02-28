#include "nemu.h"
#include "cpu/cpu.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#include "monitor/find_symbol.h"
#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,NEQ,NEG,//equal,not_equal,negative
	NUM,HEX,VAR,REG,//10 and 16
	AND,OR,XOR,NOT,//bits
	LOG_AND,LOG_OR,LOG_NOT,//&& || !
	DEREF,
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"\\+", '+'},
	{"-",'-'},
	{"\\*",'*'},
	{"/",'/'},//+-*/%
	{"%",'%'},

	{"\\(",'('},
	{"\\)",')'},//()

	{"==", EQ},
	{"!=",NEQ},

	{"0[xX][0-9a-fA-F]+",HEX},
	{"[0-9]+",NUM},
	{"[a-zA-Z0-9_]+",VAR},
	{"\\$e(ax|bx|cx|dx|sp|bp|di|si|ip)",REG},

	{"&{2}",LOG_AND},
	{"\\|{2}",LOG_OR},
	{"!",LOG_NOT},

	{"&",AND},
	{"\\|",OR},
	{"~",NOT},
	{"\\^",XOR},
	
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token=0;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				//printf("%d\n",nr_token);
				memset(tokens[nr_token].str,'\0',32);//chu shi hua 
				
				switch(rules[i].token_type) {
					case NOTYPE:break;
					case NUM: case REG:case VAR:case HEX:
					{	
						strncpy(tokens[nr_token].str,substr_start,substr_len);
			     			tokens[nr_token].type = rules[i].token_type;
						nr_token++;
						break;			
				       	}
					default: tokens[nr_token].type = rules[i].token_type;
							 nr_token ++;
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

int get_the_Pri(int op_type)
{
	switch(op_type)
	{
		case LOG_OR:return 1;break;
		case LOG_AND:return 2;break;
		case OR:return 3;break;
		case XOR:return 4;break;
		case AND:return 5;break;
		case EQ:case NEQ:
			 return 6;break;
		case '+': case '-':
			 return 7;break;
		case '*': case '/':case'%':
			 return 8;break;
		case NOT: case LOG_NOT: case DEREF: case NEG:
			 return 9;break;
		default:assert(0);
	}
}

int find_the_dominant(int p,int q)
{
	int pos=0;
	int pr_min=999;//zui da de you xian ji
	int left=0;
	for(int i=p;i<q;i++)
	{
		if(tokens[i].type=='(') left++;
		else if(tokens[i].type==')') left--;
		else if((left!=0)||tokens[i].type==NUM||tokens[i].type==VAR||tokens[i].type==REG||tokens[i].type==HEX)	continue;
		else
		{
			int temp=get_the_Pri(tokens[i].type);
			if(temp<=pr_min)
			{
				pos=i;
				pr_min=temp;
			}
		}
	}
	return pos;
}

bool check_parentheses(int p,int q)
{
	bool flag_base=(tokens[p].type=='(')&&(tokens[q].type==')');
	if(!flag_base) return  false;
	else
	{
		int left=0;
		int right=0;//zuo you"()"'s number
		for(int i=p+1;i<q;i++)
		{
			if(left<right)
			{
				return false;
			}
			if(tokens[i].type=='(')
			{
				left++;
			}
			else if(tokens[i].type==')')
			{
				left--;
				if(left<0) return 0;
				else if(left<right) return 0;
			}
		}
		if(left>0)
		{
			return false;
		}
		else return true;
	}
}


uint32_t eval(int p,int q,bool *success)
{
	if(p>q){
		/*Bad expression */
		printf("it's a  bad expression!\n");
		success=false;
		return 0;
	}
	else if(p==q){
		/*Single token
		 *must be a number
		 *return the value
		 */
		switch(tokens[p].type)
		{
			case NUM:
			{
				//printf("%d\n",res);
				return atoi(tokens[p].str);
				break;}
			case HEX:
			{
				printf("%s\n",tokens[p].str);
				uint32_t res=0;
				char *str = tokens[p].str;
				sscanf(str,"%x",&res);
				printf("%d\n",res);
				return res;
				break;
			}
			case VAR:
			{
				uint32_t res=0;
				res=look_up_symtab(tokens[p].str,success);
				if(*success==true)
				{
					//printf("%x\n",res);
					return res;
				}
				else
				{
					assert(0);
				}
			};
			case REG:
			{
				char *reg=tokens[p].str;
				if(strcmp(reg,"$eax")==0) return cpu.eax;
				else if(strcmp(reg,"$ebx")==0) return cpu.ebx;
				else if(strcmp(reg,"$ecx")==0) return cpu.ecx;
				else if(strcmp(reg,"$edx")==0) return cpu.edx;
				else if(strcmp(reg,"$ebp")==0) return cpu.ebp;
				else if(strcmp(reg,"$esp")==0) return cpu.esp;
				else if(strcmp(reg,"$edi")==0) return cpu.edi;
				else if(strcmp(reg,"$esi")==0) return cpu.esi;
				else if(strcmp(reg,"$eip")==0) return cpu.eip;
				else
				{
					success=false;
					printf("REG ERROR!\n");
				}
				break;	
			}
			default:assert(0);
		}	
	}
	else if(check_parentheses(p,q)==true){
		//printf("pi pei\n");
		//throw away the parentheses
		return eval(p+1,q-1,success);
	}
	else
	{
		//op = find the dominant operator
		int op = find_the_dominant(p,q);
		uint32_t val1=eval(op+1,q,success);
		switch(tokens[op].type)
		{
			case NEG: return -val1;break;
			case NOT: return ~val1;break;
			case LOG_NOT: return !val1;break;
			case DEREF:
			{
				return vaddr_read(val1,SREG_DS,4);
		       		break;
			}
			default:break;
		}
		uint32_t val2=eval(p,op-1,success);
		switch(tokens[op].type)
		{
			case'+': return val2+val1;break;
			case'-': return val2-val1;break;
			case'/': return ((int32_t)val2)/((int32_t)val1);break;
			case'*': return val2*val1;break;
			case'%': return val2%val1;break;
			case LOG_AND:return val2&&val1;break;
			case LOG_OR: return val2||val1;break;
			case AND: return val2&val1;break;
			case XOR: return val2 ^ val1;break;
			case OR: return val2 | val1;break;
			default:assert(0);
		}
	}

	return 0;
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	//printf("\nPlease implement expr at expr.c\n");
	//assert(0);
	
	//do with * and -
	for(int i=0;i<nr_token;i++)
	{
		if(tokens[i].type=='*'&&(i==0||tokens[i-1].type=='('||
						(tokens[i-1].type!=NUM&&tokens[i-1].type!=HEX&&
						tokens[i-1].type!=VAR&&tokens[i-1].type!=REG&&
						tokens[i-1].type!=')')))
		{
			tokens[i].type=DEREF;//zhi zhen jie yin yong
		}
		else if(tokens[i].type=='-'&&(i==0||tokens[i-1].type=='('||
						(tokens[i-1].type!=NUM&&tokens[i-1].type!=HEX&&
						tokens[i-1].type!=VAR&&tokens[i-1].type!=REG&&
						tokens[i-1].type!=')')))
		{
			tokens[i].type=NEG;//zhi zhen jie yin yong
		}
	}

	*success=true;
	return eval(0,nr_token-1,success);
}

