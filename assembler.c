/*The main file of the assembler contains the first and second loop we perform on the input files*/

#include "helper_functions.h"
#include "memory.h"
#include "assembler.h"
#include <stdio.h>  
#include "pre_assembler.h"
#include <stdio.h>      

SystemState state;

/*The main function read the argument we pass to the program when we launch it*/
int main(int argc, char *argv[]){
	
	int i = 1;
	for(; i < argc; i ++){
  	mainPreAssembler(argv[i]);
 
	assemble(argv[i]);
	}
	return 0;   
}

/*The assemble function perform once on a given input file the whole process of the assembler*/
int assemble(const char* input_file_name){
	initializeSystemState(&state); 
	
	/*We create a string for the output file with the .am extension*/
	char * asFileName = malloc(strlen(input_file_name) + strlen(".am") + 1);
	strcpy(asFileName, input_file_name);
    strcat(asFileName, ".am");
	
	
	FILE *inputFile = fopen(asFileName, "r");
	if (inputFile == NULL){
		perror("Error opening the input file?????");
		return EXIT_FAILURE;
	}
	LabelTable labels;
	LabelTable externs;
	LabelTable entries;
	initLabelTable(&labels);
	initLabelTable(&externs);
	initLabelTable(&entries);

	/*Here we perform the first assembler loop, rewind the file and the perform the second loop*/
	firstAssembler( inputFile, &labels, &externs, &entries);
	printSystemState(&state);
	rewind(inputFile);
	secondAssembler( inputFile,input_file_name, &labels, &externs, &entries);
	printSystemState(&state);
	bool entriesIsEmpty = entries.size == 0;
	if(!entriesIsEmpty){ 
	writeEntryFile(input_file_name, &entries);

	}
	writeObFile(input_file_name);
	free(asFileName);
	freeLabelTable(&labels);
	freeLabelTable(&externs);
	freeLabelTable(&entries); 
	fclose(inputFile);
	return 1;
	
}

/*Function to write the extern file*/
void writeExternFile(const char* input_file_name, StrQueue* names){
	
	char * extFileName = malloc(strlen(input_file_name) + strlen(".ext") + 1);
	strcpy(extFileName, input_file_name);
    strcat(extFileName, ".ext");
	
	FILE* filePtr = fopen(extFileName, "w+");
    if (filePtr == NULL) {
        perror("Error opening file");
        return ;
    }	
	
	int i = 100; 
	char c ;
	int len = 0;
	Queue* queueExternsLines = createQueue();
	for(; i < LC ; i ++){
		 
		len = strlen(toBinary(getCellFromMemory(i, &state.memory).value.charCell.val));
		c = toBinary(getCellFromMemory(i, &state.memory).value.charCell.val)[len - 1];
		if (c == '1'){
			enqueue(queueExternsLines, i);
		}
		
	}
	i = 0;

	while(!isEmpty(queueExternsLines) && !isStrQueueEmpty(names)){
		fprintf(filePtr, "%s %d\n", dequeueStr(names), dequeue(queueExternsLines));
		i ++;
	}
	
	
	fclose(filePtr);
	free(extFileName);
	return;
}

/*Function to write the .ent file*/
void writeEntryFile(const char* input_file_name, LabelTable *entries){
	char * entFileName = malloc(strlen(input_file_name) + strlen(".ent") + 1);
	strcpy(entFileName, input_file_name);
    strcat(entFileName, ".ent");
	
	FILE* filePtr = fopen(entFileName, "w+");
	int i = 0;
	if (filePtr == NULL) {
        perror("Error opening file");
        return ;
    }	
	for(; i < entries->size; i ++){
		fprintf(filePtr, "%s %d\n", entries->label_name[i], entries->label_address[i]);
	}
	fclose(filePtr);
	free(entFileName);
}

