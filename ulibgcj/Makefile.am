## Process this file with automake to produce Makefile.in.

AUTOMAKE_OPTIONS = foreign subdir-objects

ACLOCAL_AMFLAGS = -I .. -I ../config

SUFFIXES = .class .java .h

srcdir = @srcdir@
VPATH = @srcdir@/../libjava:@srcdir@/../libjava/classpath

AM_CPPFLAGS = \
	-I$(srcdir)/../libjava \
	-Iheaders \
	-Iinclude \
	-I$(srcdir)/../libjava/include \
	-I$(srcdir)/../libjava/classpath/include \
	-I$(srcdir)/../gcc \
	$(GCINCS) \
	$(THREADINCS)

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

jardir = $(datadir)/java
jar_DATA = libulibgcj.jar

## FIXME - this conflicts with libjava's libgcj.spec
specdir = $(libdir)
spec_DATA = libgcj.spec

ulibgcjdir = $(libdir)
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

BUILT_SOURCES = $(java_headers)

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

classes = \
	$(foreach x,$(java_sources),$(patsubst %.java,classes/%.class,$(x)))

processed_java_sources = \
	$(foreach x,$(java_sources),$(patsubst %,processed/%,$(x)))

process_flags = -DULIBGCJ

$(processed_java_sources): processed/%.java: %.java
	@mkdir -p $(dir $(@))
	@PERL@ $(srcdir)/scripts/process.pl $(process_flags) < $(<) > $(@)

$(classes): classes/%.class: processed/%.java $(processed_java_sources)
	@mkdir -p $(dir $(@))
	$(GCJ) -d classes --classpath classes --bootclasspath processed -C $(<)

ommitted_headers = headers/java/lang/Object.h headers/java/lang/Class.h

java_headers = $(filter-out $(ommitted_headers),$(foreach \
	x,$(java_sources),$(patsubst %.java,headers/%.h,$(x))))

special_headers = \
headers/java/lang/ClassLoader.h \
headers/java/lang/Thread.h \
headers/java/lang/String.h

ordinary_headers = $(filter-out $(special_headers),$(java_headers))

GCJH = @GCJH@

.PRECIOUS: %.h
%.h: files = $(shell echo $(subst $$,\$$,$(basename $(<)))*.class)
%.h: classes = $(subst $$,\$$,$(patsubst classes/%.class,%,$(files)))
$(ordinary_headers): headers/%.h: classes/%.class
	@mkdir -p $(dir $(@))
	$(GCJH) $(headerflags) $(classes)

$(libulibgcj_a_OBJECTS): $(java_headers)

headerflags = -d headers --classpath classes --bootclasspath processed

headers/java/lang/ClassLoader.h: classes/java/lang/ClassLoader.class
	@mkdir -p $(dir $(@))
	$(GCJH) $(headerflags) \
		-prepend 'jclass _Jv_FindClass (_Jv_Utf8Const *name, java::lang::ClassLoader *loader);' \
		-prepend 'void _Jv_RunMain (jclass klass, const char *name, int argc, const char **argv, bool is_jar);' \
		-friend 'jclass (::_Jv_FindClass) (_Jv_Utf8Const *name, java::lang::ClassLoader *loader);' \
		-friend 'void ::_Jv_RunMain (jclass klass, const char *name, int argc, const char **argv, bool is_jar);' \
		java/lang/ClassLoader

headers/java/lang/Thread.h: classes/java/lang/Thread.class
	@mkdir -p $(dir $(@))
	$(GCJH) $(headerflags) \
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

headers/java/lang/String.h: classes/java/lang/String.class
	@mkdir -p $(dir $(@))
	$(GCJH) $(headerflags) \
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

objects = $(foreach x,$(java_sources),$(patsubst %.java,objects/%.o,$(x)))

$(objects): objects/%.o: processed/%.java $(classes)
	@mkdir -p $(dir $(@))
	$(GCJ) -Iclasses $(GCJFLAGS) -o $(@) -c $(<)

native_objects = \
	$(foreach x,$(native_sources),$(patsubst %.cc,objects/%.o,$(x)))

$(native_objects): objects/%.o: %.cc
	@mkdir -p $(dir $(@))
	$(CXXCOMPILE) -o $(@) -c $(<)

libulibgcj.jar: $(classes)
	cd classes; $(JAR) -cfM ../libulibgcj.jar gnu java

clean:
	rm -rf $(libulibgcj_a_OBJECTS) objects classes headers processed \
		libulibgcj.a libulibgcj.jar hello core-test

## tests and examples #########################################################

AM_LDFLAGS = -lm

## the only reason we need this is that libgc needs libdl on linux for
## some obscure reason related to pthread_create:
if USING_POSIX_PLATFORM
if USING_BOEHMGC
AM_LDFLAGS += -ldl
endif
endif

linkflags = $(AM_LDFLAGS) $(LDFLAGS) $(THREADLDFLAGS) $(THREADLIBS)

javaflags = $(CPPFLAGS) -I$(srcdir)/test $(GCJFLAGS) --bootclasspath processed

hflags = -d headers -classpath classes:$(srcdir)/test -bootclasspath processed

classflags = -d classes --classpath $(srcdir)/test --bootclasspath processed

objects/%.o: $(srcdir)/test/%.java libulibgcj.a
	$(GCJ) $(javaflags) -o $(@) -c $(<)

objects/%-cpp.o: $(srcdir)/test/%.cpp headers/%.h
	$(CXXCOMPILE) -o $(@) -c $(<)

headers/%.h: classes/%.class
	$(GCJH) $(hflags) $(patsubst classes/%.class,%,$(<))

classes/%.class: $(srcdir)/test/%.java libulibgcj.a
	$(GCJ) $(classflags) -C $(<)

hello: \
		objects/Hello.o \
		objects/Hello-cpp.o \
		libulibgcj.a \
		$(GCLIBS)
	$(CXX) $(linkflags) $(^) -o $(@)

core-test: \
		objects/CoreTest.o \
		objects/CoreTest-cpp.o \
		objects/Thrower.o \
		libulibgcj.a \
		$(GCLIBS)
	$(CXX) $(linkflags) $(^) -o $(@)
