/*the memory.c contains the implementation of the functions that assures memory manipulation*
This way we can easily perform memory changes if needed.*/
#include "memory.h"
#include "helper_functions.h"
#include "constant.h"

/*The system state is the memeory picture of the assembler*/
void initializeSystemState(SystemState * systemState){
	/*We initialize to 0 the entire systemState structure*/
	memset(systemState, 0, sizeof(SystemState));
	
	/*Now we can initialize the memory cells and registers*/
	initializeMemory(&systemState->memory);
	initializeRegisters(&systemState->registers);
	systemState->PC.type = UINT_CELL;
	systemState->PC.value.uIntCell.val = 0;
	systemState->PSW = 0;
}

/*This is the initialization of the memory in the system state*/
void initializeMemory(Memory *memory){
	int i ;
	for (i= 0; i < MEMORY_SIZE; i ++){
		/*We initialize default type and default value for each cell in the memory*/
		memory->cells[i].type = INT_CELL;
		memory->cells[i].value.intCell.val = NULL;
	}
}
/*This function initialize the registers*/
void initializeRegisters(Registers *registers){
	registers->r0 = 0;
	registers->r1 = 0;
	registers->r2 = 0;
	registers->r3 = 0;
	registers->r4 = 0;
	registers->r5 = 0;
	registers->r6 = 0;
	registers->r7 = 0;
}

/*We then implement multiple functions to put values into memory*/
void putIntToMemory(int value, int address, Memory *memory){
	if (address < 0 || address >= MEMORY_SIZE){
		printf("Error: Invalid memory address %d\n", address);
		return;
	}
	
	if (value < -(1 << 14) || value >= (1 << 14)){
		printf("Error: Value exceeds 15-bit signed integer range\n");
		return;
	}
	
	memory->cells[address].type = INT_CELL;
	memory->cells[address].value.intCell.val = value;
	printf("WE JUST PUT %d INTO MEMORY ADDRESS : %d\n", value, address);
}
/*Fucntion to put a char into a given address into memory*/
void putCharToMemory(char value, int address, Memory *memory){
	if (address < 0 || address >= MEMORY_SIZE) {
        printf("Error: Invalid memory address %d\n", address);
        return;
    }

    if (value < -(1 << 14) || value >= (1 << 14)) {
        printf("Error: Value exceeds 15-bit signed integer range\n");
        return;
    }
	
	if(value < 0 || value >= 128){
		printf("Error: Value is not in ascii table\n");
        return;
	}

    memory->cells[address].type = CHAR_CELL;
    memory->cells[address].value.charCell.val = value;
	printf("WE JUST PUT %c INTO MEMORY\n", value);
}
/*Function to put an unsigned int into a given address in memery*/
void putUIntToMemory(unsigned int value, int address, Memory *memory){
	if (!isValidAddress(address)) {
        printf("Error: Invalid memory address %d\n", address);
        return;
    }

    if (value >= (1u << 15)) {
        printf("Error: Value exceeds 15-bit unsigned integer range\n");
        return;
    }

    memory->cells[address].type = UINT_CELL;
    memory->cells[address].value.uIntCell.val = value;
	printf("UINT INSERTED\n");
}
/*Function to put a cell into memory at a given address*/
void putCellToMemory(Cell cell, int address, Memory *memory){
	if (!isValidAddress(address)) {
        printf("Error: Invalid memory address %d\n", address);
        return;
    }
	memory->cells[address]= cell;
	
}

/*Function to put a given cell into a register*/
void putCellToRegister(Cell cell, int registerNumber, SystemState *state){
	if(!isValidRegister(registerNumber)){
		printf("Error: Invalid register %d\n", registerNumber);
		return;
	}
	int value;
    switch (cell.type) {
        case INT_CELL:
            value = cell.value.intCell.val;
            break;
        case CHAR_CELL:
            value = cell.value.charCell.val;
            break;
        case UINT_CELL:
            value = cell.value.uIntCell.val;
            break;
        default:
            printf("Error: Unknown cell type\n");
            return;
    }

    if (value < -(1 << 14) || value >= (1 << 14)) {
        printf("Error: Value exceeds 15 bits\n");
        return;
    }

    switch (registerNumber) {
        case 0: state->registers.r0 = value; break;
        case 1: state->registers.r1 = value; break;
        case 2: state->registers.r2 = value; break;
        case 3: state->registers.r3 = value; break;
        case 4: state->registers.r4 = value; break;
        case 5: state->registers.r5 = value; break;
        case 6: state->registers.r6 = value; break;
        case 7: state->registers.r7 = value; break;
        default:
            printf("Error: Invalid register %d\n", registerNumber);
            return;
    }
	
	
}
/*These are function to get value from memory, given an address*/
Cell getCellFromMemory(int address, Memory *memory){
	Cell cell;

    if (address < 0 || address >= MEMORY_SIZE) {
        printf("Error: Invalid memory address %d\n", address);
        /* Return an error or default value */
        cell.type = INT_CELL; /* Assuming INT_CELL as default type */
        cell.value.uIntCell.val = 0; /* Default value */
        return cell;
    }

    return memory->cells[address];
}
/*Function to assign a velue to a given cell*/
void assignValueToCell(Cell *cell, int value) {
    switch (cell->type) {
        case UINT_CELL:
            cell->value.uIntCell.val = value;
            break;
        case INT_CELL:
            cell->value.intCell.val = value;
            break;
        case CHAR_CELL:
            cell->value.charCell.val = value;
            break;
    }
}

