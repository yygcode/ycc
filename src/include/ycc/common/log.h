/*
 * log.h
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

#ifndef __YCC_COMMON_LOG_H_
#define __YCC_COMMON_LOG_H_

#include <stdarg.h>
#include <syslog.h>

#include <ycc/config-ycc.h>

__BEGIN_DECLS

#if 0
/* level macros (see sys/syslog.h) */
#define	LOG_EMERG	0	/* system is unusable */
#define	LOG_ALERT	1	/* action must be taken immediately */
#define	LOG_CRIT	2	/* critical conditions */
#define	LOG_ERR		3	/* error conditions */
#define	LOG_WARNING	4	/* warning conditions */
#define	LOG_NOTICE	5	/* normal but significant condition */
#define	LOG_INFO	6	/* informational */
#define	LOG_DEBUG	7	/* debug-level messages */
#endif
/*
 * console: !0 will print log to stdout
 * level: < 0 for default(LOG_INFO), otherwise should be LOG_* macros above
 */
/*
 * LOG_OPEN
 *
 * success return 0, otherwise return -1.
 * 
 * path: log-name(absolutely or relatively)
 *
 * console: when console is not zero LOG_WRITE/LOG_VWIRITE would
 *	    write out to stdout.
 *
 * level: when LOG_WRITE/LOG_VWRITE's level is greater than this,
 *	  messages would be discarded.
 */
#define LOG_OPEN(path, console, level)	\
		_log_open_glog((path), (console), (level))
#define LOG_SET(console, level)		\
		log_set(_glog_info, (console), (level))
#define LOG_GET(pconsole, plevel)	\
		log_get(_glog_info, (pconsole), (plevel))
#define LOG_WRITE(level, fmt, ...)	\
		log_write(_glog_info, (level), (fmt), ## __VA_ARGS__)
#define LOG_VWRITE(level, fmt, ap)	\
		log_vwrite(_glog_info, (level), (fmt), (ap))
#define LOG_FLUSH()			\
		log_flush(_glog_info)
#define LOG_CLOSE()			\
		log_close(_glog_info)


/* carefull !
 * you really need multi logs in an application ?
 * otherwise use above macros
 */
struct log_info;
const struct log_info *log_open(const char *path, int console, int level);
int log_set(const struct log_info *log, int console, unsigned int level);
int log_get(const struct log_info *log, int *console, unsigned int *level);
int log_vwrite(const struct log_info *log,
	       int level, const char *fmt, va_list ap);
static inline int
log_write(const struct log_info *log, int level, const char *fmt, ...)
{
	int i;
	va_list ap;

	va_start(ap, fmt);
	i = log_vwrite(log, level, fmt, ap);
	va_end(ap);

	return i;
}
int log_flush(const struct log_info *log);
int log_close(const struct log_info *log);

/* do not try to change the variable */
extern const struct log_info *_glog_info;
static inline int _log_open_glog(const char *path, int console, int level)
{
	log_close(_glog_info);
	_glog_info = log_open(path, console, level);
	if (_glog_info)
		return 0;

	return -1;
}

/* you really need multi logs in an application ? */

__END_DECLS

#endif /* __YCC_COMMON_LOG_H_ */

/* eof */
