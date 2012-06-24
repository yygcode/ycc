/*
 * config-ycc.h - project config header file
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

#ifndef __YCC_CONFIG_YCC_H_
#define __YCC_CONFIG_YCC_H_

#ifdef __GNUC__
#include <ycc/compiler.h>
#endif

#ifndef __aligned
#define __aligned(x)
#endif

#ifndef __always_inline
#define __always_inline inline
#endif

/*
 * offsetof(TYPE, MEMBER)
 *  the offset of MEMBER of structure TYPE */
#ifndef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE, MEMBER)	__compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER)	((size_t) &((TYPE*)0)->MEMBER)
#endif
#endif

#ifndef container_of
#define container_of(ptr, type, member)	\
	((type*)((char*)ptr - offsetof(type, member)))
#endif

#ifndef __YCC_UNUSED
#define __YCC_UNUSED(var)	((void)(var))
#endif

#ifndef __BEGIN_DECLS
#ifdef __END_DECLS
# undef __END_DECLS
#endif
#ifdef  __cplusplus
# define __BEGIN_DECLS  extern "C" {
# define __END_DECLS    }
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif
#endif

#endif /* __YCC_CONFIG_YCC_H_ */
