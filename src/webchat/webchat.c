/*
 * webchat.c - Entry of webchat
 *
 * Sun Jun 24, 2012
 *
 * Copyright (C) 2012-2013 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING, if not see
 * <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <libintl.h>
#include <getopt.h>

#include <config-os.h>

static struct option const longopts[] = {
	{"ip", required_argument, NULL, 'i'},
	{"port", required_argument, NULL, 'p'},
	{"verbose", optional_argument, NULL, 'v'},
	{"quiet", no_argument, NULL, 'q'},
	{NULL, 0, NULL, 0},
};
#define optstring "i:p:v::q"

static void usage(int i)
{
	printf("err !\n");
	exit(i);
}

int main(int argc, char **argv)
{
	int c;
	int verbose = 2;
	uint16_t port = 0;
	const char *ip = "";

	setlocale(LC_ALL, "");

	while ((c = getopt_long(argc, argv, optstring, longopts, NULL))
	       != -1) {
		switch(c) {
		case 'i':
			ip = strdup(optarg);
			break;

		case 'p':
			port = (uint16_t)atoi(optarg);
			break;

		case 'v':
			verbose = optarg ? atoi(optarg) : (verbose + 1);
			break;

		case 'q':
			verbose = 0;
			break;

		default:
			usage(EXIT_FAILURE);
		}
	}

	printf("ip = %s, port = %u, verbose = %u\n", ip, port, verbose);

	return 0;
}
