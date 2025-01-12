#define MEMORY_SIZE 4096
#include "constant.h"


typedef struct {
	int r0:15;
	int r1:15;
	int r2:15;
	int r3:15;
	int r4:15;
	int r5:15;
	int r6:15;
	int r7:15;
} Registers;

typedef struct uIntegerCell {
	unsigned int val : 15;
} uIntegerCell;

typedef struct integerCell {
	int val : 15;
} integerCell;

typedef struct characterCell {
	int val : 15;
} characterCell;

typedef union CellValue {
    uIntegerCell uIntCell;
    integerCell intCell;
    characterCell charCell;
} CellValue;

typedef enum {
    UINT_CELL,
    INT_CELL,
    CHAR_CELL
} CellType;

typedef struct Cell {
    CellType type;
    CellValue value;
} Cell;

typedef struct Memory {
    Cell cells[MEMORY_SIZE];
} Memory;

typedef struct {
    Memory memory;
    Registers registers;
	Cell PC;
	int PSW : 15;
} SystemState;

void initializeSystemState(SystemState *systemState);
void initializeMemory(Memory *memory);
void initializeRegisters(Registers *registers);

void putIntToMemory(int value, int address, Memory *memory);
void putCharToMemory(char value, int address, Memory *memory);
void putUIntToMemory(unsigned int value, int address, Memory *memory);
void putCellToMemory(Cell cell, int address, Memory *memory);
void putCellToRegister(Cell cell, int registerNumber, SystemState *state);
void putIntToRegister(int value, int registerNumber, SystemState *state);
void putCharToRegister(char value, int registerNumber, SystemState *state);
void putUIntToRegister(unsigned int value, int registerNumber, SystemState *state);

Cell getCellFromMemory(int address, Memory *memory);
Cell getCellFromRegister(int registerNumber, SystemState *state, CellType type);
void assignValueToCell(Cell *cell, int value);


bool isValidRegister(int reg);
bool isValidAddress(int address);

void moveFromAddressToRegister(int address, int registerNumber, SystemState *state);
void moveFromRegisterToAddress(int registerNumber, int address, SystemState *state);
void moveFromAddressToDestination(int srcAddress, int destAddress, Memory *memory);

bool isCellEmpty(Cell cell);
bool isRegisterEmpty(int reg);

void printSystemState(SystemState *state);

extern SystemState state;