/*Function to get cell from a given register*/
Cell getCellFromRegister(int registerNumber, SystemState *state, CellType type) {
    Cell cell = {.type = type};

    if (!isValidRegister(registerNumber)) {
        printf("Error: Invalid register %d\n", registerNumber);
        cell.type = INT_CELL; 
        cell.value.intCell.val = 0; 
        return cell;
    }

    int value = 0;
    switch (registerNumber) {
        case 0: value = state->registers.r0; break;
        case 1: value = state->registers.r1; break;
        case 2: value = state->registers.r2; break;
        case 3: value = state->registers.r3; break;
        case 4: value = state->registers.r4; break;
        case 5: value = state->registers.r5; break;
        case 6: value = state->registers.r6; break;
        case 7: value = state->registers.r7; break;
    }

    assignValueToCell(&cell, value);
    return cell;
}

/*Function to move a value from cell to cell*/
void moveFromAddressToDestination(int srcAddress, int destAddress, Memory *memory) {
    if (srcAddress < 0 || srcAddress >= MEMORY_SIZE || destAddress < 0 || destAddress >= MEMORY_SIZE) {
        printf("Error: Invalid memory addresses %d or %d\n", srcAddress, destAddress);
        return;
    } 

    Cell cellToMove = getCellFromMemory(srcAddress, memory);
    switch (cellToMove.type) {
        case INT_CELL:
            putIntToMemory(cellToMove.value.intCell.val, destAddress, memory);
            break;
        case CHAR_CELL:
            putCharToMemory((char)cellToMove.value.charCell.val, destAddress, memory);
            break;
        case UINT_CELL:
            putUIntToMemory(cellToMove.value.uIntCell.val, destAddress, memory);
            break;
        default:
            printf("Unknown type\n");
            break;
    }

    memset(&memory->cells[srcAddress], 0, sizeof(Cell));
}

/*This function allows to put an int value into a given register*/
void putIntToRegister(int value, int registerNumber, SystemState * state) {
	if (!isValidRegister(registerNumber)) {
		printf("Error: Invalid register %d\n", registerNumber);
		return ;
	}
	if (value < -(1 << 14) || value >= (1 << 14)) {
		printf("Error: Value exceeds 15 bits\n");
		return;
	}
	
	switch(registerNumber){
		case 0: state->registers.r0 = value; break;
		case 1: state->registers.r1 = value; break;
		case 2: state->registers.r2 = value; break;
		case 3: state->registers.r3 = value; break;
		case 4: state->registers.r4 = value; break;
		case 5: state->registers.r5 = value; break;
		case 6: state->registers.r6 = value; break;
		case 7: state->registers.r7 = value; break;
	}
}

/*Function to put a char value to a given register*/
void putCharToRegister(char value, int registerNumber, SystemState *state) {
    if (!isValidRegister(registerNumber)) {
        printf("Error: Invalid register %d\n", registerNumber);
        return;
    }
    if (value < -(1 << 14) || value >= (1 << 14)) {
        printf("Error: Value exceeds 15 bits\n");
        return;
    }
    switch (registerNumber) {
        case 0: state->registers.r0 = value; break;
        case 1: state->registers.r1 = value; break;
        case 2: state->registers.r2 = value; break;
        case 3: state->registers.r3 = value; break;
        case 4: state->registers.r4 = value; break;
        case 5: state->registers.r5 = value; break;
        case 6: state->registers.r6 = value; break;
        case 7: state->registers.r7 = value; break;
    }
}

