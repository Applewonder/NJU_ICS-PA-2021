#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

extern CPU_STATE cpu;

enum
{
	NOTYPE = 256,
	EQ,
	NUM,
	REG,
	SYMB,
	RV,
	NE,
	NEQ,

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // white space
	{"\\+", '+'},
	{"[0-9]{1,10}", NUM},
	{"-", '-'},
	{"\\*", '*'},
	{"\\(", '('},
	{"\\)", ')'},
	{"==", EQ},
	{"!=", NEQ},
	{"&&", '&'},
	{"\\|\\|", '|'},
	{"\\!", '!'},
	{"/", '/'},
	{"\\$ebp", REG},
	{"\\$esp", REG},
	{"\\$eax", REG},
	{"\\$edx", REG},
	{"\\$ecx", REG},
	{"\\$ebx", REG},
	{"\\$esi", REG},
	{"\\$edi", REG},
	{"\\b(?:(?:auto\\s*|const\\s*|unsigned\\s*|signed\\s*|register\\s*|volatile\\s*|static\\s*|void\\s*|short\\s*|long\\s*|char\\s*|int\\s*|float\\s*|double\\s*|_Bool\\s*|complex\\s*)+)(?:\\s+\\*?\\*?\\s*)([a-zA-Z_][a-zA-Z0-9_]*)\\s*[\\[;,=)]", SYMB}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
                
