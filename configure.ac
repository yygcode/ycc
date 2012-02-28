#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([ycc],
	m4_esyscmd_s([build-aux/git-version-gen]),
	[yygcode@gmail.com],
	[ycc],
	[http://yygcode.blog.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror gnu])
LT_INIT

AC_CONFIG_SRCDIR([ChangeLog])
AC_COPYRIGHT([GPLv2])
AC_REVISION([$Revision: 1.0 $])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_HEADER_ASSERT
AC_HEADER_STDBOOL
AC_HEADER_STDC
AC_HEADER_TIME

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_CHECK_TYPES([u_char, u_short, u_int, u_long],,,
	       [[#include <sys/types.h>]])
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.

# subst
AC_SUBST([ycc_headerdir], [src/include])

# Define
AC_DEFINE_UNQUOTED([YCC_COMPILE_TIME], ["`date`"], ["Compile-Time-Stamp"])

AC_CONFIG_FILES([
		 Makefile
		 man/Makefile
                 src/Makefile
                 src/common/Makefile
                 src/algos/Makefile
                 src/net/Makefile
                 src/tests/Makefile
                 src/tests/common/Makefile
		 ])
AC_OUTPUT
