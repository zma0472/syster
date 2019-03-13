
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
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

#include "syster.h"
#include "log.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: net_args.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

char *tstpath = NET_DEFAULT_TSTPATH;
char *pidpath = NET_DEFAULT_PIDPATH;
char *errpath = NET_DEFAULT_ERRPATH;
char *logpath = NET_DEFAULT_LOGPATH;

int
args(int argc, char **argv)
{
 int c = 0;
 char *endptr = NULL;

 DBG("args(argc=%d, ...) called", argc);

 while ((c = getopt(argc, argv, "Dd:p:t:n:e:l:E:")) != -1) {
     DBG("getopt(argc=%d, ..., \"Dd:p:t:n:e:l:E:\") = %c", argc, c);
     switch (c) {
         case 'D': debug = 1;
                   DBG("Set debug = %d", debug);
                   break;
         case 'd': delay = strtoul(optarg, &endptr, 0);
                   if (ULONG_MAX == delay) {
                       ERR("delay = (ULONG_MAX=%d)", ULONG_MAX);
                       return (-1);
                   } else if ( ('\0' !=  *optarg) &&
                               ('\0' ==  *endptr) &&
                               (delay > NET_DELAY_MIN ) ) {
                       DBG("Set delay = %d", delay); 
                       break;
                   } else {
                       ERR("Invalid delay=%d", delay);
                       return (-1);
                   }
         case 'E': echo_port = strtoul(optarg, &endptr, 0);
                   if (SHRT_MAX < echo_port) {
                       ERR("echo_port=%d > SHRT_MAX=%d", echo_port, SHRT_MAX);
                       return (-1);
                   } else if ( ('\0' !=  *optarg) &&
                               ('\0' ==  *endptr) &&
                               (echo_port > 0) ) {
                       DBG("Set echo_port = %d", echo_port);
                       break;
                   } else {
                       ERR("Invalid echo_port=%d", echo_port);
                       return (-1);
                   }
         case 'p': pidpath = optarg;
                   DBG("Set pidpath = %s", optarg);
                   break;
         case 't': tstpath = optarg;
                   DBG("Set tstpath = %s", optarg);
                   break;
         case 'e': errpath = optarg;
                   DBG("Set errpath = %s", optarg);
                   break;
         case 'l': logpath = optarg;
                   DBG("Set logpath = %s", optarg);
                   break;
         case 'n': echo_node = optarg;
                   DBG("Set echo_node = %s", optarg);
                   break;
         default:  usage();
                   ERR("Invalid command switch: %c", c);
                   return (-1);
     }
 }
 DBG("return (%d)", 0);
 return (0);
}

