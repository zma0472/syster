


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
#include <string.h>
#include <limits.h>

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: net_env.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

#if !defined(ENV_BUFSIZ)
#define ENV_BUFSIZ 64
#endif /*  !defined(ENV_BUFSIZ)  */

char *echo_node = NET_DEFAULT_ECHO_NODE;
int   echo_port = NET_DEFAULT_ECHO_PORT;

int
env(void)
{
 char *t      = NULL;
 char *endptr = NULL;

 DBG("%s called", "env()");

 if (env_common() < 0) {
     ERR("%s FAIL", "env_common()");
     DBG("return (-1)");
     return (-1);
 }
 if (NULL != (t = getenv("SYSTER_NET_DEBUG"))) {
     if (0 == strcasecmp(t, "ON")) {
         debug = 1;
     } else if (0 == strcasecmp(t, "OFF")) {
         debug = 0;
     } else {
         ERR("Invalid value SYSTER_NET_DEBUG=%s", t);
         DBG("return (-1)");
         return (-1);
     }
     DBG("Set debug=%d on SYSTER_NET_DEBUG=%s", debug, t);
 }
 if (NULL != (t = getenv("SYSTER_NET_PIDPATH"))) {
     pidpath = t;
     DBG("Set pidpath=\"%s\" from env SYSTER_NET_PIDPATH", pidpath);
 }
 if (NULL != (t = getenv("SYSTER_NET_TESTPATH"))) {
     tstpath = t;
     DBG("Set tstpath=\"%s\" from env SYSTER_NET_TESTPATH", tstpath);
 }
 if (NULL != (t = getenv("SYSTER_NET_LOGPATH"))) {
     logpath = t;
     DBG("Set logpath=\"%s\" from env SYSTER_NET_LOGPATH", logpath);
 }
 if (NULL != (t = getenv("SYSTER_NET_ERRPATH"))) {
     errpath = t;
     DBG("Set errpath=\"%s\" from env SYSTER_NET_ERRPATH", errpath);
 }
 if (NULL != (t = getenv("SYSTER_NET_DELAY"))) {
     delay = strtoul(t, &endptr, 0);
     DBG("Set delay=%d from env SYSTER_NET_DELAY", delay);
     if (ULONG_MAX == delay) {
         ERR("strtoul(t=\"%s\") returned ULONG_MAX", t);
         DBG("return (%d)", -1);
         return (-1);
     } else if ( ('\0' ==  *t)      ||
                 ('\0' !=  *endptr) ||
                 (delay < NET_DELAY_MIN) ) {
         ERR("strtoul(t=\"%s\") < (NET_DELAY_MIN=%d)", t, NET_DELAY_MIN);
         DBG("return (%d)", -1);
         return (-1);
     }
 }
 if (NULL != (t = getenv("SYSTER_NET_ECHO_NODE"))) {
     echo_node = t;
     DBG("Set echo_node=\"%s\" from env SYSTER_NET_ECHO_NODE", echo_node);
 }
 if (NULL != (t = getenv("SYSTER_NET_ECHO_PORT"))) {
     echo_port = strtoul(t, &endptr, 0);
     DBG("Set echo_port=%d from env SYSTER_NET_ECHO_PORT", echo_port);
     if (ULONG_MAX == echo_port) {
         ERR("echo_port=strtoul(t=\"%s\") returned ULONG_MAX", t);
         DBG("return (%d)", -1);
         return (-1);
     } else if ( ('\0' ==  *t) || ('\0' !=  *endptr) ) {
         ERR("echo_port=strtoul(t=\"%s\") FAIL", t);
         DBG("return (%d)", -1);
         return (-1);
     }
 }
 DBG("return (%d)", 0);
 return (0);
}

