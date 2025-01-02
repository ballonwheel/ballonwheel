/**
 * @file shared/debug.c
 * @brief Debug functions.
 *
 * Copyright (C) 2010 Dension Audio Systems Kft.
 *
 * @addtogroup shared
 * @{
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/klog.h>
#include <sys/wait.h>
#ifndef S_SPLINT_S
#include <sys/syslog.h>
#endif  /* S_SPLINT_S */
#include <sys/time.h>
#include <time.h>

#include "debug.h"
#include "misc.h"

/**
 * Default debug settings.
 */
static struct dbg_control dctrl = {
    .module_name = "",
    .timestamp = NO,
    .dbg_to_syslog_level = DBG_ALL,
    .dbg_to_stdout_level = DBG_INFO,
    .debug_file[0] = 0,
};

/**
 * Writes out a formatted debug message.
 *
 * Debug behaviour is being controlled by dctrl.
 */
void dbg_function (uint8_t level, char * format, ...)
{
    FILE * out;
    va_list args;
    time_t time_s;
    char buff[1024];
    uint32_t slevel;
    char prefix[16];
    char timestamp[32];
    struct tm * time_all;
    struct timeval time_us;

    switch (level) {
        case DBG_INFO:
            strncpy0 (prefix, "INF", sizeof (prefix));
            slevel = 6;
            break;
        case DBG_WARNING:
            strncpy0 (prefix, "WRN", sizeof (prefix));
            slevel = 4;
            break;
        case DBG_ERROR:
            strncpy0 (prefix, "ERR", sizeof (prefix));
            slevel = 3;
            break;
        case DBG_PANIC:
            strncpy0 (prefix, "PAN", sizeof (prefix));
            slevel = 1;
            break;
        case DBG_DEVEL:
            strncpy0 (prefix, "DEV", sizeof (prefix));
            slevel = 7;
            break;
        default:
            return;
    }

    va_start (args, format);
    (void) vsnprintf (buff, sizeof (buff), format, args);
    va_end (args);

    /* write into syslog */
    if (level <= dctrl.dbg_to_syslog_level) {
        openlog (dctrl.module_name, LOG_NDELAY, LOG_USER);
        syslog (slevel, "%s", buff);
        closelog ();
    }

    /* write to stdout only if allowed */
    if (level > dctrl.dbg_to_stdout_level && dctrl.debug_file[0] == '\0')
        return;

    timestamp[0] = '\0';
    if (dctrl.timestamp == ABSOLUTE) {
        time_s = time (NULL);
        time_all = localtime (&time_s);
        if (time_all)
            (void) snprintf (timestamp, sizeof (timestamp), "%04u-%02u-%02u %02u:%02u:%02u ",
                     time_all->tm_year + 1900, time_all->tm_mon + 1, time_all->tm_mday,
                     time_all->tm_hour, time_all->tm_min, time_all->tm_sec);
        else
            strncpy0 (timestamp, "\?\?\?\?-\?\?-\?\? \?\?:\?\?:\?\? ", sizeof (timestamp));
    }
    else if (dctrl.timestamp == ABSOLUTE_MS) {
        (void) gettimeofday (&time_us, NULL);
        time_s = time_us.tv_sec;
        time_all = localtime (&time_s);
        if (time_all)
            (void) snprintf (timestamp, sizeof (timestamp), "%04u-%02u-%02u %02u:%02u:%02u:%03u ",
                             time_all->tm_year + 1900, time_all->tm_mon + 1, time_all->tm_mday,
                             time_all->tm_hour, time_all->tm_min, time_all->tm_sec, (uint32_t) time_us.tv_usec / 1000);
                             else
                                 strncpy0 (timestamp, "\?\?\?\?-\?\?-\?\? \?\?:\?\?:\?\?\? ", sizeof (timestamp));
    }

    if (level <= dctrl.dbg_to_stdout_level)
        printf ("%s[%s] %s %s", timestamp, dctrl.module_name, prefix, buff);

    /* write into debug file */
    if (dctrl.debug_file[0] != '\0') {
        out = fopen (dctrl.debug_file, "a");
        if (out == NULL)
            return;
        fprintf (out, "%s[%s] %s %s", timestamp, dctrl.module_name, prefix, buff);
        (void) fclose (out);
    }
}

/**
 * Gets debug subsystem control options.
 */
void dbg_get_ctrl (struct dbg_control * ctrl)
{
    if (ctrl == NULL)
        return;
    memcpy (ctrl, &dctrl, sizeof (dctrl));
}

/**
 * Sets debug subsystem control options.
 */
void dbg_set_ctrl (struct dbg_control * ctrl)
{
    if (ctrl == NULL)
        return;
    memcpy (&dctrl, ctrl, sizeof (dctrl));
}

/* log a message to syslog */
void dbg_syslog (char* p_cp_status, char* p_cp_ie, char* p_cp_msg)
{
    char* st_msg = NULL;
    int result = 0;

    if(!p_cp_status || !p_cp_ie) {
        DBG(DBG_ERROR, "Invalid arguments provided!");
        return;
    }

    st_msg =  malloc(U32_MAX_LOG_LINE_LEN);
    if(NULL == st_msg) {
        DBG(DBG_ERROR, "Memory allocation failed!");
        return;
    }

    snprintf(st_msg, U32_MAX_LOG_LINE_LEN, "%s -p %s -t %s \"%s\"",
            ST_LOGGER_CMD, p_cp_ie, p_cp_status, p_cp_msg ? p_cp_msg : "");
    result = system(st_msg);
    if(result < 0) {
        DBG_PERRNO("system %s", st_msg);
        free(st_msg);
        return;
    }
    if(WIFEXITED(result)) {
        int exitcode = WEXITSTATUS(result);

        if(exitcode) {
            DBG(DBG_ERROR, "logger cmd returned %d (%s)", exitcode, st_msg);
            free(st_msg);
            return;
        }
    } else {
        DBG(DBG_ERROR, "logger cmd exited abnormally %d", result);
        free(st_msg);
        return;
    }

    free(st_msg);
    return;
}


/**
 * @}
 */
