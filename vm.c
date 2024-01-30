/*
GROUP MEMBERS:
Josie Goreczky
Kyutza Lopez-Herera
*/

#include <stdio.h>
#include <stdlib.h>


#define ARRAY_SIZE 500

// Instructions
#define LIT 1   // Literal
#define OPR 2   // Operation
#define RTN 2   // Return
#define LOD 3   // Load
#define STO 4   // Store
#define CAL 5   // Call Procedure
#define INC 6   // Increment
#define JMP 7   // Jump
#define JPC 8   // Jump Condition
#define SYS 9   // System

// Arithmetic/ Logical Instructions
#define ADD 1   // Add
#define SUB 2   // Subtract
#define MUL 3   // Multiply
#define DIV 4   // Divide
#define EQL 5   // Equal
#define NEQ 6   // Not Equal
#define LSS 7   // Less than
#define LEQ 8   // Less than or Equal
#define GTR 9   // Greater than
#define GEQ 10  // Greater than or Equal
#define ODD 11  // Odd

typedef struct {
    int op; // op code
    int l;  // L
    int m;  // M
} Instruction;

// Global Variables
int pas[ARRAY_SIZE];
int haltFlag = 1;


// Find base L levels down
int base (int BP, int L) {
    int arb = BP;       // arb = activation record base
    while (L > 0) {     // find base L levels down
        arb = pas[arb];
        L--;
    }
    return arb;
}

// print functions: stack, instructions


int main() {
    FILE *fp;
    Instruction IR; // instruction register
    char opcode[3];
    

    // Initial values for PM/0 CPU registers
    int BP = 499;                   // base pointer
    int SP = BP + 1;                // stack pointer
    int PC = 0;
    IR.op = 0; IR.l = 0; IR.m = 0;  

    // Initial process address space values set to zero
    for (int i = 0; i < ARRAY_SIZE; i++) {
        pas[i] = 0;
    }

    fp = fopen ("input.txt", "r");

    // Fetch Cycle
    while(!feof(fp)) {
        pas[PC] = IR.op;
        pas[PC+1] = IR.l;
        pas[PC+2] = IR.m;
        PC += 3;
    }

    // Reset PC
    PC = 0;
    

    // read instructions

    // parse the instruction?

    // switch case




    return 0;
}