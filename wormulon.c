/******************************************************************************
 *
 * wormulon
 *
 * See README and COPYRIGHT for instructions and licensing issues.
 *
 * (c) 2003 by Hendrik Scholz
 *   email: hscholz@raisdorf.net
 * website: http://www.raisdorf.net/wormulon/
 *
 ******************************************************************************
 * $Id: wormulon.c,v 1.10 2003/09/12 21:11:50 hscholz Exp $
 *****************************************************************************/

#include "prototypes.h"
#include "os.h"
#include "wormulon.h"
#include "slurm.h"	/* slurm structs */
#include "usage.c"

#ifdef __HPUX__
#include "src/hpux.h"
#include "src/hpux.c"
#elif __FreeBSD__
#include "src/freebsd.h"
#include "src/freebsd.c"
#elif __linux__
#include "src/linux.h"
#include "src/linux.c"
#elif __OpenBSD__ || __MicroBSD__
#include "src/openbsd.h"
#include "src/openbsd.c"
#elif __NetBSD__
#include "src/netbsd.h"
#include "src/netbsd.c"
#elif __Solaris__
#include "src/solaris.h"
#include "src/solaris.c"
#elif __APPLE__
#include "src/macos.h"
#include "src/macos.c"
#else
/* should not get here */
#error "OS not supported"
#endif

int main (int argc, char **argv)
{
	/* data aq. */
	double backup_in, backup_out;
	double cur_in, cur_out;
	char dim_in[4], dim_out[4];

	/* getopt */
	int opt;
	extern char *optarg;
	int mode = OPT_NONE;
	int sampletime = SAMPLE_TIME;

	bzero(ifdata.if_name, sizeof(ifdata.if_name));

	/* getopt () */
	while ((opt = getopt(argc, argv, "d:hi:o:")) != EOF)
	{
		switch (opt)
		{
			case 'd':
				/* delay aka sample time */
				if (atoi(optarg) > 0) sampletime = atoi(optarg);
				else
				{
					fprintf(stderr, "dude, delay must be positive!\n");
					usage(argv[0], 1);
				}
				break;
			case 'i':
				/* interface */
				snprintf(ifdata.if_name, 9, "%s", optarg); // memcpy
				break;
			case 'h':
				/* usage */
				usage(argv[0], 0);
				break;
			case 'o':
				/* possible output options */
				mode |= getopmode((char *) optarg);
#ifdef CUSTOM_MODE
				/* check arg if running in custom mode */
				if (mode == OPT_CUSTOM)
				{
					/* when running in custom mode the last argument
					 * should be a string containing two %% spaceholders
					 * where incoming and outgoing traffic should
					 * be inserted
					 */
					p = argv[argc-1];
					if (((p = strstr(p, "%%%")) == NULL) ||
					    ((p = strstr(++p, "%%%")) == NULL))
						usage(argv[0], 1);
					/* is there a third argument? */
					if (strstr(++p, "%%%") != NULL)
						usage(argv[0], 1);
				}
#endif
				break;
			default:
				usage(argv[0], 1);
		}
	}
	/* sanity checks */
	if (ifdata.if_name[0] == '\0') /* no interface given */
		usage(argv[0], 1);
	if (checkinterface() != TRUE)
	{
		fprintf(stderr, "invalid interface %s\n", (char *) ifdata.if_name);
		return 1;
	}
	if (mode == OPT_NONE)
	{

		/* init in a sane state */
		get_stat();
		backup_in  = stats.rx_bytes;
		backup_out = stats.tx_bytes;
		bzero(dim_in, sizeof(dim_in));
		bzero(dim_out, sizeof(dim_out));

		/* main loop */
		for (;;)
		{
			/* update */
			get_stat();
			if (backup_in > stats.rx_bytes)
				cur_in = (int) stats.rx_bytes / sampletime;
			else
				cur_in = (int) (stats.rx_bytes - backup_in) / sampletime;
			if (backup_out > stats.tx_bytes)
				cur_out = (int) stats.tx_bytes / sampletime;
			else
				cur_out = (int) (stats.tx_bytes - backup_out) / sampletime;

			/* determine traffic dimension and recalc value
		 	 * currently MB/s, KB/s and B/s are supported
		 	 */
			if (cur_in > 1024*1024)
			{
				cur_in = cur_in / (1024*1024); snprintf(dim_in, 22, "MB");
			}
			else if (cur_in > 1024)
			{
				cur_in = cur_in / 1024; snprintf(dim_in, 2, "KB");
			}
			else
				snprintf(dim_in, 2, "B");

			if (cur_out > 1024*1024)
			{
				cur_out = cur_out / (1024*1024); snprintf(dim_out, 2, "MB");
			}
			else if (cur_out > 1024)
			{
				cur_out = cur_out / 1024; snprintf(dim_out, 2, "KB");
			}
			else
				snprintf(dim_out, 2, "B");

			/* print to screen */
			printf("in %lu %s/s out %lu %s/s\n",
					(unsigned long) cur_in, dim_in,
					(unsigned long) cur_out, dim_out);
			fflush(stdout);

			/* save 'new old' values */
			backup_in = stats.rx_bytes;
			backup_out = stats.tx_bytes;

			/* sleep */	
			sleep(sampletime);
		}
	}
	else
	{
		/* non-screen modes */
		get_stat();
		backup_in  = stats.rx_bytes;
		backup_out = stats.tx_bytes;
		bzero(dim_in, sizeof(dim_in));
		bzero(dim_out, sizeof(dim_out));
		sleep(sampletime);
		get_stat();

		if (backup_in > stats.rx_bytes)
			cur_in = (int) stats.rx_bytes / sampletime;
		else
			cur_in = (int) (stats.rx_bytes - backup_in) / sampletime;
		if (backup_out > stats.tx_bytes)
			cur_out = (int) stats.tx_bytes / sampletime;
		else
			cur_out = (int) (stats.tx_bytes - backup_out) / sampletime;
	
		/*	
		 * mode dependent output
		 */

		switch (mode)
		{
			case OPT_MRTG:
			case OPT_FULLMRTG:	/* for now */
				printf("%lu\n%lu\n",
					(unsigned long) cur_in, (unsigned long) cur_out);
				break;
#ifdef CUSTOM_MODE
			case OPT_CUSTOM:
				/* substitute %%% with %lu to insert values */
				p = argv[argc-1];
				p = strstr(p, "%%%");
				memcpy(p, "%lu", 3);
				p = strstr(++p, "%%%");
				memcpy(p, "%lu",3);
				printf(argv[argc-1],
					(unsigned long) cur_in, (unsigned long) cur_out);
				break;
#endif
			default:
				fprintf(stderr, "internal error in output. please report!\n");
		}
	}
	return 0;
}

/*****************************************************************************
 *
 * getopmode()
 *
 * analyse given string to set according action
 *
 ****************************************************************************/

int getopmode (char *str)
{
	int mode = OPT_NONE;
	if (!strcmp(str, "mrtg")) mode = OPT_MRTG;
	else if (!strcmp(str, "fullmrtg")) mode = OPT_FULLMRTG;
#ifdef CUSTOM_MODE
	else if (!strcmp(str, "custom")) mode = OPT_CUSTOM;
#endif
	return mode;
}
