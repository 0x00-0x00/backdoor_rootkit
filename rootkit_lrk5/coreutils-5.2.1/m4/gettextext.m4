# Assume AM_GNU_GETTEXT([external]) and Autoconf 2.54 or later, for coreutils.

# serial 2

dnl Copyright (C) 2003 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

dnl From Paul Eggert

dnl Automake doesn't understand that AM_GNU_GETTEXT([external])
dnl never invokes the following macros, because of the [external].
dnl Insert empty macros to pacify Automake.

AC_DEFUN([AM_LC_MESSAGES])
AC_DEFUN([AM_MKINSTALLDIRS])
AC_DEFUN([gt_INTDIV0])

dnl A simpler substitute for gt_INTTYPES_PRI that assumes Autoconf 2.54
dnl or later.

AC_DEFUN([gt_HEADER_INTTYPES_H], [
  AC_REQUIRE([AC_HEADER_STDC])
  gt_cv_header_inttypes_h=$ac_cv_header_inttypes_h
])
