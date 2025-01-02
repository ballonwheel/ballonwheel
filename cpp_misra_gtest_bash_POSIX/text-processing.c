#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "shared.h"
#include "text-processing.h"

void strcatch(char * dst, char src, size_t dstlen)
{
    size_t len = strlen(dst);

    if (len >= dstlen - 1)
        return;

    dst[len] = src;
    dst[len + 1] = '\0';
}

int processing_file(char * filename, int outfd, char * start_delims, char * stop_delims, char param_delim, struct identifier_table * todo)
{
    int ret;
    int fd;
    char c;
    int delims_len = strlen(start_delims);
    int passed_delims;
    int check_delim;
    int delims_backw;
    int get_identifier;
    char identifier[IDENTIFIER_MAX_LENGTH];
    char parameter[IDENTIFIER_MAX_LENGTH];
    int i;

    if(filename) 
        ret = open(filename, O_RDONLY);
    else
        ret = STDIN_FILENO;     /* use stdin if file number is not specified */

    if (ret < 0)
        return ret;
    fd = ret;

    passed_delims = 0;
    check_delim = 0;
    delims_backw = 0;
    get_identifier = 0;
    identifier[0] = '\0';
    parameter[0] = '\0';
    for(ret = read(fd, &c, 1); ret == 1; ret = read(fd, &c, 1))
    {
        if (delims_backw == 0 && c == start_delims[check_delim]) {
            passed_delims++;
            if (passed_delims < delims_len)
                check_delim++;
            else {
                delims_backw = 1;
                passed_delims = 0;
                check_delim = 0;
                get_identifier = 1;
            }
        } else if (delims_backw == 1 && c == stop_delims[check_delim]) {
            passed_delims++;
            if (passed_delims < delims_len)
                check_delim++;
            else {
                for (i = 0; todo[i].id != NULL; i++) {
                    if (strcmp(todo[i].id, identifier) == 0) {
                        todo[i].cb(identifier, parameter, outfd);
                        break;
                    }
                }
                delims_backw = 0;
                passed_delims = 0;
                check_delim = 0;
                identifier[0] = '\0';
                parameter[0] = '\0';
                get_identifier = 0;
            }
        } else {
            if (passed_delims > 0 && get_identifier == 0) {
                ret = write(outfd, start_delims, passed_delims);
                if (ret != passed_delims)
                    return -1;
                passed_delims = 0;
                check_delim = 0;
            } else if (passed_delims > 0 && get_identifier == 1) {
                strncat(identifier, stop_delims, passed_delims);
                passed_delims = 0;
                check_delim = 0;
            } else if (passed_delims > 0 && get_identifier == 2) {
                strncat(parameter, stop_delims, passed_delims);
                passed_delims = 0;
                check_delim = 0;
            } 
            
            if (get_identifier == 1) {
                if (c == param_delim)
                    get_identifier = 2;
                else
                   strcatch(identifier, c, IDENTIFIER_MAX_LENGTH);
            } else if (get_identifier == 2) {
                strcatch(parameter, c, IDENTIFIER_MAX_LENGTH);
            } else {
                ret = write(outfd, &c, 1);
                if (ret != 1)
                    return -1;
            }
        }
    }


    close(fd);

    return 0;
}

