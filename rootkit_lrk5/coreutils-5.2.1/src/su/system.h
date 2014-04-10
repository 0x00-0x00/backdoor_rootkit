/* system-dependent definitions for fileutils, textutils, and sh-utils packages.
   Copyright (C) 1989, 1991-2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include <alloca.h>

/* Include sys/types.h before this file.  */

#if 2 <= __GLIBC__ && 2 <= __GLIBC_MINOR__
# if ! defined _SYS_TYPES_H
you must include <sys/types.h> before including this file
# endif
#endif

#include <sys/stat.h>

#if !defined HAVE_MKFIFO
# define mkfifo(path, mode) (mknod ((path), (mode) | S_IFIFO, 0))
#endif

#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

/* <unistd.h> should be included before any preprocessor test
   of _POSIX_VERSION.  */
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifndef STDIN_FILENO
# define STDIN_FILENO 0
#endif

#ifndef STDOUT_FILENO
# define STDOUT_FILENO 1
#endif

#ifndef STDERR_FILENO
# define STDERR_FILENO 2
#endif


/* limits.h must come before pathmax.h because limits.h on some systems
   undefs PATH_MAX, whereas pathmax.h sets PATH_MAX.  */
#include <limits.h>


#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

/* Since major is a function on SVR4, we can't use `ifndef major'.  */
#if MAJOR_IN_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#endif
#if MAJOR_IN_SYSMACROS
# include <sys/sysmacros.h>
# define HAVE_MAJOR
#endif
#ifdef major			/* Might be defined in sys/types.h.  */
# define HAVE_MAJOR
#endif

#ifndef HAVE_MAJOR
# define major(dev)  (((dev) >> 8) & 0xff)
# define minor(dev)  ((dev) & 0xff)
# define makedev(maj, min)  (((maj) << 8) | (min))
#endif
#undef HAVE_MAJOR

#if HAVE_UTIME_H
# include <utime.h>
#endif

/* Some systems (even some that do have <utime.h>) don't declare this
   structure anywhere.  */
#ifndef HAVE_STRUCT_UTIMBUF
struct utimbuf
{
  long actime;
  long modtime;
};
#endif

/* Don't use bcopy!  Use memmove if source and destination may overlap,
   memcpy otherwise.  */

#include <string.h>
#if ! HAVE_DECL_MEMRCHR
void *memrchr (const void *, int, size_t);
#endif

#include <errno.h>
#ifndef errno
extern int errno;
#endif

/* Some systems don't define the following symbols.  */
#ifndef ENOSYS
# define ENOSYS (-1)
#endif
#ifndef ENOTSUP
# define ENOTSUP (-1)
#endif
#ifndef EISDIR
# define EISDIR (-1)
#endif

#include <stdbool.h>

#define getopt system_getopt
#include <stdlib.h>
#undef getopt

/* The following test is to work around the gross typo in
   systems like Sony NEWS-OS Release 4.0C, whereby EXIT_FAILURE
   is defined to 0, not 1.  */
#if !EXIT_FAILURE
# undef EXIT_FAILURE
# define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
# define EXIT_SUCCESS 0
#endif

/* Exit statuses for programs like 'env' that exec other programs.
   EXIT_FAILURE might not be 1, so use EXIT_FAIL in such programs.  */
enum
{
  EXIT_FAIL = 1,
  EXIT_CANNOT_INVOKE = 126,
  EXIT_ENOENT = 127
};

/* Set exit_failure to STATUS if that's not the default already.  */
static inline void
initialize_exit_failure (int status)
{
}

#if HAVE_FCNTL_H
# include <fcntl.h>
#else
# include <sys/file.h>
#endif

#if !defined SEEK_SET
# define SEEK_SET 0
# define SEEK_CUR 1
# define SEEK_END 2
#endif
#ifndef F_OK
# define F_OK 0
# define X_OK 1
# define W_OK 2
# define R_OK 4
#endif

/* For systems that distinguish between text and binary I/O.
   O_BINARY is usually declared in fcntl.h  */
