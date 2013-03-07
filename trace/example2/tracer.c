#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define __USE_GNU

#include <dlfcn.h>

#ifdef __cplusplus
extern "C" 
{
#endif

void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));

#ifdef __cplusplus
}
#endif 

static FILE *fp;
int call_level=0;
void * last_fn;

void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
	Dl_info di;
	int i;

	if (fp == NULL) fp = fopen( "trace.txt", "w" );
	if (fp == NULL) exit(-1);

	if ( this_fn!=last_fn) ++call_level;
	for (i=0;i<=call_level;i++) {
		fprintf(fp,"\t");
	}
	fprintf(fp,  "entering %p", (int *)this_fn);
	if (dladdr(this_fn, &di)) {
		fprintf(fp,  " %s (%s)", di.dli_sname ? di.dli_sname : "<unknown>", di.dli_fname);
	}
	fputs("\n", fp);
	(void)call_site;
	last_fn = this_fn;
}


void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	int i;
	--call_level;
	for (i=0;i<=call_level;i++) {
		fprintf(fp,"\t");
	}
	fprintf(fp, "exiting %p\n", (int *)this_fn);
	(void)call_site;

}