/*Function to write the .ob file*/
void writeObFile(const char* input_file_name){
	int i = 100;
	char * obFileName = malloc(strlen(input_file_name) + strlen(".ob") + 1);
	strcpy(obFileName, input_file_name);
    strcat(obFileName, ".ob");
	
	FILE* filePtr = fopen(obFileName, "w+");
	char str[7];
	
	uIntegerCell result;
	fputs("\t\t", filePtr);
	sprintf(str ,"%d", LC - 100 -(MEMORY_SIZE - 100 - DC) ); /*We write the number of instruction opeartion*/
    fputs( str, filePtr);
	fputs("\t", filePtr);
	sprintf(str , "%d", MEMORY_SIZE - 100 - DC ); /*We write the number of data instructions*/
    fputs( str , filePtr);
	fputs("\n", filePtr);
	for(; i < LC; i ++){
		
		toOctal(i, str) ;
		fputs( str, filePtr);
		fputs("\t", filePtr);
		if (!isCellEmpty(state.memory.cells[i])) {
             
            switch (state.memory.cells[i].type) {
                case UINT_CELL:
					result.val = state.memory.cells[i].value.uIntCell.val;
				    if (result.val < 0) {
					result.val = (unsigned int)(~(-result.val) + 1);
					} else {
					result.val = (unsigned int)result.val;
					}

                    break;
                case INT_CELL:
					result.val = state.memory.cells[i].value.intCell.val;
				    if (result.val < 0) {
					result.val = (unsigned int)(~(-result.val) + 1);
					} else {
					result.val = (unsigned int)result.val;
					}

                    break;
                case CHAR_CELL:
					result.val = state.memory.cells[i].value.charCell.val;
				    if (result.val < 0) {
					result.val = (unsigned int)(~(-result.val) + 1); 
					} else {
					result.val = (unsigned int)result.val;
					}

                    break;
            } 
			
			fprintf(filePtr, "%05o", result.val);
			fputs("\n", filePtr);
        }
		else{
			fputs("00000\n", filePtr);
		} 
	 }
	fclose(filePtr);
	free(obFileName);
	
} 

/*The first assembler function perfoms the first loop*/
void firstAssembler(FILE* inputFile, LabelTable *labels, LabelTable *externs, LabelTable *entries){
	

	char line [LINE_SIZE];
	WordList wordList;
	initWordList(&wordList);
		
	
	while(fgets(line, sizeof(line), inputFile)) {

		extractWords(line, &wordList, FALSE);
		/*The checkLine function make the most important work about the first loop of the assembler*/
		checkLine(&wordList, labels, externs, entries);

		
		IC ++; 
	
		assignValueToCell(&state.PC, state.PC.value.uIntCell.val + 1);
		
		initWordList(&wordList);
		
	}

	freeWordList(&wordList);

	
	
}

