#include "shell.h"

void print_initial_chars();
void wait_child(int signum);
void execvp_command(char* command[]);
void* command_interpreter_thread(void *argument);
void command_interpreter(char *line, int length_line);
void change_directory(const char *argument);
void exit_shell(int status);

typedef struct {
    char* line;
    int length_line;
} ci_args;

void shell(bool is_thread){
    printf(GREEN);
    printf("Bem-vindo ao shell!!!\n\n");
    
    print_initial_chars();

	while (true){
        char line[SIZE];
        bzero(line, SIZE);


		int index_line = 0;
		for (; (line[index_line] = getchar()) != ('\n'); ++index_line);
		line[index_line] = '\0';

        dynalloc_strcpy_and_free_alloc(
            dynalloc_remove_excessives_spaces(line), 
            line
        );
		
        if(is_thread){
            pthread_t thread;
            
            ci_args argument;
            argument.line = line;
            argument.length_line = index_line;

            pthread_create(&thread, NULL, command_interpreter_thread, &argument);
            pthread_join(thread, NULL);
        }
        else{
            command_interpreter(line, index_line);
        }
        
	}
}

void print_initial_chars(){
    printf(GREEN);
    printf("\n>> ");

    printf(YELLOW);
}

void wait_child(int signum){
    if(wait(NULL) != -1){
        print_initial_chars();
    };
}

void execvp_command(char* command[]){
    if (execvp(command[0], command) == -1){
        printf("Nao tem como executar o comando.\n");
        exit(EXIT_FAILURE);
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

    if (strcmp(line, "exit") == 0){
        exit_shell(EXIT_SUCCESS);
    }

    if(is_last_substring(line, " &")){
        is_background = true;

        dynalloc_strcpy_and_free_alloc(
            dynalloc_remove_substring(line, " &"),
            line
        );
    }

    if (strncmp(line, "exec ", 5) == 0 && strlen(line) > 5){
        is_exec = true;

        dynalloc_strcpy_and_free_alloc(
            dynalloc_remove_substring(line, "exec "), 
            line
        );
    }

    if (strncmp(line, "cd", 2) == 0){
        if(line[2] == '\0' || line[2] == ' '){
            change_directory((line[2] == '\0') ? line + 2 : line + 3);
            print_initial_chars();
            return;
        }
    }

    const int command_size = 10;
    char *command[command_size];
    for (int index = 0; index < command_size; command[index++] = NULL);
    command[0] = line;

    int number_of_words = get_number_of_words(line);
    if (number_of_words > 1){

        int index_command = 1;
        --number_of_words;

        for (int index_line = 0; index_line < length_line; ++index_line){
            if (line[index_line] != ' '){
                continue;
            }
            
            line[index_line] = '\0';
            --number_of_words;
            if (index_command < command_size){
                command[index_command++] = line + index_line + 1;
            }
            
            if (index_command >= command_size || number_of_words == 0){
                break;
            }
        }
    }

    if (is_exec){
            execvp_command(command);
    }
    else {
        if (fork() == 0){
            execvp_command(command);
        }
    }

    
    
    if(is_background){
        signal(SIGCHLD, wait_child);
    }
    else{
        wait_child(0);
    }
}

void change_directory(const char *argument){
	const int number_of_words = get_number_of_words(argument);

	if(number_of_words == 1){
		if(chdir(argument) == 0){
			char directory[SIZE];
			strcpy(directory, "");
			printf("Mudando para diretorio %s\n", getcwd(directory, SIZE));
		}
		else{
			printf("Nao foi encontrado o diretorio.\n");
		}
	}
	else{
		if(number_of_words > 1){
			printf("Argumentos excessivos.\n");
		}
		else{
			printf("Nenhum argumento dado.\n");
		}
	}
}

void exit_shell(int status){
    printf("Saindo do MShell.\n");
    exit(status);
}