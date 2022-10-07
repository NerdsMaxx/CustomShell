#include "shell.h"
#include "utils_shell.h"

void *command_interpreter_thread(void *argument);
void command_interpreter(char *line, int length_line);

// Struct para passar como argumanto para função
// que vai ser executado em um thread.
typedef struct
{
    char *line;
    int length_line;
} ci_args;

void shell(bool is_thread)
{

    // Imprime bem-vindos
    printf(YELLOW);
    printf("Bem-vindo ao shell!!!\n\n");

    // Fica em loop infinito até usuário escrever exit e apertar enter.
    while (true)
    {
        // Inicializando linha com tamanho de 250 mais uma posição reservada
        // para caractere nulo.
        char line[SIZE + 1];
        bzero(line, SIZE + 1);

        // Imprime >>> no shell antes do usuário digitar alguma coisa.
        print_initial_chars();

        // Leia a linha antes de achar \n.
        int index_line = 0;
        for (; index_line < SIZE && (line[index_line] = getchar()) != ('\n'); ++index_line)
            ;
        line[index_line] = '\0';

        // Remover os espaços excessivos entre as palavras
        char *result = dynalloc_remove_excessives_spaces(line);

        // Caso o resultado sobrou nada depois de remover espaço,
        // provavelmente a linha só contém espaço,
        // então volta para parte inicial do while.
        if (strcmp(result, "") == 0)
        {
            dynalloc_delete(result);
            continue;
        }

        // Copiar o conteúdo do resultado para linha.
        dynalloc_strcpy_and_free_alloc(
            result,
            line);

        // Se is_thread for verdadeiro, então cria uma thread
        // e executa a função dentro dele que vai chamar
        // o intepretador de comandos.
        if (is_thread)
        {
            pthread_t thread;

            ci_args argument;
            argument.line = line;
            argument.length_line = index_line;

            pthread_create(&thread, NULL, &command_interpreter_thread, &argument);
            pthread_join(thread, NULL);
        }
        else
        {
            // Se não, chama interpretador de comandos direto.
            command_interpreter(line, index_line);
        }
    }
}

// Função para executar dentro do pthread_create.
void *command_interpreter_thread(void *argument)
{
    ci_args arg = *((ci_args *)argument);
    command_interpreter(arg.line, arg.length_line);

    pthread_exit(NULL);
}

// A função responsável pelo intepretador de comandos.
void command_interpreter(char *line, int length_line)
{

    // Inicializa is_background e is_exec como falso.
    bool is_background = false;
    bool is_exec = false;

    // Verifica se usuário digitou exit, se sim,
    // saia do shell.
    if (strncmp(line, "exit", 4) == 0)
    {
        exit_shell(EXIT_SUCCESS);
    }

    // Verifica se usuário digitou & no final de comando.
    // Se sim, setar is_background verdadeiro e remover & no
    // final do comando.
    if (is_last_substring(line, " &"))
    {
        char *result = dynalloc_remove_substring(line, " &");

        if (strcmp(result, "") == 0)
        {
            dynalloc_delete(result);
            print_error_command();
            return;
        }

        is_background = true;

        dynalloc_strcpy_and_free_alloc(
            result,
            line);
    }

    // Verifica se usuário digitou exec no inicio de comando.
    // Se sim, setar is_exec verdadeiro e remover exec no
    // inicio do comando.
    if (strncmp(line, "exec ", 5) == 0 && strlen(line) > 5)
    {
        char *result = dynalloc_remove_substring(line, "exec ");

        if (strcmp(result, "") == 0)
        {
            dynalloc_delete(result);
            print_error_command();
            return;
        }

        is_exec = true;

        dynalloc_strcpy_and_free_alloc(
            result,
            line);
    }

    // Se usuário digitou cd no inicio do comando, então
    // executar a função change_directory responsável por
    // mudar de diretório.
    if (strncmp(line, "cd", 2) == 0)
    {
        if (line[2] == '\0' || line[2] == ' ')
        {
            change_directory((line[2] == '\0') ? line + 2 : line + 3);
            return;
        }
    }

    // Se usuário não digitou cd, então executar o comando que ele digitou
    // pelo execvp.

    // O tamanho máximo do comando vai ser 10 (1 comando e 9 argumentos).
    const int command_size = 10;
    char *command[command_size];

    // Inicializando todos os comandos com NULL.
    for (int index = 0; index < command_size; command[index++] = NULL)
        ;

    // A primeira posição do comando vai armazenar um comando
    // escrito pelo usuário.
    command[0] = line;

    // Se número de palavras for maior que 1 na linha, então
    // vai separação do resto da linha.
    int number_of_words = get_number_of_words(line);
    if (number_of_words > 1)
    {

        // Como a primeira posição do comando já armazenou o comando principal,
        // então setar index para 1 e diminuir número de palavras.
        int index_command = 1;
        --number_of_words;

        // Rodando iteração até chegar no ultímo indice da linha.
        for (int index_line = 0; index_line < length_line; ++index_line)
        {
            // Quando em determinada iteração achar um espaço,
            // ele imediamente já coloca um caracter nulo nesta posição onde
            // foi achado o espaço.
            if (line[index_line] != ' ')
            {
                continue;
            }

            line[index_line] = '\0';

            // Já diminue o número de palavra, já que vai ter a próxima palavra na
            // outra posição. E o comando já vai armazernar também a próxima palavra.
            --number_of_words;
            if (index_command < command_size)
            {
                command[index_command++] = line + index_line + 1;
            }

            // Se comando não tiver mais posições para armazenar ou
            // acabar as palavras, pode quebrar a iteração.
            if (index_command >= command_size || number_of_words == 0)
            {
                break;
            }
        }
    }

    // Para receber ID do processo filho
    pid_t pid = -1;

    // Se is_exec for verdadeiro, então é para substituir o processo
    // atual para este comando. Não vai ser criado processo filho.
    if (is_exec)
    {
        execvp_command(command);
    }
    else
    {
        // Senão, então cria um processo filho que vai
        // ser substituido por este comando, sem fechar o processo pai.
        if ((pid = fork()) == 0)
        {
            execvp_command(command);
        }
    }

    // Alterando o pid internamente do utils_shell.c
    // para conseguir usar waitpid para esperar que
    // o processo filho deste pid termine.
    change_pid_for_wait(pid);

    // Se is_background for verdadeiro.
    if (is_background)
    {
        // Alterando a opção (que está no utils_shell.c) do wait para 1, 
        //para permitir que comando em background não impeça de usuário
        // digitar outros comandos enquanto background
        // está em ação.
        change_option_for_wait(1);

        // chama signal e executar a função do waitpid para
        // evitar processo zumbi por parte do processo filho.
        signal(SIGCHLD, wait_child_pid);
    }
    else
    {
        //Muda a opção do wait para 0 e chama só wait sem usar signal, 
        //para esperar que o processo filho termine normalmente.
        change_option_for_wait(0);
        wait_child_pid(0);
    }
}
