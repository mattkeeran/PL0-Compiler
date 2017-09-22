//Matthew Keeran NID: ma553501
//COP 3402 Fall 2015
//Virtual P machine

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVEL 3

//function declarations
void acode();
void ex();
void fetch();
void execute();
void OPR();
int base(int l, int base);

typedef struct instruction
{
	int op;
	int l;
	int m;
} instruction;

const char *OpCodes[] = {"lit", "opr", "lod", "sto", "cal", "inc","jmp","jpc","sio", "sio", "sio"};
int sp = 0, bp = 1, pc = 0;
int halt = 0;
int stack[MAX_STACK_HEIGHT];
instruction code[MAX_CODE_LENGTH];
int ar[MAX_LEXI_LEVEL];                            //activation record array
instruction inreg;                                 //instruction register

int j, k;
char line2[10];
int counter = 0;
FILE *file;

void acode()
{
    sp = 0, bp = 1, pc = 0;
    halt = 0, j = 0, k = 0, counter = 0;
    printf("\n");
    file = fopen("mcode.txt", "r");

    if(file == NULL)
    {
        printf( "mcode.txt not found...\n" );
        exit(1);
    }

    while(fgets(line2, sizeof(line2), file))
    {
        sscanf(line2, "%d %d %d ", &code[counter].op, &code[counter].l, &code[counter].m);               //read input from file
        counter++;
    }
    fclose(file);

    file = fopen("acode.txt","w");

    if(file == NULL)
    {
        printf("Error writing to file acode.txt\n");
        exit(1);
    }

    fprintf(file,"Line\tOP   L   M\n");                                                                 //write the top half of stacktrace

    for(k = 0; k < counter; k++)
    {
        if(k < 10)
        {
            fprintf(file," %d\t\t%s  %d   %d\n", k, OpCodes[code[k].op - 1], code[k].l, code[k].m);
            printf(" %d\t\t%s  %d   %d\n", k, OpCodes[code[k].op - 1], code[k].l, code[k].m);
        }
        else if(k >= 10)
        {
            fprintf(file,"%d\t\t%s  %d   %d\n", k, OpCodes[code[k].op - 1], code[k].l, code[k].m);
            printf("%d\t\t%s  %d   %d\n", k, OpCodes[code[k].op - 1], code[k].l, code[k].m);
        }
    }
    fclose(file);

}

void ex()
{
    printf("\n");

    file = fopen("stacktrace.txt","w");

    if(file == NULL)
    {
        printf("Error writing to file stacktrace.txt\n");
        exit(1);
    }

    fprintf(file, "\n\t\t\t\tpc\tbp\tsp\tstack\n");
    printf( "\n\t\t\t\tpc\tbp\tsp\tstack\n");
    fprintf(file, "Initial values\t%d\t%d\t%d\n", pc, bp, sp);
    printf("Initial values\t%d\t%d\t%d\n", pc, bp, sp);

    counter = 0;
    k = 0;
    int o = 0;                                                                                      //int to keep track of when to skip printing the "|"

    while(bp != 0 && halt == 0)                                                                     //while base pointer isn't 0 and halt isn't 1, fetch, execute and print
    {
        fetch();
        int n = pc - 1;
        if(n < 10)
        {
            fprintf(file, " %d\t%s\t%d\t%d\t", n, OpCodes[inreg.op - 1], inreg.l, inreg.m);
            printf(" %d\t%s\t%d\t%d\t", n, OpCodes[inreg.op - 1], inreg.l, inreg.m);
        }
        else if(n >= 10)
        {
            fprintf(file, "%d\t%s\t%d\t%d\t", n, OpCodes[inreg.op - 1], inreg.l, inreg.m);
            printf("%d\t%s\t%d\t%d\t", n, OpCodes[inreg.op - 1], inreg.l, inreg.m);
        }

        execute();

        if(bp > 1 && counter == 0)                                                                  //if the first time the base pointer is greater than 1
        {
            ar[1] = bp;                                                                             //set the second index of activation record index equal to the base pointer
            counter++;                                                                              //increment counter
        }

        if(bp > 1 && counter == 1 && ar[1] != bp)                                                   //if the second activation record
        {
            ar[2] = bp;                                                                             //set third index of activation record equal to the base pointer
            counter++;
        }

        fprintf(file, "%d\t%d\t%d", pc, bp, sp);
        printf("%d\t%d\t%d", pc, bp, sp);

        if(k == 0 && strcmp(OpCodes[inreg.op - 1],"jmp") == 0)                                      //if instruction is jump, skip line
        {
            k = 1;
            fprintf(file, "\n");
            printf("\n");
            continue;
        }

        fprintf(file, "\t");
        int ars = 0;                                                                                //int to keep track of assigning base pointer to activation record pointer array

        for(j = 1; j <= sp; j++)                                                                    //go through stack until stack pointer and print
        {
            fprintf(file, "%d ", stack[j]);
            printf("%d ", stack[j]);

            if(ar[1] != 0 && bp > 1 && ars == 0 && j == ar[1] - 1)                                  //first case to keep track of and print the "|"
            {
                if (o != 0)
                {
                    fprintf(file, "| ");
                    printf("| ");
                }
                o = 1;
                ars++;
            }

            if(ar[2] != 0 && bp > 1 && sp > bp && ars == 1 && j == ar[2] - 1)                      //second case to keep track of and print the "|"
            {
                if (inreg.op != 2 && inreg.m !=0)
                {
                    fprintf(file, "| ");
                    printf("| ");
                }
                ars++;
            }
        }
        fprintf(file, "\n");
        printf("\n");
    }

    fclose(file);

}

