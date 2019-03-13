
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
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: net_test.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

char *log_label = "NET";

#define CLOSE(a)                       \
do {                                   \
    DBG("close(%d)", (a));             \
    while (close( (a) ) < 0) {         \
        ERR("close(fd=%d) FAIL", (a)); \
        if (EINTR != errno) {          \
            DBG("sleep(%d)", 1);       \
            sleep(1);                  \
        } else {                       \
            break;                     \
        }                              \
    }                                  \
} while (0)

#define SHUTDOWN                                            \
do {                                                        \
    DBG("shutdown(fd=%d, SHUT_RDWR)", sockfd);              \
    if (shutdown(sockfd, SHUT_RDWR) < 0) {                  \
        ERR("shutdown(fd=%d, how=SHUT_RDWR) FAIL", sockfd); \
    }                                                       \
} while (0)

void
test(void)
{
 int     f      = -1;
 int     flags  = (O_WRONLY|O_SYNC|O_CREAT|O_APPEND);
 mode_t  mode   = (S_IRUSR|S_IWUSR);
 time_t  t      = -1;
 int     sockfd = -1;
 int     n      = -1;
 size_t  ret    = 0;
 int     first  = 1;

 struct sockaddr_in  servaddr;
 struct hostent     *echo_he;

 char    buf[64];

 DBG("test() called, log_label=\"%s\"", log_label);

 bzero(&servaddr, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_port   = htons(echo_port);

 /*
  *  Create a socket connection to an echo server.  Write a timestamp to the
  *  socket.  Read from the socket.  Compare the received string to the
  *  transmitted string.  Write the timestamp to the test file.  Sleep for one
  *  delay cycle.
  */

 while (0 == halt) {
     if (0 == first) {
         DBG("first=%d, calling sleep(%d)", first, delay);
         sleep(delay);
     }
     first = 0;
     DBG("Set first=%d", first);
     DBG("gethostbyname(\"echo_node=%s\")", echo_node);
     if (NULL == (echo_he = gethostbyname(echo_node))) {
         H_ERR("gethostbyname(\"%s\") FAIL", echo_node);
         continue;
     } 
     DBG("socket(AF_INET, SOCK_STREAM, %d)", 0);
     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
         ERR("socket(AF_INET, SOCK_STREAM, 0) FAIL");
         continue;
     } 
     DBG("%s", "inet_pton(AF_INET, ...)");
     if (inet_pton(AF_INET, inet_ntoa(*((struct in_addr *)echo_he->h_addr)),
                                                    &servaddr.sin_addr) <= 0) {
         ERR("inet_pton(\"%s\") FAIL", 
              inet_ntoa(*((struct in_addr *)echo_he->h_addr)));
         CLOSE(sockfd); continue;
     } 
     DBG("connect(sockfd=%d, ...)", sockfd);
     if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
         ERR("connect(sockfd=%d, &servaddr, sizeof(servaddr)) FAIL", sockfd);
         CLOSE(sockfd); continue;
     } 
     DBG("%s", "time(NULL)");
     if ((t = time(NULL)) < 0) {
         ERR("time(NULL) FAIL");
         SHUTDOWN; CLOSE(sockfd); continue;
     }
     DBG("%s", "snprintf(...)");
     if (snprintf(buf, 64, "%d %d\n", delay, (int)t) < 0) {
         ERR("snprintf(delay=%d, t=%d) FAIL", delay, (int)t);
         SHUTDOWN; CLOSE(sockfd); continue;
     } 
     DBG("writen(sockfd=%d, ..., strlen(buf)=%d)", sockfd, strlen(buf));
     if ((ret = writen(sockfd, buf, strlen(buf))) != strlen(buf) ) {
         ERR("writen(sockfd=%d, buf=%s, strlen(buf)=%d) == %d",
                                                sockfd, buf, strlen(buf), ret);
         SHUTDOWN; CLOSE(sockfd); continue;
     } 
     DBG("readn(sockfd=%d, ..., strlen(buf)=%d)", sockfd, strlen(buf));
     if (readn(sockfd, buf, strlen(buf)) != strlen(buf) ) {
         ERR("readn(sockfd=%d, buf=%s, strlen(buf)=%d) == %d",
                                                sockfd, buf, strlen(buf), ret);
         SHUTDOWN; CLOSE(sockfd); continue;
     }
     DBG("open(tstpath=\"%s\", ...)", tstpath);
     if ((f = open(tstpath, flags, mode)) < 0) {
         ERR("open(tstpath=%s) FAIL", tstpath);
         SHUTDOWN; CLOSE(sockfd); continue;
     }
     DBG("writen(f=%d, ..., strlen(buf)=%d)", f, strlen(buf));
     if ((ret = writen(f, buf, strlen(buf))) < 0) {
         ERR("writen(f=%d, buf=%s, strlen(buf)=%d) == %d",
                                                     f, buf, strlen(buf), ret);
         SHUTDOWN; CLOSE(sockfd); CLOSE(f); continue;
     }
     SHUTDOWN; CLOSE(sockfd); CLOSE(f);
 }
 DBG("halt=%d, return", halt);
 return;
}

