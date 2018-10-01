
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



// This function checks if the line is a comment
bool comment(char * ch)
{
    bool finished = false // loop condition
    int i= 0;
    while(!finished){
        if(isspace(ch[i])){
            ch++;
            i++;
        }
        else if (ch[0] == '#')
        {
            return true;
        }
        else{
            return false;
        }
    }
}

// this function disgregards all comments in the file
void commentStrip(FILE * fp){
    char buffer[MAX_SIZE];
    FILE * bakeOpen = fopen("Bakefile.txt", "a+");
    bool end = false;
    while(!end){
        while(fgets(buffer, sizeof(buffer), fp) != NULL){
            if(comment(buffer[0])){
                fprintf(bakeOpen, "%", buffer);
            }
            else{
                continue;
            }
        }
        end = true; // break
    }
}




int main(int argc, char *argv[])
{
    int commentNum = 0;
    //  ATTEMPT TO OPEN AND READ FROM PROVIDED FILENAME
    if((fopen("Bakefile.txt", "r") != NULL)) {
        FILE *fp = fopen("Bakefile.txt", "r");
        char   line[BUFSIZ];
        //  READ EACH LINE OF THE FILE
        while(fgets(line, sizeof line, fp) != NULL){
            printf("%s",line);
            // my plan is to strip all the comments from the file, so it doesnt interfere with wordScanner()
            if(comment(line)){
                commentNum++;
            }
            char *ch = line;
            wordScanner(ch);
            
        }
        printf("\n\n");
        printf("---------The number of comments is: %i ----------\n", commentNum); // this is for testing purposes
        
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






