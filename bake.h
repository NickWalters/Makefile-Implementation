#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <getopt.h>
#include <stdarg.h>
#include <errno.h>
#include <curl/curl.h>

#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/param.h>
#include <sys/wait.h>
#include  <dirent.h>
#include  <time.h>
#include <limits.h>

extern bool targetLines(char ** lines, int numLines);

extern void removeLeadingWhitespace(char * line);

extern void executeShell(char * line, char * cwd, char * targetFile);

extern bool isActionLine(char * line);

extern bool rebuildDefault(void);

extern void print(char * string, ...);

extern time_t urlDate(char * URL);

extern bool printyBois(void);

extern void push(int * arr, int index, int value, int * size, int * capacity);



