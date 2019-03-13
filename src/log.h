
/*  Copyright (C) 2007 by Matthew Alton  */

/*
 *  This file is part of Syster.
 *
 *  Syster is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Syster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*  $Id: log.h,v 1.1 2013/08/12 21:15:03 zma0472 Exp $  */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#if !defined(LOG_BUFSIZ)
#define LOG_BUFSIZ 8192
#endif /*  !defined(LOG_BUFSIZ)  */

#if !defined(ERR_BUFSIZ)
#define ERR_BUFSIZ 8192
#endif /*  !defined(ERR_BUFSIZ)  */

#if !defined(DBG_BUFSIZ)
#define DBG_BUFSIZ 8192
#endif /*  !defined(DBG_BUFSIZ)  */


extern char *log_label;

extern int debug;

#if !defined(CHOP)
#define CHOP(a)              \
do {                         \
    char *p;                 \
    for (p = (a); *p; p++) { \
        if ('\n' == *p) {    \
            *p = ' ';        \
        }                    \
    }                        \
} while (0)
#endif  /*  !defined(CHOP)  */

#if !defined(LOG)
#define LOG(...)                                                              \
do {                                                                          \
    int errno_save = errno;                                                   \
    time_t log_time = 0;                                                      \
    char buf[LOG_BUFSIZ];                                                     \
                                                                              \
    log_time = time(&log_time);                                               \
    snprintf(buf, LOG_BUFSIZ, "%s ", ctime(&log_time)); CHOP(buf);            \
    snprintf(buf+strlen(buf), LOG_BUFSIZ-strlen(buf), "%s(%d) %s(%d): ",      \
                                   log_label, getpid(), __FILE__, __LINE__);  \
    snprintf(buf+strlen(buf), LOG_BUFSIZ-strlen(buf),  __VA_ARGS__);          \
    snprintf(buf+strlen(buf), LOG_BUFSIZ-strlen(buf),  "\n");                 \
    fprintf(stdout, buf); fflush(NULL);                                       \
    errno = errno_save;                                                       \
} while (0)
#endif /*  !defined(LOG)  */

#if !defined(ERR)
#define ERR(...)                                                              \
do {                                                                          \
    int errno_save = errno;                                                   \
    time_t err_time = 0;                                                      \
    char buf[ERR_BUFSIZ];                                                     \
                                                                              \
    err_time = time(&err_time);                                               \
    snprintf(buf, ERR_BUFSIZ, "%s ", ctime(&err_time)); CHOP(buf);            \
    snprintf(buf+strlen(buf), LOG_BUFSIZ-strlen(buf), "%s(%d) %s(%d): ",      \
                                   log_label, getpid(), __FILE__, __LINE__);  \
    snprintf(buf+strlen(buf), ERR_BUFSIZ-strlen(buf),  __VA_ARGS__);          \
    snprintf(buf+strlen(buf), ERR_BUFSIZ-strlen(buf),  " errno=%d (%s)",      \
                                           errno_save, strerror(errno_save)); \
    snprintf(buf+strlen(buf), ERR_BUFSIZ-strlen(buf),  "\n");                 \
    fprintf(stderr, buf); fflush(NULL);                                       \
    errno = errno_save;                                                       \
} while (0)
#endif /*  !defined(ERR)  */

#include <netdb.h>
#include <sys/socket.h>

#if !defined(H_ERR)
#define H_ERR(...)                                                            \
do {                                                                          \
    int h_errno_save = h_errno;                                               \
    time_t err_time = 0;                                                      \
    char buf[ERR_BUFSIZ];                                                     \
                                                                              \
    err_time = time(&err_time);                                               \
    snprintf(buf, ERR_BUFSIZ, "%s ", ctime(&err_time)); CHOP(buf);            \
    snprintf(buf+strlen(buf), LOG_BUFSIZ-strlen(buf), "%s(%d) %s(%d): ",      \
                                   log_label, getpid(), __FILE__, __LINE__);  \
    snprintf(buf+strlen(buf), ERR_BUFSIZ-strlen(buf),  __VA_ARGS__);          \
    snprintf(buf+strlen(buf), ERR_BUFSIZ-strlen(buf),  " h_errno=%d (%s)",    \
                                      h_errno_save, hstrerror(h_errno_save)); \
    snprintf(buf+strlen(buf), ERR_BUFSIZ-strlen(buf),  "\n");                 \
    fprintf(stderr, buf); fflush(NULL);                                       \
    h_errno = h_errno_save;                                                   \
} while (0)
#endif /*  !defined(H_ERR)  */

#if !defined(DBG)
#define DBG(...)                                                              \
do {                                                                          \
    if (debug > 0) {                                                          \
        int errno_save = errno;                                               \
        time_t dbg_time = 0;                                                  \
        char buf[DBG_BUFSIZ];                                                 \
                                                                              \
        dbg_time = time(&dbg_time);                                           \
        snprintf(buf, DBG_BUFSIZ, "%s ", ctime(&dbg_time)); CHOP(buf);        \
        snprintf(buf+strlen(buf), LOG_BUFSIZ-strlen(buf), "%s(%d) %s(%d): ",  \
                                   log_label, getpid(), __FILE__, __LINE__);  \
        snprintf(buf+strlen(buf), DBG_BUFSIZ-strlen(buf),  __VA_ARGS__);      \
        snprintf(buf+strlen(buf), DBG_BUFSIZ-strlen(buf),  "\n");             \
        fprintf(stderr, buf); fflush(NULL);                                   \
        errno = errno_save;                                                   \
    }                                                                         \
} while (0)
#endif /*  !defined(DBG)  */

