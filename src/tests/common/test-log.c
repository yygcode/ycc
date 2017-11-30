/*
 * test-log.c
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

#include <config-os.h>

#include <ycc/common/debug.h>
#include <ycc/common/log.h>

int main()
{
	if (gl_open("/tmp/tests/common/test-log.log", 1, LOG_DEBUG)) {
		pr_err("LOG_OPEN failed");
		return 1;
	}

	gl_write(LOG_DEBUG, "This is first log, write by %s", "yanyg");
	gl_write(LOG_INFO, "This is first log, write by %s", "yanyg");
	gl_write(LOG_NOTICE, "This is first log, write by %s", "yanyg");
	gl_write(LOG_WARNING, "This is first log, write by %s", "yanyg");
	gl_write(LOG_ERR, "This is first log, write by %s", "yanyg");
	gl_write(LOG_CRIT, "This is first log, write by %s", "yanyg");
	gl_write(LOG_ALERT, "This is first log, write by %s", "yanyg");
	gl_write(LOG_EMERG, "This is first log, write by %s", "yanyg");
	gl_write(10, "This is first log, write by %s", "yanyg");

	struct log_info *log;
	
	log = log_open("/tmp/tests/common/test-yyg.log", 0, LOG_NOTICE);
	if (!log) {
		DBG_PE("log_open failed");
		return 1;
	}

	log_write(log, LOG_INFO, "This is yyg log %d", 1000);
	log_write(log, LOG_NOTICE, "This is yyg log %d", 1000);
	log_write(log, LOG_WARNING, "This is yyg log %d", 1000);

	log_close(log);

	log_write(log, LOG_INFO, "This is yyg log %d", 1000);
	log_write(log, LOG_NOTICE, "This is yyg log %d", 1000);
	log_write(log, LOG_WARNING, "This is yyg log %d", 1000);

	log = log_open("tmp/test/common/test-yyg.log", 1, LOG_WARNING);
	if (!log) {
		DBG_PE("log_open failed");
		return 1;
	}

	log_write(log, LOG_INFO, "This is yyg log %d", 2000);
	log_write(log, LOG_NOTICE, "This is yyg log %d", 2000);
	log_write(log, LOG_WARNING, "This is yyg log %d", 2000);

	log_close(log);

	log_write(log, LOG_INFO, "This is yyg log %d", 2000);
	log_write(log, LOG_NOTICE, "This is yyg log %d", 2000);
	log_write(log, LOG_WARNING, "This is yyg log %d", 2000);

	DBG_PR("success");
	return 0;
}

/* eof */
