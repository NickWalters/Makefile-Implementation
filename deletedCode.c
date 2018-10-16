#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include  <sys/types.h>
#include  <dirent.h>
#include <libgen.h>
#include <limits.h>
#include <unistd.h>



void directorySearch(void){
    //use current working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
        printf("path size: %lu", strlen(cwd));
    } else {
        perror("getcwd() error");
    }

    DIR             *dirp;
    struct dirent   *dp;
    
    dirp = opendir(cwd);
    if(dirp == NULL) {
        perror("error");
    }
    
    while((dp = readdir(dirp)) != NULL) {
        // printf( "%s\n", dp->d_name );
    }
    closedir(dirp);
}









// TODO Remove whitespace from end of fileName
bool fileExists(char * filename){
    // removeSpaces(filename);
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL){
        // printf("    %s not found\n", filename);
        return false;
    }
    else{
        // printf("    %s has been found\n", filename);
        return true;
    }
}




// remove the extra space from the end of a string (this happens to the last dependancy on a line)
char * removeWhiteSpace(char * text){
    if((text[strlen(text)-1] == ' ') || (text[strlen(text)-1] == '\n')){
        char * substring = malloc(sizeof(char) * (strlen(text)-1));
        strncpy(substring, text, strlen(text)-1);
        return substring;
    }
    else{
        return text;
    }
}





// default files stores all the dependancies from the first target line (starting from index 1). index 0 is (target:)
char *defaultFiles[900];
char *rebuildFile[900];

void target(){
    FILE *fp;
    fp = fopen("Bakefile.txt", "r");
    int targetCount = 0;
    
    if(fp != NULL) {
        char line[BUFSIZ];
        
        while( fgets(line, sizeof line, fp) != NULL) {
            for(int i=0; line[i] != '\0'; i++) {
                if ((line[i] == ':') && (targetCount==0)){
                    ++targetCount;
                    char *fileName;
                    fileName = strtok(line, " ");
                    int i = 0;
                    while (fileName != NULL)
                    {
                        fileName = removeWhiteSpace(fileName);
                        // if filename != ':' AND targetCount == 0
                        if(*fileName != ':'){
                            // if file does not exist and is not (target:), but dependancy
                            if(!fileExists(fileName)){
                                defaultFiles[i] = fileName;
                                // printf("defaultFiles[%i]: %s\n", i, defaultFiles[i]);
                                if(i != 0){
                                    rebuildFile[i] = fileName;
                                    printf("rebuildFile[%i]: %s\n", i, rebuildFile[i]);
                                }
                                
                            }
                            // if the file exists, then save it for later
                            else if(fileExists(fileName)){
                                defaultFiles[i] = fileName;
                                // printf("defaultFiles[%i]: %s\n", i, defaultFiles[i]);
                            }
                            i++;
                        }
                        fileName = strtok(NULL, " ");
                    }
                }
                // continue searching the rest of file
                // do something with the default files
                else if((line[i] == ':') && (targetCount >= 1)){
                    ++targetCount;
                    char *fName;
                    fName = strtok(line, " ");
                    // int i = 0;
                    while(fName != NULL){
                        fName = removeWhiteSpace(fName);
                        if(*fName != ':'){
                            // if is target:
                            if(i==0){
                                int j=0;
                                while(rebuildFile[j+1] != NULL){
                                    if(strcmp(rebuildFile[j], fName) == 0){
                                        printf("%s will be rebuilt, run action lines", fName);
                                    }
                                }
                            }
                            //if is dependancy
                            if(i>=1){
                                
                            }
                        }
                    }
                }
            }
        }
    }
}




// if targetCount == 1, then rebuild everything

// otherwise:

// if( !fileExists(token) ) --> rebuild

// if ( fileExists(token) ) -->




int main(int argc, char *argv[]){
    target();
    // testMethod();
    
}


