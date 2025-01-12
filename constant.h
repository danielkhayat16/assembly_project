#ifndef CONSTANT_H
#define CONSTANT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



#define MACRO_DEFINE 		"macr"
#define MACRO_END 			"endmacr"
#define BUFFER_SIZE 		1024
#define LINE_SIZE 			81
#define WORD_LIST_CAPACITY	10
#define LINE_LIST_CAPACITY	81

#define TRUE  1
#define FALSE 0
#define REGISTER_LIST { \
    "r0", "r1", "r2", "r3", \
    "r4", "r5", "r6", "r7", NULL \
}

#define REGISTER_POINTER_LIST { \
    "*r0", "*r1", "*r2", "*r3", \
    "*r4", "*r5", "*r6", "*r7", NULL \
}

typedef int bool;

#endif