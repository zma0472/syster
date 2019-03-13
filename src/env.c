


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
"$Id: env.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

#if !defined(ENV_BUFSIZ)
#define ENV_BUFSIZ 64
#endif /*  !defined(ENV_BUFSIZ)  */

int           debug = SYSTER_DEFAULT_DEBUG;
unsigned long delay = SYSTER_DEFAULT_DELAY;

int
env_common(void)
{
 char *t      = NULL;
 char *endptr = NULL;

 DBG("%s called", "env_common()");
 
 if (NULL != (t = getenv("SYSTER_DEBUG"))) {
     if ( (0 == strcmp(t, "ON")) || (0 == strcmp(t, "on")) ) {
         debug = 1;
     } else if ( (0 == strcmp(t, "OFF")) || (0 == strcmp(t, "off")) ) {
         debug = 0;
     } else {
         ERR("Invalid value SYSTER_DEBUG=%s", t);
         DBG("return (-1)");
         return (-1);
     }
     DBG("Set debug=%d on SYSTER_DEBUG=%s", debug, t);
 }
 if (NULL != (t = getenv("SYSTER_DELAY"))) {
     delay = strtoul(t, &endptr, 0);
     if (ULONG_MAX == delay) {
         ERR("strtoul(t=\"%s\") returned ULONG_MAX", t); return (-1);
     } else if ( ('\0' ==  *t)      ||
                 ('\0' !=  *endptr) ||
                 (delay < SYSTER_DELAY_MIN) ) {
         ERR("strtoul(t=\"%s\") < (SYSTER_DELAY_MIN=%d)", t, SYSTER_DELAY_MIN);
         DBG("return (-1)");
         return (-1);
     }
     DBG("Set delay=%d on SYSTER_DEBUG=%s", delay, t);
 }
 DBG("delay=%d  debug=%d", delay, debug); DBG("return (0)");
 return (0);
}

