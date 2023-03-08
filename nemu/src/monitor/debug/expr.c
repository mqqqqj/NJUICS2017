#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256,
  TK_NUM = 1,
  TK_EQ = 2,
  TK_NEQ = 3,
  TK_AND = 4,
  TK_OR = 5,
  TK_REG = 6,
  TK_HEX = 7
  /* TODO: Add more token types */
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"[0-9]+", TK_NUM},
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // sub
  {"\\*", '*'},         // multiply
	{"\\/", '/'},         //division
  {"\\(", '('},         // left bracket
  {"\\)", ')'},         // right bracket
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},       // not equal
  {"&&", TK_AND},       // and op
  {"\\|\\|", TK_OR},    // or op
  {"\\$[a-z]+", TK_REG},// registers
  {"0[xX][0-9a-fA-F]+", TK_HEX}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        int j;
				for (j = 0; j < 32; j++){ //clear
					tokens[nr_token].str[j] = '\0';
				}
        switch (rules[i].token_type) {
          case 256: //space
            break;
          case 1:   //num
            tokens[i].token_type = 1;
            strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
            nr_token ++;
            break;
          case 2:   //equal to
            token[i].token_type = 2;
            strcpy(tokens[nr_token].str, "==");
            nr_token ++;
            break;
          case 3:
            token[i].token_type = 3;
            strcpy(tokens[nr_token].str, "!=");
            nr_token ++;
            break;
          case 4:
            token[i].token_type = 4;
            strcpy(tokens[nr_token].str, "&&");
            nr_token ++;
            break;
          case 5:
            token[i].token_type = 5;
            strcpy(tokens[nr_token].str, "||");
            nr_token ++;
            break;
          case 6:
            token[i].token_type = 6;
            strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
            nr_token ++;
            break;
          case 7:
            token[i].token_type = 7;
            strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
            nr_token ++;
            break;
          case '+':
            token[i].token_type = '+';
            nr_token ++;
            break;
          case '+':
            token[i].token_type = '+';
            nr_token ++;
            break;
          case '-':
            token[i].token_type = '-';
            nr_token ++;
            break;
          case '*':
            token[i].token_type = '*';
            nr_token ++;
            break;
          case '/':
            token[i].token_type = '/';
            nr_token ++;
            break;
          case '!':
            token[i].token_type = '!';
            nr_token ++;
            break;
          case '(':
            token[i].token_type = '(';
            nr_token ++;
            break;
          case ')':
            token[i].token_type = ')';
            nr_token ++;
            break;
          default: assert(0);
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


bool check_parentheses(int p, int q) {
  return false;
}

int dominant_operator(int p, int q) {
  return 0;
}

uint32_t eval(int p, int q) {
  return 0;
}



uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}
