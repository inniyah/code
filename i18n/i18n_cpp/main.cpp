// Miriam Ruiz <little_miry@yahoo.es>, August 2007
//
// This program is in the Public Domain.
// You can use, modify or distribute it without any limitations.

#include "i18n.h"

#include <stdlib.h>
#include <stdio.h>

const char *lines[] = {
	_("Line 1\n"),
	_("Line 2\n"),
	_("Line 3\n"),
	_("Line 4\n"),
	_("Line 5\n"),
	NULL
};

int main(int arc, char *argv[])
{
	const char **line;
	for (line=lines; *line; line++)
		printf("%s", *line);
	return 0;
}
