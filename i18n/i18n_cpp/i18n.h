// Miriam Ruiz <little_miry@yahoo.es>, August 2007
//
// This program is in the Public Domain.
// You can use, modify or distribute it without any limitations.

#ifndef _I18N_H
#define _I18N_H

#ifdef USE_GETTEXT

#include <libintl.h>
#define _(String) gettext (String)

class GetTextInit
{
	protected:
		static bool IsInit;
	public:
		GetTextInit();
};

static GetTextInit gettext_init;

#else

#define _(String) String

#endif

#endif
