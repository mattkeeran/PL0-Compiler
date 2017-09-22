#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void clean();
void symbology();

char symbols[] = {'+', '-', '*', '/', '=', '(', ')', ',', ';', '.'};
int symnum[] = {4, 5, 6, 7, 9, 15, 16, 17, 18, 19};
char* reserved[] = {"odd", "begin", "end", "if", "then", "while", "do", "call", "const", "var", "procedure", "write", "read", "else"};
int resnum[] = {8, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};

void clean()
{
    FILE* in = fopen("input.txt", "r");
    FILE* clean = fopen("cleaninput.txt", "w");

    if (in == NULL)
    {
        printf("Error opening input.txt\n");
        exit(1);
    }

    char c, d;
    while((c = fgetc(in)) != EOF)
    {


        if(c == '/')
        {
            d = fgetc(in);
            if(d == '*')
            {
                com:
                while((d = fgetc(in)) != '*')
                {
                }
                d = fgetc(in);
                if(d == '/')
                {
                    continue;
                }
                else
                {
                    goto com;
                }
            }
            else
            {
                fprintf(clean, "%c", c);
                fprintf(clean, "%c", d);
                continue;
            }
        }
        fprintf(clean, "%c", c);
    }

    fclose(in);
    fclose(clean);
    //printf("Successfully wrote cleaninput.txt\n");
}

