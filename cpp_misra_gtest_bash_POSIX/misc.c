/**
 * @file shared/misc.c
 * @brief Miscellaneous shared functions.
 *
 * Copyright (C) 2010 Dension Audio Systems Kft.
 *
 * @addtogroup shared
 * @{
 */

#define _GNU_SOURCE

#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "debug.h"
#include "misc.h"

#define UNMOUNT_ATTEMPTS                        5   /**< how many times it is attempted to unmount */
#define UNMOUNT_WAIT                            50  /**< [ms] time to wait between two attempts */

/**
 * Table for keeping track of running threads.
 */
struct thread_table {
    uint8_t running;                            /**< number of threads running */
    bool_t signal_exit;                         /**< TRUE if threads must exit */
};

static pthread_mutex_t threads_mutex = PTHREAD_MUTEX_INITIALIZER;
static struct thread_table threads = {
    .running = 0,
    .signal_exit = FALSE,
};

static uint32_t timeout_diff (struct timespec * start, struct timespec * stop);

/**
 * Delays program execution (lets the scheduler switch to an
 * other, running task) by the given amount of millisecs.
 */
void ms_sleep (uint32_t millisec)
{
    struct timespec req;

    if (millisec == 0)
        return;

    req.tv_sec = millisec / 1000;
    req.tv_nsec = (millisec % 1000) * 1000000L;

    while (nanosleep(&req, &req) == -1) {
        switch (errno) {
            case EINTR:
                /* @todo print something */
                break;
            default:
                /* @todo print something */
                break;
        }
    }
}

/**
 * Performs a more exact delay than ms_sleep(...).
 *
 * On IRD, delays <= 25 ms last more than 10% longer with
 * ms_sleep (...), than they should. The less the intended
 * sleep duration is, the higher the skew. This functions tries to
 * eliminate that skew by using busy loop (thus causing high CPU load).
 *
 * Use it for sleeps < 40 ms.
 *
 * @warning Currently not thread-safe!
 */
void ms_sleep_exact (uint32_t millisec)
{
    uint64_t last, curr;
    struct timeval tv;

    if (millisec == 0)
        return;

    /* 3.2 us */
    (void) gettimeofday (&tv, NULL);
    last  = (uint64_t) tv.tv_sec * 1000000UL + tv.tv_usec;
    last += millisec * 1000 - 3;

    ms_sleep (millisec / 2);
    for (;;) {
        (void) gettimeofday (&tv, NULL);
        curr = (uint64_t) tv.tv_sec * 1000000UL + tv.tv_usec;
        if (curr >= last)
            break;
    }
}

/**
 * Copies a string safely, and terminates it unlike strncpy.
 *
 * @param buflen Length of the output buffer (including the terminating zero)
 * @param dest Destination buffer
 * @param src Source buffer
 *
 * @return Copied bytes
 */
uint32_t strncpy0 (char * dest, const char * src, uint32_t buflen)
{
    uint32_t len;

    if (dest == NULL)
        return 0;

    if (src == NULL) {
        *dest = '\0';
        return 0;
    }

    /* number of useful bytes to copy */
    len = strlen (src);
    if (len >= buflen)
        len = buflen - 1;

    memcpy (dest, src, len);
    dest[len] = '\0';
    return len;
}

/**
 * Concatenates two string.
 *
 * @param dest_len The length of the output string will not exceed this
 * (including the terminating zero).
 */
void strncat0 (char * dest, char * src, uint32_t dest_len)
{
    uint32_t rlen;
    uint32_t slen;

    slen = strlen (dest);
    rlen = dest_len - slen;
    (void) strncpy0 (&(dest[slen]), src, rlen);
}

/** Internal function to terminate a string at specified characters 
 *
 *  @param  p_st_string     input-output: string shall be processed
 *  @param  p_st_terms      string of the terminator character
 *
 *  The function terminates (inserts '\0') in the place of the 
 *  terminator characters specified in the p_st_terms argument.
 */
void string_terminate(char* p_st_string, const char* p_st_terms)
{
    uint32_t u32_index;
    uint32_t u32_length;

    /* check arguments */
    if((NULL == p_st_string) || (NULL == p_st_terms))
        return;

    /* number of terminator characters */
    u32_length = strlen(p_st_terms);
    /* for each terminator characters */
    for(u32_index = 0u; u32_index < u32_length; ++u32_index) {
        char c_term = p_st_terms[u32_index];
        char* cp_term = strchr(p_st_string, c_term);
        /* if terminator character has been found */
        if(cp_term)
            *cp_term = '\0';
    }
}

/**
 * Initializes the timeout structure.
 *
 * @param timeout timeout structure
 * @param timeout_val the desired timeout in ms
 */
void timeout_init (struct timeout * timeout, uint32_t timeout_val)
{
    timeout->timeout = timeout_val;
    clock_gettime (CLOCK_MONOTONIC, &(timeout->start));
}

/** 
 * Change the timeout value of the structure
 *
 * @param timeout timeout structure
 * @param timeout_val the desired new timeout in ms
 */
