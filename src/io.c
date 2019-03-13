
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

#include <unistd.h>
#include <errno.h>

#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: io.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

 /*
  *  The writen() and readn() functions were taken directly from W. Richard
  *  Stevens' source code accompanying his "Advanced Programming in the Unix
  *  Environment" book.  The code has been reformatted for consistency.  The
  *  design and functionality of the code is owing entirely to Mr. Stevens.
  *  See http://www.kohala.com/start/ for further information.
  */

ssize_t
writen(int fd, const void *vptr, size_t n)
{
 size_t      nleft;
 ssize_t     nwritten;
 const char *ptr;

 DBG("writen(fd=%d, ..., n=%d) called.", fd, n);

 ptr   = vptr;
 nleft = n;

 while(nleft > 0) {
     if((nwritten = write(fd, ptr, nleft)) < 0) {
         if(EINTR == errno) {
             nwritten = 0;
         } else {
             ERR("writen(fd=%d, ..., nleft=%d) FAIL.", fd, nleft);
             return(-1);
         }
     }
     nleft -= nwritten;
     ptr   += nwritten;
     DBG("nleft=%d, nwritten=%d", nleft, nwritten);
 }
 DBG("writen(fd=%d, ..., n=%d) returning %d.", fd, n, n);
 return(n);
}

ssize_t
readn(int fd, void *vptr, size_t n)
{
 size_t  nleft, nread;
 char   *ptr;

 DBG("readn(fd=%d, ..., n=%d) called.", fd, n);

 ptr   = vptr;
 nleft = n;
 while (nleft > 0) {
     if ( (nread = read(fd, ptr, nleft)) < 0) {
         ERR("read(fd=%d, ..., n=%d) FAIL", fd, n);
         return(nread);
     } else if (nread == 0) {
         DBG("nread=%d", nread);
         break;
     }
     nleft -= nread;
     ptr   += nread;
     DBG("nleft=%d, nread=%d", nleft, nread);
 }
 DBG("readn(fd=%d, ..., n=%d) returning (n=%d - nleft=%d)", fd, n, n, nleft);
 return(n - nleft);
}