/*The second assembler function complete the addresses of the labels that occures in the input file*/
void secondAssembler(FILE* inputFile,const char *input_file_name, LabelTable *labels, LabelTable *externs, LabelTable *entries){

	int j = 100;
	int i = 1;
	
	char line [LINE_SIZE];
	bool strSymbol, dataSymbol, externSymbol, entrySymbol ;
	WordList wordList; 
	initWordList(&wordList);
	/*I choosed to use queues to take care of the addresses that need to be completed*/
	Queue* queueLabel = createQueue();
	Queue* queueExterns = createQueue();
	StrQueue * externsNames = createStrQueue();
	while(fgets(line, sizeof(line), inputFile)) {
		 
		initWordList(&wordList);
		extractWords(line, &wordList, FALSE);

		if(isCommentLine(&wordList)){
			
			continue; 
		}
		
		/*These flag allows the secondAssembler to understand the type of operation*/
		entrySymbol = searchEntrySymbol(&wordList);
		strSymbol = searchStrSymbol(&wordList);
		dataSymbol = searchDataSymbol(&wordList);
		externSymbol = searchExternSymbol(&wordList);
		
		if(strSymbol || dataSymbol || externSymbol){
			continue;
		}
		i = 0;
		
		/*If we meet an extern, we add it to the extern queue*/
		while (i < wordList.size){
				int externsIndex = hasLabel(externs, wordList.words[i]);
				
				if (externsIndex != -1){
					enqueueStr(externsNames,  wordList.words[i]);
		
				}
				i ++;
		}
		
		/*Here we take care of entry line*/
		if(entrySymbol){
			i = 1;
			while (i < wordList.size){
				int labelIndex = hasLabel(entries, wordList.words[i]);
				if(labelIndex != -1){
					
					printf("Error: the entry symbol is already defined!??");
					break;
				}
				labelIndex = hasLabel(externs, wordList.words[i]);
				if(labelIndex != -1 ){
					addLabel(entries, wordList.words[i], externs->label_address[labelIndex]);
				}
				labelIndex = hasLabel(labels, wordList.words[i]);
				if(labelIndex != -1 ){
					addLabel(entries, wordList.words[i], labels->label_address[labelIndex]);
				}
				
			i++;
			
			}
			
		}
		secCheckLabel(&wordList, labels, externs, queueLabel, queueExterns);
		
		j ++;
		initWordList(&wordList);
		
	}

	completeAddress(queueLabel);
	if(externs->size != 0){
	writeExternFile(input_file_name, externsNames);
	}
	printLabelTable(labels);
	printLabelTable(entries);
	printLabelTable(externs);
	freeWordList(&wordList);
	freeQueue(queueExterns);
	freeQueue(queueLabel);
 
}

/*Here we complete addresses of the label we stored in the label table*/
void completeAddress( Queue* queueLabel){
	int i = 100; 
	
	
	for(; i < LC; i++){
		if(getCellFromMemory(i, &state.memory).value.charCell.val == '?'){
			putIntToMemory(dequeue(queueLabel), i, &state.memory);
			
		}
		
	} 
	
}
/*This function maintain the queue updated during the second loop of the assembler and adds labels to the queue*/
void secCheckLabel(WordList *wordList, LabelTable *labels,LabelTable *externs, Queue *queueLabel, Queue *queueExterns){
	int i = 0;
	int labelFlag, externsFlag;
	int val ;
	
	for(; i <wordList->size; i++){
		if(searchEntrySymbol(wordList) || searchExternSymbol(wordList)){
			break;
		}
		labelFlag = hasLabel(labels, wordList->words[i]);
		externsFlag = hasLabel(externs, wordList->words[i]);
		if( labelFlag == -1){
			
			continue;
		}
		
		if(externsFlag == -1){
		val = labels->label_address[labelFlag] << 3;
		val = setBit(val, 1);
		}
		else {
			val = 1;
		}
		
		enqueue(queueLabel, val);
		
	}
}

void printLine(const WordList *wordList){
	int j = 0;
	for (j =0; j < wordList->size; j ++){
		
		printf(" %s", wordList->words[j]);
		
	}
	
}

/*Here is a few simple helper functions that help the assembler understand what operation to deal with*/
bool searchStrSymbol(WordList* wordList){
	return searchSymbol(wordList, ".string");
}


bool searchDataSymbol(WordList* wordList){
	return searchSymbol(wordList, ".data");
}

bool searchExternSymbol(WordList *wordList){
	return searchSymbol(wordList, ".extern");
}

bool searchEntrySymbol(WordList *wordList){
	return searchSymbol(wordList, ".entry");
}
 
bool searchSymbol(WordList *wordList, char * symbol){
		int i = 0; 
	bool flag ;
	for (; i < wordList->size; i++){
		flag = compareWords(wordList->words[i], symbol);
		if (flag){
	
		return flag;
		}
	}
	
	return FALSE;
}

bool isCommentLine(WordList* wordList){
	bool flag ;
	
	flag = wordList->words[0][0] == ';';
	if (flag){
	return flag;
	}
	
	return FALSE;
}

