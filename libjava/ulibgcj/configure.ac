dnl # Process this with autoconf to create configure
AC_PREREQ(2.59)
AC_INIT(ulibgcj, 0.1)

AM_INIT_AUTOMAKE(1.9.0)

AC_CONFIG_SRCDIR([../java/lang/System.java])

AC_CONFIG_HEADERS([include/config.h])

GCC_NO_EXECUTABLES

AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_GCJ
AM_PROG_CC_C_O

AC_CHECK_PROGS(PERL, perl, false)

if test "x${with_ulibgcj_platform}" = x; then
  AC_MSG_ERROR(
    [micro-libgcj must be configured via the top-level GCC configure script])
fi

AC_CONFIG_LINKS(include/platform.h:../include/${with_ulibgcj_platform_h})
AM_CONDITIONAL(USING_WIN32_PLATFORM, test "${with_ulibgcj_platform}" = Win32)
AM_CONDITIONAL(USING_POSIX_PLATFORM, test "${with_ulibgcj_platform}" = Posix)

AC_CONFIG_LINKS(include/java-threads.h:../include/${with_ulibgcj_thread_h})
THREADLIBS=${with_ulibgcj_thread_libs}
AC_SUBST(THREADLIBS)

if test "x${with_ulibgcj_gc}" = "xboehm"; then
  AC_DEFINE(HAVE_BOEHM_GC, 1, [Define if Boehm GC in use.])
fi

AC_CONFIG_LINKS(include/java-gc.h:../include/${with_ulibgcj_gc_h})
GCLIBS=${with_ulibgcj_gc_libs}
AC_SUBST(GCLIBS)
GCINCS=${with_ulibgcj_gc_incs}
AC_SUBST(GCINCS)

AC_CONFIG_LINKS(include/java-signal.h:../${with_ulibgcj_signal_h}
  include/java-signal-aux.h:../${with_ulibgcj_signal_aux_h})

if test "x${with_ulibgcj_hash_synchronization}" = "xyes"; then
  AC_DEFINE(JV_HASH_SYNCHRONIZATION, 1,
            [Define if hash synchronization is in use])
fi

AC_DEFINE(JV_ULIBGCJ, 1, [Define if micro-libgcj is to be built])

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

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
