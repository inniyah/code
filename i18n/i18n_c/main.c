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

#define N_(String) String

const char * lines[] = {
	N_("Line 1\n"),
	N_("Line 2\n"),
	N_("Line 3\n"),
	N_("Line 4\n"),
	N_("Line 5\n"),
	NULL
};

int main(int arc, char *argv[])
{
	const char **line;

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

	for (line=lines; *line; line++)
		printf("%s", gettext(*line));
	return 0;
}
