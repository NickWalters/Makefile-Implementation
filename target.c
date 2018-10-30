/* CITS2002 Project 2018
 Name(s):        Nicholas Walters , James Caldon
 Student number(s):    22243339 , 22226341
 */
#include "bake.h"




/*
 this appends a filename to the path given.
 eg for filename "example.c" you can append to the path Desktop/Nick/Project
 so it becomes: Desktop/Nick/Project/example.c
 */
char * pathAppend(char *fileName){
    char cwd[PATH_MAX];
    // if you cannot find the current working directory
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd() error, cannot find current directory");
    }
    int strLength = strlen(fileName);
    // allocate memory for the new path size
    char * newPath = calloc((sizeof(cwd) + ((strLength+1)*(sizeof(char))))/sizeof(cwd[0]), sizeof(cwd) + ((strLength+1)*(sizeof(char))));
    // add old path + / + fileName
    strcat(newPath, cwd);
    strcat(newPath, "/");
    strcat(newPath, fileName);
    return newPath;
}







// gets the modification/creation date of the fileName provided (must use pathAppend first)
time_t getFileCreationTime(char *path) {
    struct stat stat_info;
    // stat the files info
    stat(path, &stat_info);
    return stat_info.st_mtime;
}








// check if the file exists
bool fileExists(char * filename){
    FILE *fp;
    fp = fopen(filename, "r");
    
    if(fp == NULL){
        //fclose(fp);
        return false;
    }
    else{
        fclose(fp);
        return true;
    }
}







int maxFilesCols = 256;
char ***files;

int maxRebuilds = 256;
char **rebuildFile;

int rebuildCount = 0;
int targetCount = 0;

/*
 This function compares modification dates of all the found files.
 If the modification date of dependancy is newer than the target, eg files[1] > files[0] then rebuild that line
 */
void rebuildCheck(){
    print("----Running rebuild Check----\n");
    time_t targetCreationDates[targetCount];
    rebuildFile = calloc(maxRebuilds, sizeof(char));
    if(targetCount != 0){
        for(int i=0; i<targetCount; i++){
            // can change this later. we dont always want to use 300 as max size
            for(int j=0; j<maxFilesCols; j++){
                if(files[i][j] != NULL){
                    // store the modification Date of target (=0), for comparison with dependancies later (>0)
                    if(j==0){
                        // if the target is a URL, then check modification date using curl (inside urlDate() function)
                        // store the date in a variable for later use
                        if(strstr(files[i][j],"file://") != NULL || strstr(files[i][j],"http://") != NULL || strstr(files[i][j],"https://") != NULL){
                            
                            targetCreationDates[i] = urlDate(files[i][j]);
                            if(targetCreationDates[i] == -1){
                                perror("could not find URL\n");
                            }
                        }
                        // else, its a regular file (not URL). And check modification date using normal method
                        else if(files[i][j] != NULL){
                            char * targetPath = pathAppend(files[i][j]);
                            targetCreationDates[i] = getFileCreationTime(targetPath);
                            free(targetPath);
                        }
                        else{
                            continue;
                        }
                    }
                    else{
                        // if it is a URL, then use curl to get date of url
                        if(strstr(files[i][j],"file://") != NULL || strstr(files[i][j],"http://") != NULL || strstr(files[i][j],"https://") != NULL){
                            
                            // get the creation date of the URL
                            time_t dep_creationDate = urlDate(files[i][j]);
                            if(dep_creationDate == -1){
                                perror("could not find URL\n");
                            }
                            // if the dependancy is newer than the target, then rebuild this targetLine (diffTime should return the difference in time)
                            if(difftime(dep_creationDate, targetCreationDates[i]) > 0){
                                
                                
                                int length = strlen(files[i][j]);
                                rebuildFile[rebuildCount] = malloc((length+1)*sizeof(char));
                                strncpy(rebuildFile[rebuildCount], files[i][0], length+1);
                                
                                rebuildFile[rebuildCount][length] = '\0';
                                rebuildCount++;
                            }
                        }
                        else{
                            // if it is a regular file
                            char * path = pathAppend(files[i][j]);
                            time_t dependancyCreationDate = getFileCreationTime(path);
                            free(path);
                            // if the dependancy is newer than the target, then rebuild
                            if(difftime(dependancyCreationDate, targetCreationDates[i]) > 0){
                                int length = strlen(files[i][0]);
                                rebuildFile[rebuildCount] = malloc((length+1)*sizeof(char));
                                strncpy(rebuildFile[rebuildCount], files[i][0], length+1);
                                rebuildFile[rebuildCount][length] = '\0';
                                rebuildCount++;
                            }
                        }
                    }
                }
            }
        }
    }
}






