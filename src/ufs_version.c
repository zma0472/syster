
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

#include "log.h"

#if !defined(LINT)
static char rscid[] =
"$Id: ufs_version.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

static int version_major = 0;
static int version_minor = 1;
static int version_patch = 0;

void
version(void)
{
 DBG("version() called, version_major=%d, version_minor=%d, version_patch=%d",
                                  version_major, version_minor, version_patch);

 (void)fprintf(stdout, "  syster_ufs v.%d.%d.%d ALPHA\n", version_major,
                                                          version_minor,
                                                          version_patch); 
 (void)fflush(NULL);

 return;
}

