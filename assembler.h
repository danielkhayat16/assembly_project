#ifndef ASSEMBLER_H
#define ASSEMBLER_H


#include "constant.h"
#include "pre_assembler.h"
#include <stdio.h>

typedef enum {
	MOV = 0,
	CMP,
	ADD,
	SUB,
	LEA,
	CLR,
	NOT,
	INC,
	DEC,
	JMP,
	BNE,
	RED,
	PRN,
	JSR,
	RTS,
	STOP,
	UNKNOWN
	
} OperationType;

typedef enum {
	REG = 0,
	PTR_REG,
	IMMEDIATE_VAL,
	EXTERN,
	LABEL,
	UNKNOWN_OPERAND
	
} OperandType;

typedef struct {
	OperandType type;
	char* name;
	
} OperandMap;

 

typedef struct {
	const char *name;
	OperationType type;
} OperationMap;

const OperationMap operations[] = {
	{"mov", MOV},
	{"cmp", CMP},
	{"add", ADD},
	{"sub", SUB},
	{"lea", LEA},
	{"clr", CLR},
	{"not", NOT},
	{"inc", INC},
	{"dec", DEC},
	{"jmp", JMP},
	{"bne", BNE},
	{"red", RED},
	{"prn", PRN},
	{"jsr", JSR},
	{"rts", RTS},
	{"stop", STOP},

};
int assemble(const char* input_file_name);
void firstAssembler( FILE* inputFile, LabelTable *labels, LabelTable *externs, LabelTable *entries);
void secondAssembler(FILE* inputFile,const char * input_file_name, LabelTable *labels, LabelTable *externs, LabelTable *entries);
void printLine(const WordList *wordList);
bool searchSymbol(WordList *wordList, char * symbol);
bool searchStrSymbol(WordList* wordList);
bool searchDataSymbol(WordList* wordList);
bool searchEntrySymbol(WordList *wordList);
void secCheckLabel(WordList *wordList, LabelTable *labels,LabelTable *externs, Queue *queueLabel, Queue *queueExterns);
void completeAddress( Queue* queueLabel);
bool searchExternSymbol(WordList* wordList);
bool isCommentLine(WordList* wordList);
bool isLabelDeclaration(WordList* wordList);
int checkOperation(const char *word);
int checkLine(WordList *wordList, LabelTable * labels, LabelTable * externs, LabelTable * entries);
int checkLabel(WordList* wordList, LabelTable * labelTable);
int checkDataStr(WordList* wordList);
int addressingMethod(WordList *wordList);
OperationType getOperationType(WordList* wordList) ;
void appendDataToMemory(WordList * wordList);
void appendStringToMemory(WordList * wordList);
OperandMap checkOperandType(WordList *wordList, int index);
int searchImmediateVal(WordList * wordList);

bool hasPointerToReg(WordList *wordList);
bool hasReg(WordList * wordList);

int LC = 100, DC = MEMORY_SIZE - 100, IC = 1;
SystemState state; 

void writeObFile(const char* input_file_name);
void writeEntryFile(const char* input_file_name, LabelTable *entries);
void writeExternFile(const char* input_file_name, StrQueue* names);

#endif 