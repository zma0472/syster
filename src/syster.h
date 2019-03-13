
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

/*  $Id: syster.h,v 1.1 2013/08/12 21:15:03 zma0472 Exp $  */

#include <sys/types.h>

/*
 *  Default global settings.
 */

#if !defined(SYSTER_DEFAULT_DEBUG)
#define SYSTER_DEFAULT_DEBUG 0
#endif  /*  !defined(SYSTER_DEFAULT_DEBUG) */

#if !defined(SYSTER_DEFAULT_DELAY)
#define SYSTER_DEFAULT_DELAY 6
#endif  /*  !defined(SYSTER_DEFAULT_DELAY) */

#if !defined(SYSTER_DELAY_MIN)
#define SYSTER_DELAY_MIN 1
#endif  /*  !defined(SYSTER_DELAY_MIN) */

/*
 *  Default settings for ufs test.
 */

#if !defined(UFS_DEFAULT_TSTPATH)
#define UFS_DEFAULT_TSTPATH "#PREFIX#/var/mon/syster_ufs"
#endif  /*  !defined(UFS_DEFAULT_TSTPATH) */

#if !defined(UFS_DEFAULT_PIDPATH)
#define UFS_DEFAULT_PIDPATH "#PREFIX#/var/lock/syster_ufs.pid"
#endif  /*  !defined(UFS_DEFAULT_PIDPATH) */

#if !defined(UFS_DEFAULT_LOGPATH)
#define UFS_DEFAULT_LOGPATH "/dev/null"
#endif  /*  !defined(UFS_DEFAULT_LOGPATH) */

#if !defined(UFS_DEFAULT_ERRPATH)
#define UFS_DEFAULT_ERRPATH "/dev/null"
#endif  /*  !defined(UFS_DEFAULT_ERRPATH) */

/*  Test iteration delay in seconds.  */
#if !defined(UFS_DELAY_MIN)
#define UFS_DELAY_MIN 1
#endif  /*  !defined(UFS_DELAY_MIN) */

/*  Default test iteration delay in seconds.  */
#if !defined(UFS_DEFAULT_DELAY)
#define UFS_DEFAULT_DELAY 6
#endif  /*  !defined(UFS_DEFAULT_DELAY) */

/*
 *  Default settings for cpc test.
 */

#if !defined(CPC_DEFAULT_TSTPATH)
#define CPC_DEFAULT_TSTPATH "#PREFIX#/var/mon/syster_cpc"
#endif  /*  !defined(CPC_DEFAULT_TSTPATH) */

#if !defined(CPC_DEFAULT_PIDPATH)
#define CPC_DEFAULT_PIDPATH "#PREFIX#/var/lock/syster_cpc.pid"
#endif  /*  !defined(CPC_DEFAULT_PIDPATH) */

#if !defined(CPC_DEFAULT_LOGPATH)
#define CPC_DEFAULT_LOGPATH "/dev/null"
#endif  /*  !defined(CPC_DEFAULT_LOGPATH) */

#if !defined(CPC_DEFAULT_ERRPATH)
#define CPC_DEFAULT_ERRPATH "/dev/null"
#endif  /*  !defined(CPC_DEFAULT_ERRPATH) */

/*  Test iteration delay in seconds.  */
#if !defined(CPC_DELAY_MIN)
#define CPC_DELAY_MIN 1
#endif  /*  !defined(CPC_DELAY_MIN) */

/*  Default test iteration delay in seconds.  */
#if !defined(CPC_DEFAULT_DELAY)
#define CPC_DEFAULT_DELAY 6
#endif  /*  !defined(CPC_DEFAULT_DELAY) */

/*
 *  Default settings for mem test.
 */

#if !defined(MEM_DEFAULT_TSTPATH)
#define MEM_DEFAULT_TSTPATH "#PREFIX#/var/mon/syster_mem"
#endif  /*  !defined(MEM_DEFAULT_TSTPATH) */

#if !defined(MEM_DEFAULT_PIDPATH)
#define MEM_DEFAULT_PIDPATH "#PREFIX#/var/lock/syster_mem.pid"
#endif  /*  !defined(MEM_DEFAULT_PIDPATH) */

#if !defined(MEM_DEFAULT_LOGPATH)
#define MEM_DEFAULT_LOGPATH "/dev/null"
#endif  /*  !defined(MEM_DEFAULT_LOGPATH) */

#if !defined(MEM_DEFAULT_ERRPATH)
#define MEM_DEFAULT_ERRPATH "/dev/null"
#endif  /*  !defined(MEM_DEFAULT_ERRPATH) */

/*  Test iteration delay in seconds.  */
#if !defined(MEM_DELAY_MIN)
#define MEM_DELAY_MIN 1
#endif  /*  !defined(MEM_DELAY_MIN) */

/*  Default test iteration delay in seconds.  */
#if !defined(MEM_DEFAULT_DELAY)
#define MEM_DEFAULT_DELAY 6
#endif  /*  !defined(MEM_DEFAULT_DELAY) */

/*
 *  Default settings for net test.
 */

#if !defined(NET_DEFAULT_TSTPATH)
#define NET_DEFAULT_TSTPATH "#PREFIX#/var/mon/syster_net"
#endif  /*  !defined(NET_DEFAULT_TSTPATH) */

#if !defined(NET_DEFAULT_PIDPATH)
#define NET_DEFAULT_PIDPATH "#PREFIX#/var/lock/syster_net.pid"
#endif  /*  !defined(NET_DEFAULT_PIDPATH) */

#if !defined(NET_DEFAULT_LOGPATH)
#define NET_DEFAULT_LOGPATH "/dev/null"
#endif  /*  !defined(NET_DEFAULT_LOGPATH) */

#if !defined(NET_DEFAULT_ERRPATH)
#define NET_DEFAULT_ERRPATH "/dev/null"
#endif  /*  !defined(NET_DEFAULT_ERRPATH) */

#if !defined(NET_DEFAULT_ECHO_NODE)
#define NET_DEFAULT_ECHO_NODE "localhost"
#endif  /*  !defined(NET_DEFAULT_ECHO_NODE) */

#if !defined(NET_DEFAULT_ECHO_PORT)
#define NET_DEFAULT_ECHO_PORT 7
#endif  /*  !defined(NET_DEFAULT_ECHO_PORT) */

/*  Test iteration delay in seconds.  */
#if !defined(NET_DELAY_MIN)
#define NET_DELAY_MIN 1
#endif  /*  !defined(NET_DELAY_MIN) */

/*  Default test iteration delay in seconds.  */
#if !defined(NET_DEFAULT_DELAY)
#define NET_DEFAULT_DELAY 6
#endif  /*  !defined(NET_DEFAULT_DELAY) */

/*  Global variables.  */

extern char *syster_release;
extern char *copyleft;

extern char *pidpath;
extern char *logpath;
extern char *tstpath;
extern char *errpath;
extern char *dbgpath;

extern char *echo_node;

extern unsigned long delay;
extern           int halt;
extern           int echo_port;

/*  Function declarations.  */

extern int     args(int argc, char **argv);
extern int     lock(char *path);
extern int     env(void);
extern int     env_common(void);
extern int     background(void);
extern int     unlock(void);
extern void    version(void);
extern void    test(void);
extern void    usage(void);
extern ssize_t writen(int fd, const void *vptr, size_t n);
extern ssize_t readn(int fd, const void *vptr, size_t n);
extern int     signals(void);
extern int     streams(void);
