
/* CITS2002 Project 2018
 Name(s):        Nicholas Walters , James Caldon
 Student number(s):    22243339 ,
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>


void findwords(FILE *fp)
{
    char   line[BUFSIZ];
    
    //  READ EACH LINE OF THE FILE
    while(fgets(line, sizeof line, fp) != NULL) {
        char    *ch = line;
        
        //  CONSIDER EACH CHARACTER OF THE LINE, LOOKING FOR 'WORDS'
        while(*ch != '\0') {
            char   word[BUFSIZ];
            char   *w  = word;
            *w = *w +1;
            // if the line has a " = " in it, then consider the line to be a variable declaration.
            while(isalpha(*ch)) {
                // scan each character of the line. check if its alphanumeric/letter
                *w++ = *ch++;
            }
            *w  = '\0';
            
            if(w != word) {
                //addToList(word);
            }
            ++ch;
            
            // if the line has a " target: " in it, then treat dependancies
            
            
            
            // the line after the target line is an action.
        }
    }
}



bool comment(char * ch)
{
    char hashtag = '#';
    
    if (ch[0] == hashtag)
    {
        return true;
    }
    else{
        return false;
    }
}




int main(int argc, char *argv[])
{
    //  ATTEMPT TO OPEN AND READ FROM PROVIDED FILENAME
    if(fopen("Bakefile.txt", "r") != NULL) {
        FILE *fp = fopen("Bakefile", "r");
        findwords(fp);
        fclose(fp);
    }
    //  NOT PRESENT IN DIRECTORY, SO TRY PRESENT WORKING DIRECTORY
    else {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Current working dir: %s\n", cwd);
        }
    }
}






