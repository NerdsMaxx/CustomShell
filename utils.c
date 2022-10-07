#include "utils.h"

//Função responsável por remover os espaços excessivos da string.
char* dynalloc_remove_excessives_spaces(const char *source){
	//Inicializa index da fonte com 0.
	int index_source = 0;
	//Pegue o tamanho da fonte.
	int length_source = strlen(source);

	//Aloca dinamicamente o resultado para ter o mesmo tamanho da fonte.
	//Pois a fonte pode não ter nenhum espaço excesso, então 
	//é por isso o resultado deve ter no máximo o tamanho da fonte.
	char *dynalloc_result = calloc(length_source + 1, sizeof(char));
	
	//Inicialmente inclua uma string vazia no resultado.
	strcpy(dynalloc_result, "");
	
	//Se a string da fonte estiver vazia, então retorne o
	//resultado.
	if(length_source == 0){
		return dynalloc_result;
	}
	
	//Inicialmente o tamanho do resultado vai ser 0.
	int length_result = 0;

	//Vai avançando até achar um caractere diferente do espaço.
	for(; index_source < length_source && source[index_source] == ' '; ++index_source);
	
	//Inicializa o encontrado como falso.
	bool found = false;

	//Depois que achar a posição de um caractere que não seja espaço.
	//Comece uma iteração até fim da fonte.
	for(; index_source < length_source; ++index_source){
		
		//Se uma posição tiver espaço. Dẽ o valor
		//verdadeiro para encontrado.
		if(source[index_source] == ' '){
			//Se ja tiver encontrado um caractere de espaço antes,
			//pule para próxima iteração.
			if(found){
				continue;
			}

			//Seta para verdadeiro
			found = true;
		}
		else{
			//Se não for espaço, seta encontrado para falso de novo.
			found = false;
		}

		//Adicione uma caractere da fonte ao resultado, incluindo
		//um caractere de espaço, mas somente uma entre as palavras.
		strncat(dynalloc_result, source + index_source, 1);

		//Aumente o tamanho do resultado.
		++length_result;
	}

	//Se tamanho do resultado for maior que 0,
	//verifica se no final do resultado tem uma caractere
	//espaço. Se sim, seta com caractere nulo.
	if(length_result > 0){
		if (dynalloc_result[length_result - 1] == ' '){
			dynalloc_result[length_result - 1] = '\0';
			--length_result;
		}
	}

	//Devolve o resultado.
	return dynalloc_result;
}

//Função que vai retornar número de palavras da fonte.
int get_number_of_words(const char *source){
	//Inicializando as variáveis
    int index = 0;
	int length = strlen(source);
	int number_of_words = 0;

	//Enquanto a posição da fonte for menor que tamanho.
    while(index < length){
		//Vai até encontrar um caractere diferente do espaço,
		//que possivelmente seja uma palavra.
        for(; index < length && source[index] == ' '; ++index);
		
		//Se por acaso já chegou no limite, então seta 0.
		//Se não, então achou uma palavra e soma com 1
		//número de palavras.
        number_of_words += (index < length) ? 1 : 0;
		
		//Vai até encontrar um caractere de espaço.
	    for(; index < length && source[index] != ' '; ++index);
    }

	//Retorna número de palavras.
    return number_of_words;
}


//Função responsável por remover substring da string.
char* dynalloc_remove_substring(const char *source, const char *substring){
	//Inicializa o tamanho da fonte e substring.
	int length_source = strlen(source);
	int length_substring = strlen(substring);
	

	//Aloca dinamicamente o resultado para ter o mesmo tamanho da fonte.
	//Pois a substring pode ser a mesma string que a fonte,
	//então é por isso o resultado deve ter no máximo o tamanho da
	//fonte.
	char *dynalloc_result = calloc(length_source + 1, sizeof(char));
	
	//Inicialmente inclua uma string vazia no resultado.
	strcpy(dynalloc_result, "");

	//Se a string da fonte estiver vazia, então retorne o
	//resultado.
	if(length_source == 0){
		return dynalloc_result;
	}

	//Se o tamanho da substring for maior que tamanho
	//da fonte, então copie a própria fonte para resultado
	//e retorne este.
	if(length_substring > length_source){
		strcpy(dynalloc_result, source);
		return dynalloc_result;
	}

	//Inicializa a posição do começo da substring o valor -1.
	int index_start_substring = -1;

	//Inicializa a posição limite de até onde pode verificar a fonte.
	//Se fonte tem tamanho tf e a substring tem tamanho ts, desde que 
	//seja ts <= tf, então deve verificar até a posição tf - ts + 1.
	int limit = (length_source - length_substring) + 1;

	for(int index_source = 0; index_source < length_source; ++index_source){
		//Se a posição for menor que limite e parte da fonte corresponder a substring, então
		//seta a posição do começo da string e pule posição da fonte para 
		//pcs (posição do começo da substring) + ts - 1, para justamente pular
		//esta substring para que seja removido. 
		if(index_source < limit && strncmp(source + index_source, substring, length_substring) == 0){
			index_start_substring = index_source;
			index_source = index_start_substring + length_substring - 1;
		}
		else{
			//Senão for a substring, então copie um caractere da fonte para resultado.
			strncat(dynalloc_result, source + index_source, 1);
		}
	}

	//Retorne o resultado.
	return dynalloc_result;
}

//Função responsável por copiar a fonte para destino e desalocar dinamicamente
//a fonte, isso se a fonte não for nulo.
void dynalloc_strcpy_and_free_alloc(char *dynalloc_source, char *destiny){
	if(dynalloc_source != NULL){
		strcpy(destiny, dynalloc_source);
		dynalloc_delete(dynalloc_source);
	}
}

//Função responsável por desalocar dinamicamente a fonte e torna-lo nulo.
void dynalloc_delete(char* source){
	if(source != NULL){
		free(source);
		source = NULL;
	}
}

//Função responsável por verificar a última substring da fonte, ou melhor,
//verifica se prefixo da fonte corresponde com a substring passada.
//Se sim, retorne verdadeiro. Senão retorne falso.
bool is_last_substring(const char *source, const char *substring){
	int length_source = strlen(source);
	int length_substring = strlen(substring);

	//Se tamanho da substring for maior que a fonte, retorne falso.
	if(length_substring > length_source){
		return false;
	}

	return strncmp(source + length_source - length_substring, substring, length_substring) == 0;
}