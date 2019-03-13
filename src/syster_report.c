
/*  Copyright (C) 2008 by Matthew Alton  */

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
#include <ctype.h>
#include <time.h>

#include "syster.h"
#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: syster_report.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

#define DEFAULT_TOLERANCE 1

int debug = 0;

static int asc_flag = 0, lbl_flag = 0;

char *log_label = "RPT";

static unsigned long line_no = 1;
static unsigned long current_delay = 0;
static unsigned long last_delay = 0;
static unsigned long tolerance = DEFAULT_TOLERANCE;
static unsigned long last_stamp = 0;
static unsigned long begin_stamp = 0;
static unsigned long end_stamp = 0;

#define ERR_OUT                                                               \
do {                                                                          \
    ERR("state=%d, c='%c', line_no=%lu, char_no=%lu",                         \
                                                 state, c, line_no, char_no); \
} while (0)

#define SPC_TAB ( ('\t' == c) || (' ' == c) )

#define STR_UL(a)                                                             \
do {                                                                          \
    errno = 0; (a) = strtoul(ul_buf, NULL, 10);                               \
    if (0 != errno) {                                                         \
        ERR_OUT;                                                              \
    }                                                                         \
} while (0)

#define UL_CAT                                                                \
do {                                                                          \
    if ( ul_index > 19 ) {                                                    \
           ERR("ul_index=%d overflow, c='%c', line_no=%lu, char_no=%lu",      \
                                                 state, c, line_no, char_no); \
           return (2);                                                        \
    }                                                                         \
    ul_buf[ul_index] = c; ul_buf[++ul_index] = '\0';                          \
} while (0)

#define UL_RESET                                                              \
do {                                                                          \
    int j = 0;                                                                \
    ul_index = 0;                                                             \
    for (j = 0; j < 21; j++) {                                                \
        ul_buf[j] = '\0';                                                     \
    }                                                                         \
} while (0)

#define BUFFER_SIZE 8192

void
version(void)
{
 static int version_major = 0;
 static int version_minor = 2;
 static int version_patch = 0;

 DBG("version() called, version_major=%d, version_minor=%d, version_patch=%d",
                                  version_major, version_minor, version_patch);

 (void)fprintf(stdout, "  syster_mem v.%d.%d.%d ALPHA\n", version_major,
                                                          version_minor,
                                                          version_patch);
 (void)fflush(NULL);

 return;
}

void
usage(void)
{
 (void)fprintf(stderr, "  Usage: syster_report [ -V | -h ]\n"
                       "                       [ -A ]\n"
                       "                       [ -L ]\n"
                       "                       [ -t tolerance ]\n"
                       "                       [ -d initial_delay ]\n"
                       "                       [ -b begin_stamp ]\n"
                       "                       [ -e end_stamp ]\n");
 return;
}

void
cl_args(int argc, char **argv)
{
 int c = 0;
 char *endptr = NULL;

 while ((c = getopt(argc, argv, "Vh?ALb:e:t:d:")) != -1) {
     switch (c) {
         case 'V': version(); exit(0);
                   break;
         case 'h': usage(); exit(0);
                   break;
         case '?': usage(); exit(0);
                   break;
         case 'A': asc_flag = 1;
                   break;
         case 'L': lbl_flag = 1;
                   break;
         case 't': errno = 0;
                   tolerance = strtoul(optarg, &endptr, 0);
                   if (0 != errno) {
                       ERR("strtoul(\"%s\") fail", optarg);
                       exit(2);
                   } else if ( ('\0' !=  *optarg) &&
                               ('\0' ==  *endptr) ) {
                       /*  NO-OP  */
                   } else {
                       ERR("Invalid tolerance=%s", optarg);
                       exit(2);
                   }
                   break;
         case 'b': errno = 0;
                   begin_stamp = strtoul(optarg, &endptr, 0);
                   if (0 != errno) {
                       ERR("strtoul(\"%s\") fail", optarg);
                       exit(2);
                   } else if ( ('\0' !=  *optarg) &&
                               ('\0' ==  *endptr) ) {
                       /*  NO-OP  */
                   } else {
                       ERR("Invalid begin_stamp=%s", optarg);
                       exit(2);
                   }
                   last_stamp = begin_stamp;
                   break;
         case 'e': errno = 0;
                   end_stamp = strtoul(optarg, &endptr, 0);
                   if (0 != errno) {
                       ERR("strtoul(\"%s\") fail", optarg);
                       exit(2);
                   } else if ( ('\0' !=  *optarg) &&
                               ('\0' ==  *endptr) ) {
                       /*  NO-OP  */
                   } else {
                       ERR("Invalid end_stamp=%s", optarg);
                       exit(2);
                   }
                   break;
         case 'd': errno = 0;
                   last_delay = strtoul(optarg, &endptr, 0);
                   if (0 != errno) {
                       ERR("strtoul(\"%s\") fail", optarg);
                       exit(2);
                   } else if ( ('\0' !=  *optarg) &&
                               ('\0' ==  *endptr) ) {
                       /*  NO-OP  */
                   } else {
                       ERR("Invalid initial_delay=%s", optarg);
                       exit(2);
                   }
                   break;
         default:  usage(); exit(2);
     }
 }
 return;
}

