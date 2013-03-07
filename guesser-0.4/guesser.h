/* Copyright (C) 2000-2008 Lavtech.com corp. All rights reserved.

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

#ifndef UDM_GUESSER_H
#define UDM_GUESSER_H

#ifdef __cplusplus
extern "C"
{
#endif

	/************** Language and charset guesser *************/

	#define UDM_LM_MAXGRAM   6
	#define UDM_LM_HASHMASK  0x0FFF
	#define UDM_LM_TOPCNT    200

	typedef struct
	{
		int count;
		char str[UDM_LM_MAXGRAM+3];
	} UDM_LANGITEM;

	typedef struct
	{
		float  expectation;		 /* Average value   */
		float  dispersion;		 /* Math dispersion */
		char * lang;			 /* Map Language    */
		char * charset;			 /* Map charset     */
								 /* Items list      */
		UDM_LANGITEM  memb[UDM_LM_HASHMASK+1];
		size_t topcount;
	} UDM_LANGMAP;

	typedef struct
	{
		size_t nmaps;
		UDM_LANGMAP * maps;
	} UDM_LANGMAPLIST;

	typedef struct
	{
		UDM_LANGMAPLIST LangMapList;
		int errcode;
		char errstr[1024];
	} UDM_ENV;

	#define UDM_FREE(x)     {if(x){free(x);x=NULL;}}

	/*********************************************************/

	extern void  UdmBuildLangMap(UDM_LANGMAP * map,const char * text,size_t text_len);
	extern void  UdmPrintLangMap(UDM_LANGMAP * map);
	extern void  UdmPrepareLangMap(UDM_LANGMAP * map);
	extern float UdmCheckLangMap(UDM_LANGMAP * map,UDM_LANGMAP * text);

	/*********************************************************/

	extern int   UdmLoadLangMapList(UDM_ENV * env, const char * mapdir);
	extern int   UdmLoadLangMapListMultipleDirs(UDM_ENV *Env, const char *mapdirs);
	extern int   UdmLoadLangMapFile(UDM_ENV * env, const char * mapname);
	extern void  UdmFreeLangMapList(UDM_ENV * env);

#ifdef __cplusplus
}
#endif

#endif /* UDM_GUESSER_H */
