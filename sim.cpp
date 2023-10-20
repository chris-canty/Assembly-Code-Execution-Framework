#include <stdio.h>
#include <string.h>
#include <cstdlib>
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
int convertNum(int);


int main(int argc, char *argv[]){

    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    int opcode, regA, regB, destReg, offsetField, offsetReal;


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
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++){
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1){
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

/* set the  register values equal to 0 to start*/
    for (int k = 0; k < NUMREGS; k++){
        state.reg[k] = 0;
    }

/*This for loop will run the length of memory, going through each case until the
all instructions in memory have been completed. */
    state.pc = 0;

    while (state.pc < NUMMEMORY){


        if ((state.mem[state.pc] >> 22) == 0){ //Add
            printState(&state);

            regA  = (state.mem[state.pc] >> 19) & 7;
            regB = (state.mem[state.pc] >> 16) & 7;
            destReg = (state.mem[state.pc]) & 7;

            state.reg[destReg] = state.reg[regA] + state.reg[regB];
            state.pc++;
        }


        if ((state.mem[state.pc] >> 22) == 1){ //Nand
            regA  = (state.mem[state.pc] >> 19) & 7;
            regB = (state.mem[state.pc] >> 16) & 7;
            destReg = (state.mem[state.pc]) & 7;

            state.reg[destReg] = !(state.reg[regA] && state.reg[regB]);
            state.pc++;
        }


        if ((state.mem[state.pc] >> 22) == 2){ //Load
            printState(&state);

            regA  = (state.mem[state.pc] >> 19) & 7;
            regB = (state.mem[state.pc] >> 16) & 7;
            offsetField = (state.mem[state.pc]) & 7;
            offsetReal = convertNum(offsetField);

            state.reg[(regB)] = state.mem[state.reg[regA]+offsetReal];
            state.pc++;
        }


        if ((state.mem[state.pc] >> 22) == 3){ //Store
            printState(&state);

            regA  = (state.mem[state.pc] >> 19) & 7;
            regB = (state.mem[state.pc] >> 16) & 7;
            offsetField = (state.mem[state.pc]) & 7;
            offsetReal = convertNum(offsetField);

            state.mem[state.reg[regA]+offsetReal] = state.reg[regB];

            state.pc++;
        }


        if ((state.mem[state.pc] >> 22) == 4){ //Branch
            printState(&state);

            regA = (state.mem[state.pc] >> 19) & 7;
            regB = (state.mem[state.pc] >> 16) & 7;
            offsetField = ((state.mem[state.pc]) & 65535);
            offsetReal = convertNum(offsetField);

            if (state.reg[regA] == state.reg[regB])
            {
                state.pc = state.pc+1+offsetReal;
            }
            else
            {
                state.pc++;
            }
        }


        if ((state.mem[state.pc] >> 22) == 6){ //Halt
            printState(&state);
            state.pc++;
            break;
        }


        if ((state.mem[state.pc] >> 22) == 7){ //Noop
            printState(&state);
            state.pc++;
        }
    }

/* prints one more time before loop exit*/
    printState(&state);

    return(0);
}


void printState(stateType *statePtr){

    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");

    for (i=0; i<statePtr->numMemory; i++) {
        printf("\t\t    mem[ %d ] %d\n", i, statePtr->mem[i]);
    }

    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
        printf("\t\t    reg[ %d ] %d\n", i, statePtr->reg[i]);
    }

    printf("end state\n");
}

int convertNum(int num){
    if (num & (1<<15) ){
        num -= (1<<16);
    }

    return(num);
}