void timeout_change (struct timeout * timeout, uint32_t timeout_val)
{
    timeout->timeout = timeout_val;
}


/**
 * Resets the timeout and starts from the beginning.
 */
void inline timeout_reset (struct timeout * timeout)
{
    timeout_init (timeout, timeout->timeout);
}

/**
 * Returns the amount of remaining millisecs.
 */
uint32_t timeout_remaining (struct timeout * timeout)
{
    uint32_t diff;
    struct timespec curr;

    if (timeout->timeout == 0)
        return 0;
    clock_gettime (CLOCK_MONOTONIC, &curr);
    diff = timeout_diff (&(timeout->start), &curr);

    if (diff >= timeout->timeout)
        return 0;
    return timeout->timeout - diff;
}

/**
 * Returns the number of milliseconds elapsed since the init / last reset.
 */
uint32_t timeout_elapsed (struct timeout * timeout)
{
    return timeout->timeout - timeout_remaining (timeout);
}

/**
 * Indicates if the timeout has already been expired.
 *
 * @return 0 if not expired, otherwise expired
 */
bool_t timeout_expired (struct timeout * timeout)
{
    if (timeout_remaining (timeout) == 0)
        return TRUE;
    return FALSE;
}

/**
 * Makes the current timeout expired.
 */
void timeout_make_expired (struct timeout * timeout)
{
    timeout->timeout = 0;
}

/**
 * Calculates the difference (in ms) between two timestamps.
 */
static uint32_t timeout_diff (struct timespec * start, struct timespec * stop)
{
    uint64_t begin, end;

    /*@-type@*/
    begin = (uint64_t) start->tv_sec * 1000000000UL + start->tv_nsec;
    end = (uint64_t)   stop->tv_sec * 1000000000UL  + stop->tv_nsec;
    return (end - begin) / 1000000UL;
    /*@=type@*/
}

/**
 * Starts a new thread and increases thread counter.
 *
 * The new thread is created as "detached", so no join is needed
 * after it has been terminated.
 */
int thread_start_arg (thread_func function, void * arg)
{
    int ret = RC_OK;
    pthread_t thread;
    pthread_attr_t attr;

    if (function == NULL)
        return -EINVAL;

    MUTEX_LOCK (threads_mutex);
    if (pthread_attr_init (&attr))
        DBG (DBG_ERROR, "thread attribute set error");
    if (pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED))
        DBG (DBG_ERROR, "thread detach state set error");
    ret = pthread_create (&thread, &attr, function, arg);
    if (ret == 0)
        ++threads.running;
    MUTEX_UNLOCK (threads_mutex);
    return ret;
}

/**
 * Starts a new thread and increases thread counter.
 *
 * The new thread is created as "detached", so no join is needed
 * after it has been terminated.
 */
int thread_start (thread_func function)
{
    return thread_start_arg (function, NULL);
}

/**
 * Decreases running thread counter.
 */
void thread_decrease_ctr (void)
{
    MUTEX_LOCK (threads_mutex);
    if (threads.running)
        --threads.running;
    /* @note do nothing when there are no running threads - using
    this function is the responsibility of the caller */
    MUTEX_UNLOCK (threads_mutex);
}

/**
 * Toggles the thread exit flag thus signalling all running threads
 * the exit command.
 */
void thread_signal_exit (void)
{
    MUTEX_LOCK (threads_mutex);
    threads.signal_exit = TRUE;
    MUTEX_UNLOCK (threads_mutex);
}

/**
 * Checks if the threads should exit ("broadcasted message").
 *
 * @return TRUE  threads should exit
 * @return FALSE threads may go on
 */
bool_t thread_exit_cmd (void)
{
    bool_t ret;

    MUTEX_LOCK (threads_mutex);
    ret = threads.signal_exit;
    MUTEX_UNLOCK (threads_mutex);
    return ret;
}

/**
 * Checks whether all threads have exited.
 *
 * @return TRUE all threads have exited.
 */
bool_t thread_all_exited (void)
{
    bool_t ret = FALSE;

    MUTEX_LOCK (threads_mutex);
    if (threads.running == 0)
        ret = TRUE;
    MUTEX_UNLOCK (threads_mutex);
    return ret;
}

/**
 * Creates the structure.
 */
int avg_create (struct avg_st * avg, uint32_t entries)
{
    if (avg == NULL || entries == 0)
        return -EINVAL;
    avg->value = (uint32_t *) calloc (entries, sizeof (uint32_t));
    if (avg->value == NULL)
        return -ENOMEM;
    avg->size = entries;
    avg->ctr = 0;
    return RC_OK;
}

/**
 * Releases the structure.
 */
void avg_release (struct avg_st * avg)
{
    if (avg->value) {
        free (avg->value);
        avg->value = NULL;
    }
}

/**
 * Pushes a new value into the structure.
 */
