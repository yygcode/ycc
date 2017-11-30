/*
 * nls.h - native language support
 *
 * Copyright (C) 2012-2013 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * Fri Jun 22, 2012
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

#ifndef __YCC_NLS_H_
#define __YCC_NLS_H_

#define	_(msgid)	gettext(msgid)

#ifdef ENABLE_NLS
#include <libintl.h>

#ifdef	DEFAULT_TEXT_DOMAIN
#undef	gettext
#define	gettext(msgid)	dgettext(DEFAULT_TEXT_DOMAIN, (msgid))
#undef	ngettext
#define	ngettext(msgid, msgid_plural, n)	\
		dngettext(DEFAULT_TEXT_DOMAIN, (msgid), (msgid_plural), (n))
#else

#undef	gettext
#define	gettext(msgid)	((const char*)(msgid))

#undef	dgettext
#define dgettext(domain_name, msgid)	((void)(domain_name), gettext(msgid))

#undef	dgettext
#define dcgettext(domain_name, msgid, category)	\
		((void)(domain_name), gettext(msgid))

#undef	ngettext
#define	ngettext(msgid, msgid_plural, n)	\
		((n) == 1			\
			? ((void)(msgid_plural), (const char*)(msgid))	\
			: ((void)(msgid), (const char*)(msgid_plural)))

#undef	dngettext
#define	dngettext(domain_name, msgid, msgid_plural, n)			\
		((void)(domain_name), ngettext(msgid, msgid_plural, n))

#undef	dcngettext
#define	dcngettext(domain_name, msgid, msgid_plural, n, category)	\
		((void)(category),					\
			dngettext(domain_name, msgid, msgid_plural, n))

#undef	textdomain
#define	textdomain(domain_name)		((const char*)(domain_name))

#undef	bindtextdomain
#define	bindtextdomain(domain_name, dir_name)				\
		((void)(domain_name), (const char*)(dir_name))

#undef	bind_textdomain_codeset
#define	bind_textdomain_codeset(domain_name, codeset)			\
		((void)(domain_name), (const char*)(codeset))
#endif

#endif
