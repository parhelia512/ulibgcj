## Process this file with automake to produce Makefile.in.

AUTOMAKE_OPTIONS = foreign subdir-objects

ACLOCAL_AMFLAGS = -I ../.. -I ../../config

SUFFIXES = .class .java .h

srcdir = @srcdir@
VPATH = @srcdir@/..:@srcdir@/../classpath

AM_CPPFLAGS = -I$(srcdir)/.. -I. -Iinclude -I$(srcdir)/../include \
	-I$(srcdir)/../classpath/include -I$(srcdir)/../../gcc \
	$(GCINCS) $(THREADINCS)

AM_CXXFLAGS = \
	-fno-rtti \
	-fnon-call-exceptions \
	$(THREADCXXFLAGS) \
	-fdollars-in-identifiers \
	-Wswitch-enum \
	-D_FILE_OFFSET_BITS=64 \
	@LIBGCJ_CXXFLAGS@ \
	$(WARNINGS) \
	-D_GNU_SOURCE

ulibgcjdir = $(libdir)/ulibgcj
ulibgcj_LIBRARIES = libulibgcj.a

java_sources = \
gnu/gcj/RawData.java \
gnu/gcj/RawDataManaged.java \
gnu/gcj/runtime/StringBuffer.java \
java/lang/AbstractMethodError.java \
java/lang/ArithmeticException.java \
java/lang/ArrayIndexOutOfBoundsException.java \
java/lang/ArrayStoreException.java \
java/lang/Boolean.java \
java/lang/Byte.java \
java/lang/Character.java \
java/lang/Class.java \
java/lang/ClassCastException.java \
java/lang/ClassLoader.java \
java/lang/ClassNotFoundException.java \
java/lang/CloneNotSupportedException.java \
java/lang/Comparable.java \
java/lang/Double.java \
java/lang/Error.java \
java/lang/Exception.java \
java/lang/ExceptionInInitializerError.java \
java/lang/Float.java \
java/lang/IllegalAccessError.java \
java/lang/IllegalArgumentException.java \
java/lang/IllegalMonitorStateException.java \
java/lang/IllegalStateException.java \
java/lang/IllegalThreadStateException.java \
java/lang/IncompatibleClassChangeError.java \
java/lang/IndexOutOfBoundsException.java \
java/lang/InstantiationException.java \
java/lang/Integer.java \
java/lang/InternalError.java \
java/lang/LinkageError.java \
java/lang/Long.java \
java/lang/Math.java \
java/lang/NegativeArraySizeException.java \
java/lang/NoClassDefFoundError.java \
java/lang/NoSuchFieldError.java \
java/lang/NoSuchMethodError.java \
java/lang/NullPointerException.java \
java/lang/Number.java \
java/lang/NumberFormatException.java \
java/lang/Object.java \
java/lang/OutOfMemoryError.java \
java/lang/Runnable.java \
java/lang/RuntimeException.java \
java/lang/Short.java \
java/lang/String.java \
java/lang/StringBuffer.java \
java/lang/StringIndexOutOfBoundsException.java \
java/lang/System.java \
java/lang/Thread.java \
java/lang/Throwable.java \
java/lang/VMClassLoader.java \
java/lang/VMThrowable.java \
java/lang/VerifyError.java \
java/lang/VirtualMachineError.java \
java/lang/reflect/Modifier.java

native_sources = \
gnu/gcj/runtime/natStringBuffer.cc \
java/lang/natCharacter.cc \
java/lang/natClass.cc \
java/lang/natClassLoader.cc \
java/lang/natMath.cc \
java/lang/natObject.cc \
java/lang/natString.cc \
java/lang/natStringBuffer.cc \
java/lang/natSystem.cc \
java/lang/natThread.cc \
java/lang/natVMClassLoader.cc

libulibgcj_a_SOURCES = \
prims.cc \
exception.cc \
defineclass.cc \
link.cc

libulibgcj_a_LIBADD = $(objects) $(native_objects)

if USING_BOEHMGC
libulibgcj_a_SOURCES += boehm.cc
endif

if USING_NOGC
libulibgcj_a_SOURCES += nogc.cc
endif

if USING_POSIX_PLATFORM
libulibgcj_a_SOURCES += posix.cc
endif

