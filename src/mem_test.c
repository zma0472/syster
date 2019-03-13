
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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: mem_test.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

#define TEST_BYTE(a)                                                          \
do {                                                                          \
 rslt = 0;                                                                    \
 DBG("set rslt=%d", rslt);                                                    \
 *(p+(i*psize)) = (char)(a);                                                  \
 DBG("set *((p=%d)+((i=%d)*(psize=%d))) = (a)='%c'", p, i, psize, (a));       \
 if ((char)(a) != *(p+(i*psize))) {                                           \
     ERR("memory test comparison FAIL");                                      \
     DBG("'%c' != (a)='%c'", *(p+(i*psize)), (a));                            \
 } else {                                                                     \
     rslt = 1;                                                                \
     DBG("set rslt=%d", rslt);                                                \
 }                                                                            \
} while (0)

char *log_label = "MEM";

void
test(void)
{
 int     f     = -1;
 int     flags = (O_WRONLY|O_SYNC|O_CREAT|O_APPEND);
 mode_t  mode  = (S_IRUSR|S_IWUSR);
 time_t  t     = -1;
 char   *p     = NULL;
 long    psize = -1;
 int     pnum  = 8;
 int     i     = -1;
 int     rslt  = 0;

 char    buf[64];

 DBG("%s called", "test()");
 LOG("test loop, log_label=\"%s\"", log_label);

 while (0 == halt) {
     DBG("calling sysconf(%s)", "_SC_PAGESIZE");
     if ((psize = sysconf(_SC_PAGESIZE)) < 0) {
         ERR("sysconf(_SC_PAGESIZE) = %ld", psize);
         halt = 1;
         DBG("set halt=%d on sysconf(_SC_PAGESIZE) failure", halt);
     }
     DBG("sysconf(_SC_PAGESIZE) = %ld", psize);
     DBG("calling malloc((size_t)%d)", (psize*pnum));
     if (NULL == (p = malloc((size_t)(psize*pnum)))) {
         ERR("malloc(%d) FAIL", (size_t)(psize*pnum));
     } else {
         for (i = 0; i < pnum; i++) {
             TEST_BYTE('A');
             if (0 == rslt) {
                 DBG("break on rslt=%d", rslt); break;
             }
             TEST_BYTE('B');
         }
         DBG("Calling free(p=%d)", p); free(p);
         if (0 == rslt) {
             DBG("continue on rslt=%d", rslt);
             continue;
         }
         DBG("Calling open(tstpath=\"%s\")", tstpath);
         if ((f = open(tstpath, flags, mode)) < 0) {
             ERR("open(tstpath=\"%s\") FAIL", tstpath);
         } else {
             DBG("f=%d", f);
             DBG("Calling %s", "time(NULL)");
             if ((t = time(NULL)) < 0) {
                 ERR("%s FAIL", "time(NULL)");
             } else {
                 if (snprintf(buf, 64, "%d %d\n", delay, (int)t) < 0) {
                     ERR("snprintf(buf=\"%s\", 64, delay=%d, t=%d",
                                                                buf, delay, t);
                 } else {
                     DBG("Calling writen(f=%d, buf=\"%s\", strlen(buf)=%d)",
                                                          f, buf, strlen(buf));
                     if (writen(f, buf, strlen(buf)) < 0) {
                         ERR("writen(f=%d, buf=\"%s\", strlen(buf)=%d) FAIL",
                                                          f, buf, strlen(buf));
                     }
                 }
             }
             DBG("Calling close(f=%d)", f);
             while (close(f) < 0) {
                 ERR("close(f=%d) FAIL", f);
                 DBG("Calling sleep(delay=%d)", delay);
                 sleep(delay);
             }
         }
     }
     DBG("Calling sleep(delay=%d)", delay);
     sleep(delay);
 }
 DBG("return, halt = %d", halt);
 return;
}

