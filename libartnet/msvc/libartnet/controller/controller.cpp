// discover.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Library General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
//#include <windows.h>
#include <time.h>
#include <errno.h>
#include <artnet/artnet.h>
#include <iostream>

char SHORT_NAME[] = "ArtNet Node";
char LONG_NAME[] = "libartnet setdmx example";

#define BADCH   (int)'?'
#define BADARG  (int)':'
#define EMSG    ""

int     opterr = 1,     /* if error message should be printed */
optind = 1,             /* index into parent argv vector */
optopt,                 /* character checked for validity */
optreset;               /* reset getopt */
char    *optarg;		/* argument associated with option */
int getopt(int nargc, char * const nargv[], const char *ostr)
{
	static char *place = EMSG;              /* option letter processing */
	const char *oli;                        /* option letter list index */

	if (optreset || !*place) {              /* update scanning pointer */
		optreset = 0;
		if (optind >= nargc || *(place = nargv[optind]) != '-') {
			place = EMSG;
			return (-1);
		}
		if (place[1] && *++place == '-') {      /* found "--" */
			++optind;
			place = EMSG;
			return (-1);
		}
	}                                       /* option letter okay? */
	if ((optopt = (int)*place++) == (int)':' ||
		!(oli = strchr(ostr, optopt))) {
		/*
		* if the user didn't specify '-' as an option,
		* assume it means -1.
		*/
		if (optopt == (int)'-')
			return (-1);
		if (!*place)
			++optind;
		if (opterr && *ostr != ':')
			(void)printf("illegal option -- %c\n", optopt);
		return (BADCH);
	}
	if (*++oli != ':') {                    /* don't need argument */
		optarg = NULL;
		if (!*place)
			++optind;
	}
	else {                                  /* need an argument */
		if (*place)                     /* no white space */
			optarg = place;
		else if (nargc <= ++optind) {   /* no arg */
			place = EMSG;
			if (*ostr == ':')
				return (BADARG);
			if (opterr)
				(void)printf("option requires an argument -- %c\n", optopt);
			return (BADCH);
		}
		else                            /* white space */
			optarg = nargv[optind];
		place = EMSG;
		++optind;
	}
	return (optopt);                        /* dump back option letter */
}


typedef struct {
	int verbose;
	int help;
	char *ip_addr;
	int port_addr;
	int channel;
	int value;
	float fade_time;
} opts_t;

/*
* Set our default options, command line args will overide this
*/
void init_ops(opts_t *ops) {
	ops->verbose = 0;
	ops->help = 0;
	ops->ip_addr = NULL;
	ops->port_addr = 0;
	ops->channel = 1;
	ops->value = 0;
	ops->fade_time = 0.0;
}

/*
* Parse command lines options and save in opts_s struct
*/
void parse_args(opts_t *ops, int argc, char *argv[]) {

	/*
	static struct option long_options[] = {
		{ "address",   required_argument,  0, 'a' },
		{ "port",     required_argument,  0, 'p' },
		{ "channel",   required_argument,  0, 'c' },
		{ "dmx",     required_argument,  0, 'd' },
		{ "fade",     required_argument,  0, 'f' },
		{ "help",     no_argument,     0, 'h' },
		{ "verbose",   no_argument,     0, 'v' },

		{ 0, 0, 0, 0 }
	};
	*/

	int c;
	int option_index = 0;

	while (1) {

		c = getopt(argc, argv, "a:p:c:d:f:vh");

		if (c == -1)
			break;

		switch (c) {
		case 0:
			break;
		case 'a':
			ops->ip_addr = (char *)optarg;
			break;
		case 'd':
			ops->value = atoi(optarg);
			break;
		case 'c':
			ops->channel = atoi(optarg);
			break;
		case 'p':
			ops->port_addr = atoi(optarg);
			break;
		case 'f':
			ops->fade_time = atof(optarg);
			break;
		case 'h':
			ops->help = 1;
			break;
		case 'v':
			ops->verbose = 1;
			break;

		case '?':
			break;

		default:
			;
		}
	}
}


