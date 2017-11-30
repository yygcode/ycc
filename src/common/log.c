/*
 * log.c
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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <config-os.h>

#include <ycc/common/log.h>
#include <ycc/common/debug.h>
#include <ycc/common/unistd.h>

#define LMAGIC	0x35849625

struct log_info
{
	FILE *stream;
	int console;
	int level;
	size_t magic;
	const char *path;
	pthread_mutex_t lock;
};

static struct log_info pr_info = {
	NULL,
	1,
	LOG_DEBUG,
	LMAGIC,
	"",
	PTHREAD_MUTEX_INITIALIZER,
};

struct log_info *_ycc_glog = NULL;
struct log_info *_ycc_pr = &pr_info;

static __always_inline int __log_level(int level)
{
	if (level < 0 || level > LOG_DEBUG)
		return LOG_INFO;

	return level;
}

static __always_inline bool __log_valid(const struct log_info *log)
{
	return log && log->magic == LMAGIC;
}

static inline FILE *__log_fopen(const char *path)
{
	FILE *stream;

	/* create directory recursively */
	if (*path && mkdir_p(path) < 0) {
		pr_err("empty path or create directory failed: %s", path);
		return NULL;
	}

	if (!(stream = fopen(path, "a"))) {
		pr_err("fopen '%s' failed", path);
		return NULL;
	}

	return stream;
}

struct log_info *log_open_stream(FILE *stream, int console, int level)
{
	struct log_info *log;

	if (!(log = (struct log_info*)malloc(sizeof(*log)))) {
		pr_err("malloc log failed");
		return NULL;
	}

	log->stream = stream;
	log->console = console;
	log->level = __log_level(level);
	log->magic = LMAGIC;
	log->path = "";
	pthread_mutex_init(&log->lock, NULL);

	return log;
}

struct log_info *log_open(const char *path, int console, int level)
{
	FILE *stream;
	struct log_info *log;

	if (!(stream = __log_fopen(path)))
		return NULL;

	if (!(log = log_open_stream(stream, console, level)))
		fclose(stream);

	log->path = strdup(path);

	return log;
}

int log_set(struct log_info *log, int console, unsigned int level)
{
	if (!__log_valid(log))
		return -1;

	log->console = console;
	log->level = __log_level(level);

	return 0;
}

int log_get(const struct log_info *log, int *console, unsigned int *level)
{
	if (!__log_valid(log))
		return -1;

	*console = log->console;
	*level = log->level;

	return 0;
}

int log_write(struct log_info *log, int level, const char *fmt, ...)
{
	int i;
	va_list ap;

	va_start(ap, fmt);
	i = log_vwrite(log, level, fmt, ap);
	va_end(ap);

	return i;
}

int log_vwrite(struct log_info *log, int level, const char *fmt, va_list ap)
{
	static const char *sl[] = {
		"<emerg>",
		"<alert>",
		"<crit>",
		"<err>",
		"<warn>",
		"<notice>",
		"<info>",
		"<debug>",
	};

	bool blr = false;	/* line feed */
	int rtn = -1;
	size_t i;
	time_t now;
	struct tm now_tm;

	if (!__log_valid(log))
		return -1;

	if (!log->stream && !log->console) {
		pr_err("log(%p) stream and console both off\n", log->stream);
		return -1;
	}

	if (level < 0)
		level = LOG_INFO;
	if (level > log->level) {
		DBG_PR("limit-level = %d, level = %d, ignore this log",
			   log->level, level);
		return -1;
	}

	assert(level >= 0 && level <= LOG_DEBUG);

	/* need append line-feed ? */
	if (!(i = strlen(fmt)) || fmt[i-1] != '\n')
		blr = true;

	/* lock write for multi-threads apps. */
	if (log->console) {
		static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

		pthread_mutex_lock(&lock);

		if (log->stream) {
			va_list dest;
			va_copy(dest, ap);
			rtn += vfprintf(stderr, fmt, dest);
			va_end(dest);
		} else {
			rtn += vfprintf(stderr, fmt, ap);
		}

		if (blr)
			fputc('\n', stderr);

		pthread_mutex_unlock(&lock);
	}

	if (log->stream) {
		char buf[64];
		struct stat sb;
		if (log->path && stat(log->path, &sb)) {
			/* delete by user ? */
			pr_warn("log '%s' lost, reopen it\n", log->path);
			fclose(log->stream);
			log->stream = __log_fopen(log->path);
			if (!log->stream)
				return -1;
		}

		time(&now);
		i = strftime(buf, 63, "%h %e %T ", localtime_r(&now, &now_tm));
		buf[i] = '\0';

		pthread_mutex_lock(&log->lock);

		rtn = fprintf(log->stream, "%s%-8s : ", buf, sl[level]);
		rtn += vfprintf(log->stream, fmt, ap);
		if (blr)
			fputc('\n', log->stream);

		pthread_mutex_unlock(&log->lock);

		/* stdout is line-buffered, so fflush is trival */
		if (level < LOG_WARNING)
			fflush(log->stream);
	}

	if (blr)
		++rtn;

	return rtn;
}

int log_flush(const struct log_info *log)
{
	if (!__log_valid(log))
		return -1;

	return fflush(log->stream);
}

int log_close(struct log_info *log)
{
	if (!__log_valid(log))
		return -1;

	if (log->stream)
		fclose(log->stream);

	log->magic = 0;

	free(log);

	return 0;
}

int _log_open_glog(const char *path, int console, int level)
{
	if (_ycc_glog)
		log_close(_ycc_glog);
	_ycc_glog = log_open(path, console, level);
	if (_ycc_glog)
		return 0;

	return -1;
}
