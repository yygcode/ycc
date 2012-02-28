/*
 * conf.c
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

/*
 * config-item
 *
 *    format:
 *	<KEY>[WS|=|WS]<VALUE>
 *
 *    constraint:
 *	key   -- begin char isn't a '#', any character except WS(isspace())
 *	value -- any character except beginning with WS(isspace())
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <config-os.h>

#include <ycc/common/debug.h>
#include <ycc/common/unistd.h>
#include <ycc/common/conf.h>

static inline ssize_t __key_check_len(const char *key)
{
	ssize_t n = (ssize_t)strlen(key);

	if (n && n < LINE_MAX)
		return n;

	DBG_PRINTF("key length (%zu) invalid, range(%zu, %zu))\n",
		   n, 0, LINE_MAX);

	errno = E2BIG;
	return -1;
}

ssize_t
conf_read(const char *file, const char *key, const char *value_fmt, ...)
{
	FILE *stream;
	va_list ap;
	char *beg, line[LINE_MAX];
	ssize_t key_len, rtn = -1;

	if ((key_len = __key_check_len(key)) < 0)
		return rtn;

	if (!(stream = fopen(file, "r")))
		return -1;

	while ((beg=fgets(line, LINE_MAX, stream))) {
		while (isspace(*beg))
			++beg;
		if (*beg == '#' || memcmp(key, beg, key_len))
			continue;	/* match failed */

		/* ignore field-separator, WS and first equal-sign */
		beg += key_len;
		while (isspace(*beg))
			++beg;
		if (*beg == '=')
			++beg;	/* escape just one '=' */
		while (isspace(*beg))
			++beg;
		if (!*beg)
			break;	/* empty value ? */

		va_start(ap, value_fmt);

		/* if value_fmt == "%s", treat all left chars as value */
		if (value_fmt[0] == '%' && value_fmt[1] == 's' &&
		    value_fmt[2] == '\0') {
			size_t len = strlen(beg) - 1;
			if (beg[len] == '\n')
				beg[len] = '\0';
			strcpy(va_arg(ap, char*), beg);
			rtn = 1;	/* one item */
		} else
			rtn = vsscanf(beg, value_fmt, ap);

		va_end(ap);

		break;	/* we had matched the first key */
	}

	fclose(stream);

	return rtn;
}

static inline int
__gen_line_new(char *line, const char *key, const char *value_fmt, va_list ap)
{
	int len1, len2;

	if ((len1 = sprintf(line, "%s = ", key)) < 0)
		return len1;

	if ((len2 = vsnprintf(line+len1, LINE_MAX-len1, value_fmt, ap)) < 0)
		return len2;

	len1 += len2;
	if (len1 > LINE_MAX - 2) {
		DBG_PRINTF("key,value too long: %d, range: (%d, %d)\n",
			   len1, 0, LINE_MAX);
		errno = E2BIG;
		return -1;
	}

	line[len1++] = '\n';
	line[len1] = '\0';

	return len1;
}

ssize_t
conf_write(const char *file, const char *key, const char *value_fmt, ...)
{
	va_list ap;
	FILE *stream, *stream_tmp;
	char *beg, line[LINE_MAX], file_tmp[PATH_MAX];
	bool bwrote = false;
	ssize_t key_len, rtn = -1;

	if ((key_len = __key_check_len(key)) < 0)
		return -1;

	if (!pathname_tmp(file, file_tmp, PATH_MAX))
		return -1;

	if (!(stream_tmp = fopen(file_tmp, "w+"))) {
		DBG_PERROR("open %s failed", file_tmp);
		return -1;
	}

	va_start(ap, value_fmt);

	if (!(stream = fopen(file, "r")))
		goto WRITE_NEW;

	while ((beg=fgets(line, LINE_MAX, stream))) {
		if (bwrote)
			goto WRITE_DIRECT;
		while (isspace(*beg))
			++beg;
		if (*beg == '\0' || *beg == '\n' ||
		    *beg == '#' || memcmp(key, beg, key_len))
			goto WRITE_DIRECT;

		if ((rtn = __gen_line_new(line, key, value_fmt, ap)) < 0)
			goto WRITE_FAIL;

		bwrote = true;

WRITE_DIRECT:
		if ((rtn=fputs(line, stream_tmp)) == EOF)
			goto WRITE_FAIL;
	}

WRITE_NEW:
	if (!bwrote) {
		if ((rtn = __gen_line_new(line, key, value_fmt, ap)) < 0)
			goto WRITE_FAIL;
		rtn = fputs(line, stream_tmp);
	}

WRITE_FAIL:
	va_end(ap);

	fclose(stream_tmp);
	if (stream)
		fclose(stream);

	if (rtn != -1 && rename(file_tmp, file))
		rtn = -1;

	/* return the value which returned by __gen_line_new() */
	return rtn;
}

ssize_t conf_erase(const char *file, const char *key)
{
	FILE *stream, *stream_tmp;
	char *beg, line[LINE_MAX], file_tmp[PATH_MAX];
	ssize_t key_len, rtn = -1;

	if ((key_len = __key_check_len(key)) < 0)
		return -1;

	if (!pathname_tmp(file, file_tmp, PATH_MAX))
		return -1;

	if (!(stream = fopen(file, "r")))
		return -1;

	if (!(stream_tmp = fopen(file_tmp, "w+"))) {
		DBG_PERROR("open %s failed", file_tmp);
		goto ERR_fopen_new;
	}

	rtn = 0;
	while ((beg=fgets(line, LINE_MAX, stream))) {
		while (isspace(*beg))
			++beg;
		if (0 == memcmp(key, beg, key_len)) {
			++rtn;
			continue;	/* delete one */
		}
		if (fputs(line, stream_tmp) != EOF)
			continue;
		/* failed */
		rtn = -1;
		break;
	}

	fclose(stream_tmp);

ERR_fopen_new:
	fclose(stream);

	if (rtn != -1 && rename(file_tmp, file))
		rtn = -1;

	return rtn;
}

ssize_t conf_read_string_safe(const char *file,
			      const char *key, char *value, size_t size)
{
	/* goal: prevent overflow ! */

	size_t len;
	char buf[LINE_MAX];

	if (!size)
		return -1;

	if (size >= LINE_MAX)
		return conf_read(file, key, "%s", value);

	/* read to buf and then check size */
	if (conf_read(file, key, "%s", buf) != 1)
		return -1;

	len = strlen(buf);
	if (len > size) {
		DBG_PRINTF("value too long (%zu, %zu)\n", len, size);
		errno = E2BIG;
		return -1;
	}

	strcpy(value, buf);
	return len;
}

ssize_t conf_delete(const char *file)
{
	return unlink(file);
}

/* eof */
