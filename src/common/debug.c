/*
 * debug.c - I/O routines for debuging
 *
 * Copyright (C) 2012-2013 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * Mon Jun 11, 2012
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

void __dbg_fprintf(const char *file, const char *func, size_t line,
		   bool berr, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	__dbg_vprintf(file, func, line, berr, fmt, ap);
	va_end(ap);
}

void __dbg_vprintf(const char *file, const char *func, size_t line,
		   bool berr, const char *fmt, va_list ap)
{
	static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	int i;

	/* lock write for multi-threads apps. */
	pthread_mutex_lock(&lock);

	/*
	 * debug-format: <source-name>, <line-no>, <func-name>, <format-value>
	 */
	fprintf(stderr, "%s, %zu, %s: ", file, line, func);
	vfprintf(stderr, fmt, ap);
	if (berr) fprintf(stderr, ": %s\n", strerror(errno));
	if (!(i = strlen(fmt)) || fmt[i-1] != '\n') fprintf(stderr, "\n");

	pthread_mutex_unlock(&lock);
}
