#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helper_functions.h"

/* Macro Declarations */
#define MACRO_DEFINE "macr"
#define MACRO_END "endmacr"


/* Function declarations */
char* findStartMacro(char* line, size_t* extracted_length, char* macro_name);
int findEndMacro(char* line, char** extracted_string, size_t* extracted_length);
void appendLine(char* line, char** extracted_string, size_t* extracted_length);
void processLine(char* line, int* in_macro, char** extracted_string, size_t* extracted_length, TreeNode* root);
bool isMacroStartLine(WordList* wordList, char *macro_title);
bool isMacroEndLine(WordList* wordList);
void appendLineToNode(WordList* wordList, TreeNode* curNode);
void processLineNew(WordList* wordList, bool *inMacro, TreeNode* root, char* macro_title);
void preAssemble(FILE* inputFile, FILE* outputFile, TreeNode* root);
void strWordListCat(char** word, WordList* wordList);
void processFile(TreeNode* root);
int mainPreAssembler(char* input_file_name);

#endif 