				switch (rules[i].token_type)
				{
			    NOTYPE:
			        
			        break;
			        
				default:
					tokens[nr_token].type = rules[i].token_type;
					strncpy(tokens[nr_token].str, substr_start, substr_len);
					nr_token++;
					break;
					
			    }

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

static bool check_parentheses(int p, int q) {
    int a = 0;
    int b = 0;
    for (int i = p; i <= q; i++) {
        if(tokens[i].type == '(') a++;
        if(tokens[i].type == ')') b++;
    }
    if (a == b) return true;
    return false;
}

uint32_t eval(int p, int q, bool *success) {
    if(p > q) {
        *success = false;
    } else if(p == q) { 
        /* Single token.
         * For now this token should be a number. 
         * Return the value of the number.
         */ 
         if (tokens[p].type == NUM) {
             uint32_t i = atoi(tokens[p].str);
             return i;
         }
         if (tokens[p].type == REG) {
             if (tokens[p].str[2] == 'a') {
                 return cpu.eax;
             } else if (tokens[p].str[2] == 'c') {
                 return cpu.ecx;
             } else if (tokens[p].str[2] == 'd' && tokens[p].str[3] == 'x') {
                 return cpu.edx;
             } else if (tokens[p].str[2] == 'd' && tokens[p].str[3] == 'i') {
                 return cpu.edi;
             } else if (tokens[p].str[2] == 'b' && tokens[p].str[3] == 'x') {
                 return cpu.ebx;
             } else if (tokens[p].str[2] == 'b' && tokens[p].str[3] == 'p') {
                 return cpu.ebp;
             } else if (tokens[p].str[2] == 's' && tokens[p].str[3] == 'p') {
                 return cpu.esp;
             } else if (tokens[p].str[2] == 's' && tokens[p].str[3] == 'i') {
                 return cpu.esi;
             } 
         }
         if (tokens[p].type == SYMB) {
             return look_up_fun_symtab(tokens[p].str, success);
         }
    }
    else if(check_parentheses(p, q) == true) {
        /* The expression is surrounded by a matched pair of parentheses. 
         * If that is the case, just throw away the parentheses.
         */
        return eval(p + 1, q - 1, success); 
    }
    else {
        /* We should do more things here. */
        int count = 0;
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                count++;
                for (int j = i + 1; j <= q; j++) {
                    if (tokens[j].type == ')') {
                        count++;
                }
                    if (tokens[j].type == ')') {
                        count--;
                }
                if (count == 0) {
                    i = j;
                    break;
                }
            }
            if (tokens[i].type == EQ) {
                if(eval(p, i - 1, success) != eval(i + 1, q, success)) return 0;
                return 1;
            }
        }
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                count++;
                for (int j = i + 1; j <= q; j++) {
                    if (tokens[j].type == ')') {
                        count++;
                }
                    if (tokens[j].type == ')') {
                        count--;
                }
                if (count == 0) {
                    i = j;
                    break;
                }
            }
            if (tokens[i].type == NEQ) {
                if(eval(p, i - 1, success) != eval(i + 1, q, success)) return 1;
                return 0;
            }
        }
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                count++;
                for (int j = i + 1; j <= q; j++) {
                    if (tokens[j].type == ')') {
                        count++;
                }
                    if (tokens[j].type == ')') {
                        count--;
                }
                if (count == 0) {
                    i = j;
                    break;
                }
            }
            if (tokens[i].type == '|') {
                if(eval(p, i - 1, success) || eval(i + 1, q, success)) return 1;
                return 0;
            }
        }
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                count++;
                for (int j = i + 1; j <= q; j++) {
                    if (tokens[j].type == ')') {
                        count++;
                }
                    if (tokens[j].type == ')') {
                        count--;
                }
                if (count == 0) {
                    i = j;
                    break;
                }
            }
            if (tokens[i].type == '&') {
                if(eval(p, i - 1, success) && eval(i + 1, q, success)) return 1;
                return 0;
            }
        }
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                count++;
                for (int j = i + 1; j <= q; j++) {
                    if (tokens[j].type == ')') {
                        count++;
                }
                    if (tokens[j].type == ')') {
                        count--;
                }
                if (count == 0) {
                    i = j;
                    break;
                }
            }
            if (tokens[i].type == '!') {
                if(!eval(p+1, q, success)) return 1;
                return 0;
            }
        }
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                count++;
                for (int j = i + 1; j <= q; j++) {
                    if (tokens[j].type == ')') {
                        count++;
                }
                    if (tokens[j].type == ')') {
                        count--;
                }
                if (count == 0) {
                    i = j;
                    break;
                }
            }
            if (tokens[i].type == '+') {
                return eval(p, i - 1, success) + eval(i + 1, q, success);
            }
        }
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                count++;
                for (int j = i + 1; j <= q; j++) {
                    if (tokens[j].type == ')') {
                        count++;
                }
                    if (tokens[j].type == ')') {
                        count--;
                }
                if (count == 0) {
                    i = j;
                    break;
                }
            }
            if (tokens[i].type == '-') {
                return eval(p, i - 1, success) - eval(i + 1, q, success);
            }
        }
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                count++;
                for (int j = i + 1; j <= q; j++) {
                    if (tokens[j].type == ')') {
                        count++;
                }
                    if (tokens[j].type == ')') {
                        count--;
                }
                if (count == 0) {
                    i = j;
                    break;
                }
            }
            if (tokens[i].type == '*') {
                return eval(p, i - 1, success) * eval(i + 1, q, success);
            }
        }
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == '(') {
                count++;
                for (int j = i + 1; j <= q; j++) {
                    if (tokens[j].type == ')') {
                        count++;
                }
                    if (tokens[j].type == ')') {
                        count--;
                }
                if (count == 0) {
                    i = j;
                    break;
                }
            }
            if (tokens[i].type == '/') {
                return eval(p, i - 1, success) / eval(i + 1, q, success);
            }
        }
        if (tokens[p].type == RV) {
            uint32_t m = eval(p + 1, q, success);
            return vaddr_read(m, 0, 1);
        }
        if (tokens[p].type == NE) {
            uint32_t m = eval(p + 1, q, success);
            return -m;
        }
     }
}


uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}
	
	for(int i = 0; i < nr_token; i++) {
	    if (i == 0){
	        if(tokens[0].type == '*') tokens[0].type = RV;
	        if(tokens[0].type == '-') tokens[0].type = NE;
	    }
	    if (tokens[i].type == '-') {
	        if(tokens[i-1].type == '(' || tokens[i-1].type == '+' || tokens[i-1].type == '-' || tokens[i-1].type == EQ || tokens[i-1].type == '/' || tokens[i-1].type == '*') {
	            tokens[i].type = NE;
	        }
	    }
	    if (tokens[i].type == '*') {
	        if(tokens[i-1].type != NUM && tokens[i-1].type != ')' && tokens[i-1].type != REG) {
	            tokens[i].type = RV;
	        }
	    }
	}

	uint32_t result = eval(0, nr_token, success);

	return result;
}
