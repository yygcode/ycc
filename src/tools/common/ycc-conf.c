/*
 * ycc-conf.c -- manipulate config file
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

#include <getopt.h>
#include <unistd.h>

#include <config-os.h>

#include <ycc/common/debug.h>
#include <ycc/common/log.h>
#include <ycc/common/conf.h>

static struct option const long_opts[] = {
	{ "bin", no_argument, NULL, 'b'},
	{ "key", required_argument, NULL, 'k'},
	{ "value", optional_argument, NULL,  'v'},
	{ NULL, 0, NULL, 0}
};

void usage(FILE *stream, const char *argv0)
{
	if (!stream)
		stream = stdout;

	if (!argv0)
		argv0 = "ycc-conf";

	fprintf(stream, PACKAGE " version " VERSION "\n");
	fprintf(stream, "Usage: %s [OPTION]... KEY [VALUE]\n"
			"Read or write config item.\n"
			"Without VALUE, the KEY would be read,\n"
			"Otherwise set the value of KEY to VALUE.\n"
		);
}

int main(int argc, char **argv)
{
	int c;

	OUT_SET(1, LOG_INFO);
	while ((c = getopt_long(argc, argv, ":bk:v::", long_opts, NULL)) != -1) {
		OUT_DEBUG("c = %c, optopt = %c", c, optopt);
		switch(c) {
		case 'b':
			OUT_INFO("bin");
			break;
		case 'k':
			OUT_INFO("key=%s", optarg);
			break;
		case 'v':
			OUT_INFO("value=%s", optarg ? optarg:"(null)");
			break;
		case '?':
			OUT_INFO("unknown option '%c'", optopt);
			break;
		case ':':
			OUT_INFO("option '%c' requires value", optopt);
			break;
		default:
			OUT_ERR("unknown %d", c);
			break;
		}
	}

	return 0;
}

/* eof */