void symbology()
{
    FILE* clean = fopen("cleaninput.txt", "r");
    FILE* lex = fopen("lexemetable.txt", "w");
    FILE* tok = fopen("tokenlist.txt", "w");
    fprintf(lex, "lexeme\t\ttoken type\n");
    printf("\n");
    if (clean == NULL)
    {
        printf("Error opening cleaninput.txt\n");
        exit(1);
    }
    int i, j, k;
    j = sizeof(symbols);
    int flag = 0;                                                   //flag to save lookahead char and start at top of loop
    int line = 1;                                                   //to keep track of line number for error reporting
    char c, d;                                                      //c - current char, d - lookahead char
    while((c = fgetc(clean)) != EOF)                                //while the next char is not the end of file char, loop
    {
        loop:
        for(i = 0; i < j; i++)                                      //check char against known symbols
        {
            if(c == symbols[i])                                     //store numerical value of symbol in array?
            {
                fprintf(lex, "%c\t\t%d\n", c, symnum[i]);

                fprintf(tok, "%d ", symnum[i]);
                printf("%d ", symnum[i]);
            }
        }
        if(c == ':')                                         // :=
        {
            if((d = fgetc(clean)) == '=')
            {
                fprintf(lex, "%c%c\t\t20\n", c, d);

                fprintf(tok,"20 ");
                printf("20 ");
            }
            else
            {
                c = d;
                goto loop;
            }
        }
        else if(c == '<')
        {
            if((d = fgetc(clean)) == '>')                    // <>
            {
                fprintf(lex, "%c%c\t\t10\n", c, d);

                fprintf(tok,"10 ");
                printf("10 ");
            }
            else if(d == '=')                                // <=
            {
                fprintf(lex, "%c%c\t\t12\n", c, d);

                fprintf(tok,"12 ");
                printf("12 ");
            }
            else                                             // <
            {
                fprintf(lex, "%c\t\t11\n", c);

                fprintf(tok,"11 ");
                printf("11 ");
                c = d;
                goto loop;
            }

        }
        else if(c == '>')
        {
            if((d = fgetc(clean)) == '=')                    // >=
            {
                fprintf(lex, "%c%c\t\t14\n", c, d);

                fprintf(tok,"14 ");
                printf("14 ");
            }
            else                                             // >
            {
                fprintf(lex, "%c\t\t13\n", c);

                fprintf(tok,"13 ");
                printf("13 ");
                c = d;
                goto loop;
            }
        }

        if(c == ' ')                                                                            //don't print whitespace to lexemetable
        {
            continue;
        }
        else if(isalpha(c))                                                                     //if c is a letter
        {
            char word[10];                                                                      //array to store word
            word[0] = c;
            int counter = 1;
            d = fgetc(clean);
            while(isdigit(d) || isalpha(d))                                                     //loop until next char not a digit or letter
            {
                word[counter] = d;
                counter++;
                if(counter > 11)                                                                //if length of string > 11 char, print error & halt
                {
                    printf("String exceeds acceptable length on line %d\n", line);
                    exit(1);
                }
                d = fgetc(clean);
                flag = 1;
            }
            if(counter == 3 && word[0] == 'v' && word[1] == 'a' && word[2] == 'r')              //if variable declaration
            {
                d = fgetc(clean);                                                               //set d = to next char
                if(isdigit(d))                                                                  //check to see if it starts with a number
                {
                    printf("Invalid variable name on line %d\n", line);                         //if so, print error and halt
                    exit(1);
                }
                else
                {
                    flag = 1;                                                                   //if it doesn't set flag to save char
                }
            }
            //Reserved Words
            for(k = 0; k < counter; k++)
            {
                fprintf(lex, "%c", word[k]);                                                   //print word char by char to file
            }
            if(counter == 3 && word[0] == 'o' && word[1] == 'd' && word[2] == 'd')
            {
                fprintf(lex,"\t\t8\n");

                fprintf(tok,"8 ");
                printf("8 ");
            }
            else if(counter == 5 && word[0] == 'b' && word[1] == 'e' && word[2] == 'g' && word[3]== 'i' && word[4] == 'n')
            {
                fprintf(lex,"\t\t21\n");

                fprintf(tok,"21 ");
                printf("21 ");
            }
            else if(counter == 3 && word[0] == 'e' && word[1] == 'n' && word[2] == 'd')
            {
                fprintf(lex,"\t\t22\n");

                fprintf(tok,"22 ");
                printf("22 ");
            }
            else if(counter == 2 && word[0] == 'i' && word[1] == 'f')
            {
                fprintf(lex,"\t\t23\n");

                fprintf(tok,"23 ");
                printf("23 ");
            }
            else if(counter == 4 && word[0] == 't' && word[1] == 'h' && word[2] == 'e' && word[3] == 'n')
            {
                fprintf(lex,"\t\t24\n");

                fprintf(tok,"24 ");
                printf("24 ");
            }
            else if(counter == 5 && word[0] == 'w' && word[1] == 'h' && word[2] == 'i' && word[3] == 'l' && word[4] == 'e')
            {
                fprintf(lex,"\t\t25\n");

                fprintf(tok,"25 ");
                printf("25 ");
            }
            else if(counter == 2 && word[0] == 'd' && word[1] == 'o')
            {
                fprintf(lex,"\t\t26\n");

                fprintf(tok,"26 ");
                printf("26 ");
            }
            else if(counter == 4 && word[0] == 'c' && word[1] == 'a' && word[2] == 'l' && word[3] == 'l')
            {
                fprintf(lex,"\t\t27\n");

                fprintf(tok,"27 ");
                printf("27 ");
            }
            else if(counter == 5 && word[0] == 'c' && word[1] == 'o' && word[2] == 'n' && word[3] == 's' && word[4] == 't')
            {
                fprintf(lex,"\t\t28\n");

                fprintf(tok,"28 ");
                printf("28 ");
            }
            else if(counter == 3 && word[0] == 'v' && word[1] == 'a' && word[2] == 'r')
            {
                fprintf(lex,"\t\t29\n");

                fprintf(tok,"29 ");
                printf("29 ");
            }
            else if(counter == 9 && word[0] == 'p' && word[1] == 'r' && word[2] == 'o' && word[3] == 'c' && word[4] == 'e' && word[5] == 'd'
                    && word[6] == 'u' && word[7] == 'r' && word[8] == 'e')
            {
                fprintf(lex,"\t\t30\n");

                fprintf(tok,"30 ");
                printf("30 ");
            }
            else if(counter == 5 && word[0] == 'w' && word[1] == 'r' && word[2] == 'i' && word[3] == 't' && word[4] == 'e')
            {
                fprintf(lex,"\t\t31\n");

                fprintf(tok,"31 ");
                printf("31 ");
            }
            else if(counter == 4 && word[0] == 'r' && word[1] == 'e' && word[2] == 'a' && word[3] == 'd')
            {
                fprintf(lex,"\t\t32\n");

                fprintf(tok,"32 ");
                printf("32 ");
            }
            else if(counter == 4 && word[0] == 'e' && word[1] == 'l' && word[2] == 's' && word[3] == 'e')
            {
                fprintf(lex,"\t\t33\n");

                fprintf(tok,"33 ");
                printf("33 ");
            }
            else
            {
                fprintf(lex,"\t\t2\n");

                fprintf(tok,"2 ");
                printf("2 ");

                for(k = 0; k < counter; k++)
                {
                    fprintf(tok, "%c", word[k]);                                                   //print word char by char to file
                    printf("%c", word[k]);
                }

                fprintf(tok, " ");
                printf(" ");
            }

            if(flag == 1)                                                                      //set c equal to d and loop with lookahead char
            {
                c = d;
                goto loop;
            }
        }
        else if(isdigit(c))                                                                   //if char is a digit
        {
            char numb[4];                                                                     //array for number
            numb[0] = c;
            int counter = 1;
            d = fgetc(clean);

            while(isdigit(d))                                                                 //while subsequent chars are digits
            {
                numb[counter] = d;                                                            //add to array
                counter++;

                if(counter > 5)                                                               //if larger than 5 digits, print error & halt
                {
                    printf("Number exceeds acceptable size on line %d\n", line);
                    exit(1);
                }
                d = fgetc(clean);
                flag = 1;                                                                      //if next char isn't a digit, set flag to start loop with lookahead char
            }
            fprintf(tok,"3 ");
            printf("3 ");

            for(k = 0; k < counter; k++)
            {
                fprintf(lex, "%c", numb[k]);                                                   //print number digit by digit to file
                fprintf(tok, "%c", numb[k]);
                printf("%c", numb[k]);
            }

            fprintf(tok, " ");
            printf(" ");

            if(counter < 5)                                                                    //formatting
            {
                fprintf(lex, "\t\t3\n");
            }
            if(counter == 5)                                                                   //formatting 2, the sequel
            {
                fprintf(lex, "\t3\n");
            }

            c = d;
            goto loop;

        }
        else if(c == '\n')
        {
            line++;                                                                           //increment line count for error reference
        }
        else if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '_' || c == '~'
                  || c == '{' || c == '}' || c == '[' || c == ']' || c == '|' || c == '\\' || c == '\''  || c == '?' || c == '`')
        {
            printf("Invalid character on line %d\n", line);
            exit(1);
        }

    }
    fclose(clean);
    fclose(lex);
    fclose(tok);
    //printf("Successfully wrote lexemetable.txt\n");
    printf("\nSuccessfully wrote tokenlist.txt\n");
}
