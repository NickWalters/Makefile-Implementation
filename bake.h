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
#include  <dirent.h>
#include  <time.h>
#include <limits.h>

extern void targetLines(char * line);
extern void print(char * string, ...);
extern char *files[300][300];
extern char *rebuildFile[300];
extern int rebuildCount;
extern int targetCount;
extern time_t urlDate(char * URL);

