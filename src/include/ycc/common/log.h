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
#define GLOG_OPEN(path, console, level)	\
		(_log_open_glog((path), (console), (level)))
#define GLOG_SET(console, level)		\
		log_set(_ycc_glog, (console), (level))
#define GLOG_GET(pconsole, plevel)	\
		log_get(_ycc_glog, (pconsole), (plevel))
#define GLOG_DEBUG(fmt, ...)		\
		log_write(_ycc_glog, LOG_DEBUG, (fmt), ## __VA_ARGS__)
#define GLOG_INFO(fmt, ...)		\
		log_write(_ycc_glog, LOG_INFO, (fmt), ## __VA_ARGS__)
#define GLOG_NOTICE(fmt, ...)		\
		log_write(_ycc_glog, LOG_NOTICE, (fmt), ## __VA_ARGS__)
#define GLOG_WARN(fmt, ...)		\
		log_write(_ycc_glog, LOG_WARNING, (fmt), ## __VA_ARGS__)
#define GLOG_ERR(fmt, ...)		\
		log_write(_ycc_glog, LOG_ERR, (fmt), ## __VA_ARGS__)
#define GLOG_CRIT(fmt, ...)		\
		log_write(_ycc_glog, LOG_CRIT, (fmt), ## __VA_ARGS__)
#define GLOG_ALERT(fmt, ...)		\
		log_write(_ycc_glog, LOG_ALERT, (fmt), ## __VA_ARGS__)
#define GLOG_EMERG(fmt, ...)		\
		log_write(_ycc_glog, LOG_EMERG, (fmt), ## __VA_ARGS__)
#define GLOG_WRITE(level, fmt, ...)	\
		log_write(_ycc_glog, (level), (fmt), ## __VA_ARGS__)
#define GLOG_VWRITE(level, fmt, ap)	\
		log_vwrite(_ycc_glog, (level), (fmt), (ap))
#define GLOG_FLUSH()			\
		log_flush(_ycc_glog)
#define GLOG_CLOSE()			\
		log_close(_ycc_glog)

#define OUT_SET(console, level)		\
		log_set(_ycc_gout, (console), (level))
#define OUT_GET(pconsole, plevel)		\
		log_get(_ycc_gout, (pconsole), (plevel))
#define OUT_DEBUG(fmt, ...)		\
		log_write(_ycc_gout, LOG_DEBUG, (fmt), ## __VA_ARGS__)
#define OUT_INFO(fmt, ...)		\
		log_write(_ycc_gout, LOG_INFO, (fmt), ## __VA_ARGS__)
#define OUT_NOTICE(fmt, ...)		\
		log_write(_ycc_gout, LOG_NOTICE, (fmt), ## __VA_ARGS__)
#define OUT_WARN(fmt, ...)		\
		log_write(_ycc_gout, LOG_WARNING, (fmt), ## __VA_ARGS__)
#define OUT_ERR(fmt, ...)		\
		log_write(_ycc_gout, LOG_ERR, (fmt), ## __VA_ARGS__)
#define OUT_CRIT(fmt, ...)		\
		log_write(_ycc_gout, LOG_CRIT, (fmt), ## __VA_ARGS__)
#define OUT_ALERT(fmt, ...)		\
		log_write(_ycc_gout, LOG_ALERT, (fmt), ## __VA_ARGS__)
#define OUT_EMERG(fmt, ...)		\
		log_write(_ycc_gout, LOG_EMERG, (fmt), ## __VA_ARGS__)

/* Do you really need multi logs in an application ? over-design? */
struct log_info;
struct log_info *log_open(const char *path, int console, int level);
int log_set(struct log_info *log, int console, unsigned int level);
int log_get(const struct log_info *log, int *console, unsigned int *level);
int log_vwrite(struct log_info *log, int level, const char *fmt, va_list ap);
static inline int
log_write(struct log_info *log, int level, const char *fmt, ...)
{
	int i;
	va_list ap;

	va_start(ap, fmt);
	i = log_vwrite(log, level, fmt, ap);
	va_end(ap);

	return i;
}
int log_flush(const struct log_info *log);
int log_close(struct log_info *log);

/* do not try to change the variable */
extern struct log_info *_ycc_glog;
extern struct log_info *_ycc_gout;
static inline int _log_open_glog(const char *path, int console, int level)
{
	if (_ycc_glog)
		log_close(_ycc_glog);
	_ycc_glog = log_open(path, console, level);
	if (_ycc_glog)
		return 0;

	return -1;
}

/* you really need multi logs in an application ? */

__END_DECLS

#endif /* __YCC_COMMON_LOG_H_ */

/* eof */
