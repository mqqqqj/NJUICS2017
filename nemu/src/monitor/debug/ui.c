#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);//-1为无符号数的最大整数，这样用可以执行完所有指令
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args) {
  char *arg = strtok(NULL, " ");
  int n = 1;
  if(arg != NULL) {
    sscanf(arg, "%d", &n);
  }
  cpu_exec(n);
  return 0;
}

static int cmd_info(char *args) {
  char *arg = strtok(NULL, " ");
  int i;
  if(strcmp(arg,"r") == 0) {
    for(i = 0; i < 8; i ++) {
      printf("%s\t\t", regsl[i]);
      printf("0x%08x\t\t%d\n", cpu.gpr[i]._32, cpu.gpr[i]._32);
    }
    printf("eip:\t\t0x%08x\t\t%d\n", cpu.eip, cpu.eip);
  } else if(strcmp(arg,"w") == 0) {
    puts("print watch point infomation...... Nothing happened");
  } else {
    puts("unknown arg or miss input");
  }
  return 0;
}

static int cmd_p(char *args) {
  // Expression evaluation
  return 0;
}

static int cmd_x(char *args) {
  // scan mem
  char *arg_1 = strtok(NULL, " ");
  char *arg_2 = strtok(NULL, " ");
  int step, address;
  sscanf(arg_1, "%d", &step);
  sscanf(arg_2, "%x", &address);
  printf("0x%x:",address);  
  int i;
  for(i = 0; i < step; i ++){  
    printf("0x%8x  0x%x\n",address + i*32,swaddr_read(address + i * 32,32));
  }
  return 0;  
}

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "One step execute for N times", cmd_si},
  { "info", "print registers status | watchpoint infomations", cmd_info},
  { "x", "scan memory", cmd_x},
  { "p", "expression evaluation", cmd_p},

  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
