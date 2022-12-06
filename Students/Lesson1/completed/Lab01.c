/*
Lab01: Extract word, its position and number of that word (DONE)
Lab02: Add checking word meaning
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

typedef struct {
	int line;
	char word[20];
	int collumn;
} string;

typedef struct {
	int lines[10];
	char word[20];
	int collumns[10];
	int type;
} mapping;


string* getLine(FILE* f);
char** acceptedWords(char** skipped_words, FILE* f);
char** skippedWords(FILE* f);
int checkWordType(char* str);


int main() {
	mapping word_list[100];
	for (int i = 0; i < 100; i++) {
		strcpy(word_list[i].word, "");
		word_list[i].type = 9;
		for (int j = 0; j < 10; j++) {
			word_list[i].lines[j] = 0;
			word_list[i].collumns[j] = 0;
		}
	}

	int count = 0;
	FILE *f_txt = fopen("./vanban.txt", "r+");
	FILE *f_skip = fopen("./stopw.txt", "r+");
	char** skipped_words = skippedWords(f_skip); 
	char** accepted_words = acceptedWords(skipped_words, f_txt);
	fclose(f_txt);
	fclose(f_skip);
	f_txt = fopen("./vanban.txt", "r+");	
	string* line_info = getLine(f_txt);

	for (int i = 0; accepted_words[i] != NULL; i++) {
		int index = 0;
		strcpy(word_list[count].word, accepted_words[i]);
		for (int j = 0; j < 200; j++) {
			if (strcmp(line_info[j].word, accepted_words[i]) == 0) {
				word_list[count].lines[index] = line_info[j].line;
				word_list[count].collumns[index] = line_info[j].collumn;
				index++;
			}
		}
		count++;
	}

	for (int i = 0; i < count; i++) {
		int type = checkWordType(word_list[i].word);
		printf("%d, %s: \t\t", i, word_list[i].word);//print word
		int index = 0;
		while(word_list[i].lines[index]) {
			index++; //Number of time appeare
		}
		printf("%d %d ", type, index);
		for (int j = 0; j < index; j++) {
			printf("(%d, %d) ", word_list[i].collumns[j], word_list[i].lines[j]);//Coordination
		}
		printf("\n");
	}
}

/*****************CHECK WORD TYPE**********************/
int checkWordType(char* str){
	//0: normal
	//1: identical, city, ...(capital first letter)
	//3: real number
	int wordType = 0;
	//1. if its is interger
	int isInt = 1;
	for(int i = 0; i < strlen(str); i++){
		if(!isdigit(str[i])){
			isInt = 0;
			break;
		}
	}
	if(isInt == 1){
		wordType = 2;//2: integer
		return wordType;
	}

	//2. if its is real number
	int isRealNum = 1;
	for(int i = 0; i < strlen(str); i++){
		if(isalpha(str[i])){
			isRealNum = 0;
			break;
		}
	}
	if(isRealNum == 1){
		wordType = 3;//3: Real number
		return wordType;
	}

	//3. if its is Identical, name, city,....
	int isSpecial = 0;
	if((int)str[0] >= 65 && (int)str[0] <= 90){
		isSpecial = 1;
	}
	if(isSpecial){
		wordType = 1;//1: Special character
		return wordType;
	}
	return wordType; //Default is 0
}
/*****************END CHECK WORD TYPE**********************/


char** skippedWords(FILE* f) {
	char** result = malloc(20 * sizeof(char*));
	for(int i = 0; i < 20; i++) {
		result[i] = malloc(10 * sizeof(char));
	}

	char temp[10] = "";
	int count = 0;
	while(!feof(f)) {
		fscanf(f, "%s\n", temp);
		strcpy(result[count], temp);
		count++;
	}

	result[count] = NULL;
	return result;
}



static int compare(const void *arg1, const void *arg2) {
    return strcmp( *(char * const *) arg1, *(char * const *) arg2);
}

char** wordListWithoutBadWords(char** list, char** bad_words) {
	char** result = malloc(100 * sizeof(char*));
	for(int i = 0; i < 100; i++) {
		result[i] = malloc(50 * sizeof(char));
	}
	int count = 0;
	for (int i = 0; list[i] != NULL; i++) {
		int check = 1;
		for (int j = 0; bad_words[j] != NULL; j++) {
			if (strcmp(bad_words[j], list[i]) == 0) {
				check = 0;
				break;	
			}
		}
		if (check) {
			strcpy(result[count], list[i]);
			count++;
		}
	}
	result[count] = NULL;
	// free(result);
	return result;
}

