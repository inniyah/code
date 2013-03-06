/**
 * Miriam Ruiz <little_miry@yahoo.es>, August 2007
 * 
 * This program is in the Public Domain.
 * You can use, modify or distribute it without any limitations.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef USE_GETTEXT
#include <libintl.h>
#include <locale.h>
#define _(String) gettext (String)
#else
#define _(String) String
#endif

int main(int arc, char *argv[])
{
	FILE * file ;
	char buffer[256];

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

	if ((file = fopen("lines.txt", "r")) == NULL)
	{
		fprintf(stderr, _("Error opening file \"%s\": %s\""), "lines.txt", strerror(errno));
		exit(-1);
	}
	while (fgets(buffer, sizeof(buffer)-1, file) != NULL)
	{
		char *c = buffer;
		while (*c && *c!='\n' && *c!='\r') c++;
		*c = '\0';
		if (*buffer && *buffer!='#' && *buffer!='\n')
			printf("%s\n", gettext(buffer));
	}
	fclose(file);
	return 0;
}
