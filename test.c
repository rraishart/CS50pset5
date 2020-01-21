int main()
{
    // header files
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


    const char* line = "Git REQUESTCRAP HTTP/1.1";
    char* parseline = malloc(25);
    strcpy(parseline, line);
    char* method = strtok(parseline, " ");
    char* request = strtok(NULL, " ");
    char* http = strtok(NULL, " ");
    
    // check method
    if (strcmp(method, "GET") != 0)
    {
        printf("%s %s", method, request);
        return 0;
    }
    //check http version
    if (strcmp(http, "HTTP/1.1") != 0 )
    {
        
        return 0;
    }
    return 0;
}