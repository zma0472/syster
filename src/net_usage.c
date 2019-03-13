
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
"$Id: net_usage.c,v 1.1 2013/08/12 21:15:03 zma0472 Exp $";
#endif /*  !defined(LINT)  */

static char bnf[] =
"  Usage: syster_net -V |\n"
"                   [-D ]\n"
"                   [-d delay]\n"
"                   [-p pidpath]\n"
"                   [-E echo_port]\n"
"                   [-n echo_node]\n"
"                   [-l logpath]\n"
"                   [-e errpath]\n"
"                   [-t testpath]\n";

void
usage(void)
{
 DBG("usage() called, log_label=\"%s\"", log_label);

 (void)fprintf(stderr, "%s", bnf);
 return;
}

