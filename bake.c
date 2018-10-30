/* CITS2002 Project 2018
 Name(s):        Nicholas Walters , James Caldon
 Student number(s):    22243339 , 22226341
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





//Checks to see if the line is a comment
bool isComment(char * line) {
    for (int i=0; line[i] != '\0'; i++) {
        if (line[i] == '#') return true;
    }
    
    return false;
}





//Start with a small number because who doesn't like dynamic allocations
int numOfKeyValPairs = 8;
//Stores Keys
char ** keyArray;

int nextKey = 0;

//Stores Values to the keys
char ** valArray;
int nextVal = 0;
// ADD VARIABLES TO BE USED LATER (FROM THE BAKEFILE)
void addVariables(char * key, char * val) {
    if (nextKey >= numOfKeyValPairs) {
        numOfKeyValPairs = numOfKeyValPairs*2;
        char ** keys;
        char ** vals;
        keys = realloc(keyArray, (numOfKeyValPairs) * sizeof(char*));
        vals = realloc(valArray, (numOfKeyValPairs) * sizeof(char*));
        if (keys == NULL || vals == NULL) {
            exit(EXIT_FAILURE);
        }
        keyArray = keys;
        valArray = vals;
    }
    int keyLength = strlen(key);
    int valLength = strlen(val);
    keyArray[nextKey] = malloc((keyLength+1)*sizeof(char));
    valArray[nextVal] = malloc((valLength+1)*sizeof(char));

    if (keyArray[nextKey] == NULL || valArray[nextVal] == NULL) exit(EXIT_FAILURE);
    
    strncpy(keyArray[nextKey], key, keyLength + 1);
    strncpy(valArray[nextVal], val, valLength + 1);
    
    keyArray[nextKey++][keyLength] = '\0';
    valArray[nextVal++][valLength] = '\0';
    
}



// definition of variables from a LINE
void defineVariables(char * line) {

    int startPotKey = 0;
    int endPotKey = 0;
    
    int equalsPos = 0;
    
    int startPotVal = 0;
    int endOfLine = strlen(line);
    
    for (int i=0; line[i] != '=' && line[i] != '\0'; i++) {
        if (isspace(line[i]) && !isspace(line[i+1]) && line[i+1] != '=' && startPotKey == 0) startPotKey = i+1;
        if ((isspace(line[i+1]) || line[i+1] == '=') && !isspace(line[i])) endPotKey = i+1;
        if (line[i+1] == '=' && equalsPos == 0) {
            equalsPos = i+1;
            //print("%i\n", equalsPos);
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
            //Issue here?
            endOfLine = i + 1;
            break;
        }
    }
    
    
    int keyLength = endPotKey-startPotKey;
    int valLength = endOfLine-startPotVal;
    
    
    char * var = malloc((keyLength+1) * sizeof(char));
    char * val = malloc((valLength+1) * sizeof(char));

    
    strncpy(var, line+startPotKey, keyLength + 1);
    var[keyLength] = '\0';
    strncpy(val, line+startPotVal, valLength + 1);
    val[valLength] = '\0';
    addVariables(var, val);
    free(var);
    free(val);
}


// expand the variables on the line, in where the variable is mentioned
void expandVariable(char * line, int l, char * var, int i, int endPos, int keyLength, char * val) {
    //Process Line
    int valueLength = strlen(val);
    char * newLine = malloc((l - keyLength + valueLength+1)*sizeof(char));
    if (newLine == NULL) exit(EXIT_FAILURE);
    strncpy(newLine, line, i);
    strncpy(newLine+i, val, valueLength);
    strncpy(newLine+i+valueLength, line + endPos+1, l - endPos+1);
    newLine[l- keyLength + valueLength] = '\0';
    
    strcpy(line, newLine);
    free(newLine);
}



// same process
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
            strncpy(var, line + i+2, varLength+1);
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
    int valLength = snprintf(NULL, 0, "%d", value);
    char * val = malloc(valLength+1);
    snprintf(val, valLength+1, "%i", value);
    addVariables(key, val);
    free(val);
}
char ** lines;
int numComments;
int numLines = 0;


struct Options {
    char * dir;
    char * filename;
    bool ignore;
    bool fakeout;
    bool print;
    bool silent;
};
struct Options options = {NULL, NULL, false, false, false, false};







void print(char * string, ...) {
    va_list valist;
    va_start(valist, string);
    
    /* clean memory reserved for valist */
    va_end(valist);
    
    if (!options.silent)
    vprintf(string, valist);
}





bool isActionLine(char * line) {
    return (line[0] == '\t');
}



bool isTargetLine(char * line) {
    return (strchr(line, ':') != NULL);
}



