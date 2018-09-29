
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
        FILE *fp = fopen("Bakefile.txt", "r");
        
        char   line[BUFSIZ];
        //  READ EACH LINE OF THE FILE
        while(fgets(line, sizeof line, fp) != NULL){
            printf("%s",line);
        }
        // if the line has a " = " in it, then consider the line to be a variable declaration.
        
        
        // if the line has a " target: " in it, then treat dependancies
        
        
        
        // the line after the target line is an action.
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






