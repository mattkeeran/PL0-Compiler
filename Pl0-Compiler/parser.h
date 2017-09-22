#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run();
void gen();
void getToken();
void program();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
void symlook1();
void symlook2();

int left[500]; //left column of instructions
int mid[500];  //mid column of instructions
int right[500];//right column of instructions
int b = 0, c = 0, d = 0, i = 0, z = 0;//indecies

int ifstart[15];//start locations of if statements
int whilestart[15];//start locations of while loops

char line[2000];//char array to store tokenlist
int temp = 0;   //int val of token
char *word;     //pointer to token
int op = 0;     //op code
int level = 0;  //level counter
int instr = 0;  //instruction counter
int prev = 0;   //temp var for correct order of ops
int v = 0;      //var counter
char *nam[15];  //symbol names
int type[15];   //symbol types
int lvl[15];    //symbol levels
int val[15];    //symbol values
int lv = 0;     //level temp var
int vl = 0;     //value temp var
int lhl = 0;    //left hand level
int lhv = 0;    //left hand value
int f1 = 0;     //else flag
int f2 = 0;     //end of proc flag
int f3 = 0;     //const flag

FILE* symtable;                                                             //file for symbol table

void run()
{
    b = 0, c = 0, d = 0, i = 0, z = 0;
    op = 0, level = 0, instr = 0, prev = 0, v = 0;
    lv = 0, vl = 0, lhl = 0, lhv = 0, f1 = 0, f2 = 0, f3 = 0;

    FILE* in = fopen("tokenlist.txt", "r");                                 //input
    symtable = fopen("symlist.txt", "w");                                   //symbol table

    fprintf(symtable, "Name\tType\tLevel\tValue\n");                        //print header

    if(in == NULL)
    {
        printf("Error opening tokenlist.txt\n");
        exit(1);
    }

    while(1)
    {
        if (fgets(line, 2000, in) == NULL) break;                           //store token list in array
    }

    word = strtok(line, " ");                                               //get the first token from the array
    temp = atoi(word);                                                      //get the integer value of the token

    program();

    fclose(in);
    fclose(symtable);



}

void gen()
{
    printf("\nbegin gen\n\n");
    FILE* mcode= fopen("mcode.txt", "w");                                   //print machine code to .txt
    int k;
    for(k = 0; k < instr; k++)
    {
        fprintf(mcode, "%d %d %d\n", left[k], mid[k], right[k]);
        printf("%d %d %d\n", left[k], mid[k], right[k]);
    }
    fclose(mcode);
}

void getToken()
{
    word = strtok(NULL, " ");
    temp = atoi(word);
}

void program()
{
    printf("\n");
    left[instr] = 7, mid[instr] = 0, right[instr] = 0;
    instr++;
    left[instr] = 7, mid[instr] = 0, right[instr] = 2;
    instr++;
    block();
    if(temp != 19)
    {
        printf("Period expected.\n");
    }
    printf("\nParser logic defeated!\n");
}

