#include "utils.h"

char* dynalloc_remove_excessives_spaces(const char *source){
	int index_source = 0;
	int length_source = strlen(source);
	
	char *dynalloc_result = calloc(length_source + 1, sizeof(char));
	int length_result = 0;
	strcpy(dynalloc_result, "");
	
	while(index_source < length_source){
		for(; index_source < length_source && source[index_source] == ' '; ++index_source);
		int index_begin_word = index_source;

		for(; index_source < length_source && source[index_source] != ' '; ++index_source, ++length_result);

		if(index_begin_word >= length_source){
			continue;
		}

		if(length_result <= length_source){
			strncat(dynalloc_result, source + index_begin_word, index_source - index_begin_word);

			if(length_result < length_source){
				strcat(dynalloc_result, " ");
				++length_result;
			}
		}
		else{
			length_result = length_result - (index_source - index_begin_word);

			if(length_result < length_source){
				strncat(dynalloc_result, source + index_begin_word, length_source - length_result);
				length_result = length_source;
			}
		}
	}

	if(dynalloc_result[length_result - 1] == ' '){
		dynalloc_result[length_result - 1] = '\0';
		--length_result;
	}

	return dynalloc_result;
}

// char* dynalloc_remove_excessives_spaces(char *source){
// 	char *strings[SMALL_SIZE]; 

// 	for (int index = 0; index < SMALL_SIZE; strings[index++] = NULL);

// 	int index_source = 0;
// 	int index_strings = 0;
// 	char *dynalloc_result = calloc(SIZE, sizeof(char));
// 	int length_result = 0;
// 	strcpy(dynalloc_result, "");
	
// 	for(; index_strings < SMALL_SIZE; ++index_strings){
// 		for(; source[index_source] == ' '; ++index_source);
// 		strings[index_strings] = source + index_source;

// 		for(; source[index_source] != '\0' && source[index_source] != ' '; ++index_source);
		
// 		int aux_length = length_result + (source + index_source) - strings[index_strings];
// 		if(aux_length < SIZE){
// 			strncat(dynalloc_result, strings[index_strings], (source + index_source) - strings[index_strings]);
// 			length_result = aux_length;
// 		}
// 		else{
// 			if(length_result < SIZE){
// 				strncat(dynalloc_result, strings[index_strings], (SIZE) - length_result);
// 				length_result = SIZE;
// 			}
// 		}

// 		if(source[index_source] == '\0' || length_result == SIZE){
// 			if(dynalloc_result[length_result - 1] == ' '){
// 				dynalloc_result[ length_result - 1] = '\0';
// 				--length_result;
// 			}
            
//             return dynalloc_result;
// 		}
// 		else{
// 			if(length_result < SIZE){
// 				strcat(dynalloc_result, " ");
// 				++length_result;
// 			}
// 		}
// 	}

// 	return dynalloc_result;
// }

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

	if(length_substring > length_source){
		return NULL;
	}

	char *dynalloc_result = calloc(length_source + 1, sizeof(char));
	strcpy(dynalloc_result, "");

	int index_start_substring = -1;
	int limit = (length_source - length_substring) + 1;
	for(int index = 0; index < limit; ++index){
		if(strncmp(source + index, substring, length_substring) == 0){
			index_start_substring = index;
			break;
		}
	}

	if(index_start_substring == -1){
		return NULL;
	}

	if(index_start_substring != 0){
		strncpy(dynalloc_result, source, index_start_substring);
	}
	
	int size = length_source - length_substring - index_start_substring;
	if(size != 0){
		strncat(dynalloc_result, 
			source + index_start_substring + length_substring, 
			size
		);
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
		free(dynalloc_source);
		dynalloc_source = NULL;
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