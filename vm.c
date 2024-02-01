#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
bool haltFlag = true;

// Find base L levels down
int base (int BP, int L) {
    int arb = BP;       // arb = activation record base
    while (L > 0) {     // find base L levels down
        arb = pas[arb];
        L--;
    }
    return arb;
}

// Print functions: stack, instructions
void print(Instruction IR, int PC, int BP, int SP) {
    // Print instructions
    printf(" %d %d \t%d\t%d\t%d\t", IR.l, IR.m, PC, BP, SP);

    // Print Stack
    for (int i = 499; i > SP-1; i--) {
        if (i != BP || i == 499)
            printf("%d ", pas[i]);
        else {
            printf("| %d ", pas[i]);
        }
    }
    printf("\n");
}

int main(int argc, char **argv) {
    FILE *fp;
    Instruction IR; // instruction register
    char opcode[4];
    
    // Open and read file
    fp = fopen (argv[1], "r");

    // Initial values for PM/0 CPU registers
    int BP = 499;                   // base pointer
    int SP = BP + 1;                // stack pointer
    int PC = 0;
    IR.op = 0; IR.l = 0; IR.m = 0;  

    // Initial process address space values set to zero
    for (int i = 0; i < ARRAY_SIZE; i++) {
        pas[i] = 0;
    }
    
    // Fetch Cycle
    while(fscanf(fp, "%d %d %d", &IR.op, &IR.l, &IR.m) != EOF) {
        pas[PC] = IR.op;
        pas[PC+1] = IR.l;
        pas[PC+2] = IR.m;
        PC += 3;
    }

    // Close File
    fclose(fp);

    // Reset PC
    PC = 0;

    // Print header and initial values
    printf("\t\t\tPC\tBP\tSP\tstack\n"); 
    printf("Initial Values:\t\t%d\t%d\t%d\n\n", PC, BP, SP);   

    // Initiating IR
    IR.op = pas[PC];
    IR.l = pas[PC+1]; 
    IR.m = pas[PC+2];

    // Execution Cycle
    while (haltFlag) {
        switch (IR.op)
        {
        case LIT:
            SP--;
            pas[SP] = IR.m;

            printf("\tLIT");
            print(IR, PC, BP, SP);
            break;
        case 2: // OPR or Return
            if (IR.m !=0) {
                switch (IR.m)
                {
                case ADD:
                    pas[SP+1] = pas[SP+1] + pas[SP];
                    SP++;
                    
                    printf("\tADD");
                    print(IR, PC, BP, SP);
                    break;
                case SUB:
                    pas[SP+1] = pas[SP+1] - pas[SP];
                    SP++;
                    
                    printf("\tSUB");
                    print(IR, PC, BP, SP);
                    break;
                case MUL:
                    pas[SP+1] = pas[SP+1] * pas[SP];
                    SP++;
                    
                    printf("\tMUL");
                    print(IR, PC, BP, SP);
                    break;
                case DIV:
                    pas[SP+1] = pas[SP+1] / pas[SP];
                    SP++;
                        
                    printf("\tDIV"); 
                    print(IR, PC, BP, SP);
                    break;
                case EQL:
                    pas[SP+1] = (pas[SP+1] == pas[SP]);
                    SP++;
                        
                    printf("\tEQL");
                    print(IR, PC, BP, SP);
                    break;
                case NEQ:
                    pas[SP+1] = pas[SP+1] != pas[SP];
                    SP++;
                    
                    printf("\tNEQ");
                    print(IR, PC, BP, SP);
                    break;
                case LSS:
                    pas[SP+1] = pas[SP+1] < pas[SP];
                    SP++;
                    
                    printf("\tLSS");
                    print(IR, PC, BP, SP);
                    
                    break;
                case LEQ:
                    pas[SP+1] = pas[SP+1] <= pas[SP];
                    SP++;
                        
                    printf("\tLEQ");
                    print(IR, PC, BP, SP);
                    
                    break;
                case GTR:
                    pas[SP+1] = pas[SP+1] > pas[SP];
                    SP++;
                        
                    printf("\tGTR");
                    print(IR, PC, BP, SP);
                    
                    break;
                case GEQ:
                    pas[SP+1] = pas[SP+1] >= pas[SP];
                    SP++;
                        
                    printf("\tGEQ");
                    print(IR, PC, BP, SP);
                  
                    break;
                case ODD:
                    pas[SP] = pas[SP] % 2;
                        
                    printf("\tODD");
                    print(IR, PC, BP, SP);
                  
                    break;
                }
            } else {
            SP = BP + 1;
            BP = pas[SP-2];
            PC = pas[SP-3];

            printf("\tRTN");
            print(IR, PC, BP, SP);
            }

            break;
        case LOD:
            SP--;
            pas[SP] = pas[base(BP,IR.l) - IR.m];

            printf("\tLOD");    
            print(IR, PC, BP, SP);

            break;
        case STO:
            pas[base(BP, IR.l) - IR.m] = pas[SP];
            SP++;
                
            printf("\tSTO");
            print(IR, PC, BP, SP);

            break;
        case CAL:
            pas[SP-1] = base(BP, IR.l); // Static Link
            pas[SP-2] = BP;             // Dynamic Link
            pas[SP-3] = PC;             // Return Address
            BP = SP - 1;
            PC = IR.m;

            printf("\tCAL");
            print(IR, PC, BP, SP);

            break;
        case INC:
            SP -= IR.m;

            printf("\tINC");                
            print(IR, PC, BP, SP);
            
            break;
        case JMP:
            PC = IR.m;
            
            printf("\tJMP");
            print(IR, PC, BP, SP);
            break;
        case JPC:
            if (pas[SP] == 0)
                PC = IR.m;

            SP += 1;
            
            printf("\tJPC");
            print(IR, PC, BP, SP);
            
            break;
        case SYS:
            switch (IR.m)
            {
            case 1:
                SP++;
                printf("Output result is: %d\n", pas[SP-1]);

                printf("\tSOU");
                print(IR, PC, BP, SP);

                break;
            case 2:
                SP--;

                printf("Please Enter an Integer: ");
                scanf("%d", &pas[SP]);
                
                printf("\tSIN");
                print(IR, PC, BP, SP);
                
                break;
            case 3:
                haltFlag = false;

                printf("\tEOP");
                print(IR, PC, BP, SP);
                break;
            }
            break;
        }
        
        // Update IR
        IR.op = pas[PC];
        IR.l = pas[PC+1]; 
        IR.m = pas[PC+2];

        // Update PC depending on RTN
        if (IR.op != 2) {
            PC+=3;
        } else if (IR.m != 0) {
            PC += 3;
        }
    }
    return 0;
}