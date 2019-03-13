
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
#include <signal.h>

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: signal.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

int halt = 0;

void
set_halt(int signo)
{
 DBG("set_halt(signo=%d) called", signo);
 halt = 1;

 LOG("Set halt=%d on signal=%d", halt, signo); return;
}

void
toggle_debug(int signo)
{
 DBG("toggle_debug(signo=%d) called", signo);
 debug = (1 == debug) ? 0 : 1;
 DBG("toggle_debug(signo=%d) called", signo);

 LOG("Set debug=%d on signal=%d", debug, signo); return;
}

int
signals(void)
{
 struct sigaction halt_action, toggle_action, ignore_action;

 halt_action.sa_handler = set_halt; halt_action.sa_flags = 0;
 sigemptyset(&halt_action.sa_mask);

 toggle_action.sa_handler = toggle_debug; toggle_action.sa_flags = 0;
 sigemptyset(&toggle_action.sa_mask);

 ignore_action.sa_handler = SIG_IGN; ignore_action.sa_flags = 0;
 sigemptyset(&ignore_action.sa_mask);

 /*
  *  It's a good idea to ignore SIGPIPE in daemons.
  */
 DBG("sigaction(%s)", "SIGPIPE");
 if (sigaction(SIGPIPE, &ignore_action, NULL) < 0) {
     ERR("sigaction(%s) FAIL", "SIGPIPE");
     return (-1);
 }
 /*
  *  We might want to implement a reconfigure on HUP later on.
  */
 DBG("sigaction(%s)", "SIGHUP");
 if (sigaction(SIGHUP,  &ignore_action, NULL) < 0) {
     ERR("sigaction(%s) FAIL", "SIGHUP");
     return (-1);
 }
 DBG("sigaction(%s)", "SIGINT");
 if (sigaction(SIGINT,  &halt_action, NULL) < 0) {
     ERR("sigaction(%s) FAIL", "SIGINT");
     return (-1);
 }
 DBG("sigaction(%s)", "SIGTERM");
 if (sigaction(SIGTERM, &halt_action, NULL) < 0) {
     ERR("sigaction(%s) FAIL", "SIGTERM");
     return (-1);
 }
 DBG("sigaction(%s)", "SIGUSR2");
 if (sigaction(SIGUSR2, &toggle_action, NULL) < 0) {
     ERR("sigaction(%s) FAIL", "SIGUSR2");
     return (-1);
 }
 DBG("return (%d)", 0);
 return (0);
}

