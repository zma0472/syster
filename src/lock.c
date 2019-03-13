
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


#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: lock.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

enum { BUF_SIZ = 32 };

/*
 * The lock function attempts to make sure that only one instance of the
 * program is running at a time.  We use the standard open(2) mechanism to
 * create a lockfile.  We then store our PID in the file and close the
 * lockfile descriptor.  Note that this version of lock() is not suitable for
 * use on lockfiles in an NFS-mounted filesystem.
 */

int
lock(char *path)
{
 int    fd    = -1, val = -1;
 int    flags = (O_WRONLY|O_CREAT|O_EXCL|O_SYNC);
 mode_t mode  = (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
 char   str[BUF_SIZ];
 
 DBG("lock(path=\"%s\") called", path);

 if ((fd = open(path, flags, mode)) < 0) {
     ERR("open(path=\"%s\") FAIL", path);
 } else {
     if (snprintf(str, BUF_SIZ, "%d", getpid()) < 1) {
         ERR("snprintf(str=\"%s\", BUF_SIZ=%d, \"%d\") FAIL",
                                                       str, BUF_SIZ, getpid());
     } else if (strlen(str) == writen(fd, str, strlen(str))) {
         val = 0;
         DBG("Set val=%d", val);
     } else {
         ERR("writen(fd=%d, str=\"%s\", strln(str)=%d) FAIL",
                                                         fd, str, strlen(str));
     }
     for(;;) {
         if (close(fd) < 0) {
             if (EINTR != errno) {
                 ERR("close(fd=%d) FAIL", fd);
                 val = -1; break;
             }
         } else {
             DBG("close(fd=%d) OK", fd);
             break;
         }
     }
     if (val < 0) {
         if (unlink(path) < 0) {
             ERR("unlink(path=\"%s\") FAIL", path);
         }
     }
 }
 DBG("lock(path=\"%s\") returning %d", path, val);
 return (val);
}

/*
 * The unlock function attempts to read the lockfile created by the lock
 * function.  If the lockfile is found to contain the PID of the running
 * process, then we unlink it.  Note that this version of unlock() is not
 * suitable for use on NFS-mounted filesystems.  If close(2) fails on the file
 * descriptor, we attempt to unlink(2) anyway.
 */

int
unlock(void)
{
 int    fd = -1, val = -1, flags = O_RDONLY;
 char   str[BUF_SIZ];
 
 DBG("unlock() called, pidpath=\"%s\"", pidpath);

 if ((fd = open(pidpath, flags)) < 0) {
     ERR("open(pidpath=\"%s\") FAIL", pidpath);
 } else {
     if (readn(fd, str, BUF_SIZ-1) > 0) {
         str[BUF_SIZ-1] = '\0';
         if (getpid() == atol(str)) {
             DBG("getpid()=%d == atol(str=\"%s\")", getpid(), str);
             val = 0;
         }
     }
     for(;;) {
         DBG("Calling close(fd=%d)", fd);
         if (close(fd) < 0) {
             if (EINTR != errno) {
                 ERR("close(fd=%d) FAIL", fd);
                 break;
             }
         } else {
             DBG("close(fd=%d) OK", fd);
             break;
         }
     }
 }
 if (0 == val) {
     DBG("Calling unlink(pidpath=\"%s\")", pidpath);
     if (unlink(pidpath) < 0) {
         ERR("unlink(pidpath=\"%s\" FAIL", pidpath); val = -1;
     }
 }
 DBG("unlock() returning val=%d", val);
 return (val);
}