void fetch()
{
	inreg = code[pc];
	pc++;
}

void execute()
{
    switch(inreg.op)
    {
        case 1:                                                     //LIT
            sp++;
            stack[sp] = inreg.m;
            break;
        case 2:                                                     //OPR
            OPR();
            break;
        case 3:                                                     //LOD
            sp++;
            stack[sp] = stack[base(inreg.l,bp) + inreg.m];
            break;
        case 4:                                                     //STO
            stack[base(inreg.l, bp) + inreg.m] = stack[sp];
            sp--;
            break;
        case 5:                                                     //CAL
            stack[sp + 1] = 0;
            stack[sp + 2] = base(inreg.l, bp);
            stack[sp + 3] = bp;
            stack[sp + 4] = pc;
            bp = sp + 1;
            pc = inreg.m;
            break;
        case 6:                                                     //INC
            sp += inreg.m;
            break;
        case 7:                                                     //JMP
            pc = inreg.m;
            break;
        case 8:                                                     //JPC
            if (stack[sp] == 0)
            {
                pc = inreg.m;
            }
            sp--;
            break;
        case 9:                                                     //SIO 0,1
            printf("%d", stack[sp]);
            sp--;
            break;
        case 10:                                                    //SIO 0,2
            sp++;
            break;
        case 11:                                                    //SIO 0,3
            pc = 0;
            bp = 0;
            sp = 0;
            halt = 1;
            break;
      }
}

void OPR()                                                          //ALU Operations
{
    switch(inreg.m)
    {
        case 0:
            sp = bp - 1;
            pc = stack[sp + 4];
            bp = stack[sp + 3];
            break;
        case 1:                                                     //NEG
            stack[sp] = -stack[sp];
            break;
        case 2:                                                     //ADD
            sp--;
            stack[sp] += stack[sp + 1];
            break;
        case 3:                                                     //SUB
            sp--;
            stack[sp] -= stack[sp + 1];
            break;
        case 4:                                                     //MUL
            sp--;
            stack[sp] *= stack[sp + 1];
            break;
        case 5:                                                     //DIV
            sp--;
            stack[sp] /= stack[sp + 1];
            break;
        case 6:                                                     //ODD
            stack[sp] %= 2;
            break;
        case 7:                                                     //MOD
            sp--;
            stack[sp] %= stack[sp + 1];
            break;
        case 8:                                                     //EQL
            sp--;
            stack[sp] = stack[sp] == stack[sp + 1];
            break;
        case 9:                                                     //NEQ
            sp--;
            stack[sp] = stack[sp] != stack[sp + 1];
            break;
        case 10:                                                    //LSS
            sp--;
            stack[sp] = stack[sp] < stack[sp + 1];
            break;
        case 11:                                                    //LEQ
            sp--;
            stack[sp] = stack[sp] <= stack[sp + 1];
            break;
        case 12:                                                    //GTR
            sp--;
            stack[sp] = stack[sp] > stack[sp + 1];
            break;
        case 13:                                                    //GEQ
            sp--;
            stack[sp] = stack[sp] >= stack[sp + 1];
            break;

      }
}

int base(int l, int base)
{
    int b1;
    b1 = base;
    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l--;
    }
    return b1;
}