if USING_WIN32_PLATFORM
libulibgcj_a_SOURCES += win32.cc
endif

if USING_DARWIN_CRT
libulibgcj_a_SOURCES += darwin.cc
endif

if USING_POSIX_THREADS
libulibgcj_a_SOURCES += posix-threads.cc
endif

if USING_WIN32_THREADS
libulibgcj_a_SOURCES += win32-threads.cc
endif

if USING_NO_THREADS
libulibgcj_a_SOURCES += no-threads.cc
endif

classes = $(java_sources:.java=.class)

processed_java_sources = \
	$(foreach x,$(java_sources),$(patsubst %,processed/%,$(x)))

process_flags = -DULIBGCJ

if ULIBGCJ_JNI
  process_flags += -DULIBGCJ_JNI
endif

$(processed_java_sources): processed/%.java: %.java
	@mkdir -p $(dir $(@))
	@PERL@ $(srcdir)/scripts/process.pl $(process_flags) < $(<) > $(@)

ommitted_headers = java/lang/Object.h java/lang/Class.h

java_headers = $(filter-out $(ommitted_headers),$(java_sources:.java=.h))

special_headers = \
java/lang/ClassLoader.h \
java/lang/Thread.h \
java/lang/String.h

ordinary_headers = $(filter-out $(special_headers),$(java_headers))

GCJH = @GCJH@

.PRECIOUS: %.h
%.h: files = $(shell echo $(subst $$,\$$,$(basename $(<)))*.class)
%.h: classes = $(subst $$,\$$,$(patsubst %.class,%,$(files)))
$(ordinary_headers): %.h: %.class
	@mkdir -p $(dir $(@))
	$(GCJH) -classpath '' -bootclasspath processed $(classes)

$(libulibgcj_a_OBJECTS): $(java_headers)

$(classes): %.class: processed/%.java $(processed_java_sources)
	@mkdir -p $(dir $(@))
	$(GCJ) -d . --classpath '' --bootclasspath processed -C $(<)

java/lang/ClassLoader.h: java/lang/ClassLoader.class
	$(GCJH) -classpath '' -bootclasspath processed \
		-prepend 'jclass _Jv_FindClass (_Jv_Utf8Const *name, java::lang::ClassLoader *loader);' \
		-prepend 'void _Jv_RunMain (jclass klass, const char *name, int argc, const char **argv, bool is_jar);' \
		-friend 'jclass (::_Jv_FindClass) (_Jv_Utf8Const *name, java::lang::ClassLoader *loader);' \
		-friend 'void ::_Jv_RunMain (jclass klass, const char *name, int argc, const char **argv, bool is_jar);' \
		java/lang/ClassLoader

java/lang/Thread.h: java/lang/Thread.class
	$(GCJH) -classpath '' -bootclasspath processed \
		-prepend 'class _Jv_JNIEnv;' \
		-prepend '#define _JV_NOT_OWNER 1' \
		-prepend '#define _JV_INTERRUPTED 2' \
		-prepend '_Jv_JNIEnv * _Jv_GetCurrentJNIEnv ();' \
		-prepend 'void _Jv_SetCurrentJNIEnv (_Jv_JNIEnv *env);' \
		-prepend 'void _Jv_ThreadRun (java::lang::Thread* thread);' \
		-prepend 'jint _Jv_AttachCurrentThread(java::lang::Thread* thread);' \
		-prepend 'java::lang::Thread* _Jv_AttachCurrentThread (jstring name, java::lang::ThreadGroup* group);' \
		-prepend 'java::lang::Thread* _Jv_AttachCurrentThreadAsDaemon (jstring name, java::lang::ThreadGroup* group);' \
		-prepend 'jint _Jv_DetachCurrentThread ();' \
		-friend '_Jv_JNIEnv * ::_Jv_GetCurrentJNIEnv ();' \
		-friend 'void ::_Jv_SetCurrentJNIEnv (_Jv_JNIEnv *env);' \
		-friend 'void ::_Jv_ThreadRun (java::lang::Thread* thread);' \
		-friend 'jint (::_Jv_AttachCurrentThread) (java::lang::Thread* thread);' \
		-friend 'java::lang::Thread* ::_Jv_AttachCurrentThread (jstring name, java::lang::ThreadGroup* group);' \
		-friend 'java::lang::Thread* ::_Jv_AttachCurrentThreadAsDaemon (jstring name, java::lang::ThreadGroup* group);' \
		-friend 'jint (::_Jv_DetachCurrentThread) ();' \
		java/lang/Thread