void block()
{
    if(temp == 28)                                                           //if constsym
    {
        do                                                                  //do while commasym
        {
            getToken();
            if(temp != 2)                                                   //if not identsym
            {
                printf("const, var, procedure must be followed by identifier.\n");
                exit(EXIT_FAILURE);
            }
            getToken();
            fprintf(symtable, "%s\t\tconst\t%d\t\t", word, level);          //print to symbol table
            printf( "%s\t\tconst\t%d\t\t", word, level);
            nam[i] = word;                                                  //save the symbol name in array
            type[i] = 1;                                                    //save the symbol type in array
            lvl[i] = level;                                                 //save the symbol lvl in array
            getToken();                                                     //get next token
            if(temp != 9)                                                   //if not eqsym
            {
                printf("= expected.\n");
                exit(EXIT_FAILURE);
            }
            getToken();
            if(temp != 3)                                                   //if not numbersym
            {
                printf("number expected.\n");
                exit(EXIT_FAILURE);
            }
            getToken();
            fprintf(symtable, "%d\n", temp);                                //print value of symbol
            printf("%d\n", temp);
            val[i] = temp;                                                  //save value of symbol
            i++;                                                            //increment i
            getToken();
        }while(temp == 17);
        if(temp != 18)                                                      //if semi colon
        {
            printf("Semicolon or comma missing.\n");
            exit(EXIT_FAILURE);
        }
        getToken();
    }
    if(temp == 29)                                                          //if VAR
    {
        int offset = 4;
        do
        {
            getToken();

            if(temp != 2)                                                   //if not identsym
            {
                printf("const, var, procedure must be followed by identifier.\n");
                exit(EXIT_FAILURE);
            }
            getToken();
            fprintf(symtable, "%s\t\tvar\t\t%d\t\t%d\n", word, level, offset);
            printf("%s\t\tvar\t\t%d\t\t%d\n", word, level, offset);
            offset++;
            if(level == 0)
            {
                v++;
            }
            nam[i] = word;
            type[i] = 2;
            lvl[i] = level;
            val[i] = offset - 1;
            i++;
            getToken();
        }while(temp == 17);                                                  //while commasym
        if(temp != 18)                                                       //if semi colon
        {
            printf("Semicolon or comma missing.\n");
            exit(EXIT_FAILURE);
        }
        getToken();

        if(level > 0)
        {
            left[instr] = 6, mid[instr] = 0, right[instr] = offset;
            instr++;
        }
    }
    while(temp == 30)                                                       //while PROCEDURE
    {
        getToken();
        if(temp != 2)                                                       //if not identsym
        {
            printf("const, var, procedure must be followed by identifier.\n");
            exit(EXIT_FAILURE);
        }
        getToken();
        fprintf(symtable, "%s\tproc\t%d\t\t1\n", word, level);
        printf("%s\tproc\t%d\t\t1\n", word, level);
        getToken();

        if(temp != 18)                                                      //if semi colon
        {
            printf("Semicolon or comma missing.\n");
            exit(EXIT_FAILURE);
        }
        getToken();

        level++;
        block();
        if(temp != 18)                                                      //if semi colon
        {
            printf("Semicolon or comma missing.\n");
            exit(EXIT_FAILURE);
        }
        getToken();
    }
    statement();
    if(temp == 18)
    {
        right[0] = instr + 1;
        left[instr] = 2, mid[instr] = 0, right[instr] = 0;
        instr++;

        left[instr] = 6, mid[instr] = 0, right[instr] = v + 4;
        instr++;
        f2 = 1;
    }
}

void statement()
{
    if(temp == 2)                                                           //if identsym
    {
        op = 0;
        getToken();                                                         //var name
        symlook2();
        getToken();                                                         //should be equals
        if(temp != 20)
        {
            printf("Identifier must be followed by :=.\n");
            exit(EXIT_FAILURE);
        }
        getToken();
        expression();

        left[instr] = 4, mid[instr] = lhl, right[instr] = lhv;
        instr++;
    }
    if(temp == 27)                                                          //if callsym
    {
        getToken();
        if(temp != 2)                                                       //if not identsym
        {
            printf("call must be followed by an identifier.\n");
            exit(EXIT_FAILURE);
        }
        if(f2 == 0)
        {
            left[instr] = 5, mid[instr] = 1, right[instr] = 1;
        }
        if(f2 == 1)
        {
            left[instr] = 5, mid[instr] = 0, right[instr] = 1;
        }
        instr++;
        getToken();                                                         //call name
        getToken();                                                         //semi colon
    }
    if(temp == 21)                                                          //if begin
    {
        f1 = 0;
        getToken();
        statement();
        while(temp == 18)                                                   //while semi colon
        {
            getToken();
            statement();
        }
        if(temp != 22)                                                      //if not end
        {
            printf("end expected.\n");
            exit(EXIT_FAILURE);
        }
        getToken();

        if(temp == 19)                                                      //if period
        {
            left[instr] = 11, mid[instr] = 0, right[instr] = 3;
            instr++;
        }
        if(temp == 33)                                                      //if else
        {
            getToken();
            f1 = 1;
        }

    }
    if(temp == 23)                                                           //if ifsym
    {
        op = 0;
        getToken();
        condition();
        if(op != 0)
        {
            left[instr] = 2, mid[instr] = 0, right[instr] = op;
            instr++;
            ifstart[z] = instr;                                             //save instruction w/ jump address
            z++;
            left[instr] = 8, mid[instr] = 0, right[instr] = 0;
            instr++;
        }

        if(temp != 24)                                                      //if not thensym
        {
            printf("then expected.\n");
            exit(EXIT_FAILURE);
        }
        getToken();
        statement();

        left[instr] = 7, mid[instr] = 0, right[ifstart[z - 1]] = instr + 1;

        if(z > 1)
        {
            right[ifstart[z - 2]] = instr + 1;
            z--;
        }

        if(f1 == 1)
        {
            right[instr] = instr + 3;
            c++;
        }
        if(f1 == 0)
        {
            right[instr] = instr + 1;
            c++;
        }
        instr++;

        if(f1 != 0)
        {
            statement();
        }
    }
    if(temp == 25)                                                          //if while
    {
        op = 0;
        getToken();
        condition();
        if(op != 0)
        {
            left[instr] = 2, mid[instr] = 0, right[instr] = op;
            instr++;
            whilestart[b] = instr; b++;
            left[instr] = 8, mid[instr] = 0, right[instr] = 0;
            instr++;
        }
        if(temp != 26)                                                      //if not dosym
        {
            printf("do expected.\n");
            exit(EXIT_FAILURE);
        }
        getToken();
        statement();

        left[instr] = 7, mid[instr] = 0, right[instr] = whilestart[d] - 3;
        right[whilestart[d]] = instr + 1;
        d++;
        instr++;
    }
}