/*
 This function will check if a file that needs to be rebuilt, has been mentioned before in a target line. Thus it will rebuild wherever the dependancy is mentioned
 */
void checkLayered(void) {
    // layered time dependancies
    int rebuildCountCopy = rebuildCount;
    
    // scan all the files, and compare if it needs rebuilding
    for(int i = 0; i < rebuildCountCopy; i++) {
        int m;
        int p;
        for(m=0; m<targetCount; m++){
            for(p=1; p<maxFilesCols; p++){
                if(files[m][p] != NULL){
                    // if a.o uses b.o, and b.o has been modified recently. Compile both
                    // if the file has been mentioned before, and it is not a target
                    if(strcmp(rebuildFile[i], files[m][p]) == 0){
                        rebuildFile[rebuildCount] = malloc((strlen(files[m][0])+1)*sizeof(char));
                        strncpy(rebuildFile[rebuildCount], files[m][0], (strlen(files[m][0])+1));
                        rebuildFile[rebuildCount][strlen(files[m][0])] = '\0';
                        ++rebuildCount;
                    }
                }
                else{
                    break;
                }
            }
        }
    }
}




/*
 Checks target lines to see if it needs to be rebuild. Used in main method
 */
bool needsRebuilding(char * targetName) {
    for(int i = 0; i < rebuildCount; i++) {
        if (strcmp(rebuildFile[i], targetName) == 0) return true;
    }
    return false;
}





int maxDependanciesToTest = 256;
int * dependancyMap;
int * dIndex;
int dependancyCount = 0;
void checkDependencies(void) {
    for(int j=0; j<dependancyCount; j++) {
        bool dependancyFail = true;
        int targetFail = -1; //Should be array to hold all failures.
        for(int i=0; i < targetCount; i++) {
            
            
            
            if (strcmp(files[i][0],files[dependancyMap[j]][dIndex[j]])==0) {
                dependancyFail = false;
                
            } else {
                targetFail = i;
            }
            
            
            
            
        }
        if (dependancyFail) {
            print("Target %s cannot be rebuilt\n", files[targetFail][0]);
            exit(EXIT_FAILURE);
        }
    }
    if (dependancyCount > 0) {
        free(dependancyMap);
        free(dIndex);
    }
}





/*
  Target lines is the main function of Target.c, it scans the bakefile/makefile
 and extracts all the files mentioned in the bakefile. It then searches for these files in the cwd, to see if they exist, and stores these to see if they need rebuilding
 */

