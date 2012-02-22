/*
 * debug.c
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

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#include <config-os.h>

#include <ycc/common/debug.h>

static bool bstamp = true;

void set_dbgstamp(bool stamp)
{
	bstamp = stamp;
}

bool get_dbgstamp()
{
	return bstamp;
}

void __dvfprintf(FILE *stream,
	      const char *file,
	      const char *func,
	      size_t line,
	      const char *fmt,
	      va_list ap)
{
	char buf[32];
	size_t i = 0;
	static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	if(bstamp) {
		time_t now = time(NULL);
		struct tm now_tm;
		i = strftime(buf, 31,
			     "%h %e %T ",
			     localtime_r(&now, &now_tm));
	}
	buf[i] = '\0';

	/* lock write for multi-threads apps. */
	pthread_mutex_lock(&lock);

	fprintf(stream, "%s %s, %zu, %s: ", buf, file, line, func);
	vfprintf(stream, fmt, ap);
	if (stream == stderr) {
		fprintf(stream, ": %s\n", strerror(errno));
	} else if (stream == stdout && *fmt) {
		/* append a LR if fmt's suffix is not LF */
		i = strlen(fmt);
		--i;	/* i is always positive */
		if (fmt[i] != '\n')
			fprintf(stream, "\n");
	}

	pthread_mutex_unlock(&lock);
}

/* eof */
