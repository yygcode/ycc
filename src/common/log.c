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

#define LOG_MAGIC	0x35849625

struct log_info
{
	FILE *pf;
	int console;
	int level;
	size_t magic;
};
#define LOG_INFOP(p)		((struct log_info*)(p))
#define LOG_INFO_PF(p)		(LOG_INFOP(p)->pf)
#define LOG_INFO_CONSOLE(p)	(LOG_INFOP(p)->console)
#define LOG_INFO_LEVEL(p)	(LOG_INFOP(p)->level)
#define LOG_INFO_MAGIC(p)	(LOG_INFOP(p)->magic)

const struct log_info *_glog_info = NULL;

static inline int __log_level(int level)
{
	if (level < 0 || level > LOG_DEBUG)
		return LOG_INFO;

	return level;
}

static inline bool __log_valid(const struct log_info *log)
{
	DBG_INSERT(
		if(!(log && LOG_INFO_MAGIC(log) == LOG_MAGIC))
			DBG_PRINTF("log invalid: %p", log););

	return log && LOG_INFO_MAGIC(log) == LOG_MAGIC;
}

const struct log_info *log_open(const char *path, int console, int level)
{
	FILE *pf;
	struct log_info *log;
	char *pe, *const p = strdup(path);
	if (!p || !*p)
		return NULL;

	/* create directory recursively */
	pe = p;
	while ((pe = strchr(pe+1, '/'))) {
		struct stat sb;
		*pe = '\0';
		if (stat(p, &sb) && mkdir(p, ACCESSPERMS)) {
			DBG_PERROR("log is '%s', stat/create '%s' failed",
				   path, p);
			free(pe);
			return NULL;
		}
		*pe = '/';
	}
	free(pe);

	log = (struct log_info*)malloc(sizeof(*log));
	if (!log)
		return NULL;

	pf = fopen(path, "a");
	if (!pf) {
		free(log);
		return NULL;
	}

	log->pf = pf;
	log->console = console;
	log->level = __log_level(level);
	log->magic = LOG_MAGIC;

	return log;
}

int log_set(const struct log_info *log, int console, unsigned int level)
{
	if (!__log_valid(log))
		return -1;

	LOG_INFO_CONSOLE(log) = console;
	LOG_INFO_LEVEL(log) = __log_level(level);

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

int log_vwrite(const struct log_info *log,
	       int level, const char *fmt, va_list ap)
{
	int rtn;
	size_t i;
	char buf[32];
	time_t now;
	struct tm now_tm;
	static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	static char *sl[LOG_DEBUG+1] = {
		"<emerg>",
		"<alert>",
		"<crit>",
		"<err>",
		"<warn>",
		"<notice>",
		"<info>",
		"<debug>",
	};

	if (!__log_valid(log))
		return -1;

	if (level < 0)
		level = LOG_INFO;
	if (level > log->level) {
		DBG_PRINTF("limit-level = %d, level = %d, ignore this log",
			   log->level, level);
		return 0;
	}

	time(&now);
	i = strftime(buf, 31, "%h %e %T ", localtime_r(&now, &now_tm));
	buf[i] = '\0';

	/* lock write for multi-threads apps. */
	pthread_mutex_lock(&lock);

	if (log->console) {
		va_list dest;
		va_copy(dest, ap);
		fprintf(stdout,  "%s %-8s : ", buf, sl[level]);
		vfprintf(stdout, fmt, dest);
		va_end(dest);
	}
	rtn = fprintf(LOG_INFO_PF(log), "%s %-8s : ", buf, sl[level]);
	rtn += vfprintf(LOG_INFO_PF(log), fmt, ap);

	if (*fmt) {
		/* append a LR if fmt's suffix is not LF */
		i = strlen(fmt);
		--i;	/* i is always positive */
		if (fmt[i] != '\n') {
			fputc('\n', LOG_INFO_PF(log));
			++rtn;
			if (log->console)
				fputc('\n', stdout);
		}
	}

	/* stdout is line-buffered, so fflush is trival */
	if (level < LOG_WARNING)
		fflush(LOG_INFO_PF(log));

	pthread_mutex_unlock(&lock);

	return rtn;
}

int log_flush(const struct log_info *log)
{
	if (!__log_valid(log))
		return -1;

	return fflush(LOG_INFO_PF(log));
}

int log_close(const struct log_info *log)
{
	if (!__log_valid(log))
		return -1;

	if (LOG_INFO_PF(log))
		fclose(LOG_INFO_PF(log));

	LOG_INFOP(log)->magic = 0;

	free(LOG_INFOP(log));

	return 0;
}

/* eof */
