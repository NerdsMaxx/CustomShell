#include "shell.h"

void print_initial_chars(void);
void print_error_command(void);
void change_pid_for_wait(pid_t pid_arg);
void change_option_for_wait(int option_arg);
void wait_child_pid(int signum);
void execvp_command(char* command[]);
void change_directory(const char *argument);
void exit_shell(int status);
