/******************************************************************************
 *
 * usage.c
 *
 * go figure out what it does :)
 *
 ******************************************************************************
 * $Id: usage.c,v 1.5 2003/09/12 21:11:50 hscholz Exp $
 *****************************************************************************/

#ifndef _USAGE_C
#define _USAGE_C

void usage (char *path, int exitcode)
{
	fprintf(stderr, "wormulon %s - "
			"hscholz@raisdorf.net - http://raisdorf.net/\n", VERSION);
	fprintf(stderr, "usage: %s [-h] [-o mode] [-d delay] -i interface\n", path);
	/* print some verbose information when -h is given */
	if (exitcode != 0)
		exit(exitcode);
	fprintf(stderr,
		"    -o mode  : use one of the following modes:\n"
		"               mrtg, fullmrtg\n"
		"    -d delay : delay in seconds\n"
		"\nIn case of problems run wormulon outside screen!\n");
	exit(0);
}

#endif
