// Miriam Ruiz <little_miry@yahoo.es>, August 2007
//
// This program is in the Public Domain.
// You can use, modify or distribute it without any limitations.

#ifdef USE_GETTEXT

#include "i18n.h"

#include <libintl.h>
#include <locale.h>

GetTextInit::GetTextInit()
{
	if  (!IsInit)
	{
		IsInit = true;
		setlocale (LC_MESSAGES, "");
		setlocale (LC_CTYPE, "");
		setlocale (LC_COLLATE, "");
		textdomain (PO_NAME);
#ifdef PO_DIR
		bindtextdomain (PO_NAME, PO_DIR);
#else
		bindtextdomain (PO_NAME, NULL);
#endif
	}
}

bool GetTextInit::IsInit = false;

#endif
