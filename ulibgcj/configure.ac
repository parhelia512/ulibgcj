dnl # Process this with autoconf to create configure
AC_PREREQ(2.59)
AC_INIT(ulibgcj, 0.1)

AC_CANONICAL_SYSTEM

AC_SUBST(target_noncanonical)

AM_INIT_AUTOMAKE(1.9.0)

AC_CONFIG_SRCDIR([../libjava/java/lang/System.java])

AC_CONFIG_HEADERS([include/config.h])

GCC_NO_EXECUTABLES

AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_GCJ
AM_PROG_CC_C_O

AC_CHECK_PROGS(PERL, perl, false)

libgcj_cflags=
libgcj_cxxflags=
libgcj_javaflags=

. ${srcdir}/../libjava/configure.host

LIBGCJ_CFLAGS="${libgcj_cflags}"
LIBGCJ_CXXFLAGS="${libgcj_cxxflags}"
LIBGCJ_JAVAFLAGS="${libgcj_javaflags}"
LIBGCJ_LD_SYMBOLIC="${libgcj_ld_symbolic}"
AC_SUBST(LIBGCJ_CFLAGS)
AC_SUBST(LIBGCJ_CXXFLAGS)
AC_SUBST(LIBGCJ_JAVAFLAGS)
AC_SUBST(LIBGCJ_LD_SYMBOLIC)

libjava=../libjava

## platform ###################################################################

case "$host" in
  *mingw*)
    PLATFORM=Win32
    PLATFORMNET=Win32
    PLATFORMH=win32.h
    CHECK_FOR_BROKEN_MINGW_LD
    ;;
  *)
    PLATFORM=Posix
    PLATFORMNET=Posix
    PLATFORMH=posix.h
    ;;
esac

AC_CONFIG_LINKS(include/platform.h:${libjava}/include/${PLATFORMH})
AM_CONDITIONAL(USING_WIN32_PLATFORM, test "${PLATFORM}" = Win32)
AM_CONDITIONAL(USING_POSIX_PLATFORM, test "${PLATFORM}" = Posix)

case "$host" in
  *-darwin*) DARWIN_CRT=true ;;
  *) DARWIN_CRT=false ;;
esac
AM_CONDITIONAL(USING_DARWIN_CRT, $DARWIN_CRT)

## gc #########################################################################

AC_MSG_CHECKING([for garbage collector to use])
AC_ARG_ENABLE(java-gc,
  AS_HELP_STRING([--enable-java-gc=TYPE],
                 [choose garbage collector (default is boehm)]),
  [GC=$enableval],
  [GC=boehm])

GCLIBS=
GCINCS=
case "$GC" in
 boehm)
    AC_MSG_RESULT(boehm)
    GCLIBS=../boehm-gc/.libs/libgcjgc_convenience.a
    GCINCS='-I$(top_srcdir)/../boehm-gc/include -I../boehm-gc/include'
    GCHDR=boehm-gc.h
    # The POSIX thread support needs to know this.
    AC_DEFINE(HAVE_BOEHM_GC, 1, [Define if Boehm GC in use.])
    ;;
 no)
    AC_MSG_RESULT(none)
    GCHDR=no-gc.h
    ;;
 *)
    AC_MSG_ERROR([unrecognized collector "$GC"])
    ;;
esac

AC_CONFIG_LINKS(include/java-gc.h:${libjava}/include/${GCHDR})
AC_SUBST(GCLIBS)
AC_SUBST(GCINCS)
AM_CONDITIONAL(USING_BOEHMGC, test "${GC}" = boehm)
AM_CONDITIONAL(USING_NOGC, test "${GC}" = no)

## threads ####################################################################

AC_MSG_CHECKING([for thread model used by GCC])
THREADS=`$CC -v 2>&1 | sed -n 's/^Thread model: //p'`
AC_MSG_RESULT([$THREADS])

case "$THREADS" in
 no | none | single)
    THREADS=none
    ;;
 posix | posix95 | pthreads)
    THREADS=posix
    case "$host" in
     *-*-linux*)
	AC_DEFINE(LINUX_THREADS, 1, [Define if using POSIX threads on Linux.])
	;;
    esac
    ;;
 win32)
    ;;
 *)
    AC_MSG_ERROR($THREADS is an unknown thread package)
    ;;
esac

THREADLIBS=
THREADH=
case "$THREADS" in
 posix)
    THREADLIBS=-lpthread
    THREADH=posix-threads.h
    AC_DEFINE(HAVE_PTHREAD_MUTEXATTR_INIT, 1,
      [Define if using POSIX threads that have the mutexattr functions.])
    AC_DEFINE(_POSIX_PTHREAD_SEMANTICS, 1,
      [Required define if using POSIX threads])
    ;;

 win32)
    THREADH=win32-threads.h
    ;;

 none)
    THREADH=no-threads.h
    ;;
esac

AC_CONFIG_LINKS(include/java-threads.h:${libjava}/include/${THREADH})
AC_SUBST(THREADLIBS)
AM_CONDITIONAL(USING_POSIX_THREADS, test "${THREADS}" = posix)
AM_CONDITIONAL(USING_WIN32_THREADS, test "${THREADS}" = win32)
AM_CONDITIONAL(USING_NO_THREADS, test "${THREADS}" = none)

## signals ####################################################################

SIGNAL_HANDLER_AUX=
case "${host}" in
 i?86-*-linux*)
    SIGNAL_HANDLER=include/i386-signal.h
    ;;
 powerpc*-*-linux*)
    SIGNAL_HANDLER=include/powerpc-signal.h
    ;;
 x86_64*-*-linux*)
    SIGNAL_HANDLER=include/x86_64-signal.h
    SIGNAL_HANDLER_AUX=include/i386-signal.h
    ;;
 *mingw*)
    SIGNAL_HANDLER=include/win32-signal.h
    ;;
 *-*-darwin*)
    SIGNAL_HANDLER=include/darwin-signal.h
    ;;
 *)
    SIGNAL_HANDLER=include/default-signal.h
    ;;
esac

# If we're using sjlj exceptions, forget what we just learned.
if test "$enable_sjlj_exceptions" = yes; then
   SIGNAL_HANDLER=include/default-signal.h
   SIGNAL_HANDLER_AUX=
fi

AC_CONFIG_LINKS(include/java-signal.h:${libjava}/${SIGNAL_HANDLER}
                include/java-signal-aux.h:${libjava}/${SIGNAL_HANDLER_AUX})

###############################################################################

AC_DEFINE(JV_HASH_SYNCHRONIZATION, 1,
          [Define if hash synchronization is in use])

built_gcc_dir="`cd ../../${host_subdir}/gcc && ${PWDCMD-pwd}`"
GCJ="${built_gcc_dir}/gcj -B${built_gcc_dir}/"
GCJH="${built_gcc_dir}/gcjh"
AC_SUBST(GCJ)
AC_SUBST(GCJH)
AC_SUBST(GCJFLAGS)

if test -d sysdep; then true; else mkdir sysdep; fi
AC_CONFIG_LINKS(sysdep/locks.h:${libjava}/sysdep/$sysdeps_dir/locks.h)

AC_DEFINE(JV_ULIBGCJ, 1, [Define if micro-libgcj is to be built])

AC_DEFINE(HAVE_MEMMOVE, 1, [Define if memmove is available])

mkdir -p gcj
echo > gcj/libgcj-config.h

AC_CHECK_HEADERS([unistd.h bstring.h sys/time.h string.h sys/types.h \
		  pwd.h sys/config.h stdint.h langinfo.h locale.h \
		  dirent.h math.h])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