void
test_stamp(unsigned long stamp)
{
 unsigned long outage = 0, out_days = 0, out_hours = 0, out_minutes = 0;
 unsigned long out_seconds = 0;

 if ( (0 < begin_stamp) && (begin_stamp > stamp) ) {
    return;
 }
 if ( (0 < end_stamp) && (end_stamp < stamp) ) {
    stamp = end_stamp;
 }
 if ( (0 < last_stamp) || (stamp == end_stamp) ) {
     if (stamp < last_stamp) {
         ERR("line_no=%lu : stamp=%lu < last_stamp=%lu",
                                                   line_no, stamp, last_stamp);
     } else {
         outage = stamp - last_stamp;
         if ( outage > (last_delay + tolerance) ) {
             if (0 == asc_flag) {
                 if (0 == lbl_flag) { /*  asc_flag=0  lbl_flag=0  */
                     printf("%lu %lu %lu %lu %lu\n",
                             line_no, current_delay, tolerance, outage, stamp);
                 } else { /*  asc_flag=0  lbl_flag=1  */
                     printf( "line=%lu delay=%lu tolerance=%lu"
                             " outage=%lu stamp=%lu\n",
                             line_no, current_delay, tolerance, outage, stamp);
                 }
             } else {
                 out_days    =   outage/86400;
                 out_hours   =  (outage%86400)/3600;
                 out_minutes = ((outage%86400)%3600)/60;
                 out_seconds = ((outage%86400)%3600)%60;
                 if (0 == lbl_flag) { /*  asc_flag=1  lbl_flag=0  */
                     printf("%lu %lu %lu %lud%luh%lum%lus %s\n",
                         line_no, current_delay, tolerance, 
                         out_days, out_hours, out_minutes, out_seconds,
                                                 ctime((time_t *)&last_stamp));
                 } else { /* asc_flag=1  lbl_flag=1  */
                     printf( "line=%lu delay=%lu tolerance=%lu"
                             " outage=%lud%luh%lum%lus %s\n",
                         line_no, current_delay, tolerance,
                         out_days, out_hours, out_minutes, out_seconds,
                                                 ctime((time_t *)&last_stamp));
                 }
             }
         }
     }
 }
 if ( stamp == end_stamp ) {
     exit(0);
 }
 last_stamp = stamp; last_delay = current_delay; return;
}

#define TEST_STAMP test_stamp(stamp)

int
main(int argc, char **argv)
{
 unsigned long stamp = 0, char_no = 1;
 int state = 0, i = 0, ul_index = 0;
 ssize_t nread = 0;
 enum { START_STATE   = 0,
        DELAY_STATE   = 1,
        DELIM_STATE   = 2,
        STAMP_STATE   = 3,
        TAIL_STATE    = 4,
        COMMENT_STATE = 5,
        RECOVER_STATE = 6
 };
 char c = '\0', buf[BUFFER_SIZE], ul_buf[21];

 cl_args(argc, argv);

 while ( (nread = readn(STDIN_FILENO, buf, BUFFER_SIZE)) > 0 ) {
     for (i = 0; i < nread; i++) {
         c = buf[i];
         if (START_STATE == state) {
             if ( SPC_TAB ) {
                 /*  NO-OP  */
             } else if ('\n' == c) {
                 /*  NO-OP  */
             } else if ('#' == c) {
                 state = COMMENT_STATE;
             } else if (isdigit(c)) {
                 state = DELAY_STATE; UL_RESET; UL_CAT;
             } else {
                 ERR_OUT; state = RECOVER_STATE;
             }
         } else if (DELAY_STATE == state) {
             if ( SPC_TAB ) {
                 state = DELIM_STATE; STR_UL(current_delay); UL_RESET;
             } else if (isdigit(c)) {
                 UL_CAT;
             } else {
                 ERR_OUT; state = RECOVER_STATE;
             }
         } else if (DELIM_STATE == state) {
             if ( SPC_TAB ) {
                 /*  NO-OP  */
             } else if (isdigit(c)) {
                 state = STAMP_STATE; UL_RESET; UL_CAT;
             } else {
                 ERR_OUT; state = RECOVER_STATE;
             }
         } else if (STAMP_STATE == state) {
             if ( SPC_TAB ) {
                 state = TAIL_STATE; STR_UL(stamp); UL_RESET; TEST_STAMP;
             } else if ('\n' == c) {
                 state = START_STATE; STR_UL(stamp); UL_RESET; TEST_STAMP;
             } else if (isdigit(c)) {
                 UL_CAT;
             } else {
                 ERR_OUT; state = RECOVER_STATE;
             }
         } else if (TAIL_STATE == state) {
             if ( SPC_TAB ) {
                 /*  NO-OP  */
             } else if ('\n' == c) {
                 state = START_STATE;
             } else if ('#' == c) {
                 state = COMMENT_STATE;
             } else {
                 ERR_OUT; state = RECOVER_STATE;
             }
         } else if (COMMENT_STATE == state) {
             if ('\n' == c) {
                 state = START_STATE;
             }
         } else if (RECOVER_STATE == state) {
             if ('\n' == c) {
                 state = START_STATE;
             }
         }
         if ('\n' == c) {
             line_no++; char_no = 1;
         } else {
             char_no++;
         }
     }
 }
 if ( 0 < end_stamp ) {
     test_stamp(end_stamp);
 }
 return (0);
}