/* Function to put unsigned int value to a register */
void putUIntToRegister(unsigned int value, int registerNumber, SystemState *state) {
    if (!isValidRegister(registerNumber)) {
        printf("Error: Invalid register %d\n", registerNumber);
        return;
    }
    if (value >= (1 << 15)) {
        printf("Error: Value exceeds 15 bits\n");
        return;
    }
    switch (registerNumber) {
        case 0: state->registers.r0 = value; break;
        case 1: state->registers.r1 = value; break;
        case 2: state->registers.r2 = value; break;
        case 3: state->registers.r3 = value; break;
        case 4: state->registers.r4 = value; break;
        case 5: state->registers.r5 = value; break;
        case 6: state->registers.r6 = value; break;
        case 7: state->registers.r7 = value; break;
    }
}

/*This is a helper function to check if the register number is valid*/
bool isValidRegister(int reg){
	return reg >= 0 && reg <=7 ? TRUE : FALSE;
}
/*Helper function to check if the address we ar trying to acces into memory is valid*/
bool isValidAddress(int address){
	return (address >= 0 && address < MEMORY_SIZE) ? TRUE : FALSE;
}

/*Function to move a cell from a given address to a register*/
void moveFromAddressToRegister(int address, int registerNumber, SystemState *state){
	if (!isValidAddress(address)){
		printf("Error: Invalid address %d\n", address);
		return;
	}
	
	if (!isValidRegister(registerNumber)){
		printf("Error: Invalid register %d\n", registerNumber);
		return;
	}
	Cell cell = getCellFromMemory(address, &state->memory);
	putCellToRegister(cell, registerNumber, state);
	memset(&state->memory.cells[address], 0, sizeof(Cell));
}
/*Function to move a given register to an address*/
void moveFromRegisterToAddress(int registerNumber, int address, SystemState *state){
	if (!isValidAddress(address)){
		printf("Error: Invalid address %d\n", address);
		return;
	}
	
	if (!isValidRegister(registerNumber)){
		printf("Error: Invalid register %d\n", registerNumber);
		return;
	}
	Cell cell = getCellFromRegister(registerNumber, state, INT_CELL);/* In the future i would like to get the type of cell in the address and put it as argument*/
	putCellToMemory(cell, address, &state->memory);
	switch (registerNumber) {
        case 0: state->registers.r0 = 0; break;
        case 1: state->registers.r1 = 0; break;
        case 2: state->registers.r2 = 0; break;
        case 3: state->registers.r3 = 0; break;
        case 4: state->registers.r4 = 0; break;
        case 5: state->registers.r5 = 0; break;
        case 6: state->registers.r6 = 0; break;
        case 7: state->registers.r7 = 0; break;
        default:
            printf("Error: Invalid register %d\n", registerNumber);
            return;
    }
}


/*Funciton to check if a cell is empty*/
bool isCellEmpty(Cell cell) {
    switch (cell.type) {
        case UINT_CELL:
            return cell.value.uIntCell.val == NULL;
        case INT_CELL:
            return cell.value.intCell.val == NULL;
        case CHAR_CELL:
            return cell.value.charCell.val == NULL;
        default:
            return TRUE;
    }
}
/*Function to check if a register is empty or not*/
bool isRegisterEmpty(int registerNumber) {
    return registerNumber == 0;
}
/*Function to print the system state: the values stored in the memory and the registers*/
void printSystemState(SystemState *state) {
	int i;
    printf("Memory:\n");
    for (i = 0; i < MEMORY_SIZE; i++) {
        if (!isCellEmpty(state->memory.cells[i])) {
            printf("Address %d: ", i);
            switch (state->memory.cells[i].type) {
                case UINT_CELL:
                    printf("UINT %s\n", toBinary(state->memory.cells[i].value.uIntCell.val));
                    break;
                case INT_CELL:
                    printf("INT %s\n", toBinary(state->memory.cells[i].value.intCell.val));
                    break;
                case CHAR_CELL:
                    printf("CHAR %c\n", (state->memory.cells[i].value.charCell.val));
                    break;
            }
        }
    }

    printf("Registers:\n");
    if (!isRegisterEmpty(state->registers.r0)) printf("R0: %s\n", toBinary(state->registers.r0));
    if (!isRegisterEmpty(state->registers.r1)) printf("R1: %s\n", toBinary(state->registers.r1));
    if (!isRegisterEmpty(state->registers.r2)) printf("R2: %s\n", toBinary(state->registers.r2));
    if (!isRegisterEmpty(state->registers.r3)) printf("R3: %s\n", toBinary(state->registers.r3));
    if (!isRegisterEmpty(state->registers.r4)) printf("R4: %s\n", toBinary(state->registers.r4));
    if (!isRegisterEmpty(state->registers.r5)) printf("R5: %s\n", toBinary(state->registers.r5));
    if (!isRegisterEmpty(state->registers.r6)) printf("R6: %s\n", toBinary(state->registers.r6));
    if (!isRegisterEmpty(state->registers.r7)) printf("R7: %s\n", toBinary(state->registers.r7));
}