#if !defined O_BINARY && defined _O_BINARY
  /* For MSC-compatible compilers.  */
# define O_BINARY _O_BINARY
# define O_TEXT _O_TEXT
#endif

#if !defined O_NDELAY
# define O_NDELAY 0
#endif

#if !defined O_NONBLOCK
# define O_NONBLOCK O_NDELAY
#endif

#if !defined O_NOCTTY
# define O_NOCTTY 0
#endif

#ifdef __BEOS__
  /* BeOS 5 has O_BINARY and O_TEXT, but they have no effect.  */
# undef O_BINARY
# undef O_TEXT
#endif

#if O_BINARY
# ifndef __DJGPP__
#  define setmode _setmode
#  define fileno(_fp) _fileno (_fp)
# endif /* not DJGPP */
# define SET_MODE(_f, _m) setmode (_f, _m)
# define SET_BINARY(_f) do {if (!isatty(_f)) setmode (_f, O_BINARY);} while (0)
# define SET_BINARY2(_f1, _f2)		\
  do {					\
    if (!isatty (_f1))			\
      {					\
        setmode (_f1, O_BINARY);	\
	if (!isatty (_f2))		\
	  setmode (_f2, O_BINARY);	\
      }					\
  } while(0)
#else
# define SET_MODE(_f, _m) (void)0
# define SET_BINARY(f) (void)0
# define SET_BINARY2(f1,f2) (void)0
# define O_BINARY 0
# define O_TEXT 0
#endif /* O_BINARY */

#if HAVE_DIRENT_H
# include <dirent.h>
# define NLENGTH(direct) (strlen((direct)->d_name))
#else /* not HAVE_DIRENT_H */
# define dirent direct
# define NLENGTH(direct) ((direct)->d_namlen)
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif /* HAVE_SYS_NDIR_H */
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif /* HAVE_SYS_DIR_H */
# if HAVE_NDIR_H
#  include <ndir.h>
# endif /* HAVE_NDIR_H */
#endif /* HAVE_DIRENT_H */

#if CLOSEDIR_VOID
/* Fake a return value. */
# define CLOSEDIR(d) (closedir (d), 0)
#else
# define CLOSEDIR(d) closedir (d)
#endif

/* Get or fake the disk device blocksize.
   Usually defined by sys/param.h (if at all).  */
#if !defined DEV_BSIZE && defined BSIZE
# define DEV_BSIZE BSIZE
#endif
#if !defined DEV_BSIZE && defined BBSIZE /* SGI */
# define DEV_BSIZE BBSIZE
#endif
#ifndef DEV_BSIZE
# define DEV_BSIZE 4096
#endif

/* Extract or fake data from a `struct stat'.
   ST_BLKSIZE: Preferred I/O blocksize for the file, in bytes.
   ST_NBLOCKS: Number of blocks in the file, including indirect blocks.
   ST_NBLOCKSIZE: Size of blocks used when calculating ST_NBLOCKS.  */
#ifndef HAVE_STRUCT_STAT_ST_BLOCKS
# define ST_BLKSIZE(statbuf) DEV_BSIZE
# if defined _POSIX_SOURCE || !defined BSIZE /* fileblocks.c uses BSIZE.  */
#  define ST_NBLOCKS(statbuf) \
  ((statbuf).st_size / ST_NBLOCKSIZE + ((statbuf).st_size % ST_NBLOCKSIZE != 0))
# else /* !_POSIX_SOURCE && BSIZE */
#  define ST_NBLOCKS(statbuf) \
  (S_ISREG ((statbuf).st_mode) \
   || S_ISDIR ((statbuf).st_mode) \
   ? st_blocks ((statbuf).st_size) : 0)
# endif /* !_POSIX_SOURCE && BSIZE */
#else /* HAVE_STRUCT_STAT_ST_BLOCKS */
/* Some systems, like Sequents, return st_blksize of 0 on pipes.
   Also, when running `rsh hpux11-system cat any-file', cat would
   determine that the output stream had an st_blksize of 2147421096.
   So here we arbitrarily limit the `optimal' block size to 4MB.
   If anyone knows of a system for which the legitimate value for
   st_blksize can exceed 4MB, please report it as a bug in this code.  */
