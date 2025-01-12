#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helper_functions.h"


/*Function to find the start macro delimiter and initialize extraction*/
char* findStartMacro(char* line, size_t* extracted_length, char* macro_name){
	char* start = strstr(line, MACRO_DEFINE);
	int start_macro = strncmp(line, MACRO_DEFINE, 4);

	
	if(start_macro == 0 && start != NULL){
	start += strlen(MACRO_DEFINE);
	char* extracted_string = (char*)malloc(2);
	if (extracted_string == NULL){
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	stpcpy(macro_name, start);
	strcpy(extracted_string, "\n");
	*extracted_length = 1;
	return extracted_string;
	}
	return NULL;
	
}
 
/*Function to find the end macro delimiter and finalize the extraction of the macro code*/
int findEndMacro(char* line, char** extracted_string, size_t* extracted_length){
	char* end = strstr(line, MACRO_END);
	
	if (end != NULL){
		*end = '\0';
		size_t length = strlen(line);
		*extracted_string = (char*)realloc(*extracted_string, *extracted_length + length + 1);
		
		if(*extracted_string == NULL){
			fprintf(stderr, "Memory allocation failed\n");
			return 0;
		}

		strcpy(*extracted_string + *extracted_length, line);
		/*printf("%s",extracted_string[0]);*/
		*extracted_length += length;
		return 1;
	}
	return 0;
}

/*Function to append a new line to the current macro buffer*/
void appendLine(char* line, char** extracted_string, size_t* extracted_length){
	
	size_t length = strlen(line);
	*extracted_string = (char*) realloc(*extracted_string, *extracted_length + length + 1);
	if (*extracted_string == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return;
	}
	strcpy(*extracted_string + *extracted_length, line);
	*extracted_length += length;
}


/*Boolena function to check if this line contains a macro definition*/
bool isMacroStartLine(WordList* wordList, char * macro_title){
	int i = 0;
	for (i = 0; i < wordList->size; i ++){
		if (strcmp(wordList->words[i], "macr") == 0){
			strcpy(macro_title, wordList->words[i+1]);
			printf("%s", macro_title);
			return TRUE;
		}
	}
	return FALSE;
}
/*Boolean function to check if we got an end of macro definition*/
bool isMacroEndLine(WordList* wordList){
	int i = 0;
	for(i = 0 ; i < wordList->size; i ++){
		if (strcmp(wordList->words[i], "endmacr") == 0){
			return TRUE;
		}
	}
	return FALSE;
}

/*This function adds a line of code to a macro in the binary tree*/
void appendLineToNode(WordList *wordList, TreeNode *curNode){
	int i = 0;
	for(i = 0; i < wordList->size; i ++){
		strcat(curNode->macro_code, wordList->words[i] );
		strcat(curNode->macro_code," ");

	}
	
}

/**/
void preAssemble(FILE* inputFile, FILE* outputFile, TreeNode* root){
	char line[LINE_SIZE];
	char lineCopy[LINE_SIZE];
	
	while(fgets(line, sizeof(line), inputFile)){
		stpcpy(lineCopy, line);
		
		/*If we are in a definition line (start or end) we just keep going*/
		if (strncmp(lineCopy, MACRO_DEFINE, 4) == 0 || strncmp(lineCopy, MACRO_END,7) == 0){
			continue;			
		}
		TreeNode* macro = searchNode(root, lineCopy);
		
		/*If the line is a macro already defined, we can remplace it by its code else we write the line in the output file*/
		if (macro != NULL && macro->macro_code != NULL){
			fputs(macro->macro_code, outputFile);
		}
		else{
			
			fputs(line, outputFile);
		}
		freeTree(macro);
	}
	
}

/*This function concatenate the words of a wordList into a given string (by address)*/
void strWordListCat(char ** word, WordList* wordList){
	int i =0;

	for(; i < wordList->size; i ++){

		strcat(*word, " ");
		strcat(*word, wordList->words[i]);
	}

}

/*This function processes the whole file with the use of helper functions*/
void processFile(TreeNode* root, FILE* inputFile, FILE* outputFile){
	
	TreeNode* cur_node = (TreeNode*) malloc(sizeof(TreeNode));
	initTreeNode(cur_node);
	WordList wordList;
	initWordList(&wordList);
	char * code_buffer = (char *) malloc (LINE_SIZE * sizeof(char));
	int buffer_size = LINE_SIZE;
	char * title_buffer= (char *) malloc (LINE_SIZE * sizeof(char));
	bool inMacro = FALSE;
	bool isMacroStart, isMacroEnd;
	char line [LINE_SIZE];
	int lineLen = 0;
	int i = 0;
	while(fgets(line, sizeof(line), inputFile)) {
		lineLen = strlen(line);
		extractWords(line, &wordList, TRUE); /*We extract the current line wordsand initiate two flag for the current line*/
		isMacroStart = compareWords(wordList.words[0], MACRO_DEFINE);
		isMacroEnd = compareWords(wordList.words[0], MACRO_END);
		
		if(inMacro){/*If we are in a macro, we continue to add more memory to the buffer*/
			while(buffer_size < strlen(code_buffer) + lineLen + 1){
				buffer_size *= 2;
				code_buffer = (char*)realloc(code_buffer, buffer_size * sizeof(char));
				if (code_buffer == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                fclose(inputFile);
				fclose(outputFile);
				}
			}
			/*if we are not at the end of the macro, we add code to the buffer*/
			if(!isMacroEnd){
			strWordListCat(&code_buffer, &wordList);
			}
		}
		
		/*If it is the start of a macro we copy only the title of the macro into the title_buffer*/
		if(isMacroStart){
			inMacro = TRUE;
			strcpy(title_buffer, wordList.words[1]);
		}
		/*If it is the end of a macro' we can add the macro to our binary tree and reinitiate our buffer*/
		if(isMacroEnd){
			root = insertNode(root, title_buffer, code_buffer);
			inMacro = FALSE;
			strcpy(title_buffer, "");
			strcpy(code_buffer, "");
		}

		/*If we are currently in a macro, we can already write it into the output file*/
		cur_node = searchNode(root, wordList.words[0]);
		if(cur_node != NULL){
			fputs(cur_node->macro_code, outputFile);
		}
		else if(!(isMacroStart || isMacroEnd)){
			fputs(line, outputFile);
		}

		
		for ( i = 0; i < wordList.size; i ++){
		printf(" %s",wordList.words[i]);
	}
	initWordList(&wordList);	
	}
	
	free(code_buffer);
	freeTree(cur_node);
	freeWordList(&wordList);
}

/*This function helps the assembler to use the preAssembler*/
int mainPreAssembler(char * input_file_name){
	char * asFileName = malloc(strlen(input_file_name) + strlen(".as") + 1);
	strcpy(asFileName, input_file_name);
    strcat(asFileName, ".as");
	FILE *inputFile = fopen(asFileName, "r");
	
	char * amFileName = malloc(strlen(input_file_name) + strlen(".am") + 1);
	strcpy(amFileName, input_file_name);
    strcat(amFileName, ".am");
	FILE* outputFile = fopen(amFileName, "w+");
	

	if (inputFile == NULL){
		perror("Error opening the input file::::::");
		return EXIT_FAILURE;
	}
	else{
	if(outputFile == NULL){
		perror("Error opening the output file"); 
		fclose(inputFile);
		return EXIT_FAILURE;
	}
	
	TreeNode * root = (TreeNode*) malloc(sizeof(TreeNode));
	initTreeNode(root);
	
	
	

	processFile(root, inputFile, outputFile);

	freeTree(root);
	fclose(inputFile);
	fclose(outputFile);
	return EXIT_SUCCESS;
	}
}