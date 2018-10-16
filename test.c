
#include "test.h"

int main(int argc, char *argv[]){
    if(argCheck(argc)){
        int val = multiply(atoi(argv[1]), atoi(argv[2]));
        return val;
    }
    else{
        printf("you need to input command line arguments!");
    }
}