void condition()
{
    if(temp == 8)                                                          //if oddsym
    {
        op = 6;
        getToken();
        expression();
    }
    else
    {
        expression();
        if(temp != 9 && temp != 10 && temp != 11 && temp != 12 && temp != 13 && temp != 14)                                                        //if not a relation, hardcode?
        {
            printf("Relational operator expected.\n");
            exit(EXIT_FAILURE);
        }
        if(temp == 9)
        {
            op = 8;
        }
        if(temp == 10)
        {
            op = 9;
        }
        if(temp == 11)
        {
            op = 11;
        }
        if(temp == 12)
        {
            op = 10;
        }
        if(temp == 13)
        {
            op = 12;
        }
        if(temp == 14)
        {
            op = 13;
        }
        getToken();
        expression();
    }
}

void expression()
{
    prev = 0;
    if(temp == 4 || temp == 5)              //if plus or minus
    {
        getToken();
    }
    term();
    while(temp == 4 || temp == 5)           //while plus or minus
    {
        if(temp == 4)
        {
            prev = 2;
        }
        if(temp == 5)
        {
            prev = 3;
        }
        getToken();
        term();
        if(prev == 2 || prev == 3)
        {
            left[instr] = 2, mid[instr] = 0, right[instr] = prev;
            instr++;
        }
    }
}

void term()
{
    factor();
    while(temp == 6 || temp == 7)           //while asterisk or divide
    {
        if(temp == 6)
        {
            op = 4;
        }
        if(temp == 7)
        {
            op = 5;
        }
        getToken();
        factor();
        if(op == 4 || op == 5)
        {
            left[instr] = 2, mid[instr] = 0, right[instr] = op;
            instr++;
        }
    }
}

void factor()
{
    if(temp == 2)                           //if identsym
    {
        f3 = 0;
        getToken();
        symlook1();
        if(f3 == 0)
        {
            left[instr] = 3, mid[instr] = lv, right[instr] = vl;

        }
        if(f3 == 1)
        {
            left[instr] = 1, mid[instr] = lv, right[instr] = vl;
        }
        instr++;
        getToken();
    }
    else if(temp == 3)                      //if number
    {
        getToken();
        left[instr] = 1, mid[instr] = 0, right[instr] = temp;
        instr++;
        getToken();
    }
    else if(temp == 15)                     //if left paren
    {
        getToken();
        expression();
        if(temp != 16)                      //if not right paren
        {
            printf("Right parenthesis missing.\n");
            exit(EXIT_FAILURE);
        }
        getToken();
    }
    else
    {
        printf("The preceding factor cannot begin with this symbol.\n");//* correct error?
        exit(EXIT_FAILURE);
    }
}

void symlook1()
{
    int p = 0;
    for(p = 0; p < i; p++)
    {
        if(strcmp(word,nam[p]) == 0)
        {
            if(type[p] == 1)
            {
                f3 = 1;
            }
            if(lvl[p] == 0 && f2 == 0)
            {
                lv = 1;
            }
            if(lvl[p] != 0 && f2 == 0)
            {
                lv = lvl[p] - 1;
            }
            if(f2 == 1)
            {
                lhl = 0;
            }
            vl = val[p];
        }
    }
}

void symlook2()
{
    int p = 0;
    for(p = 0; p < i; p++)
    {
        if(strcmp(word,nam[p]) == 0)
        {
            if(type[p] == 1)
            {
                f3 = 1;
            }
            if(lvl[p] == 0 && f2 == 0)
            {
                lhl = 1;
            }
            if(lvl[p] != 0 && f2 == 0)
            {
                lhl = lvl[p] - 1;
            }
            if(f2 == 1)
            {
                lhl = 0;
            }
            lhv = val[p];
        }
    }
}

