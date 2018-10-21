
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
    // print("%s\n", newPath);
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
    //print("FILENAME: %s \n| %lu\n", filename, strlen(filename));
    
    if(fp == NULL){
        //fclose(fp);
        return false;
    }
    else{
        fclose(fp);
        return true;
    }
}




// remove the extra space from the end of a string (this happens to the last dependancy on a line)
char * removeWhiteSpace(char * text){
    if((text[strlen(text)-1] == ' ') || (text[strlen(text)-1] == '\n')){
        char * substring = malloc(sizeof(char) * (strlen(text)-1));
        strncpy(substring, text, strlen(text)-1);
        substring[strlen(text)-2] = '\0';
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




/*
void rebuildCheck(){
    char * target;
    time_t targetCreationDate = time(NULL);
    
    for(int j=0; j<300; j++){
        if(j==0){
            target = files[targetCount][j];
            if(files[targetCount][j] != NULL){
                char * targetPath = pathAppend(files[targetCount][j]);
                targetCreationDate = getFileCreationTime(targetPath);
            }
            else{
                continue;
            }
        }
        else if(files[targetCount][j] == NULL){
            continue;
        }
        else{
            if(files[targetCount][j] != NULL){
                    char * path = pathAppend(files[targetCount][j]);
                    time_t dependancyCreationDate = getFileCreationTime(path);
                    if(difftime(dependancyCreationDate, targetCreationDate) > 0){
                        rebuildFile[rebuildCount] = malloc(strlen(files[targetCount][j])*sizeof(char));
                        strcpy(rebuildFile[rebuildCount], files[targetCount][0]);
                        //rebuildFile[rebuildCount] = files[i][j];
                        print("rebuildFile[%i]: %s supposed to be: %s\n", rebuildCount, rebuildFile[rebuildCount], files[targetCount][0]);
                        ++rebuildCount;
                    }
                }
                else{
                    continue;
                }
            }
        }
    }
*/


char *files[300][300];
char *rebuildFile[300];
int rebuildCount = 0;
int targetCount = 0;


void rebuildCheck(){
    printf("----Running rebuild Check----\n");
    // can i replace this target value? or is it constant?
    time_t targetCreationDate = time(NULL);
    
    if(targetCount != 0){
        for(int i=1; i<targetCount+1; i++){
            // can change this later. we dont always want to use 300 as max size
            for(int j=0; j<300; j++){
                if(files[i][j] != NULL){
                    // store the modification Date of target (=0), for comparison with dependancies later (>0)
                    if(j==0){
                        // if it is a URL, then check modification date using curl (inside urlDate() function)
                        if(strstr(files[i][j],"file://") != NULL || strstr(files[i][j],"http://") != NULL || strstr(files[i][j],"https://") != NULL){
                            
                            targetCreationDate = urlDate(files[i][j]);
                            if(targetCreationDate == -1){
                                perror("could not find URL");
                            }
                        }
                        // else, its a regular file. And check modification date using normal method
                        else if(files[i][j] != NULL){
                            char * targetPath = pathAppend(files[i][j]);
                            targetCreationDate = getFileCreationTime(targetPath);
                        }
                        else{
                            continue;
                        }
                    }
                    else{
                        // if it is a URL, then use curl to get date of url
                        if(strstr(files[i][j],"file://") != NULL || strstr(files[i][j],"http://") != NULL || strstr(files[i][j],"https://") != NULL){
                            
                            time_t dep_creationDate = urlDate(files[i][j]);
                            if(dep_creationDate == -1){
                                perror("could not find URL");
                            }
                            if(difftime(dep_creationDate, targetCreationDate) > 0){
                                rebuildFile[rebuildCount] = malloc(strlen(files[i][j])*sizeof(char));
                                strcpy(rebuildFile[rebuildCount], files[i][0]);
                                //rebuildFile[rebuildCount] = files[i][j];
                                print("rebuildFile[%i]: %s supposed to be: %s\n", rebuildCount, rebuildFile[rebuildCount], files[targetCount][0]);
                                ++rebuildCount;
                            }
                        }
                        else{
                            // if it is a regular file
                            char * path = pathAppend(files[targetCount][j]);
                            time_t dependancyCreationDate = getFileCreationTime(path);
                            if(difftime(dependancyCreationDate, targetCreationDate) > 0){
                                rebuildFile[rebuildCount] = malloc(strlen(files[i][j])*sizeof(char));
                                strcpy(rebuildFile[rebuildCount], files[i][0]);
                                //rebuildFile[rebuildCount] = files[i][j];
                                print("rebuildFile[%i]: %s supposed to be: %s\n", i, rebuildFile[rebuildCount], files[i][0]);
                                ++rebuildCount;
                            }
                        }
                    }
                }
                else{
                    continue;
                }
            }
        }
    }
}





void targetLines(char * line){
    for(int i=0; line[i] != '\0'; i++) {
        if (line[i] == ':'){
            ++targetCount;
            char * lineCopy = malloc(strlen(line)*sizeof(char));
            strcpy(lineCopy, line);
            char *fileName;
            fileName = strtok(lineCopy, " ");
            int i = 0;
            while (fileName != NULL)
            {
                // if i==0 then create a new array for target line
                fileName = removeWhiteSpace(fileName);
                if(*fileName != ':'){
                    if(strstr(fileName,"file://") != NULL || strstr(fileName,"http://") != NULL || strstr(fileName,"https://") != NULL){
                        // work with URL here
                        files[targetCount][i] = malloc(strlen(fileName) * sizeof(char));
                        strcpy(files[targetCount][i], fileName);
                        print("files[%i][%i]: %s\n",targetCount, i, files[targetCount][i]);
                    }
                    
                    // if file does not exist and is not (target:), but dependancy
                    else if(!fileExists(fileName)){
                        files[targetCount][i] = malloc(strlen(fileName)*sizeof(char));
                        strcpy(files[targetCount][i], fileName);
                        print("files[%i][%i]: %s\n",targetCount, i, files[targetCount][i]);
                        if((i != 0) && (targetCount==1)){
                            rebuildFile[rebuildCount] = malloc(strlen(fileName)*sizeof(char));
                            strcpy(rebuildFile[rebuildCount], fileName);
                            print("rebuildFile[%i]: %s\n", rebuildCount, rebuildFile[rebuildCount]);
                            ++rebuildCount;
                        }
                        else if((!fileExists(fileName)) && (i != 0) && (targetCount>1)){

                            perror("Cannot rebuild the file/target, because one or more of its dependancies dont exist.\n");
                            // ABORT REBUILDING HERE
                            //exit(EXIT_FAILURE);
                        }
                    }
                    // if the file exists, then save it for later
                    else if(fileExists(fileName)){
                        files[targetCount][i] = malloc(strlen(fileName)*sizeof(char));
                        strcpy(files[targetCount][i], fileName);
                        print("files[%i][%i]: %s\n",targetCount, i, files[targetCount][i]);
                    }
                    i++;
                }
                fileName = strtok(NULL, " ");
            }
        }
    }
    print("\n\n");
    rebuildCheck();
}






/**
int main(int argc, char *argv[]){
    print("%i", fileExists("haversine.h"));
    targetLines();
    print("-- Checking Modification Dates --\n");
    rebuildCheck();
    // directorySearch();
    // testMethod();
    
}
 **/
