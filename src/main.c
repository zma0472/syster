
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
#include <string.h>

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: main.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

char *syster_release = "Syster v.1.0.0 BETA";
char *copyleft       = "Copyright (C) 2007 by Matthew Alton";

int
main(int argc, char **argv)
{
 if ( (2 == argc) && (0 == strcmp("-V", argv[1])) ) {
     DBG("argc=%d, argv[1]=\"%s\" exit(0)", argc, argv[1]);
     version(); exit(0);
 } 
 DBG("Calling %s", "env()");
 if (env() < 0) {
     ERR("%s FAIL", "env()");
     DBG("exit(%d)", 2);
     exit(2);
 }
 DBG("Calling args(argc=%d)", argc);
 if (args(argc, argv) < 0) {
     ERR("args(argc=%d) FAIL", argc);
     DBG("exit(%d)", 2);
     exit(2);
 }
 DBG("Calling %s", background());
 if (background() < 0) {
     ERR("%s FAIL", "background()");
     DBG("exit(%d)", 2);
     exit(2);
 }
 DBG("Calling %s", "signals()");
 if (signals() < 0) {
     ERR("%s FAIL", "signals()");
     DBG("_exit(%d)", 2);
     _exit(2);
 }
 DBG("Calling lock(pidpath=\"%s\")", pidpath);
 if (lock(pidpath) < 0) {
     ERR("lock(pidpath=%s) FAIL", pidpath);
     DBG("exit(%d)", 2);
     _exit(2);
 }
 DBG("Calling %s", "streams()");
 if (streams() < 0) {
     ERR("%s FAIL", "streams()");
     DBG("_exit(%d)", 2);
     DBG("Calling unlock(), pidpath=\"%s\"", pidpath);
     if (unlock() < 0) {
         ERR("%s FAIL", "unlock()"); _exit(2);
     }
     _exit(2);
 }
 LOG("%s Initialized", syster_release);
 DBG("Calling %s", "test()");
 test();
 DBG("Calling unlock(), pidpath=\"%s\"", pidpath);
 if (unlock() < 0) {
     ERR("%s FAIL", "unlock()"); _exit(2);
 }
 DBG("halt=%d", halt);
 LOG("Test %s terminated on halt=%d", log_label, halt);
 DBG("return %d", 0);
 return (0);
}

