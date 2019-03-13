

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
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: daemon.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

int
streams(void)
{
 long  fdmax = sysconf(_SC_OPEN_MAX), i = -1;

 DBG("Closing %d file descriptors excluding %d, %d, %d",
                            fdmax, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
 for (i = 0; i < fdmax; i++) {
     if ( (STDIN_FILENO  != i)  &&
          (STDOUT_FILENO != i)  &&
          (STDERR_FILENO != i) ) {
         (void)close(i);
     }
 }
 if (NULL == freopen("/dev/null", "r", stdin)) {
     ERR("freopen(path=\"%s\", mode=\"%s\", stream=stdin) FAIL",
                                                "/dev/null", "r"); return (-1);
 } else {
     DBG("freopen(path=\"%s\", mode=\"%s\", stream=stdin) OK",
                                                "/dev/null", "r");
 }
 if (NULL == freopen(logpath, "a", stdout)) {
     ERR("freopen(path=\"%s\", mode=\"%s\", stream=stdout) FAIL",
                                                    logpath, "a"); return (-1);
 } else {
     DBG("freopen(path=\"%s\", mode=\"%s\", stream=stdout) OK", logpath, "a");
 }
 fprintf(stderr, "\n%s\n%s\n\n", syster_release, copyleft);
 if (NULL == freopen(errpath, "a", stderr)) {
     ERR("freopen(path=\"%s\", mode=\"%s\", stream=stderr) FAIL",
                                                    errpath, "a"); return (-1);
 } else {
     DBG("freopen(path=\"%s\", mode=\"%s\", stream=stderr) OK", errpath, "a");
 }
 DBG("return(%d)", 0);
 return (0);
}

int
background(void)
{
 pid_t pid   = -1;

 DBG("background(%s) called", "void");

 DBG("First fork() %s", "...");
 if ((pid = fork()) < (pid_t)0) {
     ERR("%s FAIL", "fork()"); return (-1);
 }
 if (0 != pid) {
     DBG("exit(%d)", 0);
     exit(0);
 }
 if (setsid() < (pid_t)0) {
     ERR("%s FAIL", "setsid()"); return (-1);
 }
 DBG("Second fork() %s", "...");
 if ((pid = fork()) < (pid_t)0) {
     ERR("%s FAIL", "fork()"); return (-1);
 }
 if (0 != pid) {
     DBG("_exit(%d)", 0);
     _exit(0);
 }
 if (chdir("/") < 0) {
     ERR("chdir(path=\"%s\") FAIL", "/"); return (-1);
 }
 DBG("chdir(%s)", "\"/\"");
 DBG("umask(%s)", "022");
 (void)umask(022);
 DBG("return(%d)", 0);
 return (0);
}

