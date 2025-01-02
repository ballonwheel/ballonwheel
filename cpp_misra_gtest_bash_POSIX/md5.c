/**
 * @file md5.c
 * @brief MD5 hash generation functions
 *
 * $Revision: $
 * $Date: $
 *
 *  For MD5 hash generation the external md5sum application is called.
 *
 * Author: Balint Viragh <bviragh@dension.com>
 * Copyright (C) 2010 Dension Audio Systems Kft.
 */

/**
 * @addtogroup shared
 * @{
 */

/* System headers */
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

/* external library headers */   
#include "shared.h"

/* Interface of other modules */

/* Interface header of this module */
#include "md5.h"

/*================================[ INTERNAL MACROS ]========================*/

/** Used extrenal md5sum command */
#define L_ST_MD5SUM_COMMAND "md5sum"

/** index of the file descriptor to read */
#define L_FD_READ           0
/** index of the file descriptor to write */
#define L_FD_WRITE          1

/** sleep between two md5sum wait check in [us] */
#define L_U32_MD5_SLEEP_US      10000u
/** waiting for md5sum child process timeout in [us] */
#define L_U32_MD5SUM_TIMEOUT_US 1000000u

/*================================[ INTERNAL TYPEDEFS ]======================*/

/*================================[ INTERNAL GLOBALS ]=======================*/

/** PID of process that runs the external md5sum application */
static pid_t l_pid_md5sum;

/** file descriptors to read and write standard input of the md5sum application */
static int l_afd_to_md5sum[2];

/** file descriptors to read and write standard output of the md5sum application */
static int l_afd_from_md5sum[2];

/** md5sum input stream */
static FILE* l_sp_stream = NULL;

/*================================[ EXTERNAL GLOBALS ]=======================*/

/*================================[ INTERNAL FUNCTION PROTOTYPES ]===========*/

/*================================[ INTERNAL FUNCTION DEFINITIONS ]==========*/

/*================================[ EXPORTED FUNCTION DEFINITIONS ]==========*/

/* Function to generate the md5 sum of a string (see header for more info) */
extern int md5_sum_str_f(const char* p_st_input, char* p_st_md5sum)
{
    int return_value=0;
    FILE* sp_stream;    /* stream where the input of the MD5 calculation shall be written */
    int res;

    if(NULL == (sp_stream = md5_init_f()))
    {
        DBG(DBG_ERROR,"Can not initialize MD5 sum calculator");
        return -1;
    }

    if(fputs(p_st_input,sp_stream) < 0)
    {
        DBG(DBG_ERROR,"can not write into md5sum stream");
        return_value = -1;
    }
    
    if(0 != (res = md5_final_f(p_st_md5sum)))
    {
        DBG(DBG_ERROR,"MD5 sum calculation returns error %d", res);
        return_value = -1;
    }

    return return_value;
}

/* Function to initialize MD5 calclation (see header for more info) */
extern FILE* md5_init_f(void)
{
    l_sp_stream = NULL;
    /* create pipes to communicate with the new process */
    if(-1 == pipe(l_afd_to_md5sum))
    {
        DBG_PERRNO("pipe");
        return NULL;
    }
    if(-1 == pipe(l_afd_from_md5sum))
    {
        DBG_PERRNO("pipe");
        return NULL;
    }
    /* create new process */
    if((l_pid_md5sum = fork()) < 0)
    {
        DBG_PERRNO("fork");
        return NULL;
    }
    if(0 == l_pid_md5sum)   /* Child process: extrenal md5sum */
    {
        setbuf(stdout,NULL);
        /* close the write end of the pipe the process reads from */
        if(-1 == close(l_afd_to_md5sum[L_FD_WRITE]))
        {
            DBG_PERRNO("close write end of to-md5sum pipe");
            _exit(EXIT_FAILURE);
        }
        /* close the read end of the pipe the process write to */
        if(-1 == close(l_afd_from_md5sum[L_FD_READ]))
        {
            DBG_PERRNO("close read end of from-md5sum pipe");
            _exit(EXIT_FAILURE);
        }
        /* direct read end of the pipe into the standard input */
        if(-1 == dup2(l_afd_to_md5sum[L_FD_READ],STDIN_FILENO))
        {
            DBG_PERRNO("dup2 stdin");
            _exit(EXIT_FAILURE);
        }
        /* close excess file descriptors */
        if(-1 == close(l_afd_to_md5sum[L_FD_READ]))
        {
            DBG_PERRNO("close excess read end of to-md5sum pipe");
            _exit(EXIT_FAILURE);
        }
        /* direct standarad output to the write end of the pipe */
        if(-1 == dup2(l_afd_from_md5sum[L_FD_WRITE],STDOUT_FILENO))
        {
            DBG_PERRNO("dup2 stdout");
            _exit(EXIT_FAILURE);
        }
        /* close excess file descriptors */
        if(-1 == close(l_afd_from_md5sum[L_FD_WRITE]))
        {
            DBG_PERRNO("close write end of from-md5sum pipe");
            _exit(EXIT_FAILURE);
        }
        if(system(L_ST_MD5SUM_COMMAND))
        {
            DBG(DBG_ERROR,"md5sum execution error");    
            (void)fclose(stdout);
            _exit(EXIT_FAILURE);
        }
        if(fclose(stdout))
        {
            DBG(DBG_ERROR,"Can not close stdout in md5sum process");
        }
        _exit(EXIT_SUCCESS);
    }
    else                    /* parent process */
    {
        /* close the write end of the pipe the process reads from */
        if(-1 == close(l_afd_from_md5sum[L_FD_WRITE]))
        {
            DBG_PERRNO("close write end of from-md5sum pipe");
            return NULL;
        }
        /* close the read end of the pipe the process write to */
        if(-1 == close(l_afd_to_md5sum[L_FD_READ]))
        {
            DBG_PERRNO("close read end of to-md5sum pipe");
            return NULL;
        }
        if(NULL == (l_sp_stream = fdopen(l_afd_to_md5sum[L_FD_WRITE],"w")))
        {
            DBG(DBG_ERROR,"fdopen error for the write end of to-md5sum pipe");
            return NULL;
        }
    }

    return l_sp_stream;
}