# define ST_BLKSIZE(statbuf) ((0 < (statbuf).st_blksize \
			       && (statbuf).st_blksize <= (1 << 22)) /* 4MB */ \
			      ? (statbuf).st_blksize : DEV_BSIZE)
# if defined hpux || defined __hpux__ || defined __hpux
/* HP-UX counts st_blocks in 1024-byte units.
   This loses when mixing HP-UX and BSD filesystems with NFS.  */
#  define ST_NBLOCKSIZE 1024
# else /* !hpux */
#  if defined _AIX && defined _I386
/* AIX PS/2 counts st_blocks in 4K units.  */
#   define ST_NBLOCKSIZE (4 * 1024)
#  else /* not AIX PS/2 */
#   if defined _CRAY
#    define ST_NBLOCKS(statbuf) \
  (S_ISREG ((statbuf).st_mode) \
   || S_ISDIR ((statbuf).st_mode) \
   ? (statbuf).st_blocks * ST_BLKSIZE(statbuf)/ST_NBLOCKSIZE : 0)
#   endif /* _CRAY */
#  endif /* not AIX PS/2 */
# endif /* !hpux */
#endif /* HAVE_STRUCT_STAT_ST_BLOCKS */

#ifndef ST_NBLOCKS
# define ST_NBLOCKS(statbuf) ((statbuf).st_blocks)
#endif

#ifndef ST_NBLOCKSIZE
# define ST_NBLOCKSIZE 512
#endif

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for native VMS. */
#ifndef initialize_main
# define initialize_main(ac, av)
#endif

#ifndef S_IFMT
# define S_IFMT 0170000
#endif

#if STAT_MACROS_BROKEN
# undef S_ISBLK
# undef S_ISCHR
# undef S_ISDIR
# undef S_ISDOOR
# undef S_ISFIFO
# undef S_ISLNK
# undef S_ISNAM
# undef S_ISMPB
# undef S_ISMPC
# undef S_ISNWK
# undef S_ISREG
# undef S_ISSOCK
#endif


#ifndef S_ISBLK
# ifdef S_IFBLK
#  define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)
# else
#  define S_ISBLK(m) 0
# endif
#endif

#ifndef S_ISCHR
# ifdef S_IFCHR
#  define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
# else
#  define S_ISCHR(m) 0
# endif
#endif

#ifndef S_ISDIR
# ifdef S_IFDIR
#  define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
# else
#  define S_ISDIR(m) 0
# endif
#endif

#ifndef S_ISDOOR /* Solaris 2.5 and up */
# ifdef S_IFDOOR
#  define S_ISDOOR(m) (((m) & S_IFMT) == S_IFDOOR)
# else
#  define S_ISDOOR(m) 0
# endif
#endif

#ifndef S_ISFIFO
# ifdef S_IFIFO
#  define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
# else
#  define S_ISFIFO(m) 0
# endif
#endif

#ifndef S_ISLNK
# ifdef S_IFLNK
#  define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
# else
#  define S_ISLNK(m) 0
# endif
#endif

#ifndef S_ISMPB /* V7 */
# ifdef S_IFMPB
#  define S_ISMPB(m) (((m) & S_IFMT) == S_IFMPB)
#  define S_ISMPC(m) (((m) & S_IFMT) == S_IFMPC)
# else
#  define S_ISMPB(m) 0
#  define S_ISMPC(m) 0
# endif
#endif

#ifndef S_ISNAM /* Xenix */
# ifdef S_IFNAM
#  define S_ISNAM(m) (((m) & S_IFMT) == S_IFNAM)
# else
#  define S_ISNAM(m) 0
# endif
#endif

#ifndef S_ISNWK /* HP/UX */
# ifdef S_IFNWK
#  define S_ISNWK(m) (((m) & S_IFMT) == S_IFNWK)
# else
#  define S_ISNWK(m) 0
# endif
#endif

#ifndef S_ISREG
# ifdef S_IFREG
#  define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
# else
#  define S_ISREG(m) 0
# endif
#endif

