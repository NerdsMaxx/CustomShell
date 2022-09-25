#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

char* dynalloc_remove_excessives_spaces(char *source);
int get_number_of_words(const char *source);
char* dynalloc_remove_substring(const char *source, const char *substring);
void print_int_chars(const char *source);
void dynalloc_strcpy_and_free_alloc(char *dynalloc_source, char *destiny);
bool contain_last_substring(const char *source, const char *substring);