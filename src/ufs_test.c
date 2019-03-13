
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

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: ufs_test.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

char *log_label = "UFS";

void
test(void)
{
 int     f     = -1;
 int     flags = (O_WRONLY|O_SYNC|O_CREAT|O_APPEND);
 mode_t  mode  = (S_IRUSR|S_IWUSR);
 time_t  t     = -1;
 int     first = 1;

 char    buf[64];

 DBG("test() called, log_label=\"%s\"", log_label);

 while (0 == halt) {
     if (0 == first) {
         DBG("first=%d, calling sleep(%d)", first, delay);
         sleep(delay);
     }
     first = 0;
     DBG("Set first=%d", first);
     DBG("open(tstpath=\"%s\", ...)", tstpath);
     if ((f = open(tstpath, flags, mode)) < 0) {
         ERR("open(tstpath=\"%s\")", tstpath);
         continue;
     }
     DBG("%s", "time(NULL)");
     if ((t = time(NULL)) < 0) {
         ERR("%s FAIL", "time(NULL)");
         continue;
     }
     DBG("%s", "snprintf(...)");
     if (snprintf(buf, 64, "%d %d\n", delay, (int)t) < 0) {
         ERR("snprintf(buf=\"%s\", 64, delay=%d, t=%d", buf, delay, t);
         continue;
     }
     DBG("writen(f=%d, ..., strlen(buf)=%d)", f, strlen(buf));
     if (writen(f, buf, strlen(buf)) < 0) {
         ERR("writen(f=%d, buf=\"%s\", strlen(buf)=%d) FAIL",
                                                          f, buf, strlen(buf));
         continue;
     }
     while (close(f) < 0) {
         ERR("close(f=%d) FAIL", f);
         DBG("sleep(%d)", delay);
         sleep(delay);
     }
 }
 DBG("halt=%d, return", halt);
 return;
}