#ifndef S_ISSOCK
# ifdef S_IFSOCK
#  define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
# else
#  define S_ISSOCK(m) 0
# endif
#endif


#ifndef S_TYPEISSEM
# ifdef S_INSEM
#  define S_TYPEISSEM(p) (S_ISNAM ((p)->st_mode) && (p)->st_rdev == S_INSEM)
# else
#  define S_TYPEISSEM(p) 0
# endif
#endif

#ifndef S_TYPEISSHM
# ifdef S_INSHD
#  define S_TYPEISSHM(p) (S_ISNAM ((p)->st_mode) && (p)->st_rdev == S_INSHD)
# else
#  define S_TYPEISSHM(p) 0
# endif
#endif

#ifndef S_TYPEISMQ
# define S_TYPEISMQ(p) 0
#endif


/* If any of the following are undefined,
   define them to their de facto standard values.  */
#if !S_ISUID
# define S_ISUID 04000
#endif
#if !S_ISGID
# define S_ISGID 02000
#endif

/* S_ISVTX is a common extension to POSIX.  */
#ifndef S_ISVTX
# define S_ISVTX 01000
#endif

#if !S_IRUSR && S_IREAD
# define S_IRUSR S_IREAD
#endif
#if !S_IRUSR
# define S_IRUSR 00400
#endif
#if !S_IRGRP
# define S_IRGRP (S_IRUSR >> 3)
#endif
#if !S_IROTH
# define S_IROTH (S_IRUSR >> 6)
#endif

#if !S_IWUSR && S_IWRITE
# define S_IWUSR S_IWRITE
#endif
#if !S_IWUSR
# define S_IWUSR 00200
#endif
#if !S_IWGRP
# define S_IWGRP (S_IWUSR >> 3)
#endif
#if !S_IWOTH
# define S_IWOTH (S_IWUSR >> 6)
#endif

#if !S_IXUSR && S_IEXEC
# define S_IXUSR S_IEXEC
#endif
#if !S_IXUSR
# define S_IXUSR 00100
#endif
#if !S_IXGRP
# define S_IXGRP (S_IXUSR >> 3)
#endif
#if !S_IXOTH
# define S_IXOTH (S_IXUSR >> 6)
#endif

#if !S_IRWXU
# define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)
#endif
#if !S_IRWXG
# define S_IRWXG (S_IRGRP | S_IWGRP | S_IXGRP)
#endif
#if !S_IRWXO
# define S_IRWXO (S_IROTH | S_IWOTH | S_IXOTH)
#endif

/* S_IXUGO is a common extension to POSIX.  */
#if !S_IXUGO
# define S_IXUGO (S_IXUSR | S_IXGRP | S_IXOTH)
#endif

#ifndef S_IRWXUGO
# define S_IRWXUGO (S_IRWXU | S_IRWXG | S_IRWXO)
#endif

/* All the mode bits that can be affected by chmod.  */
#define CHMOD_MODE_BITS \
  (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO)


#ifndef RETSIGTYPE
# define RETSIGTYPE void
#endif

#ifdef __DJGPP__
  /* We need the declaration of setmode.  */
# include <io.h>
  /* We need the declaration of __djgpp_set_ctrl_c.  */
# include <sys/exceptn.h>
#endif

#if HAVE_STDINT_H
# include <stdint.h>
#endif

#if HAVE_INTTYPES_H
# include <inttypes.h> /* for the definition of UINTMAX_MAX */
#endif

#if !defined PRIdMAX || PRI_MACROS_BROKEN
# undef PRIdMAX
# define PRIdMAX (sizeof (uintmax_t) == sizeof (long) ? "ld" : "lld")
#endif
#if !defined PRIoMAX || PRI_MACROS_BROKEN
# undef PRIoMAX
# define PRIoMAX (sizeof (uintmax_t) == sizeof (long) ? "lo" : "llo")
#endif
#if !defined PRIuMAX || PRI_MACROS_BROKEN
# undef PRIuMAX
# define PRIuMAX (sizeof (uintmax_t) == sizeof (long) ? "lu" : "llu")
#endif
#if !defined PRIxMAX || PRI_MACROS_BROKEN
# undef PRIxMAX
# define PRIxMAX (sizeof (uintmax_t) == sizeof (long) ? "lx" : "llx")
#endif