/*
*
*
*/
void display_help_and_exit(opts_t *ops, char *argv[]) {
	printf(
		"Usage: %s --address <ip_address> --port <port_no> --channel <channel> --dmx <dmx_value> --fade <fade_time> \n"
		"\n"
		"Control lla port <-> universe mappings.\n"
		"\n"
		"  -a, --address <ip_address>      Address to send from.\n"
		"  -c, --channel <channel>         Channel to set (starts from 0).\n"
		"  -d, --dmx <value>               Value to set the channel to.\n"
		"  -h, --help                      Display this help message and exit.\n"
		"  -f, --fade <fade_time>          Total time of fade.\n"
		"  -p, --port <port_no>            Universe (port) address.\n"
		"  -v, --verbose                   Be verbose.\n"
		"\n",
		argv[0]);
	exit(0);
}


void msleep(long time) {
#ifndef WIN32
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 1000000 * time;
	nanosleep(&ts, NULL);
#else
	Sleep(time);
#endif
}


/*
*
*
*/
int do_fade(artnet_node node, opts_t *ops) {
	int chan = ops->channel;
	float fadetime = ops->fade_time;
	int val = ops->value;
	uint8_t* dmx = new uint8_t[chan];
	float p;

	memset(dmx, 0x00, chan);

	chan--;

	artnet_send_dmx(node, 0, chan, dmx);
	msleep(40);

	const unsigned long tstart = timeGetTime();
	const unsigned long tend = tstart + (int)(fadetime*1000.0);
	unsigned long t = tstart;

	while (t <= tend) {
		t = timeGetTime();

		if (fadetime)
			p = (float)(t - tstart) / 1000.0f / fadetime;
		else
			p = 1.0;

		std::cout << (float)val*p << "\n";

		dmx[chan] = (int)(float)val*p;

		msleep(40);
		//  printf("%f %i %i %f %li\n", p, chan, dmx[chan] , fadetime, t-tstart);

		// we have to call raw_send here as it sends a sequence number of 0
		// otherwise each execution of artnet_setdmx starts the sequence from 0
		// which confuses some devices
		if (artnet_raw_send_dmx(node, ops->port_addr, chan + 1, dmx)) {
			printf("failed to send: %s\n", artnet_strerror());
		}

		t = timeGetTime(); // get current time, because the last time is too old (due to the sleep)
	}

	delete[] dmx;

	return 0;
}


int quit(int error_code)
{
	int n;
	std::cout << "enter any key to quit: ";
	std::cin >> n;
	return error_code;
}

int main(int argc, char *argv[])
{
	opts_t ops;
	artnet_node node;

	// init and parse the args
	init_ops(&ops);
	parse_args(&ops, argc, argv);

	// do some checks
	if (ops.help)
		display_help_and_exit(&ops, argv);

	if (ops.channel < 1 || ops.channel > 512)
		display_help_and_exit(&ops, argv);

	if (ops.value < 0 || ops.value > 255)
		display_help_and_exit(&ops, argv);

	if (ops.port_addr < 0 || ops.port_addr > 4)
		display_help_and_exit(&ops, argv);

	// create new artnet node, and set config values
	node = artnet_new(ops.ip_addr, ops.verbose);;

	artnet_set_short_name(node, SHORT_NAME);
	artnet_set_long_name(node, LONG_NAME);
	artnet_set_node_type(node, ARTNET_RAW);

	artnet_set_port_type(node, 0, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
	artnet_set_port_addr(node, 0, ARTNET_INPUT_PORT, ops.port_addr);

	if (artnet_start(node) != ARTNET_EOK) {
		printf("Failed to start: %s\n", artnet_strerror());
		goto error_destroy;
	}

	printf("channel is %i\n", ops.channel);
	do_fade(node, &ops);

	return 0;

error_destroy:
	artnet_destroy(node);

	free(ops.ip_addr);
	exit(1);
}

