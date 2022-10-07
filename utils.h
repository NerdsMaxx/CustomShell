#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

char* dynalloc_remove_excessives_spaces(const char *source);
int get_number_of_words(const char *source);
char* dynalloc_remove_substring(const char *source, const char *substring);
void dynalloc_strcpy_and_free_alloc(char *dynalloc_source, char *destiny);
void dynalloc_delete(char* source);
bool is_last_substring(const char *source, const char *substring);