#include <ctype.h>

/* Jim Meyering writes:

   "... Some ctype macros are valid only for character codes that
   isascii says are ASCII (SGI's IRIX-4.0.5 is one such system --when
   using /bin/cc or gcc but without giving an ansi option).  So, all
   ctype uses should be through macros like ISPRINT...  If
   STDC_HEADERS is defined, then autoconf has verified that the ctype
   macros don't need to be guarded with references to isascii. ...
   Defining isascii to 1 should let any compiler worth its salt
   eliminate the && through constant folding."

   Bruno Haible adds:

   "... Furthermore, isupper(c) etc. have an undefined result if c is
   outside the range -1 <= c <= 255. One is tempted to write isupper(c)
   with c being of type `char', but this is wrong if c is an 8-bit
   character >= 128 which gets sign-extended to a negative value.
   The macro ISUPPER protects against this as well."  */

#if STDC_HEADERS || (!defined (isascii) && !HAVE_ISASCII)
# define IN_CTYPE_DOMAIN(c) 1
#else
# define IN_CTYPE_DOMAIN(c) isascii(c)
#endif

#ifdef isblank
# define ISBLANK(c) (IN_CTYPE_DOMAIN (c) && isblank (c))
#else
# define ISBLANK(c) ((c) == ' ' || (c) == '\t')
#endif
#ifdef isgraph
# define ISGRAPH(c) (IN_CTYPE_DOMAIN (c) && isgraph (c))
#else
# define ISGRAPH(c) (IN_CTYPE_DOMAIN (c) && isprint (c) && !isspace (c))
#endif

/* This is defined in <sys/euc.h> on at least Solaris2.6 systems.  */
#undef ISPRINT

#define ISPRINT(c) (IN_CTYPE_DOMAIN (c) && isprint (c))
#define ISALNUM(c) (IN_CTYPE_DOMAIN (c) && isalnum (c))
#define ISALPHA(c) (IN_CTYPE_DOMAIN (c) && isalpha (c))
#define ISCNTRL(c) (IN_CTYPE_DOMAIN (c) && iscntrl (c))
#define ISLOWER(c) (IN_CTYPE_DOMAIN (c) && islower (c))
#define ISPUNCT(c) (IN_CTYPE_DOMAIN (c) && ispunct (c))
#define ISSPACE(c) (IN_CTYPE_DOMAIN (c) && isspace (c))
#define ISUPPER(c) (IN_CTYPE_DOMAIN (c) && isupper (c))
#define ISXDIGIT(c) (IN_CTYPE_DOMAIN (c) && isxdigit (c))
#define ISDIGIT_LOCALE(c) (IN_CTYPE_DOMAIN (c) && isdigit (c))

#if STDC_HEADERS
# define TOLOWER(Ch) tolower (Ch)
# define TOUPPER(Ch) toupper (Ch)
#else
# define TOLOWER(Ch) (ISUPPER (Ch) ? tolower (Ch) : (Ch))
# define TOUPPER(Ch) (ISLOWER (Ch) ? toupper (Ch) : (Ch))
#endif

/* ISDIGIT differs from ISDIGIT_LOCALE, as follows:
   - Its arg may be any int or unsigned int; it need not be an unsigned char.
   - It's guaranteed to evaluate its argument exactly once.
   - It's typically faster.
   POSIX says that only '0' through '9' are digits.  Prefer ISDIGIT to
   ISDIGIT_LOCALE unless it's important to use the locale's definition
   of `digit' even when the host does not conform to POSIX.  */
#define ISDIGIT(c) ((unsigned) (c) - '0' <= 9)

/* Take care of NLS matters.  */

#if HAVE_LOCALE_H
# include <locale.h>
#else
# define setlocale(Category, Locale) /* empty */
#endif

#if ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /* empty */
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /* empty */
#endif

