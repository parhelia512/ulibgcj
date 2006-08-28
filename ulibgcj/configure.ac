dnl # Process this with autoconf to create configure
AC_PREREQ(2.59)
AC_INIT(ulibgcj, 0.1)

AC_CANONICAL_SYSTEM

AC_SUBST(target_noncanonical)

AM_INIT_AUTOMAKE(1.9.0)

AC_CONFIG_SRCDIR([../libjava/java/lang/System.java])

AC_CONFIG_HEADERS([include/config.h gcj/libgcj-config.h])

GCC_NO_EXECUTABLES

AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_GCJ
AM_PROG_CC_C_O

AC_CHECK_PROGS(PERL, perl, false)

TL_AC_GXX_INCLUDE_DIR

# most of the following was taken from libjava/configure.ac.

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
GCSPEC=
JC1GCSPEC=
case "$GC" in
 boehm)
    AC_MSG_RESULT(boehm)
    GCSPEC='-lgc'
    JC1GCSPEC='-fuse-boehm-gc'
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
AC_SUBST(GCSPEC)
AC_SUBST(JC1GCSPEC)
AM_CONDITIONAL(USING_BOEHMGC, test "${GC}" = boehm)
AM_CONDITIONAL(USING_NOGC, test "${GC}" = no)

## exceptions #################################################################

AC_MSG_CHECKING([for exception model to use])
AC_LANG_PUSH(C++)
AC_ARG_ENABLE(sjlj-exceptions,
  AS_HELP_STRING([--enable-sjlj-exceptions],
                 [force use of builtin_setjmp for exceptions]),
[:],
[dnl Botheration.  Now we've got to detect the exception model.
dnl Link tests against libgcc.a are problematic since -- at least
dnl as of this writing -- we've not been given proper -L bits for
dnl single-tree newlib and libgloss.
dnl
dnl This is what AC_TRY_COMPILE would do if it didn't delete the
dnl conftest files before we got a change to grep them first.
cat > conftest.$ac_ext << EOF
[#]line __oline__ "configure"
struct S { ~S(); };
void bar();
void foo()
{
  S s;
  bar();
}
EOF
old_CXXFLAGS="$CXXFLAGS"  
CXXFLAGS=-S
if AC_TRY_EVAL(ac_compile); then
  if grep _Unwind_SjLj_Resume conftest.s >/dev/null 2>&1 ; then
    enable_sjlj_exceptions=yes
  elif grep _Unwind_Resume conftest.s >/dev/null 2>&1 ; then
    enable_sjlj_exceptions=no
  fi
fi
CXXFLAGS="$old_CXXFLAGS"
rm -f conftest*])
if test x$enable_sjlj_exceptions = xyes; then
  AC_DEFINE(SJLJ_EXCEPTIONS, 1,
	[Define if the compiler is configured for setjmp/longjmp exceptions.])
  ac_exception_model_name=sjlj
elif test x$enable_sjlj_exceptions = xno; then
  ac_exception_model_name="call frame"
else
  AC_MSG_ERROR([unable to detect exception model])
fi
AC_LANG_POP(C++)
AC_MSG_RESULT($ac_exception_model_name)

# If we are non using SJLJ exceptions, and this host does not have support 
# for unwinding from a signal handler, enable checked dereferences and divides.
if test $can_unwind_signal = no && test $enable_sjlj_exceptions = no; then
  CHECKREFSPEC=-fcheck-references
  DIVIDESPEC=-fuse-divide-subroutine
  EXCEPTIONSPEC=
fi
AC_SUBST(DIVIDESPEC)
AC_SUBST(CHECKREFSPEC)
AC_SUBST(EXCEPTIONSPEC)
AC_SUBST(BACKTRACESPEC)

AC_CHECK_HEADERS(execinfo.h unistd.h dlfcn.h)
AC_CHECK_LIB(dl, dladdr, [
  AC_DEFINE(HAVE_DLADDR, 1, [Define if you have dladdr()])], [
  AC_CHECK_LIB(dld, dladdr, [
  AC_DEFINE(HAVE_DLADDR, 1, [Define if you have dladdr()])])])

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
THREADSPEC=
case "$THREADS" in
 posix)
    THREADLIBS=-lpthread
    THREADSPEC=-lpthread
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
AC_SUBST(THREADSPEC)
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

AC_ARG_ENABLE(hash-synchronization,
  AS_HELP_STRING([--enable-hash-synchronization],
                 [use global hash table for monitor locks]))

if test -z "$enable_hash_synchronization"; then
   enable_hash_synchronization=$enable_hash_synchronization_default
fi

HASH_SYNC_SPEC=
# Hash synchronization is only useful with threads right now.
if test "$enable_hash_synchronization" = yes && test "$THREADS" != "none"; then
   HASH_SYNC_SPEC=-fhash-synchronization
   AC_DEFINE(JV_HASH_SYNCHRONIZATION, 1, [Define if hash synchronization is in use])
fi
AC_SUBST(HASH_SYNC_SPEC)

built_gcc_dir="`cd ../../${host_subdir}/gcc && ${PWDCMD-pwd}`"
GCJ="${built_gcc_dir}/gcj -B${built_gcc_dir}/"
GCJH="${built_gcc_dir}/gcjh"
built_fastjar_dir="`cd ../../${host_subdir}/fastjar && ${PWDCMD-pwd}`"
AC_SUBST(GCJ)
AC_SUBST(GCJH)
AC_SUBST(GCJFLAGS)

AC_CHECK_PROGS([JAR], [jar fastjar], no)
AC_PATH_PROG([ZIP], [zip], no)
AC_PATH_PROG([UNZIP], [unzip], unzip)
AM_CONDITIONAL(BASH_JAR, test "$JAR" = no)
if test "$ZIP" = no; then
  if test "$JAR" = no; then
    AC_MSG_ERROR([cannot find neither zip nor jar, cannot continue])
  fi
else
  # InfoZIP available, use the 'guaranteed' Bourne-shell JAR to build libjava
  JAR=`pwd`/scripts/jar
fi

if test -d sysdep; then true; else mkdir sysdep; fi
AC_CONFIG_LINKS(sysdep/locks.h:${libjava}/sysdep/$sysdeps_dir/locks.h)
AC_CONFIG_LINKS(sysdep/backtrace.h:${libjava}/$fallback_backtrace_h)
AC_CONFIG_LINKS(sysdep/descriptor.h:${libjava}/$descriptor_h)

AC_DEFINE(JV_ULIBGCJ, 1, [Define if micro-libgcj is to be built])

AC_DEFINE(HAVE_MEMMOVE, 1, [Define if memmove is available])

test -f libgcj.spec || echo > libgcj.spec

AC_CHECK_HEADERS([unistd.h bstring.h sys/time.h string.h sys/types.h \
		  pwd.h sys/config.h stdint.h langinfo.h locale.h \
		  dirent.h math.h])

AC_CONFIG_FILES([Makefile libgcj.spec])

AC_CONFIG_FILES([scripts/jar:../libjava/scripts/jar.in],
                [chmod +x scripts/jar])

AC_OUTPUT