bool isLabelDeclaration(WordList* wordList){
	int len;
	int lastChar;
	bool flag ;
	
	len = strlen(wordList->words[0]) ;
	lastChar = len - 1; 
	if (len < 2){
	return FALSE; 
	}
	
	flag = (wordList->words[0][lastChar] == ':') ? TRUE : FALSE;
	
	if (flag){
	wordList->words[0][lastChar] = '\0';
	return flag;
	}
	
	return FALSE;

}

/*Function that search for a label in the whole line*/
int checkLabel(WordList * wordList, LabelTable * labelTable){
	int i =0;
	int flag ;
	for(;i < wordList->size; i ++){
		flag = hasLabel(labelTable, wordList->words[i]);
		if(flag != -1){
		return flag;
		}
	}
	return -1;
}

/*Check if there is a data or string line*/
bool checkDataStr(WordList *wordList){
	bool flag = FALSE;
	flag = searchDataSymbol(wordList) || searchStrSymbol(wordList) ;
	return flag;
	
}

/*Here is a set of function to check what kind of operand we got in the current line*/
int hasPointerToReg(WordList *wordList){
	int i =0;
	const char *registersPointersList[] = REGISTER_POINTER_LIST;
	for(i = 0; i < wordList->size; i ++){
		if(wordIsInList(wordList->words[i], registersPointersList)){
			
			if(i == wordList->size -1){
			
				return 2;
			}
			
			
			return 1;
		}
	}
	return -1;
}
bool wordIsReg(const char* word){
	const char *registersList[] = REGISTER_LIST;
	
	return wordIsInList(word, registersList);
}

bool wordIsPtrReg(const char * word){
	const char *registerPtrList[] = REGISTER_POINTER_LIST;
	return wordIsInList(word, registerPtrList);
}
int hasReg(WordList * wordList){
	int i =0;
	const char *registersList[] = REGISTER_LIST;
	for(i = 0; i < wordList->size; i ++){
		if(wordIsInList(wordList->words[i], registersList)){
			if(i == wordList->size -1){
				return 2;
			}
			
			return 1;
		}
	}
	return -1;
}

bool checkImmediateVal(const char* word){
	
	return word[0] == '#';
} 

/*This function gets the operation type of the current line*/
OperationType getOperationType(WordList *wordList) {
    int num_operations = sizeof(operations) / sizeof(operations[0]);
	int i, j;
	for(j = 0; j < wordList->size; j ++){
		for (i = 0; i < num_operations; i++) {
			
			if (strcmp(wordList->words[j], operations[i].name) == 0) {
				
				return operations[i].type;
			}
		}
	}
	
    return UNKNOWN; 
}


/*Function that loop over the words in the wordList and add data to memory*/
void appendDataToMemory(WordList * wordList){
	int i =0;
	int curData = 0;

	while(i < wordList->size && !compareWords(wordList->words[i], ".data") )
		{
		i++;
		}
		i++;
	
	
	for(; i < wordList->size; i++){
		curData = atoi(wordList->words[i]);
		if(curData != 0 && !isNewLine(wordList->words[i][0])){
			
			putIntToMemory(curData, DC, &state.memory);
			putIntToMemory(curData, LC, &state.memory);
			DC --;
			LC ++;
		}
	
	}
} 
/*Functio that append strig to memory char by char*/
void appendStringToMemory(WordList *wordList){
	int i =0;
	int j = 0;
	do{
		i++;
	}
	while(i < wordList->size && !compareWords(wordList->words[i], ".string") );
	i++;
				
	while(! isNewLine(wordList->words[i][0]) && i < wordList->size){
		removeQuotes(wordList->words[i]);
		int len = strlen(wordList->words[i]);
		while(j < len){
			if(wordList->words[i][j] < 0 || wordList->words[i][j] >= 128){
			printf("Error: Value is not in ascii table\n");
			j++;
			continue;
			}	
			putCharToMemory(wordList->words[i][j], DC, &state.memory);
			putUIntToMemory((int)wordList->words[i][j], LC, &state.memory);
			DC--;
			LC ++;
			j ++; 
		}
		putCharToMemory('\0', DC, &state.memory);
		LC++;
		DC --;
		i++;
	}

}

