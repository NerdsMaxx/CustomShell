#include "utils.h"

char* dynalloc_remove_excessives_spaces(const char *source){
	int index_source = 0;
	int length_source = strlen(source);

	char *dynalloc_result = calloc(length_source + 1, sizeof(char));
	strcpy(dynalloc_result, "");
	
	if(length_source == 0){
		return dynalloc_result;
	}
	
	int length_result = 0;

	for(; index_source < length_source && source[index_source] == ' '; ++index_source);
	
	bool found = false;
	for(; index_source < length_source; ++index_source){
		if(source[index_source] == ' '){
			if(found){
				continue;
			}

			found = true;
		}
		else{
			found = false;
		}

		strncat(dynalloc_result, source + index_source, 1);
		++length_result;
	}

	if(length_result > 0){
		if (dynalloc_result[length_result - 1] == ' '){
			dynalloc_result[length_result - 1] = '\0';
			--length_result;
		}
	}

	return dynalloc_result;
}

int get_number_of_words(const char *source){
    int index = 0;
	int length = strlen(source);
	int number_of_words = 0;

    while(index < length){
        for(; index < length && source[index] == ' '; ++index);
        number_of_words += (index < length) ? 1 : 0;
	    for(; index < length && source[index] != ' '; ++index);
    }

    return number_of_words;
}

char* dynalloc_remove_substring(const char *source, const char *substring){
	int length_source = strlen(source);
	int length_substring = strlen(substring);
	
	char *dynalloc_result = calloc(length_source + 1, sizeof(char));
	strcpy(dynalloc_result, "");

	if(length_source == 0){
		return dynalloc_result;
	}

	if(length_substring > length_source){
		strcpy(dynalloc_result, source);
		return dynalloc_result;
	}

	int index_start_substring = -1;
	int index_result = 0;
	int limit = (length_source - length_substring) + 1;
	for(int index_source = 0; index_source < length_source; ++index_source){
		if(index_source < limit && strncmp(source + index_source, substring, length_substring) == 0){
			index_start_substring = index_source;
			index_source = index_start_substring + length_substring - 1;
		}
		else{
			dynalloc_result[index_result++] = source[index_source];
		}
	}

	return dynalloc_result;
}

void print_int_chars(const char *source){
	for(int index = 0; source[index] != '\0'; ++index){
		printf("%d ", source[index]);
	}
	printf("\n");
}

void dynalloc_strcpy_and_free_alloc(char *dynalloc_source, char *destiny){
	if(dynalloc_source != NULL){
		strcpy(destiny, dynalloc_source);
		dynalloc_delete(dynalloc_source);
	}
}

void dynalloc_delete(char* source){
	if(source != NULL){
		free(source);
		source = NULL;
	}
}

bool is_last_substring(const char *source, const char *substring){
	int length_source = strlen(source);
	int length_substring = strlen(substring);

	if(length_substring > length_source){
		return false;
	}

	return strncmp(source + length_source - length_substring, substring, length_substring) == 0;
}