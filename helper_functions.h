#ifndef HELPER_FUNCTION_H
#define HELPER_FUNCTION_H

#include "constant.h"


typedef struct TreeNode {
	
	char *macro_name;
	char *macro_code;
	struct TreeNode* left;
	struct TreeNode* right;
} TreeNode;

void initTreeNode(TreeNode *root);
TreeNode* createNode(const char* macro_name, const char* macro_code);
TreeNode* insertNode (TreeNode* root, const char* macro_name, const char* macro_code);
TreeNode* searchNode(TreeNode* root, const char* macro_name);
void inOrderTraversal(TreeNode* root);
void freeTree(TreeNode* root);

typedef struct {
	char **words;
	int size;
	int capacity;
} WordList;

void initWordList(WordList *wordList);
void addWord(WordList *wordList, const char *word);
void extractWords(const char *line, WordList *wordList, bool withComa);
WordList readFileByWords(const char *filename);
void freeWordList(WordList *wordList);

typedef struct {
	WordList line;
	int size;
	int capacity;
} LineList;

typedef struct {
	char **label_name;
	int *label_address;
	int size;
	int capacity;
} LabelTable;

void initLabelTable(LabelTable *labelTable);
void addLabel(LabelTable * labelTable, const char *word, int label_address);

int hasLabel(LabelTable *labelTable, const char *word);
int getLabelAddress(LabelTable *labelTable, int index);
void freeLabelTable(LabelTable *labelTable);
void printLabelTable(LabelTable *labelTable);

void remove_blanks(char *str);
void readFile(FILE* file);
char *strdup(const char *s);

void freeLineList(LineList *lineList);

void printAsciiValuesAndTotal(const char *str);
bool compareWords(const char* word, const char* word2);

char* toBinary(int number) ;
void toOctal(int num, char *result);
void removeQuotes(char *str);
int isNewLine(char c);
bool wordIsInList(const char* word, const char** list);
int getLength(const char** list);
int setBit(int num, int bitPosition);
int insertOperationBits(int num, int operation);


typedef struct Node {
    int data;
    struct Node* next;
} Node;


typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;


Node* createQueueNode(int data);
Queue* createQueue();
int isEmpty(Queue* queue);
void enqueue(Queue* queue, int data);
int dequeue(Queue* queue);
int peek(Queue* queue);
void display(Queue* queue);
void freeQueue(Queue* queue);


typedef struct StrNode {
    char* data;
    struct StrNode* next;
} StrNode;

typedef struct {
    StrNode* front;
    StrNode* rear;
} StrQueue;

StrNode* createStrNode(const char* data);
StrQueue* createStrQueue();
int isStrQueueEmpty(StrQueue* queue);
void enqueueStr(StrQueue* queue, const char* data);
char* dequeueStr(StrQueue* queue);
char* peekStrQueue(StrQueue* queue);
void displayStrQueue(StrQueue* queue);
void freeStrQueue(StrQueue* queue);


#endif 