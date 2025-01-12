/*This helper_functions file contains mainly
data structures function such as a tree and queues that help the assembler*/

#include "helper_functions.h"
#include <stdio.h>

/*Function to initiate a binary Tree*/
void initTreeNode(TreeNode *root){
	if (root == NULL){
		return;
	}
	root->macro_name = malloc(WORD_LIST_CAPACITY * sizeof(char *));
	
	if (root->macro_name == NULL) {
        fprintf(stderr, "Memory allocation for macro_name failed\n");
        exit(1);
    }

    root->macro_code = malloc(WORD_LIST_CAPACITY * sizeof(char *));
    if (root->macro_code == NULL) {
        fprintf(stderr, "Memory allocation for macro_code failed\n");
        free(root->macro_name); 
        exit(1);
    }

	root->left  = NULL;
	root->right = NULL;

}

/*Function to create a new node into the binary Tree of the macros, given the macro name and its code*/
TreeNode* createNode(const char* macro_name, const char* macro_code) {

    TreeNode *new_node = (TreeNode *)malloc(sizeof(TreeNode));
	initTreeNode(new_node);
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
		free(new_node);
        exit(1);
    }
	strcpy(new_node->macro_name, macro_name);
	if(new_node->macro_name == NULL){
		fprintf(stderr, "Memory allocation for macro_name failed\n");
        free(new_node);
        exit(EXIT_FAILURE);
	}
	strcpy(new_node->macro_code, macro_code);
	if (new_node->macro_code == NULL) {
        fprintf(stderr, "Memory allocation for macro_code failed\n");
        free(new_node->macro_name);
        free(new_node);
        exit(EXIT_FAILURE);
    }
	new_node->left = NULL;
	new_node->right = NULL;
	return new_node;
}

/*Function to insert a new macro into the macro binary tree at the right place*/
TreeNode* insertNode (TreeNode* root, const char* macro_name, const char* macro_code){
	
	if (root == NULL ){
		return createNode(macro_name, macro_code);
		
	}
	if (strcmp(macro_name, root->macro_name) < 0){
		root->left = insertNode(root->left, macro_name, macro_code);
	}
	else if (strcmp(macro_name, root->macro_name) > 0){
		root->right = insertNode(root->right, macro_name, macro_code);
	}
	else {
		printf("Error: this macro was already defined");
	}
	
	return root;
}

/*Function to search for a macro by its name*/
TreeNode* searchNode(TreeNode* root, const char* macro_name) {
	if (root == NULL || strcmp(macro_name, root->macro_name) == 0) {
		return root;
	}
	if (strcmp(macro_name, root->macro_name) < 0){
		return searchNode(root->left, macro_name);
	}
	else{
		return searchNode(root->right, macro_name);
	}
}
/*Function to print the binary tree inorder*/
void inOrderTraversal(TreeNode* root) {
	
    if (root == NULL) {
        return;
    }
    inOrderTraversal(root->left);
	
	printf("Macro Name: %s, Macro Code: %s\n", root->macro_name, root->macro_code);
	
    inOrderTraversal(root->right);
}

/*Function to free memory of at the end of the use of the macro binary tree*/
void freeTree(TreeNode* root){
	if (root == NULL){
		return;
	}
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}
/*Function to remove blank characters from a string*/
void remove_blanks(char *str) {
	if(str != NULL){
    char *src = str, *dst = str;

    while (*src != '\0') {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
	}
}
/*Function to read a file character by character*/
void readFile(FILE* file){

    int c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }
}