void avg_push (struct avg_st * avg, uint32_t value)
{
    if (avg->ctr < avg->size)
        avg->value[avg->ctr++] = value;
    else {
        /* shift values, oldest is removed */
        memmove (avg->value, &(avg->value[1]), sizeof (uint32_t) * (avg->size - 1));
        avg->value[avg->size - 1] = value;
    }
}

/**
 * Resets structure contents.
 */
void avg_reset (struct avg_st * avg)
{
    avg->ctr = 0;
    memset (avg->value, 0, avg->size * sizeof (uint32_t));
}

/**
 * Calculates average.
 */
uint32_t avg_calc_avg (struct avg_st * avg)
{
    uint32_t pos, sum = 0;

    for (pos = 0; pos < avg->ctr; ++pos)
        sum += avg->value[pos];
    if (avg->ctr == 0)
        return 0;
    return sum / avg->ctr;
}

/**
 * Checks if the character device with the given name exists.
 */
int char_device_exists (char * filename)
{
    struct stat info;

    if (stat (filename, &info) == 0) {
        if (S_ISCHR (info.st_mode))
            return RC_OK;
    }
    return -ENODEV;
}

/**
 * Checks if the block device with the given name exists.
 */
int block_device_exists (char * filename)
{
    struct stat info;

    if (stat (filename, &info) == 0) {
        if (S_ISBLK (info.st_mode))
            return RC_OK;
    }
    return -ENODEV;
}

/**
 * Checks if the given file exists.
 */
int file_exists (char * filename)
{
    struct stat info;

    if (stat (filename, &info) == 0) {
        if (S_ISREG (info.st_mode))
            return RC_OK;
    }
    return -ENOENT;
}

/**
 * Gets file size.
 */
int file_size32 (char * filename, uint32_t * size)
{
    struct stat info;

    if (file_exists (filename) != RC_OK)
        return -ENOENT;

    if (stat (filename, &info) == 0) {
        *size = info.st_size;
        return RC_OK;
    }
    return -EINVAL;
}

/**
 * Returns with RC_OK if the given path points to a directory.
 */
int is_dir (char * path)
{
    struct stat info;

    if (stat (path, &info))
        return ERROR_GENERAL;
    if (S_ISDIR (info.st_mode))
        return RC_OK;
    return ERROR_GENERAL;
}

/**
 * Returns with RC_OK if the given path points to a file.
 */
int is_file (char * path)
{
    struct stat info;

    if (stat (path, &info))
        return ERROR_GENERAL;
    if (S_ISREG (info.st_mode))
        return RC_OK;
    return ERROR_GENERAL;
}

/**
 * Unmounts the given storage.
 */
int unmount_storage (char * mount_point)
{
    uint32_t try;

    /* soft unmount */
    for (try = 0; try < UNMOUNT_ATTEMPTS; ++try) {
        if (umount2 (mount_point, 0) == 0)
            return RC_OK;
        ms_sleep (UNMOUNT_WAIT);
    }

    /* agressive mode */
    for (try = 0; try < UNMOUNT_ATTEMPTS; ++try) {
        if (umount2 (mount_point, MNT_FORCE) == 0)
            return RC_OK;
        ms_sleep (UNMOUNT_WAIT);
    }
    return ERROR_GENERAL;
}

/**
 * Executes a shell command.
 */
int execute_command (const char * fmt, ...)
{
    int ret;
    va_list args;
    char * full_cmd_line = NULL;

    va_start (args, fmt);
    ret = vasprintf (&full_cmd_line, fmt, args);
    va_end (args);

    if (ret < 0)
        return ERROR_GENERAL;

    ret = system (full_cmd_line);
    free (full_cmd_line);

    if (ret == -1)
        return ERROR_GENERAL;

    return (WEXITSTATUS (ret));
}

/**
 * Reads a line from file
 */
int read_file_line(int fd, char * buf, int blen)
{
    int ret;
    int i;

    memset(buf, 0, blen);
    for (i = 0; i < blen; i++) {
        ret = read(fd, &buf[i], 1);
        if (ret != 1)
            break;
        if (buf[i] == '\n') {
            buf[i] = '\0';
            break;
        }
    }

    return i;
}

/**
 * Concatenate files
 *
 * @param dst Destinationfile descriptor
 * @param src File descriptor
 *
 * @return Error code (0 = OK)
 */

int filecat(int dst, int src)
{
    int ret, res = 0;
    off_t offs;
    char buf[512];

    offs = lseek(dst, 0, SEEK_END);
    if (offs == (off_t) - 1)
        return -1;

    offs = lseek(src, 0, SEEK_SET);
    if (offs == (off_t) - 1)
        return -1;

    while ( (ret = read(src, buf, 512)) > 0 ) {
        res += write(dst, buf, ret);
    }

    return 0;
}

/**
 * Returns a random number which is not equal to or greater than the limit.
 */
uint32_t lrandom (uint32_t limit)
{
    uint64_t num;

    num = random ();
    num *= limit;
    num /= RAND_MAX;
    return (uint32_t) num;
}

/**
 * @}
 */