void executeShell(char * line, char * cwd, char * targetFile) {
    bool ignoreErrors = false;
    bool noPrint = false;
    if ((line[0] == '@' && line[1] == '-') || ((line[1] == '@' && line[0] == '-'))) {
        //Ignore Errors and don't print output
        ignoreErrors = true;
        noPrint = true;
        line++;
        line++;

    } else if (line[0] == '@' || line[0] == '@') {
        //Do not print output
        ignoreErrors = false;
        noPrint = true;
        line++;
    } else if (line[0] == '-' || line[1] == '-') {
        //Ignore Errors
        ignoreErrors = true;
        noPrint = false;
        line++;
    } else {
        //Don't continue on error and print output
        ignoreErrors = false;
        noPrint = false;
    }
    pid_t pid = fork();
    
    if (pid == -1)
    {
        print("The fork failed! Exiting...\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            print("Command executing!\n");
            if (WEXITSTATUS(status) != 0) {
                
                if(!options.ignore && !ignoreErrors) {
                    print("Command failed!!!\n");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            
            if(!options.ignore && !ignoreErrors) {
                print("Oh no, child process could not start execution of the command!!!\n");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        //Child Process
        char * shellShocked;
        if (getenv("SHELL") == NULL) {
            shellShocked = malloc((strlen("/bin/bash")+1)*sizeof(char));
            strcpy(shellShocked, "/bin/bash");
        } else {
            shellShocked = malloc((strlen(getenv("SHELL"))+1)*sizeof(char));
            strcpy(shellShocked, getenv("SHELL"));
        }
        if (targetFile == NULL) {
            if(options.print) {

                print("Executing Command: %s -c %s\n", shellShocked, line);
            } else {
                if (!noPrint) print("Executing Command: %s -c %s\n", shellShocked, line);
                if(!options.fakeout) {
                    execl(shellShocked, cwd, "-c", line, NULL);
                    exit(EXIT_FAILURE);   // exec never returns
                }
                exit(EXIT_SUCCESS);
            }
            
            exit(EXIT_SUCCESS);   // exec never returns
        } else {
            if(options.print) {

                print("Executing Command: %s -c %s -o %s\n", shellShocked, line, targetFile);
            } else {
                if (!noPrint) print("Executing Command: %s -c %s -o %s\n", shellShocked, line, targetFile);
                if(!options.fakeout) {
                    execl(shellShocked, cwd, "-c", line, "-o", targetFile, NULL);
                    
                    exit(EXIT_FAILURE);   // exec never returns
                }
                exit(EXIT_SUCCESS);
            }
            
            exit(EXIT_SUCCESS);   // exec never returns
        }
    }
}





void removeLeadingWhitespace(char * line) {
    
    int length = strlen(line);
    int posOfChar = 0;
    for(int i = 0; i < length; i++)
    {
        if (!isspace(line[i])) {
            posOfChar = i;
            break;
        }
    }
    memmove(line, line + posOfChar, length - posOfChar + 1);
    line[length - posOfChar] = '\0';
}





int main(int argc, char *argv[])
{
    
    //Initial amount for key value arrays i.e. 32 key value pairs
    keyArray = malloc(numOfKeyValPairs * sizeof(char*));
    valArray = malloc(numOfKeyValPairs * sizeof(char*));
    
    //  ATTEMPT TO OPEN AND READ FROM PROVIDED FILENAME
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
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
    
    
    //CHANGE
    FILE * fp = fopen("bakefile.txt", "r");
    if(fp == NULL) {
        fp = fopen("Bakefile.txt", "r");
    }
    if(fp == NULL) {
        fp = fopen("bakefile", "r");
    }
    if(fp == NULL) {
        fp = fopen("Bakefile", "r");
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

        addVariables("PWD",  cwd);
        addDefaultVariables("RAND", rand());
        
        char bufferedLine[BUFSIZ];
        int bufferedPos = 0;
        bool buffered = false;
        
        int numLines = 0;
        while(fgets(line, sz, fp) != NULL){
            int l = strlen(line);
            
            //Check if line bleeds onto next line
            if (l > 2 && line[l-2] == '\\') {

                strncpy(bufferedLine + bufferedPos, line, l-2);
                bufferedPos += l-2;
                bufferedLine[bufferedPos] = '\0';
                buffered = true;
                
                continue;
            } else if (buffered) {
                
                removeLeadingWhitespace(line);
                strncpy(bufferedLine + bufferedPos, line, l);
                bufferedPos += l;
                bufferedLine[bufferedPos] = '\0';
                strcpy(line, bufferedLine);
                bufferedLine[0] = '\0';
                buffered = false;
                
            }
            
            l = strlen(line);
            
            if (isComment(line)) {numComments++; continue;}
            

            expandVariables(line);

            line[strcspn(line, "\n")] = '\0';
            defineVariables(line);
            l = strlen(line);
            if (l > 0) {
                lines[numLines] = malloc((l+1) *sizeof(char));
                strncpy(lines[numLines], line, (l+1));
                lines[numLines++][l] = '\0';
            }
        }
        if (options.print) {
            for (int i=0; i < nextKey; i++) {
                print("Key: %s | Val: %s\n", keyArray[i], valArray[i]);
            }
        }

        targetLines(lines, numLines);
        
        
        print("\n");
        fclose(fp);
    }
}

bool printyBois(void) {
    return options.print;
}