/*The WordList Type is a type we defined to read input files in a more convienent way in a list of words*/
/*This way it is easier to handle and check every word of the operation*/
void initWordList(WordList *wordList){
	wordList->words = malloc(WORD_LIST_CAPACITY * sizeof(char *));
	wordList->size = 0;
	wordList->capacity = WORD_LIST_CAPACITY;
}
/*This function add a word to a given wordList*/
void addWord(WordList *wordList, const char *word){
	if (wordList->size >= wordList->capacity) {
		wordList->capacity *= 2;
		wordList->words = realloc(wordList->words, wordList->capacity * sizeof(char *));
		
	}
	wordList->words[wordList->size] = strdup(word);
	wordList->size++;
}
/*This function extracts all the words of a line in a list of words, it ignores spaces and can ignore comas if needed*/
void extractWords(const char *line, WordList *wordList, bool withComa){
	char *word;
	char *tempLine = strdup(line);
	char cmpStr[6] = "  \t\n\r";
	if(!withComa){
		cmpStr[0] = ',';
	}
	
	word = strtok(tempLine, cmpStr);
	while (word != NULL){
		addWord(wordList, word);
		word = strtok(NULL, cmpStr);
		remove_blanks(word);
	}
	addWord(wordList, "\n");
	free(tempLine);
	
}
/*This functio reads a given file word by word and keep the file into a wprdList*/
WordList readFileByWords(const char *filename){
	FILE *file = fopen(filename, "r");
	if(file == NULL){
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	
	char line[LINE_SIZE];
	WordList wordList;
	initWordList(&wordList);
	
	while(fgets(line, sizeof(line), file)) {
		extractWords(line, &wordList, FALSE);
		
	}
	
	fclose(file);
	return wordList;
}

/*Function to free a wordList after we re done using it. Important since a wordList uses dynamic memory allocation*/
void freeWordList(WordList *wordList){
		int i;
		for (i = 0; i < wordList->size; i++){
			free(wordList->words[i]);
		}
		free(wordList->words);
}

/*Function to duplicate a string and return the copy*/
char *strdup(const char *s){
	size_t len = strlen(s) + 1;
	char *copy = malloc(len);
	if (copy != NULL){
		strcpy(copy, s);
	}
	return copy;
}

/*The labelTable type is used to save label externs and entry declaration over the file
It contains the name of the label as well as the current PC of the operation*/
void initLabelTable(LabelTable *labelTable){
	labelTable->label_name = malloc(WORD_LIST_CAPACITY * sizeof(char *));
	labelTable->label_address = malloc(WORD_LIST_CAPACITY * sizeof(int));
	labelTable->size = 0;
	labelTable->capacity = WORD_LIST_CAPACITY;

}

/*This function adds a label to a given LabelTbale*/
void addLabel(LabelTable * labelTable, const char *name, int address){
	if(hasLabel(labelTable, name) != -1){
		printf("Error, the symbole is already declared");
		return;
	}
	if (labelTable->size >= labelTable->capacity) {
		labelTable->capacity *= 2;
		labelTable->label_name = realloc(labelTable->label_name, labelTable->capacity * sizeof(char *));
		labelTable->label_address = realloc(labelTable->label_address, labelTable->capacity * sizeof(int));
		
	}
	if(name[0] != '\n'){
	printf("%s", name);
	labelTable->label_name[labelTable->size] = strdup(name);
	labelTable->label_address[labelTable->size] = address;
	labelTable->size++;
	printf("WE JUST ADDED A NEW LABEL : %s with ADDRESS: %d",name, address);
	}
}


/*Function to get the address of the declaration in the given file*/
int getLabelAddress(LabelTable *labelTable, int index){
	return labelTable->label_address[index];
	
}


/*Function to set the address of a given label*/
int setLabelAddress(LabelTable *labelTable, const char* word, int address){
	
	int i = 0;
	for(; i < labelTable->size; i ++){
		if (compareWords(word, labelTable->label_name[i])){
			labelTable->label_address[i] = address;
			return address;
		}
	}
	return -1;
}
/*The function that return the index of a label if it has already been defind' else return -1*/
int hasLabel(LabelTable *labelTable, const char *word){
	int i;
	bool flag = FALSE;
	for (i = 0; i < labelTable->size; i ++){
		flag = strcmp(labelTable->label_name[i], word) == 0 ? TRUE : FALSE;
		if (flag){
			return i;
		}
	}
	return -1;
}

/*Function to free memory allocation after using LabelTable*/
void freeLabelTable(LabelTable *labelTable){
	int i;
	for(i = 0; i < labelTable->size; i++){
		free(labelTable->label_name[i]);
		
	}
	free(labelTable->label_address);
	
}
/*Function to pribnt a label table*/
void printLabelTable(LabelTable * labelTable){
	int i ;
	for (i = 0; i < labelTable->size ; i ++){
		printf("Label %s - Address: %d\n", labelTable->label_name[i], labelTable->label_address[i]);
	}
}

/*This function helpped me checking that the value I assigned were right*/
void printAsciiValuesAndTotal(const char *str) {
    int totalAsciiValue = 0;
    
    while (*str != '\0') {
        int asciiValue = (int)*str;
        printf("Character: %c, ASCII Value: %d\n", *str, asciiValue);
        totalAsciiValue += asciiValue;
        str++;
    }
    
    printf("Total ASCII Value: %d\n", totalAsciiValue);
} 

/*Function that compare two words char by char and return TRUE or FALSE*/
bool compareWords(const char* word, const char* word2){
	int len = strlen(word);
	int len2 = strlen(word2);
	
	int i = 0;
	if(len != len2) {
		return FALSE;
	}
	for (; i < len; i++){
		if(word2[i] != word[i]){
			return FALSE;
		}
	}
	return TRUE;
}

/*Function to return a string representation of a decimal number in binary (base 2)*/
char* toBinary(int number) {
   
    char* binary = (char*)malloc(16);
    int i;
    if (binary == NULL) {
        return NULL;
    }
    
 
    binary[15] = '\0';

    
    for (i = 14; i >= 0; i--) {
        binary[i] = (number & 1) ? '1' : '0';
        number >>= 1;
    }

    return binary;
}


/*Function to get the octal representation of an int in a given string*/
void toOctal(int num, char *result) {
    char buffer[6]; 
    int index = 4;
	int i = 0;

    
    for (; i < 5; i++) {
        buffer[i] = '0';
    }
    buffer[5] = '\0'; 

    while (num > 0 && index >= 0) {
        buffer[index] = (num % 8) + '0'; 
        num /= 8;
        index--;
    }

    strcpy(result, buffer);
}
/*This functions removs quotes signs in strings declaration*/
void removeQuotes(char *str) {
    size_t len = strlen(str);

    if (len < 2) {
        return;  
    }

    if ((str[0] == '"' || str[0] == '\'') && (str[len - 1] == '"' || str[len - 1] == '\'')) {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}
/*checking if a char is a backspace*/
int isNewLine(char c) {
    return c == '\n';
}
/*Function to check if a string is in a list of strings*/
bool wordIsInList(const char* word, const char** list){
	int len , i;
	len = getLength(list);
	for(i = 0; i < len ; i ++){
		if(compareWords(word, list[i])){
			return TRUE;
		}
	}
	return FALSE;
}
/*Get the length of a string list*/
int getLength(const char** list){
	int i =0;
	while(list[i] != NULL){
		i++;
	}
	return i;
}
/*Given a int and an index it mask the bit a the index to be a 1*/
int setBit(int num, int bitPosition){
	int mask = 1 << bitPosition;
	
	return num | mask;
}
/*Given an int and a number of operation it turn on the relevent bits of the given operation*/
int insertOperationBits(int num, int operation){
	operation &= 15;
	
	int mask = ~(15 << 11);
	num &= mask;
	operation <<= 11;
	return num | operation;
}

/*This function create a Queue Node for the Queue structure*/
Node* createQueueNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation error\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

/*Function to create a queue*/
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        printf("Memory allocation error\n");
        exit(1);
    }
    queue->front = queue->rear = NULL;
    return queue;
}