/*Functinon to get the type of operand in the current operation*/
OperandMap getOperandType(WordList *wordList, int index){
	OperandMap opMap = {UNKNOWN_OPERAND, ""};
	bool checkExtern = FALSE;
	if(index >= wordList->size)
	{
		printf("Index out of bounds");
		return opMap;
	}
	if(wordIsReg(wordList->words[index])) {
		opMap.type = REG ;
	}
	else if(wordIsPtrReg(wordList->words[index])){
		opMap.type = PTR_REG;
	}
	else if(checkImmediateVal(wordList->words[index]) ){
		opMap.type = IMMEDIATE_VAL;
	}
	else if(checkExtern){
	}
	else {
	opMap.type = LABEL; 

	}
	opMap.name = (char*) malloc((strlen(wordList->words[index]) + 1) * sizeof(char));
	strcpy(opMap.name, wordList->words[index]);

	
	
	return opMap;
}

/*Get the immediate value that appears in the line*/
int getImmediateVal(const char word[]){
	const char *ptr = (char *) malloc ((strlen(word)-1) * sizeof(char));
	ptr = word + 1;
	int val = atoi(ptr);
	free(ptr);
	return val;
	
}
/*Function to set bits of the current operation according the operands*/
void addOperandsWords(WordList *wordList, OperandMap operand, bool originOperand){
	
	int regNum ;

	int immediateVal;
	int val = 0;
	int addressingIndex = originOperand ? 6 : 3;
	
	immediateVal = getImmediateVal(operand.name);

	switch(operand.type){
		case IMMEDIATE_VAL:
			val = immediateVal << 3;
			val = setBit(val, 2);
			
			putIntToMemory(val, LC, &state.memory);
			LC++ ;
			
			break;
		case LABEL:
			val = setBit(val, 1);
		
			putCharToMemory('?', LC, &state.memory);
			LC++;
			
			break;
		case EXTERN:
			val = setBit(val, 0);

			LC ++;
			
			break;
		case REG:
			
			
			regNum = operand.name[1] - '0'; 
			val = regNum << addressingIndex;
			val = setBit(val, 2);
			putIntToMemory(val, LC, &state.memory);
			LC ++;
			
			break;
		case PTR_REG:
			
			
			regNum = operand.name[2] - '0'; 
			
			val = regNum << addressingIndex;
			val = setBit(val, 2);
			putIntToMemory(val, LC, &state.memory);
			LC++;
			
			break;
		case UNKNOWN_OPERAND:
			
			printf("Unknown operand");

			break;
			
	}
	
}


