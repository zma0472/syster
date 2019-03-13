
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: cpc_test.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

char *log_label = "CPC";

void
test(void)
{
 int     f     = -1;
 int     flags = (O_WRONLY|O_SYNC|O_CREAT|O_APPEND);
 mode_t  mode  = (S_IRUSR|S_IWUSR);
 time_t  t     = -1;
 pid_t   pid   = -1;
 pid_t   wpid  = -1;

 char    buf[64];

 DBG("CPC test(%s) called", "void");

 while (0 == halt) {
     if((pid = fork()) < 0) {
         ERR("fork() FAIL pid=%d", pid);
     } else if (0 == pid) {
         if ((f = open(tstpath, flags, mode)) < 0) {
             ERR("open(tstpath=\"%s\")", tstpath);
         } else {
             if ((t = time(NULL)) < 0) {
                 ERR("%s FAIL", "time(NULL)");
             } else {
                 if (snprintf(buf, 64, "%d %d\n", delay, (int)t) < 0) {
                     ERR("snprintf(buf=\"%s\", 64, delay=%d, t=%d",
                                                                buf, delay, t);
                 } else {
                     if (writen(f, buf, strlen(buf)) < 0) {
                         ERR("writen(f=%d, buf=\"%s\", strlen(buf)=%d) FAIL",
                                                          f, buf, strlen(buf));
                     }
                 }
             }
             DBG("(void)close(f=%d)", f);
             (void)close(f);
         }
         DBG("_exit(%d)", 0);
         _exit(0);
     } else {
         if (pid != (wpid = waitpid(pid, NULL, 0))) {
             ERR("waitpid(pid=%d, NULL, 0) = %d", pid, wpid);
         } else {
             DBG("waitpid(pid=%d, NULL, 0) = %d", pid, wpid);
         }
     }
     DBG("sleep(%d)", delay);
     sleep(delay);
 }
 DBG("CPC test(%s) returning", "void");
 return;
}

