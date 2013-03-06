# Miriam Ruiz <little_miry@yahoo.es>, August 2007
# 
# This program is in the Public Domain.
# You can use, modify or distribute it without any limitations.

PROGRAM=test
CFLAGS=-Wall -Werror -ansi -pedantic
LOCALEDIR=.
DEFS=-DUSE_GETTEXT -DPO_NAME="\"$(PROGRAM)\"" -DPO_DIR="\"$(LOCALEDIR)\""

all: $(PROGRAM) $(PROGRAM).pot build-locales

clean: clean-locales
	rm -f *.o *.mo test test.pot

SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) $(DEFS) -c $< -o $@

$(PROGRAM): main.o aux.o
	$(CC) $+ -o $@

%.pot:
	xgettext -k -k_ -kN_ -o $@ $+ --from-code=iso-8859-1

$(PROGRAM).pot: main.c aux.c

build-locales:
	@if ls *.po 2>/dev/null ; then  \
		for f in *.po; do \
			i=`basename $$f .po` ; \
			echo Building Language \"$$i\" ; \
			msgfmt $$f -o $$i.mo ; \
		done \
	fi
	if ls *.mo 2>/dev/null ; then \
		for f in *.mo; do \
			i=`basename $$f .mo` ; \
			echo Installing Language \"$$i\" ; \
			mkdir -p $(LOCALEDIR)/$$i/LC_MESSAGES ; \
			mv $$f $(LOCALEDIR)/$$i/LC_MESSAGES/test.mo ; \
		done \
	fi

clean-locales:
	@if ls *.po 2>/dev/null ; then  \
		for f in *.po; do \
			i=`basename $$f .po` ; \
			echo Deleting Language \"$$i\" ; \
			rm -rf $$i ; \
		done \
	fi
