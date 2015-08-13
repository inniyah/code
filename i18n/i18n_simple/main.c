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
#include <locale.h>
#define _(String) gettext (String)
#else
#define _(String) String
#endif

void aux(void);

int main(int arc, char *argv[])
{
#ifdef USE_GETTEXT
	setlocale (LC_MESSAGES, "");
	setlocale (LC_CTYPE, "");
	setlocale (LC_COLLATE, "");
	textdomain (PO_NAME);
#ifdef PO_DIR
	bindtextdomain (PO_NAME, PO_DIR);
#else
	bindtextdomain (PO_NAME, NULL);
#endif
#endif

	printf(_("Hello, World!\n"));
	aux();
	return 0;
}
