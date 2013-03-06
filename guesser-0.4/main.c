/* Copyright (C) 2000-2008 Lavtech.com corp. All rights reserved.
   Copyright (C) 2010 Miriam Ruiz <little_miry@yahoo.es>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <guesser.h>

/* Structure to sort guesser results */
typedef struct
{
	UDM_LANGMAP * map;
	float quality;
} UDM_MAPSTAT;

static int statcmp(const void * i1, const void * i2)
{
	float fres;
	fres= ((const UDM_MAPSTAT*)(i2))->quality - ((const UDM_MAPSTAT*)(i1))->quality;
	if (fres<0) return -1;
	if (fres>0) return +1;
	return 0;
}


static void usage(const char * program)
{
	printf("Language Guesser %s\n\n",VERSION);
	printf("To guess use:\n");
	printf("\t%s [-d dir] [-n maxhits] < FILENAME\n\n", program);
	printf("To create a new language map use:\n");
	printf("\t%s -p -c charset -l language [-t numtopgrams] < FILENAME\n", program);
}


int main(int argc, char * const * argv)
{
	int ch;
	int verbose= 0;
	int print= 0;
	int n= 1000;
	char buf[1024]= "";
	char *charset= NULL;
	char *lang= NULL;
	const char *dir= MAPSDIR;

	UDM_ENV env;
	UDM_LANGMAP mchar;

	/* Init structures */
	memset(&env,0,sizeof(env));
	memset(&mchar,0,sizeof(mchar));
	mchar.topcount= UDM_LM_TOPCNT;

	while((ch= getopt(argc,argv,"pv?c:l:n:d:t:"))!= -1)
	{
		switch(ch)
		{
			case 'd':
				dir= optarg;
				break;
			case 'n':
				n= atoi(optarg);
				break;
			case 'c':
				charset= optarg;
				break;
			case 'l':
				lang= optarg;
				break;
			case 'p':
				print++;
				break;
			case 'v':
				verbose++;
				break;
			case 't':
				mchar.topcount= atoi(optarg);
				break;
			case '?':
			default:
				usage(argv[0]);
				exit(1);
		}
	}
	argc-= optind;
	argv+= optind;

	if (!print)
	{
		/* Load all available lang ngram maps */
		if(verbose)
		{
			fprintf(stderr,"Loading language maps from '%s'\n", dir);
		}
		UdmLoadLangMapListMultipleDirs(&env, dir);
		if(env.errcode)
		{
			printf("Error: '%s'\n",env.errstr);
			return 1;
		}

		if(verbose)
		{
			fprintf(stderr,"%ld maps found\n",(long)env.LangMapList.nmaps);
		}
	}

	/* Add each STDIN line statistics */
	while(fgets(buf,sizeof(buf),stdin))
	{
		UdmBuildLangMap(&mchar,buf,strlen(buf));
	}

	if (print)
	{
		/* Display built langmap */
		if (!charset)
		{
			fprintf(stderr,"You must specify charset using -c\n");
		}
		else if(!lang)
		{
			fprintf(stderr,"You must specify language using -l\n");
		}
		else
		{
			mchar.lang= strdup(lang);
			mchar.charset= strdup(charset);
			UdmPrintLangMap(&mchar);
		}
	}
	else
	{
		int i;
		UDM_MAPSTAT * mapstat;

		/* Prepare map to comparison */
		UdmPrepareLangMap(&mchar);

		/* Allocate memory for comparison statistics */
		mapstat= (UDM_MAPSTAT *)malloc(env.LangMapList.nmaps*sizeof(UDM_MAPSTAT));

		/* Calculate each lang map        */
		/* correlation with text          */
		/* and store in mapstat structure */

		for (i= 0; i < env.LangMapList.nmaps; i++)
		{
			mapstat[i].quality= UdmCheckLangMap(&env.LangMapList.maps[i],&mchar);
			mapstat[i].map= &env.LangMapList.maps[i];
		}

		/* Sort statistics in quality order */
		qsort(mapstat,env.LangMapList.nmaps,sizeof(UDM_MAPSTAT),&statcmp);

		/* Display results. Best language is shown first. */
		for (i= 0; (i < env.LangMapList.nmaps) && (i < n); i++)
		{
			printf("%.10f\t%s\t%s\n",
				mapstat[i].quality<0?0:mapstat[i].quality,
				mapstat[i].map->lang,
				mapstat[i].map->charset
			);
		}

		/* Free variables */
		free(mapstat);
	}

	UdmFreeLangMapList(&env);

	return 0;
}
