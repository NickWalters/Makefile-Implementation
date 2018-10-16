/* CITS2002 Project 2018
 Name(s):        Nicholas Walters , James Caldon
 Student number(s):    22243339 ,
 */

/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
//#include <linux/limits.h>
 */

#include "bake.h"

#define INITIAL_CAPACITY 20

int commentNum = 0;
int varNum = 0;




// this function used to create dynamic arrays, to save memory.
void push(int *arr, int index, int value, int *size, int *capacity){
    if(*size > *capacity){
        int *fail = realloc(arr, (*size+1) * sizeof(arr[0])); //dynamically re-size the array, to fit the next added element
        if(fail == NULL || arr == NULL){
            // TREAT THE ERROR
        }
        *capacity = sizeof(arr) * 2;
    }
    arr[index] = value;
    *size = *size + 1;
}






// This function checks if the line is a comment
bool comment(char * ch)
{
    bool finished = false; // loop condition. Could probably use ch != '/0', while loop instead
    int i= 0;
    while(!finished){
        if(isspace(ch[i])){
            ++ch;
            ++i;
        }
        else if (ch[i] == '#')
        {
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}



// DOESNT WORK YET
// this function checks if the following line contains a variable definition '='
bool var(char * ch){
    int i = 0;
    while(*ch != '\0'){
        if(ch[i] == '='){
            ++varNum;
            return true;
        }
        printf("%c", ch[i]);
        ++i;
        ++ch;
    }
    return false;
}






// this returns an int pointer array. each value refers to the line number of a comment
int *commentStrip(FILE * fp){
    int size = 0;
    int capacity = INITIAL_CAPACITY;
    int *commentLines = malloc(INITIAL_CAPACITY * sizeof(int));
    if(commentLines == NULL){
        // TREAT THE ERROR
    }
    // TODO: should figure out a way to dynamically allocate memory, instead of just using 9000 for the array. We need to allocate the exact amount of memory needed.
    char line[BUFSIZ];
    // scan each line of the file
    int lineNum = 0;
    while(fgets(line, sizeof(line), fp) != NULL){
        if(comment(&line[0])){
            push(commentLines, commentNum, lineNum, &size, &capacity);
            ++commentNum;
            ++lineNum;
        }
        else{
            ++lineNum;
        }
    }
    return commentLines;
}





void variableSearch(FILE * fp){
    char line[BUFSIZ];
    printf("%s", "wow");
    // scan each line of the file
    while(fgets(line, sizeof(line), fp) != NULL){
        printf("%s", "wow");
        if(var(&line[0])){
            // DO SOMETHING
            printf("%s", "wow");
            ++varNum;
        }
        else{
            //
        }
    }
}

bool isComment(char * line) {
    for (int i=0; line[i] != '\0'; i++) {
        if (line[i] == '#') return true;
    }
    
    return false;
}
/**
 int ** buckets;
 
 int hashCode(char * string) {
 int hash = 0;
 int strLen = strlen(string);
 
 for(int i=0; i < strLen; i++) {
 hash = 31 * hash + str[i];
 }
 }
 **/

char * keyArray[100];
int nextKey = 0;
char * valArray[100];
int nextVal = 0;
void addVariables(char * key, char * val) {
    //TODO check if array is large enough and for malloc nulls
    keyArray[nextKey] = malloc(strlen(key)*sizeof(char));
    valArray[nextVal] = malloc(strlen(val)*sizeof(char));
    
    //keyArray[nextKey++] = key;
    //valArray[nextVal++] = val;
    strcpy(keyArray[nextKey++], key);
    strcpy(valArray[nextVal++], val);
    
    //printf("%s", keyArray[nextKey-1]);
}

void defineVariables(char * line) {
    int startPotKey = 0;
    int endPotKey = 0;
    
    int equalsPos = 0;
    
    int startPotVal = 0;
    int endOfLine = strlen(line);
    
    for (int i=0; line[i] != '=' && line[i] != '\0'; i++) {
        if (isspace(line[i]) && !isspace(line[i+1]) && line[i+1] != '=') startPotKey = i+1;
        if ((isspace(line[i+1]) || line[i+1] == '=') && !isspace(line[i])) endPotKey = i+1;
        if (line[i+1] == '=') {
            equalsPos = i+1;
            //printf("%i\n", equalsPos);
        }
    }
    if (equalsPos == 0) return;
    
    
    
    for (int i=equalsPos+1; line[i] != '=' && line[i] != '\0'; i++) {
        if (!isspace(line[i]) && startPotVal == 0) {
            startPotVal = i;
            break;
        }
    }
    
    for (int i = endOfLine-1; line[i] != '=' && i >= 0; i--) {
        if (!isspace(line[i-1])) {
            endOfLine = i;
            break;
        }
    }
    
    //printf("Key start: %i\n", startPotKey);
    //printf("Key end: %i\n", endPotKey);
    
    //printf("Value start: %i\n", startPotVal);
    //printf("Value end: %i\n", endOfLine);
    
    int keyLength = endPotKey-startPotKey;
    int valLength = endOfLine-startPotVal;
    char * var = malloc(keyLength * sizeof(char));
    char * val = malloc(valLength * sizeof(char));
    
    
    strncpy(var, line+startPotKey, endPotKey);
    var[keyLength] = '\0';
    strncpy(val, line+startPotVal, endOfLine);
    val[valLength] = '\0';
    
    addVariables(var, val);
    
    // printf("Found variable name:--%s--\n", var);
    //printf("Found value name:--%s--\n", val);
}

void expandVariable(char * line, int l, char * var, int i, int endPos, int keyLength, char * val) {
    //Process Line
    int valueLength = strlen(val);
    //printf("Found this cool thing: %s\n Which should become: %s\n", var, val);
    char * newLine = malloc((l-keyLength+valueLength)*sizeof(char));
    
    strncpy(newLine, line, i);
    strncpy(newLine+i, val, valueLength);
    strncpy(newLine+i+valueLength, line + endPos+1, l - endPos);
    newLine[l- keyLength + valueLength] = '\0';
    //printf("Old line: %s\n New line: %s\n", line, newLine);
    
    strcpy(line, newLine);
    free(newLine);
}

void expandVariables(char * line) {
    for (int i=0; line[i] != '\0'; i++) {
        
        if (line[i] == '$') {
            int l = strlen(line);
            int endPos = 0;
            for (int j = i+1; line[j] != '\0'; j++) {
                if (line[j] == ')') {
                    endPos = j;
                    break;
                }
            }
            int varLength = (endPos-i-2);
            char * var = malloc((varLength+1)*sizeof(char));
            strncpy(var, line + i+2, varLength);
            var[varLength] = '\0';
            bool found = false;
            for (int k = 0; k < nextKey; k++) {
                if (strcmp(keyArray[k], var) == 0) {
                    expandVariable(line, l, var, i, endPos, strlen(keyArray[k]), valArray[k]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (getenv(var) != NULL) {
                    expandVariable(line, l, var, i, endPos, endPos-i, getenv(var));
                    found = true;
                }
            }
            if (!found) {
                expandVariable(line, l, var, i, endPos, endPos-i, "");
                found = true;
            }
            free(var);
        }
    }
}

void addDefaultVariables(char * key, int value) {
    char * val = malloc(sizeof(value));
    sprintf(val, "%i", value);
    addVariables(key, val);
    free(val);
}
char ** lines;
int numComments;
int numLines = 0;

char *options[6];

struct Options {
    char * dir;
    char * filename;
    bool ignore;
    bool fakeout;
    bool print;
    bool silent;
};

int main(int argc, char *argv[])
{
    //  ATTEMPT TO OPEN AND READ FROM PROVIDED FILENAME
    //char * testString = " VARIABLE = man curl";
    // defineVariables(testString);
    //exit(0);
    struct Options options;
    int option;
    while ((option = getopt(argc, argv, "c:f:inps")) != -1)
    {
        switch (option)
        {
            case 'c':
                if (strlen(optarg) == 0) exit(1);
                options.dir = malloc(strlen(optarg)*sizeof(char));
                strcpy(options.dir, optarg);
                chdir(options.dir);
                break;
            case 'f':
                if (strlen(optarg) == 0) exit(1);
                options.filename = malloc(strlen(optarg)*sizeof(char));
                strcpy(options.filename, optarg);
                break;
            case 'i':
                options.ignore = true;
                break;
            case 'n':
                options.fakeout = true;
                break;
            case 'p':
                options.print = true;
                break;
            case 's':
                options.silent = true;
                break;
        }
    }
    
    FILE * fp = fopen("bakefile.txt", "r");
    if(fp == NULL) {
        fp = fopen("Bakefile.txt", "r");
    }
    if (options.filename != NULL) {
        fp = fopen(options.filename, "r");
    }
    if(fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        int sz = ftell(fp);
        rewind(fp);
        
        char line[BUFSIZ];
        
        lines = (char **) malloc(sz*sizeof(*lines));
        addDefaultVariables("PID",  getpid());
        addDefaultVariables("PPID", getppid());
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        addVariables("PWD",  cwd);
        addDefaultVariables("RAND", rand());
        
        char bufferedLine[BUFSIZ];
        int bufferedPos = 0;
        bool buffered = false;
        
        int numLines = 0;
        while(fgets(line, sz, fp) != NULL){
            
            int l = strlen(line);
            if (line[l-2] == '\\') {
                strncpy(bufferedLine + bufferedPos, line, l-2);
                bufferedPos += l-2;
                bufferedLine[bufferedPos] = '\0';
                buffered = true;
                continue;
            } else if (buffered) {
                strncpy(bufferedLine + bufferedPos, line, l);
                bufferedPos += l;
                bufferedLine[bufferedPos] = '\0';
                strcpy(line, bufferedLine);
                bufferedLine[0] = '\0';
                buffered = false;
            }
            
            l = strlen(line);
            char * debug = malloc((l+1)*sizeof(*debug));
            
            strncpy(debug, line, l);
            
            debug[l] = '\0';
            
            //printf("___%s___\n", debug);
            
            if (isComment(line)) {numComments++; continue;}
            expandVariables(line);
            defineVariables(line);
            
            l = strlen(line);
            printf("Length: %i |%s", l, line);
            lines[numLines] = malloc(l*sizeof(char));
            strncpy(lines[numLines++], line, l+1);
            
        }
        for (int i = 0; i < numLines; i++) {
            printf("Length: %lu |%s", strlen(lines[i]), lines[i]);
        }
        printf("\n");
        //
        fclose(fp);
    }
    //  NOT PRESENT IN DIRECTORY, SO TRY PRESENT WORKING DIRECTORY
    else {
        /**
         char cwd[PATH_MAX];
         if (getcwd(cwd, sizeof(cwd)) != NULL) {
         printf("Current working dir: %s\n", cwd);
         }
         **/
    }
}







// if the line has a " = " in it, then consider the line to be a variable declaration.


// if the line has a " target: " in it, then treat dependancies


// the line after the target line is an action.
