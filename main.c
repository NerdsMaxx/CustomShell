#include "shell.h"

int main(void)
{
    /*
        Onde vai ser executado a função shell, podendo escolher
        se interpretador de comandos vai ser executado por outro thread
        ou no thread principal.
    */
    bool is_thread = true;
    shell(is_thread);
}
