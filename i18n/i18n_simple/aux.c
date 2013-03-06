/**
 * Miriam Ruiz <little_miry@yahoo.es>, August 2007
 * 
 * This program is in the Public Domain.
 * You can use, modify or distribute it without any limitations.
 */

#include <stdlib.h>
#include <stdio.h>

#ifdef USE_GETTEXT
#include <libintl.h>
#define _(String) gettext (String)
#else
#define _(String) String
#endif

void aux(void)
{
	printf(_("World, hello!\n"));
}