int targetMap[256];
bool targetLines(char ** lines, int numLines){
    //There can only be targets less than the amount of lines.
    files = calloc(numLines, sizeof(char**));
    dependancyMap = malloc(maxDependanciesToTest * sizeof(int));
    dIndex = malloc(maxDependanciesToTest * sizeof(int));
    for(int k=0; k < numLines; k++) {
        if (files[targetCount] == NULL) files[targetCount] = calloc(maxFilesCols, sizeof(char*));
        for(int i=0; lines[k][i] != '\0'; i++) {
            if (lines[k][i] == ':'){
                targetMap[targetCount] = k;
                int strLength = strlen(lines[k]);
                char * lineCopy = malloc((strLength+1)*sizeof(char));
                strncpy(lineCopy, lines[k], strLength+1);
                lineCopy[strLength] = '\0';
                char *fileName;
                // split into filenames
                fileName = strtok(lineCopy, " ");
                int i = 0;
                while (fileName != NULL)
                {
                    // if i==0 then create a new array for target line
                    
                    if(*fileName != ':'){
                        // if it is a URL
                        if(strstr(fileName,"file://") != NULL || strstr(fileName,"http://") != NULL || strstr(fileName,"https://") != NULL){
                            // work with URL here
                            int length = strlen(fileName);
                            files[targetCount][i] = malloc((length+1) * sizeof(char));
                            strncpy(files[targetCount][i], fileName, length+1);
                            files[targetCount][i][length] = '\0';
                            
                        }
                        
                        // if file does not exist and is not (target:), but dependancy
                        else if(!fileExists(fileName)){
                            int length = strlen(fileName);
                            files[targetCount][i] = malloc((length+1)*sizeof(char));
                            strncpy(files[targetCount][i], fileName, length+1);
                            files[targetCount][i][length] = '\0';
                            if(i==0){
                                int length = strlen(fileName);
                                rebuildFile[rebuildCount] = malloc((length+1)*sizeof(char));
                                strncpy(rebuildFile[rebuildCount], fileName, length+1);
                                rebuildFile[rebuildCount][length] = '\0';
                                ++rebuildCount;
                            }
                            // if the file doesnt exist, and isnt a target
                            else if((!fileExists(fileName)) && (i != 0) && (targetCount>1)){
                                perror("Cannot rebuild the file/target, because one or more of its dependancies dont exist.\n");
                                if (dependancyCount >= maxDependanciesToTest) {
                                    maxDependanciesToTest = maxDependanciesToTest * 2;
                                    int * dMap = realloc(dependancyMap, maxDependanciesToTest);
                                    int * dInd = realloc(dIndex, maxDependanciesToTest);
                                    if(dMap == NULL || dInd == NULL) {
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                dependancyMap[dependancyCount] = targetCount;
                                dIndex[dependancyCount++] = i;
                                int length = strlen(fileName);
                                rebuildFile[rebuildCount] = malloc((length+1)*sizeof(char));
                                strncpy(rebuildFile[rebuildCount], fileName, length+1);
                                rebuildFile[rebuildCount][length] = '\0';
                                ++rebuildCount;
                            }
                        }
                        // if the file exists, then save it for later
                        else if(fileExists(fileName)){
                            
                            int length = strlen(fileName);
                            files[targetCount][i] = malloc((length+1)*sizeof(char));
                            strncpy(files[targetCount][i], fileName, length+1);
                            files[targetCount][i][length] = '\0';
                            
                        }
                        i++;
                    }
                    fileName = strtok(NULL, " ");
                }
                targetCount++;
                free(lineCopy);
            }
            
        }
    }
    // formatting
    if (printyBois()) {
        for(int i = 0; i < targetCount; i++) {
            removeLeadingWhitespace(lines[targetMap[i]]);
            print("Target Line: %s\n", lines[targetMap[i]]);
        }
        for(int i = 0; i < targetCount; i++) {
            for(int j = 0; files[i][j] != NULL; j++) {
                print("files[%i][%i]: %s\n", i, j, files[i][j]);
            }
        }
    }
    
    for(int i = 0; i < rebuildCount; i++) {
            print("RebuildCheck: %s\n", rebuildFile[i]);
    }
    checkDependencies();
    
    
    rebuildCheck();
    checkLayered();
    
    
    
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    for(int i = targetCount+1; i >= 0; i--) {
        if (isActionLine(lines[targetMap[i]+1])) {
            if (printyBois()) print("Action Line: %s\n", lines[targetMap[i]+1]);
            removeLeadingWhitespace(lines[targetMap[i]+1]);
            if (needsRebuilding(files[i][0])) {
                print("Rebuilding required for: %s\n", files[i][0]);
                print("Execution Commencing! Rebuilding %s\n", files[i][0]);
                print("Action Line to be executed: %s\n", lines[targetMap[i]+1]);
                executeShell(lines[targetMap[i]+1], cwd, files[i][0]);
                
                
                
            } else {
                print("%s Does not need rebuilding\n", files[i][0]);
            }
        }
    }
    
    return false;
}


