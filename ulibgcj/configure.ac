dnl # Process this with autoconf to create configure
AC_PREREQ(2.59)
AC_INIT(ulibgcj, 0.1)

AC_CANONICAL_SYSTEM
_GCC_TOPLEV_NONCANONICAL_BUILD
_GCC_TOPLEV_NONCANONICAL_TARGET

AC_SUBST(target_noncanonical)

AM_INIT_AUTOMAKE(1.9.0)

AC_CONFIG_SRCDIR([../java/lang/System.java])

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

. ${srcdir}/../configure.host

LIBGCJ_CFLAGS="${libgcj_cflags}"
LIBGCJ_CXXFLAGS="${libgcj_cxxflags}"
LIBGCJ_JAVAFLAGS="${libgcj_javaflags}"
LIBGCJ_LD_SYMBOLIC="${libgcj_ld_symbolic}"
AC_SUBST(LIBGCJ_CFLAGS)
AC_SUBST(LIBGCJ_CXXFLAGS)
AC_SUBST(LIBGCJ_JAVAFLAGS)
AC_SUBST(LIBGCJ_LD_SYMBOLIC)

if test "x${with_ulibgcj_platform}" = x; then
  AC_MSG_ERROR(
    [micro-libgcj must be configured via the top-level GCC configure script])
fi

AC_CONFIG_LINKS(include/platform.h:../include/${with_ulibgcj_platform_h})
AM_CONDITIONAL(USING_WIN32_PLATFORM, test "${with_ulibgcj_platform}" = Win32)
AM_CONDITIONAL(USING_POSIX_PLATFORM, test "${with_ulibgcj_platform}" = Posix)

case "$host" in
  *-darwin*) DARWIN_CRT=true ;;
  *) DARWIN_CRT=false ;;
esac
AM_CONDITIONAL(USING_DARWIN_CRT, $DARWIN_CRT)

AC_CONFIG_LINKS(include/java-threads.h:../include/${with_ulibgcj_thread_h})
THREADLIBS=${with_ulibgcj_thread_libs}
AC_SUBST(THREADLIBS)
AM_CONDITIONAL(USING_POSIX_THREADS, test "${with_ulibgcj_threads}" = posix)
AM_CONDITIONAL(USING_WIN32_THREADS, test "${with_ulibgcj_threads}" = win32)
AM_CONDITIONAL(USING_NO_THREADS, test "${with_ulibgcj_threads}" = none)

GCLIBS=
GCINCS=
if test "x${with_ulibgcj_gc}" = "xboehm"; then
  AC_DEFINE(HAVE_BOEHM_GC, 1, [Define if Boehm GC in use.])
  GCLIBS=../../boehm-gc/.libs/libgcjgc_convenience.a
  GCINCS='-I$(top_srcdir)/../../boehm-gc/include -I../../boehm-gc/include'
fi

AC_CONFIG_LINKS(include/java-gc.h:../include/${with_ulibgcj_gc_h})
AC_SUBST(GCLIBS)
AC_SUBST(GCINCS)
AM_CONDITIONAL(USING_BOEHMGC, test "${with_ulibgcj_gc}" = boehm)
AM_CONDITIONAL(USING_NOGC, test "${with_ulibgcj_gc}" = no)

AC_CONFIG_LINKS(include/java-signal.h:../${with_ulibgcj_signal_h}
  include/java-signal-aux.h:../${with_ulibgcj_signal_aux_h})

if test "x${with_ulibgcj_hash_synchronization}" = "xyes"; then
  AC_DEFINE(JV_HASH_SYNCHRONIZATION, 1,
            [Define if hash synchronization is in use])
fi

if test "x${with_ulibgcj_which_gcj}" = x; then
  AC_MSG_ERROR([with_ulibgcj_which_gcj not set])
fi

GCJ=gcj
GCJH=gcjh
case "${with_ulibgcj_which_gcj}" in
  built)
   GCJ="$built_gcc_dir/gcj -B`${PWDCMD-pwd}`/ -B$built_gcc_dir/"
   GCJH='../../../$(host_subdir)/gcc/gcjh'
   ;;
 cross)
   GCJ="${target_noncanonical}-gcj"
   GCJH='$(target_noncanonical)-gcjh'
   ;;
esac
AC_SUBST(GCJ)
AC_SUBST(GCJH)
AC_SUBST(GCJFLAGS)

if test -d sysdep; then true; else mkdir sysdep; fi
AC_CONFIG_LINKS(sysdep/locks.h:../sysdep/$sysdeps_dir/locks.h)

AC_DEFINE(JV_ULIBGCJ, 1, [Define if micro-libgcj is to be built])

mkdir -p gcj
echo > gcj/libgcj-config.h

AC_ARG_ENABLE(micro-libgcj-jni,
  AS_HELP_STRING([--enable-micro-libgcj-jni],
                 [provide JNI support in micro-libgcj build]))

if test -z "$enable_micro_libgcj_jni"; then
   enable_micro_libgcj_jni=no
fi

if test "$enable_micro_libgcj_jni" = yes; then
   AC_DEFINE(JV_ULIBGCJ_JNI, 1,
             [Define if micro-libgcj is to be built with JNI enabled])
fi
AM_CONDITIONAL(ULIBGCJ_JNI, test "$enable_micro_libgcj_jni" = yes)

AC_CHECK_HEADERS([unistd.h bstring.h sys/time.h sys/types.h \
		  pwd.h sys/config.h stdint.h langinfo.h locale.h \
		  dirent.h math.h])

AC_CHECK_FUNCS(memmove)

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
