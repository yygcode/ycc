/*
 * file-io.c  -- create directory recursively
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

#include <errno.h>
#include <unistd.h>

#include <config-os.h>

#include <ycc/common/unistd.h>
#include <ycc/common/debug.h>

static inline ssize_t read_EINTR(int fd, void *buf, size_t count)
{
	ssize_t r;

	do {
		r = read(fd, buf, count);
	} while (-1 == r && EINTR == errno);

	return r;
}

static inline ssize_t write_EINTR(int fd, const void *buf, size_t count)
{
	ssize_t r;

	do {
		r = write(fd, buf, count);
	} while (-1 == r && EINTR == errno);

	return r;
}

ssize_t readn(int fd, void *buf, size_t count)
{
	char *p = (char*)buf;
	ssize_t left = (ssize_t)count;

	do {
		ssize_t r = read_EINTR(fd, p, left);

		if (r <= 0) {
			if (count == left)
				return r;
			return (ssize_t)count - left;
		}

		left -= r;
		p += r;
	} while (left > 0);

	return (ssize_t)count;	/* success */
}

ssize_t writen(int fd, const void *buf, size_t count)
{
	const char *p = (const char*)buf;
	ssize_t left = (ssize_t)count;

	do {
		ssize_t r = write_EINTR(fd, p, left);

		if (r <= 0) {
			if (count == left)
				return r;

			return (ssize_t)count - left;
		}

		left -= r;
		p += r;
	} while (left > 0);

	return (ssize_t)count;
}

/* eof */
