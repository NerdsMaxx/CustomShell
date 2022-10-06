#include "utils_shell.h"

//Imprimir >>> no shell antes do usuário digitar.
void print_initial_chars(void){
    printf("\n>>> ");
}

//Imprimir erro de comando caso não consiga executar o comando.
void print_error_command(void){
    printf("Nao deu para executar o comando.\n");
}

//Variáveis globais pid e option, para modificar o waitpid.
static pid_t pid = -1;
static int option = 0;

//Setar um pid_arg para variável global pid.
void change_pid_for_wait(pid_t pid_arg){
    pid = pid_arg;
}

//Setar um option_arg para variável global option.
void change_option_for_wait(int option_arg){
    option = option_arg;
}

//Função que vai chamar waitpid e que serve para
//embutir dentro da função signal presente no shell.c.
void wait_child_pid(int signum){
	//Aqui estão as variáveis globais pid e option para waitpid.
	waitpid(pid, NULL, option);
}

//Função que vai executar execvp e se caso falhar
//imprime a mensagem de erro e saia do processo filho
//que estava executando.
void execvp_command(char* command[]){
    if (execvp(command[0], command) == -1){
        print_error_command();
        exit(EXIT_FAILURE);
    }
}

//Funçao responsável por mudar de diretório.
void change_directory(const char *argument){
	//Pegue número de palavras presente na string.
	const int number_of_words = get_number_of_words(argument);

	//Se ela for 1, então contém somente o argumento de diretório. 
	if(number_of_words == 1){
		//Muda de diretório e imprime Mudando para diretorio e o diretorio dela.
		if(chdir(argument) == 0){
			char directory[SIZE + 1];
			strcpy(directory, "");
			printf("Mudando para diretorio %s\n", getcwd(directory, SIZE));
		}
		else{
			//Se chdir falhar, então quer dizer não foi encontrado o diretório
			//e imprime a mensagem de que não foi encontrado o diretorio.
			printf("Nao foi encontrado o diretorio.\n");
		}
	}
	else{
		//Se número de palavras for maior que 1,
		//então usuário colocou mais de 1 argumento para cd.
		//Imprime a mensagem de argumentos execessivos.
		if(number_of_words > 1){
			printf("Argumentos excessivos.\n");
		}
		else{
			//Se número de palavras for 0, então o argumento não
			//foi passado pelo usuário. Imprime a mensagem
			//de nenhum argumento dado.
			printf("Nenhum argumento dado.\n");
		}
	}
}

//Função para sair do shell.
void exit_shell(int status){
    printf("Saindo do shell.\n");
    exit(status);
}