/*Function to check the addrssing type of operation*/
int checkAddressing(WordList *wordList, OperandMap operand, int * lineVal, bool originOperand){
	
	int addressingVal ;

	int addressingIndex ;

	addressingIndex = originOperand ? 7 : 3;

	switch(operand.type){
		case IMMEDIATE_VAL:
			*lineVal = setBit(*lineVal, addressingIndex);
			addressingVal = 0;
			break;
		case LABEL:
			*lineVal = setBit(*lineVal, addressingIndex + 1);
			addressingVal = 1;
			break;
		case EXTERN:
			*lineVal = setBit(*lineVal, addressingIndex + 1);
			addressingVal = 1;
			break;
		case REG:
			*lineVal = setBit(*lineVal, addressingIndex + 3);
			addressingVal = 3;
			break;
		case PTR_REG:
			*lineVal = setBit(*lineVal, addressingIndex + 2);
			addressingVal = 2;
			break;
		case UNKNOWN_OPERAND:
			printf("Unknown operand");
			addressingVal = -1;
			break;
			
	}
	
	return addressingVal;
}
/*Function to get the index of the word of the operation in the line*/
int getIndexOfOperation(WordList *wordList, OperationType curOp){
	int i;
	for (i = 0; i < wordList->size ; i ++){
		
			if(curOp <= 15 && compareWords(wordList->words[i], operations[curOp].name)){
			return i+1;
		
		}

	}
	return -1;
}
/*Function that takes care of the operands in the current line and perform all the operation needed to translate to operation into binary*/
void checkOperands(WordList* wordList, OperationType curOp, int lineVal){
	

	int numOfOperand ;
	OperandMap operand, operand2;
	
	int index = getIndexOfOperation(wordList, curOp);
	numOfOperand = wordList->size - index - 1;
	switch(numOfOperand){
			case 0:
				putUIntToMemory(lineVal, LC, &state.memory);
				LC ++;
				return ;
			case 1:
				
				operand = getOperandType(wordList, index);
				checkAddressing(wordList, operand, &lineVal, FALSE);
				putUIntToMemory(lineVal, LC, &state.memory);
				LC ++;
				addOperandsWords(wordList, operand, FALSE);
				break;
			case 2:
				operand = getOperandType(wordList, index);
				operand2 = getOperandType(wordList, index+1);
				
				checkAddressing(wordList, operand, &lineVal, TRUE);
				checkAddressing(wordList, operand2, &lineVal, FALSE);
				
				putUIntToMemory(lineVal, LC, &state.memory);
				LC ++;

				addOperandsWords(wordList, operand, TRUE);
				addOperandsWords(wordList, operand2, FALSE);
				if((operand.type == REG || operand.type == PTR_REG) && (operand2.type == REG || operand2.type == PTR_REG)){
					LC -= 2;
					int val = getCellFromMemory(LC, &state.memory).value.intCell.val | getCellFromMemory(LC + 1, &state.memory).value.intCell.val;
					putIntToMemory(val, LC, &state.memory);
					LC ++;
					putIntToMemory(0, LC, &state.memory);
					
				}
				break;
			default:
				return;
			
	}
		
	
}

/*Function to exxtract the immediate value after the # symbol*/
int extractNumberAfterHash(const char* str) {
    int number;
  
    if (str[0] == '#') {

        if (sscanf(str + 1, "%d", &number) == 1) {
            return number;
        }
    }
    printf("No immediate value was found");
    return -1;
}
/*Function to check if there is an immediate value in the current line*/
int searchImmediateVal(WordList * wordList){
	int i ;
	for (i = 0 ; i < wordList->size; i ++){
			if(wordList->words[i][0] == '#'){
				printf("Immediate val found: %s", wordList->words[i]);
				
				return extractNumberAfterHash(wordList->words[i]);
			}
	}
	return -1;
}

/*This function performs many operations on the current wordLine */
int checkLine(WordList *wordList, LabelTable * labels, LabelTable * externs, LabelTable * entries){


	int lineVal = 0;
	bool hasDataSymbol = searchDataSymbol(wordList);
	bool hasStrSymbol = searchStrSymbol(wordList);
	bool labelFlag = isLabelDeclaration(wordList);
	bool externFlag = searchExternSymbol(wordList);
	

	
    if(isCommentLine(wordList)){
		return 1;
	}
	if(externFlag){
		int i = 1;
		while (i < wordList->size){
			addLabel(labels, wordList->words[i], 1);
			addLabel(externs, wordList->words[i], 1);
			i++;

		} 

		
	}
	OperationType curOp = {0};

	curOp = getOperationType(wordList);


	
	
	if(labelFlag){
		if(hasLabel(labels, wordList->words[0]) != -1){
		}
		else {
			addLabel(labels, wordList->words[0], LC);
		}
	} 
	if(hasDataSymbol){
		appendDataToMemory(wordList);
		return 1;
			}
	
	if(hasStrSymbol){
		appendStringToMemory(wordList);
		return 1;
	}
	

	if(curOp != UNKNOWN){
		lineVal = insertOperationBits(lineVal, curOp);
		lineVal = setBit(lineVal, 2);

		checkOperands(wordList, curOp, lineVal);
		return (int) curOp;
		}
	
	return -1;
}  