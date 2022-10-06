#include "shell.h"
#include "utils_shell.h"

void* command_interpreter_thread(void *argument);
void command_interpreter(char *line, int length_line);

typedef struct {
    char* line;
    int length_line;
} ci_args;

void shell(bool is_thread){
    printf(YELLOW);
    printf("Bem-vindo ao shell!!!\n\n");

	while (true){
        char line[SIZE + 1];
        bzero(line, SIZE + 1);

        print_initial_chars();

		int index_line = 0;
		for (; index_line < SIZE && (line[index_line] = getchar()) != ('\n'); ++index_line);
		line[index_line] = '\0';
        
        char* result = dynalloc_remove_excessives_spaces(line);

        if (strcmp(result, "") == 0) {
            dynalloc_delete(result);
            print_error_command();
            continue;
        }

        dynalloc_strcpy_and_free_alloc(
            result,
            line
        );

        if (is_thread) {
            pthread_t thread;
            
            ci_args argument;
            argument.line = line;
            argument.length_line = index_line;

            pthread_create(&thread, NULL, &command_interpreter_thread, &argument);
            pthread_join(thread, NULL);
        }
        else {
            command_interpreter(line, index_line);
        }
        
	}
}

void* command_interpreter_thread(void *argument){
    ci_args arg = *((ci_args *) argument);
    command_interpreter(arg.line, arg.length_line);

    pthread_exit(NULL);
}

void command_interpreter(char *line, int length_line){
    
    bool is_background = false;
    bool is_exec = false;

    if (strcmp(line, "exit") == 0) {
        exit_shell(EXIT_SUCCESS);
    }

    if (is_last_substring(line, " &")) {
        char* result = dynalloc_remove_substring(line, " &");

        if(strcmp(result, "") == 0){
            dynalloc_delete(result);
            print_error_command();
            return;
        }

        is_background = true;

        dynalloc_strcpy_and_free_alloc(
            result,
            line
        );
    }

    if (strncmp(line, "exec ", 5) == 0 && strlen(line) > 5) {
        char* result = dynalloc_remove_substring(line, "exec ");

        if(strcmp(result, "") == 0){
            dynalloc_delete(result);
            print_error_command();
            return;
        }

        is_exec = true;

        dynalloc_strcpy_and_free_alloc(
            result,
            line
        );
    }

    if (strncmp(line, "cd", 2) == 0) {
        if (line[2] == '\0' || line[2] == ' ') {
            change_directory((line[2] == '\0') ? line + 2 : line + 3);
            return;
        }
    }

    const int command_size = 10;
    char *command[command_size];
    for (int index = 0; index < command_size; command[index++] = NULL);
    command[0] = line;

    int number_of_words = get_number_of_words(line);
    if (number_of_words > 1) {

        int index_command = 1;
        --number_of_words;

        for (int index_line = 0; index_line < length_line; ++index_line) {
            if (line[index_line] != ' ') {
                continue;
            }
            
            line[index_line] = '\0';
            --number_of_words;
            if (index_command < command_size) {
                command[index_command++] = line + index_line + 1;
            }
            
            if (index_command >= command_size || number_of_words == 0) {
                break;
            }
        }
    }

    pid_t pid = -1;
    if (is_exec) {
            execvp_command(command);
    }
    else {
        if ( (pid = fork()) == 0) {
            execvp_command(command);    
        }
    }

    change_pid_for_wait(pid);
    if (is_background){
        change_option_for_wait(1);
        signal(SIGCHLD, wait_child_pid);
    }
    else {
        change_option_for_wait(0);
        wait_child_pid(0);
    }
}

