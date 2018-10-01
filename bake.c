
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


int commentNum = 0;
int *commentLines = NULL;


// This function checks if the line is a comment
bool comment(char * ch)
{
    bool finished = false; // loop condition
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




// this function checks if the following line contains a variable definition '='
bool variable(char * ch){
    
}






// this function disgregards all comments in the file
void commentStrip(FILE * fp){
    // TODO: should figure out a way to dynamically allocate memory, instead of just using 9000 for the array. We need to allocate the exact amount of memory needed.
    char line[9000];
    FILE * bakeOpen = fopen("Bakefile.txt", "a");
    // scan each line of the file
    int lineNum = 0;
    while(fgets(line, sizeof(line), fp) != NULL){
        if(comment(&line[0])){
            commentLines = realloc(commentLines,(commentNum+1) * sizeof(commentLines[0]));
            commentLines[commentNum] = lineNum;
            ++commentNum;
        }
        else{
            continue;
        }
    }
}



void variableSearch(FILE * fp){
    char line[9000];
    FILE * bakeOpen = fopen("Bakefile.txt", "a");
    // scan each line of the file
    while(fgets(line, sizeof(line), fp) != NULL){
        if(variable(&line[0])){
            
        }
        else{
            //
        }
    }
}



int main(int argc, char *argv[])
{
    //  ATTEMPT TO OPEN AND READ FROM PROVIDED FILENAME
    if((fopen("Bakefile.txt", "r") != NULL)) {
        FILE * fp = fopen("Bakefile.txt", "r");
        commentStrip(fp);
        
        printf("\n\n");
        int length = sizeof(commentLines)/sizeof(int);
        for(int i=0; i<length; i++){
            printf("Comment on Line: %x\n", *commentLines);
            ++commentLines;
        }
        printf("---------The number of comments is: %i ----------\n", commentNum); // this is for testing purposes
        
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







// if the line has a " = " in it, then consider the line to be a variable declaration.


// if the line has a " target: " in it, then treat dependancies


// the line after the target line is an action.