#define _(msgid) gettext (msgid)
#define N_(msgid) msgid

#ifndef HAVE_SETLOCALE
# define HAVE_SETLOCALE 0
#endif

#define STREQ(a, b) (strcmp ((a), (b)) == 0)

#if !HAVE_DECL_FREE
void free ();
#endif

#if !HAVE_DECL_MALLOC
char *malloc ();
#endif

#if !HAVE_DECL_MEMCHR
char *memchr ();
#endif

#if !HAVE_DECL_REALLOC
char *realloc ();
#endif

#if !HAVE_DECL_STPCPY
# ifndef stpcpy
char *stpcpy ();
# endif
#endif

#if !HAVE_DECL_STRNDUP
char *strndup ();
#endif

#if !HAVE_DECL_STRSTR
char *strstr ();
#endif

#if !HAVE_DECL_GETENV
char *getenv ();
#endif

#if !HAVE_DECL_LSEEK
off_t lseek ();
#endif

/* This is needed on some AIX systems.  */
#if !HAVE_DECL_STRTOUL
unsigned long strtoul ();
#endif

#if !HAVE_DECL_GETLOGIN
char *getlogin ();
#endif

#if !HAVE_DECL_TTYNAME
char *ttyname ();
#endif

#if !HAVE_DECL_GETEUID
uid_t geteuid ();
#endif

#if !HAVE_DECL_GETPWUID
struct passwd *getpwuid ();
#endif

#if !HAVE_DECL_GETGRGID
struct group *getgrgid ();
#endif

#if !HAVE_DECL_GETUID
uid_t getuid ();
#endif

#if ! defined HAVE_MEMPCPY && ! defined mempcpy
/* Be CAREFUL that there are no side effects in N.  */
# define mempcpy(D, S, N) ((void *) ((char *) memcpy (D, S, N) + (N)))
#endif

/* Include automatically-generated macros for unlocked I/O.  */

#define SAME_INODE(Stat_buf_1, Stat_buf_2) \
  ((Stat_buf_1).st_ino == (Stat_buf_2).st_ino \
   && (Stat_buf_1).st_dev == (Stat_buf_2).st_dev)

#define DOT_OR_DOTDOT(Basename) \
  (Basename[0] == '.' && (Basename[1] == '\0' \
			  || (Basename[1] == '.' && Basename[2] == '\0')))

#if SETVBUF_REVERSED
# define SETVBUF(Stream, Buffer, Type, Size) \
    setvbuf (Stream, Type, Buffer, Size)
#else
# define SETVBUF(Stream, Buffer, Type, Size) \
    setvbuf (Stream, Buffer, Type, Size)
#endif

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
enum
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};

#define GETOPT_HELP_OPTION_DECL \
  "help", no_argument, 0, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, 0, GETOPT_VERSION_CHAR

#define case_GETOPT_HELP_CHAR			\
  case GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    break;

#define HELP_OPTION_DESCRIPTION \
  _("      --help     display this help and exit\n")
#define VERSION_OPTION_DESCRIPTION \
  _("      --version  output version information and exit\n")


#define case_GETOPT_VERSION_CHAR(Program_name, Authors)			\
  case GETOPT_VERSION_CHAR:						\
    version_etc (stdout, Program_name, PACKAGE, VERSION, Authors,	\
                 (char *) NULL);					\
    exit (EXIT_SUCCESS);						\
    break;

#ifndef MAX
# define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef CHAR_BIT
# define CHAR_BIT 8
#endif

/* The extra casts work around common compiler bugs.  */
#define TYPE_SIGNED(t) (! ((t) 0 < (t) -1))
/* The outer cast is needed to work around a bug in Cray C 5.0.3.0.
   It is necessary at least when t == time_t.  */
#define TYPE_MINIMUM(t) ((t) (TYPE_SIGNED (t) \
			      ? ~ (t) 0 << (sizeof (t) * CHAR_BIT - 1) : (t) 0))
#define TYPE_MAXIMUM(t) ((t) (~ (t) 0 - TYPE_MINIMUM (t)))