/* this function gets md5sum result (see header for more info) */
extern int md5_final_f(char* p_st_md5)
{
    FILE* sp_result = NULL;
    uint32_t u32_index;
    int status;
    pid_t pid_wait;
    uint32_t u32_timeout;

    if(NULL == l_sp_stream)
    {
        DBG(DBG_ERROR,"md5sum stream has not been opened");
        return -1;
    }
    if(NULL == (sp_result = fdopen(l_afd_from_md5sum[L_FD_READ],"r")))
    {
        DBG(DBG_ERROR,"fdopen error for the read end of from-md5sum pipe");
        return -2;
    }
    if(fclose(l_sp_stream))
    {
        DBG(DBG_ERROR,"can not close md5sum stream");
        l_sp_stream = NULL;
        return -3;
    }
    l_sp_stream = NULL;
    /* wait for md5sum */
    u32_timeout = 0u;
    while(0 == (pid_wait = waitpid(l_pid_md5sum, &status, WNOHANG)))
    {
        if(usleep(L_U32_MD5_SLEEP_US))
        {
            DBG(DBG_WARNING,"usleep error %d us",L_U32_MD5_SLEEP_US);
        }
        u32_timeout += L_U32_MD5_SLEEP_US;
        if(u32_timeout >= L_U32_MD5SUM_TIMEOUT_US)
        {
            (void)kill(l_pid_md5sum,SIGTERM);            /* kill the child process */
            (void)waitpid(l_pid_md5sum,&status,WNOHANG);  /* read result */
            (void)fclose(sp_result);
            DBG(DBG_ERROR,"MD5 sum timeout error process: %d",l_pid_md5sum);
            return -4;
        }
    }
    if((!WIFEXITED(status)) || (0 != WEXITSTATUS(status)))
    {
        DBG(DBG_ERROR,"MD5 sum returned error");
        (void)fclose(sp_result);
        return -5;
    }
    if( NULL == fgets(p_st_md5,MD5_SUM_LENGTH + 1,sp_result))
    {
        DBG(DBG_ERROR,"Can not read result from md5sum application");
        (void)fclose(sp_result);
        return -6;
    }
    (void)fclose(sp_result);
    /* check md5sum */
    for(u32_index = 0u; u32_index < MD5_SUM_LENGTH; ++u32_index)
    {
        if(!isxdigit(p_st_md5[u32_index]))
        {
            DBG(DBG_ERROR,"%s is not an md5sum (%d)",p_st_md5,u32_index);
            return 1;
        }
    }
    return 0;
}

/* Function to generate MD5 sum of a file (see header for more details) */
extern int md5_sum_file_f(const char* p_st_file, char* p_st_md5sum)
{
    int return_value = 0;
    FILE* sp_out;
    char st_command[MAX_PATH_LENGTH+1];

    /* check arguments */
    if((NULL == p_st_file) || (NULL == p_st_md5sum))
    {
        DBG(DBG_ERROR, "NULL argument for md5sum file");
        return -1;
    }

    /* generate and execute md5sum command */
    (void)snprintf(st_command, sizeof(st_command), "md5sum '%s'", p_st_file);
    sp_out = popen(st_command, "r");
    if(NULL != sp_out)
    {
        /* read stdout */
        if(NULL == fgets(p_st_md5sum,MD5_SUM_LENGTH + 1u, sp_out))
        {
            DBG(DBG_ERROR,"Can not read the output of md5sum calculation");
            return_value = -1;
        }
        if(MD5_SUM_LENGTH != strlen(p_st_md5sum))
        {
            DBG(DBG_ERROR,"The length of the generated md5sum (%s) differs from expected (%d)",
                p_st_md5sum, MD5_SUM_LENGTH);
            return_value = -1;
        }
        if(-1 == pclose(sp_out))
        {
            DBG_PERRNO("pclose for md5sum");
            return_value = -1;
        }
    }
    else
    {
        DBG_PERRNO("popen for md5sum");
        return_value = -1;
    }

    return return_value;
}

/**@}*/
