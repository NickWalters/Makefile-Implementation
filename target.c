/*
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/param.h>
#include  <dirent.h>
#include  <time.h>
#include <unistd.h>
#include <limits.h>
 */
#include "bake.h"




char * pathAppend(char *fileName){
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd() error, cannot find current directory");
    }
    char * newPath = malloc(sizeof(cwd) + (sizeof(fileName) + (sizeof(char) * 2)));
    strcat(newPath, cwd);
    strcat(newPath, "/");
    strcat(newPath, fileName);
    // printf("%s\n", newPath);
    return newPath;
}



time_t getFileCreationTime(char *path) {
    struct stat stat_info;
    stat(path, &stat_info);
    
    struct tm *modDate = malloc(sizeof(localtime(&stat_info.st_mtime)));
    modDate = localtime(&stat_info.st_mtime);
    time_t var = mktime(modDate);
    return var;
}




bool fileExists(char * filename){
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL){
        return false;
    }
    else{
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





/*
 files stores all the dependancies from the target lines (starting from index 1). index 0 is (target:)
 
 rebuildFile, stores all the files needing to be rebuilt
 
 if index 0 has a ":" character on the end, then this target has no dependancies
 
 */

char *files[300][300];
char *rebuildFile[300];
int rebuildCount = 0;

void targetLines(){
    FILE *fp;
    fp = fopen("Bakefile.txt", "r");
    int targetCount = 0;
    
    if(fp != NULL) {
        char line[BUFSIZ];
        
        while( fgets(line, sizeof line, fp) != NULL) {
            for(int i=0; line[i] != '\0'; i++) {
                if (line[i] == ':'){
                    ++targetCount;
                    char *fileName;
                    fileName = strtok(line, " ");
                    int i = 0;
                    while (fileName != NULL)
                    {
                        // if i==0 then create a new array for target line
                        fileName = removeWhiteSpace(fileName);
                        if(*fileName != ':'){
                            // if file does not exist and is not (target:), but dependancy
                            if(!fileExists(fileName)){
                                files[targetCount][i] = malloc(strlen(fileName)*sizeof(char));
                                strcpy(files[targetCount][i], fileName);
                                printf("files[%i][%i]: %s\n",targetCount, i, files[targetCount][i]);
                                if((i != 0) && (targetCount==1)){
                                    rebuildFile[rebuildCount] = malloc(strlen(fileName)*sizeof(char));
                                    strcpy(rebuildFile[rebuildCount], fileName);
                                    printf("rebuildFile[%i]: %s\n", rebuildCount, rebuildFile[rebuildCount]);
                                    ++rebuildCount;
                                }
                                else if((i != 0) && (targetCount>1)){
                                    perror("Cannot rebuild the file/target, because one or more of its dependancies dont exist.");
                                    // ABORT REBUILDING HERE
                                    exit(EXIT_FAILURE);
                                }
                            }
                            // if the file exists, then save it for later
                            else if(fileExists(fileName)){
                                files[targetCount][i] = malloc(strlen(fileName)*sizeof(char));
                                strcpy(files[targetCount][i], fileName);
                                printf("files[%i][%i]: %s\n",targetCount, i, files[targetCount][i]);
                            }
                            i++;
                        }
                        fileName = strtok(NULL, " ");
                    }
                }
            }
        }
    }
    printf("\n\n");
}

/*
 for each file in the target lines
 if DEPENDANCY does not exist, rebuild.
 rebuild it by further searching for dependancy target line.
 once target line has been found, check if these new dependancies exist and go to line after (action line)
 
 if DEPENDANCY does exist, store in files[]
 for each file in files[]
 if dependancy (i.e > 1) is newer than target (=0), rebuild
 
 
 */

void rebuildCheck(){
    int i, j;
    
    for(i=1; i<300; i++){
        char * target;
        time_t targetCreationDate = time(NULL);
        
        for(j=0; j<300; j++){
            if(j==0){
                target = files[i][j];
                if(files[i][j] != NULL){
                    char * targetPath = pathAppend(files[i][j]);
                    targetCreationDate = getFileCreationTime(targetPath);
                }
                else{
                    continue;
                }
            }
            else if(files[i][j] == NULL){
                continue;
            }
            else{
                if(files[i][j] != NULL){
                    if(files[i][j] != NULL){
                        char * path = pathAppend(files[i][j]);
                        time_t dependancyCreationDate = getFileCreationTime(path);
                        if(difftime(dependancyCreationDate, targetCreationDate) > 0){
                            rebuildFile[rebuildCount] = files[i][j];
                            printf("rebuildFile[%i]: %s\n", rebuildCount, rebuildFile[rebuildCount]);
                            ++rebuildCount;
                    }
                }
                else{
                    continue;
                }
                }
            }
        }
    }
}




int main(int argc, char *argv[]){
    targetLines();
    printf("-- Checking Modification Dates --\n");
    rebuildCheck();
    // directorySearch();
    // testMethod();
    
}