/*Function to check if a queue is empty*/
int isEmpty(Queue* queue) {
    return queue->front == NULL;
}

/*Enqueue a node to a queue given an int*/
void enqueue(Queue* queue, int data) {
    Node* newNode = createQueueNode(data);
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;
}
/*Dequeue a node from a queue*/
int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return -1;
    }
    Node* temp = queue->front;
    int data = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return data;
}

/*Function to print the first node of a queue*/
int peek(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return -1;
    }
    return queue->front->data;
}

/*Function to print the whole queue*/
void display(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }
    Node* temp = queue->front;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

/*Function to free the queue after usage*/
void freeQueue(Queue* queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

/*These are functions for the string queue data structure*/
/*Function to create a string node*/
StrNode* createStrNode(const char* data) {
    StrNode* node = (StrNode*)malloc(sizeof(StrNode));
    node->data = strdup(data);
    node->next = NULL;
    return node;
}

/*Function to create a strQueue*/
StrQueue* createStrQueue() {
    StrQueue* queue = (StrQueue*)malloc(sizeof(StrQueue));
    queue->front = queue->rear = NULL;
    return queue;
}
/*Check if the StrQueue is empty*/
int isStrQueueEmpty(StrQueue* queue) {
    return queue->front == NULL;
}

/*Function to add a node to a given queue, given a string*/
void enqueueStr(StrQueue* queue, const char* data) {
    StrNode* node = createStrNode(data);
    if (queue->rear == NULL) {
        queue->front = queue->rear = node;
        return;
    }
    queue->rear->next = node;
    queue->rear = node;
}
/*Function to dequeue the first node of the queue*/
char* dequeueStr(StrQueue* queue) {
    if (isStrQueueEmpty(queue)) {
        fprintf(stderr, "Queue is empty.\n");
        exit(EXIT_FAILURE);
    }
    StrNode* temp = queue->front;
    char* data = strdup(temp->data);
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp->data);
    free(temp);
    return data;
}

/*Function to print the first element of the string queue*/
char* peekStrQueue(StrQueue* queue) {
    if (isStrQueueEmpty(queue)) {
        fprintf(stderr, "Queue is empty.\n");
        exit(EXIT_FAILURE);
    }
    return queue->front->data;
}
/*Function to display the qhole str queue*/
void displayStrQueue(StrQueue* queue) {
    StrNode* current = queue->front;
    while (current != NULL) {
        printf("%s -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

/*Functon to free the str queueu after usage*/
void freeStrQueue(StrQueue* queue) {
    StrNode* current = queue->front;
    while (current != NULL) {
        StrNode* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(queue);
}