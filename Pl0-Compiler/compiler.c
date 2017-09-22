#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "parser.h"
#include "pm.h"

void scan();
void parse();
void run();

int main()
{
    printf("Pm0 Compiler\nCOP 3402 Fall 2015\nWritten by Matthew Keeran\nNarrated by Your Roommate's Cat\n");
    int halt = 0;
    char cmd[50];
    do
    {
      /*-t The token list
        -s The symbol table
        -m The machine code
        -a The disassembled code
        -v The virtual machine execution stack trace*/

        printf("\n   Menu Options\npress a key, then hit enter\nt\ttoken list\ns\tsymbol table\nm\tmachine code\na\tdisassembled code\nv\tstack trace\ne\texit\n\n");
        fgets(cmd, sizeof cmd, stdin);

        if(strcmp(cmd, "t\n") == 0)
        {   //scan
            clean();
            symbology();
        }
        else if(strcmp(cmd, "s\n") == 0)
        {
            clean();
            symbology();
            run();
        }
        else if(strcmp(cmd, "m\n") == 0)
        {
            clean();
            symbology();
            run();
            gen();
        }
        else if(strcmp(cmd, "a\n") == 0)
        {
            clean();
            symbology();
            run();
            gen();
            acode();
        }
        else if(strcmp(cmd, "v\n") == 0)
        {
            clean();
            symbology();
            run();
            gen();
            acode();
            ex();
        }
        else if(strcmp(cmd, "e\n") == 0)
        {
            break;
        }
        else
        {
            printf("\nCommand not recognized, please try again\n");
        }


    }while(halt == 0);



    return 0;
}