/* Upper bound on the string length of an integer converted to string.
   302 / 1000 is ceil (log10 (2.0)).  Subtract 1 for the sign bit;
   add 1 for integer division truncation; add 1 more for a minus sign.  */
#define INT_STRLEN_BOUND(t) ((sizeof (t) * CHAR_BIT - 1) * 302 / 1000 + 2)

#ifndef CHAR_MIN
# define CHAR_MIN TYPE_MINIMUM (char)
#endif

#ifndef CHAR_MAX
# define CHAR_MAX TYPE_MAXIMUM (char)
#endif

#ifndef SCHAR_MIN
# define SCHAR_MIN (-1 - SCHAR_MAX)
#endif

#ifndef SCHAR_MAX
# define SCHAR_MAX (CHAR_MAX == UCHAR_MAX ? CHAR_MAX / 2 : CHAR_MAX)
#endif

#ifndef UCHAR_MAX
# define UCHAR_MAX TYPE_MAXIMUM (unsigned char)
#endif

#ifndef SHRT_MIN
# define SHRT_MIN TYPE_MINIMUM (short int)
#endif

#ifndef SHRT_MAX
# define SHRT_MAX TYPE_MAXIMUM (short int)
#endif

#ifndef INT_MAX
# define INT_MAX TYPE_MAXIMUM (int)
#endif

#ifndef INT_MIN
# define INT_MIN TYPE_MINIMUM (int)
#endif

#ifndef UINT_MAX
# define UINT_MAX TYPE_MAXIMUM (unsigned int)
#endif

#ifndef LONG_MAX
# define LONG_MAX TYPE_MAXIMUM (long)
#endif

#ifndef ULONG_MAX
# define ULONG_MAX TYPE_MAXIMUM (unsigned long)
#endif

#ifndef SIZE_MAX
# define SIZE_MAX TYPE_MAXIMUM (size_t)
#endif

#ifndef SSIZE_MAX
# define SSIZE_MAX TYPE_MAXIMUM (ssize_t)
#endif

#ifndef UINTMAX_MAX
# define UINTMAX_MAX TYPE_MAXIMUM (uintmax_t)
#endif

#ifndef OFF_T_MIN
# define OFF_T_MIN TYPE_MINIMUM (off_t)
#endif

#ifndef OFF_T_MAX
# define OFF_T_MAX TYPE_MAXIMUM (off_t)
#endif

#ifndef UID_T_MAX
# define UID_T_MAX TYPE_MAXIMUM (uid_t)
#endif

#ifndef GID_T_MAX
# define GID_T_MAX TYPE_MAXIMUM (gid_t)
#endif

#ifndef PID_T_MAX
# define PID_T_MAX TYPE_MAXIMUM (pid_t)
#endif

/* Use this to suppress gcc's `...may be used before initialized' warnings. */
#ifdef lint
# define IF_LINT(Code) Code
#else
# define IF_LINT(Code) /* empty */
#endif

#ifndef __attribute__
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 8) || __STRICT_ANSI__
#  define __attribute__(x)
# endif
#endif

#ifndef ATTRIBUTE_NORETURN
# define ATTRIBUTE_NORETURN __attribute__ ((__noreturn__))
#endif

#ifndef ATTRIBUTE_UNUSED
# define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#endif

#if defined strdupa
# define ASSIGN_STRDUPA(DEST, S)		\
  do { DEST = strdupa (S); } while (0)
#else
# define ASSIGN_STRDUPA(DEST, S)		\
  do						\
    {						\
      const char *s_ = (S);			\
      size_t len_ = strlen (s_) + 1;		\
      char *tmp_dest_ = alloca (len_);		\
      DEST = memcpy (tmp_dest_, (s_), len_);	\
    }						\
  while (0)
#endif

#ifndef EOVERFLOW
# define EOVERFLOW EINVAL
#endif

#if ! HAVE_FSEEKO && ! defined fseeko
# define fseeko(s, o, w) ((o) == (long) (o)		\
			  ? fseek (s, o, w)		\
			  : (errno = EOVERFLOW, -1))
#endif