char** wordListNoDuplication(char** list) {
	char** result = malloc(100 * sizeof(char*));
	for(int i = 0; i < 100; i++) {
		result[i] = malloc(50 * sizeof(char));
	}
	strcpy(result[0], list[0]);
	int count = 1;
	for (int i = 1; list[i] != NULL; i++) {
		if (strcmp(list[i], list[i - 1]) == 0)
			continue;
		strcpy(result[count], list[i]);
		count++;
	}
	result[count] = NULL;
	
	return result;
}


char** acceptedWords(char** skipped_words, FILE* f) {
	char** raw_word_ls = malloc(200 * sizeof(char*));
	for(int i = 0; i < 200; i++) {
		raw_word_ls[i] = malloc(50 * sizeof(char));
	}

	char temp[50] = "";
	char prev_word[50];
	int count = 1;
	fscanf(f, "%s ", prev_word);
	strcpy(raw_word_ls[0], prev_word);

	while(!feof(f)) {
		fscanf(f, "%s ", temp);
		if ((int)temp[0] >= 65 && (int)temp[0] <= 90 && prev_word[strlen(prev_word) - 1] != '.') { 
			strcpy(prev_word, temp);
			continue;
		}
		
		if (temp[strlen(temp) - 1] == '.' || temp[strlen(temp) - 1] == ',' || temp[strlen(temp) - 1] == ')' || temp[strlen(temp) - 1] == ' ') {
			temp[strlen(temp) - 1] = '\0';
		}

		if (temp[0] == '(' || temp[0] == ' ') {
			for(int j = 0; j < strlen(temp) - 1; j++) {
				temp[j] = temp[j + 1];
			}
			temp[strlen(temp) - 1] = '\0';
		}

		strcpy(raw_word_ls[count], temp);
		count++;
		strcpy(prev_word, temp);
	}
	raw_word_ls[count] = NULL;
	qsort(raw_word_ls, count, sizeof(char*), compare);
	
	char** no_duplicated_word_list = wordListNoDuplication(raw_word_ls);

	char** word_list = wordListWithoutBadWords(no_duplicated_word_list, skipped_words);

	return word_list;
}



string* getLine(FILE* f) {
	string* result = malloc(200 * sizeof(string));

	char line[1000];
	int count = 0;
	int lineNum = 1;
	while(fgets(line, sizeof(line), f)) {
    	char newString[20][20]; 
    	int i, j, ctr;
 
	    j = 0; ctr = 0;
    	for(i = 0; i <= (strlen(line)); i++) {
        	if (line[i] == ' ' || line[i] == '\0')
        		{
            		newString[ctr][j] = '\0';
            		ctr++;  //for next word
            		j = 0;    //for next word, init index to 0
				} else {
            		newString[ctr][j] = line[i];
            		j++;
        		}
    	}

		for(i = 0; i < ctr; i++) {
			result[count].line = lineNum;
			strcpy(result[count].word, newString[i]);
			result[count].collumn = i + 1;
			count++;
		}
		lineNum++;
	}

	for (int i = 0; i < count; i++) {
		
		if (result[i].word[0] == '(' || result[i].word[0] == ' ') {
			for(int j = 0; j < strlen(result[i].word) - 1; j++) {
				result[i].word[j] = result[i].word[j + 1];
			}
			result[i].word[strlen(result[i].word) - 1] = '\0';
		}

		if (result[i].word[strlen(result[i].word) - 1] == '.' || result[i].word[strlen(result[i].word) - 1] == ',' || result[i].word[strlen(result[i].word) - 1] == ')' || result[i].word[strlen(result[i].word) - 1] == ' ') {
			result[i].word[strlen(result[i].word) - 1] = '\0';
		}

		for (int k = 0; k < strlen(result[i].word); k++) {
			if ((result[i].word[k] == '.' || result[i].word[k] == ',') && (result[i].word[k + 1] < 48 || result[i].word[k + 1] > 57))
				result[i].word[k] = '\0';
		}
	}
	return result;
}


