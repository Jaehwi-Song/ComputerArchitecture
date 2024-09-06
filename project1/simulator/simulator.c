/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);


int main(int argc, char *argv[])
{   
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    int opcode, regA, regB, destReg, offsetField;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);   
    }
    // if the input file is empty.
    if (state.numMemory == 0) {
        printf("Error! No machine instructions in input file!\n");
        exit(1);
    }

		/* TODO: */
    // initialize all registers to 0
    for (int i = 0; i < NUMREGS; i++) {
        state.reg[i] = 0;
    }
    // initialize pc to 0
    state.pc = 0;
    int instruction, total = 0;

    while(1) {
        // error when the PC value exceed the available range of instructions on memory.
        if (state.pc >= state.numMemory && state.pc < 0) {
            printf("Error! PC value exceeded the range of available instructions on memory!\n");
            exit(1);
        }
        printState(&state);
        // fetch instruction and update PC value by 1
        instruction = state.mem[state.pc++];
        opcode = instruction >> 22;

        // O-type do not have register field
        if (opcode != 0b110 && opcode != 0b111) {
            regA = (instruction>>19) & 0b111;
            regB = (instruction>>16) & 0b111;
        }
        
        // R-type
        if (opcode == 0b000 || opcode == 0b001) {
            destReg = instruction & 0b111;
            // add
            if (opcode == 0b000)
                state.reg[destReg] = state.reg[regA] + state.reg[regB];
            // nor
            else
                state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
        }
        // I-type
        else if (opcode == 0b010 || opcode == 0b011 || opcode == 0b100) {
            offsetField = instruction & (0xffff);
            // sign extenstion
            offsetField = convertNum(offsetField);
            // lw
            if (opcode == 0b010) {
                state.reg[regB] = state.mem[state.reg[regA] + offsetField];
            }
            // sw
            else if (opcode == 0b011) {
                state.mem[state.reg[regA] + offsetField] = state.reg[regB];
            }
            // beq
            else {
                if (state.reg[regA] == state.reg[regB]) {
                    if (offsetField == -1){
                        printf("Error! Branching to address -1!\n");
                        exit(1);
                    }
                    state.pc += offsetField;
                }
            }
        }
        // J-type (jalr)
        else if (opcode == 0b101) {
            state.reg[regB] = state.pc;
            state.pc = state.reg[regA];
        }
        // halt
        else if (opcode == 0b110) {
            break;
        }
        // noop
        else if (opcode == 0b111) {
            continue;
        }
        else {
            printf("Error! Invalid opcode!\n");
            exit(1);
        }
        total++;
    }
    printf("machine halted\n");
    // halt instruction do not increase total value.
    printf("total of %d instructions executed!\n", ++total);
    printf("final state of machine:\n");
    printState(&state);
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}