java/lang/String.h: java/lang/String.class
	$(GCJH) -classpath '' -bootclasspath processed \
	    -prepend 'jchar* _Jv_GetStringChars (jstring str);' \
	    -prepend 'jstring* _Jv_StringFindSlot (jchar*, jint, jint);' \
	    -prepend 'jstring* _Jv_StringGetSlot (jstring);' \
	    -prepend 'jstring _Jv_NewStringUtf8Const (_Jv_Utf8Const* str);' \
	    -prepend 'jstring _Jv_NewStringLatin1 (const char*, jsize);' \
	    -prepend 'jstring _Jv_AllocString (jsize);' \
	    -friend 'jchar* ::_Jv_GetStringChars (jstring str);' \
	    -friend 'jstring* ::_Jv_StringFindSlot (jchar*, jint, jint);' \
	    -friend 'jstring* ::_Jv_StringGetSlot (jstring);' \
	    -friend 'jstring (::_Jv_NewStringUtf8Const) (_Jv_Utf8Const* str);' \
	    -friend 'jstring (::_Jv_NewStringLatin1) (const char*, jsize);' \
	    -friend 'jstring (::_Jv_AllocString) (jsize);' \
	    java/lang/String

AM_GCJFLAGS = @LIBGCJ_JAVAFLAGS@ --classpath '' --bootclasspath processed

objects = $(java_sources:.java=.o)

$(objects): %.o: processed/%.java $(classes)
	@mkdir -p $(dir $(@))
	$(GCJ) -I. $(GCJFLAGS) -o $(@) -c $(<)

native_objects = $(native_sources:.cc=.o)

$(native_objects): %.o: %.cc
	@mkdir -p $(dir $(@))
	$(CXXCOMPILE) -o $(@) -c $(<)

clean:
	-rm -f $$(find -name '*.o' -o -name '*.class')

## test #######################################################################

hello: \
		Hello.o \
		Hello-cpp.o \
		libulibgcj.a \
		$(GCLIBS)
	$(CXX) $(LDFLAGS) -lm -ldl $(THREADLDFLAGS) $(THREADLIBS) $(^) -o $(@)

Hello.o: $(srcdir)/test/Hello.java libulibgcj.a
	$(GCJ) -I. -I$(srcdir)/.. $(GCJFLAGS) -o $(@) -c $(<)

Hello-cpp.o: $(srcdir)/test/Hello.cpp Hello.h
	$(CXXCOMPILE) -o $(@) -c $(<)

Hello.h: Hello.class
	$(GCJH) -classpath '' -bootclasspath processed $(<:.class=)

Hello.class: $(srcdir)/test/Hello.java libulibgcj.a
	$(GCJ) -d . --classpath '' --bootclasspath processed -C $(<)

test: \
		CoreTest.o \
		CoreTest-cpp.o \
		Thrower.o \
		libulibgcj.a \
		$(GCLIBS)
	$(CXX) $(LDFLAGS) -lm -ldl $(THREADLDFLAGS) $(THREADLIBS) $(^) -o $(@)

CoreTest.o: $(srcdir)/test/CoreTest.java libulibgcj.a
	$(GCJ) -I. -I$(srcdir)/.. -I$(srcdir)/test $(GCJFLAGS) -o $(@) -c $(<)

Thrower.o: $(srcdir)/test/Thrower.java libulibgcj.a
	$(GCJ) -I. -I$(srcdir)/.. $(GCJFLAGS) -o $(@) -c $(<)

CoreTest-cpp.o: $(srcdir)/test/CoreTest.cpp CoreTest.h
	$(CXXCOMPILE) -I. -I$(srcdir)/.. -o $(@) -c $(<)

CoreTest.h: CoreTest.class
	$(GCJH) -classpath .:$(srcdir)/test -bootclasspath processed $(<:.class=)

CoreTest.class: $(srcdir)/test/CoreTest.java libulibgcj.a
	$(GCJ) -d . --classpath $(srcdir)/test --bootclasspath processed -C $(<)
