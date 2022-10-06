#include "utils_shell.h"

void print_initial_chars(void){
    printf("\n>> ");
}

void print_error_command(void){
    printf("Nao deu para executar o comando.\n");
}

static pid_t pid = -1;

void change_pid_for_wait(pid_t pid_arg){
    pid = pid_arg;
}

void wait_child_pid(int signum){
    if(waitpid(pid, NULL, WNOHANG) != -1){
        print_initial_chars();
    };
}

void execvp_command(char* command[]){
    if (execvp(command[0], command) == -1){
        print_error_command();
        exit(EXIT_FAILURE);
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
    printf("Saindo do shell.\n");
    exit(status);
}