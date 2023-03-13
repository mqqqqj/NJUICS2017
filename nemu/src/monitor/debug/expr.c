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
  TK_HEX = 7,
  TK_DEREF = 8,
  TK_NEG = 9
  /* TODO: Add more token types */
};

enum {
  PRI_OR,
  PRI_AND,
  PRI_EQ,
  PRI_ADD,
  PRI_MUL
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
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {// find a rule match the char e
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
            tokens[i].type = 1;
            strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
            nr_token ++;
            break;
          case 2:   //equal to
            tokens[i].type = 2;
            strcpy(tokens[nr_token].str, "==");
            nr_token ++;
            break;
          case 3:
            tokens[i].type = 3;
            strcpy(tokens[nr_token].str, "!=");
            nr_token ++;
            break;
          case 4:
            tokens[i].type = 4;
            strcpy(tokens[nr_token].str, "&&");
            nr_token ++;
            break;
          case 5:
            tokens[i].type = 5;
            strcpy(tokens[nr_token].str, "||");
            nr_token ++;
            break;
          case 6:
            tokens[i].type = 6;
            strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
            nr_token ++;
            break;
          case 7:
            tokens[i].type = 7;
            strncpy(tokens[nr_token].str, &e[position - substr_len], substr_len);
            nr_token ++;
            break;
          case '+':
            tokens[i].type = '+';
            strcpy(tokens[nr_token].str, "+");
            nr_token ++;
            break;
          case '-':
            tokens[i].type = '-';
            strcpy(tokens[nr_token].str, "-");
            nr_token ++;
            break;
          case '*':
            tokens[i].type = '*';
            strcpy(tokens[nr_token].str, "*");
            nr_token ++;
            break;
          case '/':
            tokens[i].type = '/';
            strcpy(tokens[nr_token].str, "/");
            nr_token ++;
            break;
          case '!':
            tokens[i].type = '!';
            strcpy(tokens[nr_token].str, "!");
            nr_token ++;
            break;
          case '(':
            tokens[i].type = '(';
            strcpy(tokens[nr_token].str, "(");
            nr_token ++;
            break;
          case ')':
            tokens[i].type = ')';
            strcpy(tokens[nr_token].str, ")");
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
  printf("%s\n",tokens[p].type);
  printf("%s\n",tokens[q].type);
  if(tokens[p].type == '(' && tokens[q].type == ')') {
    // int num_lb = 0, num_rb = 0, index = p;
    // for ( ; index <= q; index ++) {
    //   if(tokens[index].type == '(') num_lb ++;
    //   if(tokens[index].type == ')') num_rb ++;
    // }
    // if(num_lb == num_rb) return true;
    // else return false;
    printf("222\n");
    int n = 0, index = p;
    for ( ; index <= q; index ++) {
      if(tokens[index].type == '(') n ++;
      if(tokens[index].type == ')') n --;
      if(n < 0) return false;
    }
    printf("n:%d",n);
    if(n == 0) return true;
    else return false;
  }  
  return false;
}

int dominant_operator(int p, int q) {
  int loc = -1, i = p, is_in_br = 0, priority = 99;
  for( ; i <= q; i ++) {
    if(tokens[i].type == '(') {
      is_in_br ++;
    } else if(tokens[i].type == ')') {
      is_in_br --;
    }
    if(is_in_br == 0) {
      if(tokens[i].type == TK_OR) {
        if(priority > PRI_OR) {
          priority = PRI_OR, loc = i;
        }
      } else if(tokens[i].type == TK_AND) {
        if(priority > PRI_AND) {
          priority = PRI_AND, loc = i;
        }
      } else if(tokens[i].type == TK_EQ || tokens[i].type == TK_NEQ) {
        if(priority > PRI_EQ) {
          priority = PRI_EQ, loc = i;
        }
      } else if(tokens[i].type == '+' || tokens[i].type == '-') {
        if(priority > PRI_ADD) {
          priority = PRI_ADD, loc = i;
        }
      } else if(tokens[i].type == '*' || tokens[i].type == '/') {
        if(priority > PRI_MUL) {
          priority = PRI_MUL, loc = i;
        }
      }
    }
  }
  return loc;
}

uint32_t eval(int p, int q) {
  if(p > q) {
    // bad expression
    assert(0);
  } else if(p == q) {
    // single token
    if(tokens[p].type == 1) {
      // oct num
      int val = 0;
      sscanf(tokens[p].str, "%d", &val);
      return val;
    } else if(tokens[p].type == 7) {
      // hex num
      int val = 0;
      sscanf(tokens[p].str, "%x", &val);
      return val;      
    } else if(tokens[p].type == 6) {  //reg
      if(!strcmp(tokens[p].str, "$eax")){
					return cpu.eax;
      } else if (!strcmp(tokens[p].str, "$ecx")){
        return cpu.ecx;
      } else if (!strcmp(tokens[p].str, "$edx")){
        return cpu.edx;
      } else if (!strcmp(tokens[p].str, "$ebx")){
        return cpu.ebx;
      } else if (!strcmp(tokens[p].str, "$esp")){
        return cpu.esp;
      } else if (!strcmp(tokens[p].str, "$ebp")){
        return cpu.ebp;
      } else if (!strcmp(tokens[p].str, "$esi")){
        return cpu.esi;
      } else if (!strcmp(tokens[p].str, "$edi")){
        return cpu.edi;
      } else if (!strcmp(tokens[p].str, "$eip")){
        return cpu.eip;
      } else {
        return 0;
      }
    } else {
      assert(0);
    }
  } else if(check_parentheses(p, q) == true) {
    // check the ( and ) is match
    printf("check parentheses ok.\n");
    return eval(p + 1, q - 1);
  } else {
    int op = dominant_operator(p, q);
    if(op == -1) {  // * ! -
      switch (tokens[p].type) {
        //case TK_DEREF: return *(eval(p + 1, q));
        case '!': return !eval(p + 1, q);
        case TK_NEG: return -eval(p + 1, q);
        default : assert(0);
      }
    } else {
      printf("%d\n",op);
      int val1 = eval(p, op - 1);
      int val2 = eval(op + 1, q);
      switch(tokens[op].type) {
        case '+': return val1 + val2;
        case '-': return val1 - val2;
        case '*': return val1 * val2;
        case '/': return val1 / val2;
        case TK_OR: return val1 || val2;
        case TK_AND: return val1 && val2;
        case TK_EQ: return (val1 == val2);
        case TK_NEQ: return (val1 != val2);
        default: assert(0);
      }
    }
  }
  return 0;
}


uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  /* TODO: Insert codes to evaluate the expression. */
  int i = 0;
  for( ; i < nr_token ;i ++) {
    if(tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type != TK_NUM && tokens[i - 1].type != TK_HEX && tokens[i - 1].type != ')'))) {
      tokens[i].type = TK_DEREF;
    } else if(tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type != TK_NUM && tokens[i - 1].type != TK_HEX && tokens[i - 1].type != ')'))) {
      tokens[i].type = TK_NEG;
    }
  }
  printf("nr token:%d\n", nr_token);
  for(i = 0; i < nr_token; i ++) printf("%s",tokens[i].str);
  printf("\n");
  return eval(0, nr_token - 1);
}
