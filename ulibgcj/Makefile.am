## Process this file with automake to produce Makefile.in.

ulibgcj_version = 0.2

AUTOMAKE_OPTIONS = foreign subdir-objects

ACLOCAL_AMFLAGS = -I .. -I ../config

SUFFIXES = .class .java .h

srcdir = @srcdir@
VPATH = @srcdir@/src:@srcdir@/../libjava:@srcdir@/../libjava/classpath

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
	-DJV_ULIBGCJ_VERSION=\"$(ulibgcj_version)\" \
	@LIBGCJ_CXXFLAGS@ \
	$(WARNINGS) \
	-D_GNU_SOURCE

gcc_version := $(shell cat $(top_srcdir)/../gcc/BASE-VER)

jardir = $(datadir)/java
jar_DATA = libgcj-$(gcc_version).jar

toolexeclib_LIBRARIES = libulibgcj.a libgc.a
toolexecmainlib_DATA = libgcj.spec

java_sources = \
gnu/gcj/Core.java \
gnu/gcj/RawData.java \
gnu/gcj/RawDataManaged.java \
gnu/gcj/runtime/FinalizerThread.java \
gnu/gcj/runtime/StringBuffer.java \
gnu/java/net/URLParseError.java \
gnu/java/net/protocol/core/CoreInputStream.java \
gnu/java/util/WeakIdentityHashMap.java \
java/io/BufferedInputStream.java \
java/io/BufferedReader.java \
java/io/ByteArrayInputStream.java \
java/io/ByteArrayOutputStream.java \
java/io/EOFException.java \
java/io/File.java \
java/io/FileInputStream.java \
java/io/FileNotFoundException.java \
java/io/FileOutputStream.java \
java/io/FilterInputStream.java \
java/io/FilterOutputStream.java \
java/io/IOException.java \
java/io/InputStream.java \
java/io/InputStreamReader.java \
java/io/OutputStream.java \
java/io/OutputStreamWriter.java \
java/io/PrintStream.java \
java/io/PrintWriter.java \
java/io/Reader.java \
java/io/Serializable.java \
java/io/Writer.java \
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
java/lang/Cloneable.java \
java/lang/Comparable.java \
java/lang/Double.java \
java/lang/Enum.java \
java/lang/Error.java \
java/lang/Exception.java \
java/lang/ExceptionInInitializerError.java \
java/lang/Float.java \
java/lang/IllegalAccessError.java \
java/lang/IllegalAccessException.java \
java/lang/IllegalArgumentException.java \
java/lang/IllegalMonitorStateException.java \
java/lang/IllegalStateException.java \
java/lang/IllegalThreadStateException.java \
java/lang/IncompatibleClassChangeError.java \
java/lang/IndexOutOfBoundsException.java \
java/lang/InstantiationException.java \
java/lang/InheritableThreadLocal.java \
java/lang/Integer.java \
java/lang/InternalError.java \
java/lang/InterruptedException.java \
java/lang/Iterable.java \
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
java/lang/Process.java \
java/lang/Runnable.java \
java/lang/Runtime.java \
java/lang/RuntimeException.java \
java/lang/SecurityException.java \
java/lang/Short.java \
java/lang/String.java \
java/lang/StringBuffer.java \
java/lang/StringBuilder.java \
java/lang/StringIndexOutOfBoundsException.java \
java/lang/System.java \
java/lang/Thread.java \
java/lang/ThreadLocal.java \
java/lang/Throwable.java \
java/lang/TypeNotPresentException.java \
java/lang/UnsatisfiedLinkError.java \
java/lang/UnsupportedOperationException.java \
java/lang/VMClassLoader.java \
java/lang/VMThrowable.java \
java/lang/VerifyError.java \
java/lang/VirtualMachineError.java \
java/lang/annotation/Annotation.java \
java/lang/ref/Reference.java \
java/lang/ref/ReferenceQueue.java \
java/lang/ref/WeakReference.java \
java/lang/reflect/Modifier.java \
java/nio/Buffer.java \
java/nio/BufferOverflowException.java \
java/nio/BufferUnderflowException.java \
java/nio/ByteBuffer.java \
java/nio/ByteBufferHelper.java \
java/nio/ByteBufferImpl.java \
java/nio/ByteOrder.java \
java/nio/InvalidMarkException.java \
java/nio/ReadOnlyBufferException.java \
java/util/AbstractCollection.java \
java/util/AbstractList.java \
java/util/AbstractMap.java \
java/util/AbstractSet.java \
java/util/AbstractSequentialList.java \
java/util/ArrayList.java \
java/util/Arrays.java \
java/util/Calendar.java \
java/util/Collection.java \
java/util/Collections.java \
java/util/Comparator.java \
java/util/ConcurrentModificationException.java \
java/util/Date.java \
java/util/Enumeration.java \
java/util/EventListener.java \
java/util/EventObject.java \
java/util/GregorianCalendar.java \
java/util/HashMap.java \
java/util/HashSet.java \
java/util/Hashtable.java \
java/util/Iterator.java \
java/util/LinkedList.java \
java/util/List.java \
java/util/ListIterator.java \
java/util/Locale.java \
java/util/Map.java \
java/util/MissingResourceException.java \
java/util/NoSuchElementException.java \
java/util/Properties.java \
java/util/PropertyResourceBundle.java \
java/util/ResourceBundle.java \
java/util/Set.java \
java/util/SimpleTimeZone.java \
java/util/SortedMap.java \
java/util/SortedSet.java \
java/util/TimeZone.java \
java/util/TreeMap.java \
java/util/TreeSet.java \
java/util/Vector.java \
java/util/logging/Handler.java \
java/util/logging/Level.java \
java/util/logging/Logger.java \
java/util/logging/LogManager.java \
java/util/logging/LogRecord.java \
java/util/zip/DataFormatException.java \
java/util/zip/Inflater.java

native_sources = \
gnu/gcj/natCore.cc \
gnu/gcj/runtime/natStringBuffer.cc \
gnu/gcj/runtime/natFinalizerThread.cc \
gnu/java/net/protocol/core/natCoreInputStream.cc \
java/lang/natCharacter.cc \
java/lang/natClass.cc \
java/lang/natClassLoader.cc \
java/lang/natMath.cc \
java/lang/natObject.cc \
java/lang/natString.cc \
java/lang/natStringBuffer.cc \
java/lang/natStringBuilder.cc \
java/lang/natSystem.cc \
java/lang/natThread.cc \
java/lang/natVMClassLoader.cc \
java/lang/ref/natReference.cc \
java/util/zip/natInflater.cc \
java_io.cc

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
AM_CXXFLAGS += -DJV_ULIBGCJ_WIN32
libulibgcj_a_SOURCES += win32.cc
endif

if USING_DARWIN_CRT
AM_CXXFLAGS += -DJV_ULIBGCJ_DARWIN
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

$(classes): $(processed_java_sources)
	@mkdir -p $(dir $(@))
	$(GCJ) -d classes --classpath classes --bootclasspath processed -C -w $(^)

java_headers = $(foreach	x,$(java_sources),$(patsubst %.java,headers/%.h,$(x)))

special_headers = \
headers/java/lang/Object.h \
headers/java/lang/Class.h \
headers/java/lang/ClassLoader.h \
headers/java/lang/Thread.h \
headers/java/lang/String.h

ordinary_headers = $(filter-out $(special_headers),$(java_headers))

gcj_headers = array.h cni.h field.h javaprims.h method.h

install-data-local: $(java_headers)
	$(PRE_INSTALL)
	@set -e; for header in $(foreach x,$(java_headers),$(patsubst headers/%,%,$(x))); do \
	   directory="$$(echo $${header} | sed -e 's,/[^/]*$$,,')"; \
		 echo "$(mkinstalldirs) $(DESTDIR)$(gxx_include_dir)/$${directory}"; \
	   $(mkinstalldirs) $(DESTDIR)$(gxx_include_dir)/$${directory}; \
		 echo "$(INSTALL_DATA) headers/$${header} $(DESTDIR)$(gxx_include_dir)/$${header}"; \
		 $(INSTALL_DATA) headers/$${header} $(DESTDIR)$(gxx_include_dir)/$${header}; \
	 done
	$(mkinstalldirs) $(DESTDIR)$(gxx_include_dir)/gcj
	@set -e; for header in $(gcj_headers); do \
		 echo "$(INSTALL_DATA) @srcdir@/../libjava/gcj/$${header} $(DESTDIR)$(gxx_include_dir)/gcj/$${header}"; \
		 $(INSTALL_DATA) @srcdir@/../libjava/gcj/$${header} $(DESTDIR)$(gxx_include_dir)/gcj/$${header}; \
	 done
	$(INSTALL_DATA) gcj/libgcj-config.h $(DESTDIR)$(gxx_include_dir)/gcj/libgcj-config.h

GCJH = @GCJH@

.PRECIOUS: %.h
%.h: files = $(shell echo $(subst $$,\$$,$(basename $(<)))*.class)
%.h: classes = $(subst $$,\$$,$(patsubst classes/%.class,%,$(files)))
$(ordinary_headers): headers/%.h: classes/%.class
	@mkdir -p $(dir $(@))
	$(GCJH) $(headerflags) $(classes)

$(libulibgcj_a_OBJECTS): $(java_headers)

headerflags = -d headers --classpath classes --bootclasspath processed

headers/java/lang/Object.h headers/java/lang/Class.h: headers/%.h: @srcdir@/../libjava/%.h
	@mkdir -p $(dir $(@))
	cat $(<) >>$(@)

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

$(objects): objects/%.o: classes/%.class $(classes)
	@mkdir -p $(dir $(@))
	$(GCJ) -Iclasses $(GCJFLAGS) -o $(@) -c $(<) \
		$$(find classes -path '$(basename $(<))$$*.class')

native_objects = \
	$(foreach x,$(native_sources),$(patsubst %.cc,objects/%.o,$(x)))

$(native_objects): objects/%.o: %.cc
	@mkdir -p $(dir $(@))
	$(CXXCOMPILE) -o $(@) -c $(<)

libgcj-$(gcc_version).jar: $(classes)
	cd classes; $(JAR) -cfM ../libgcj-$(gcc_version).jar gnu java

clean:
	rm -rf $(libulibgcj_a_OBJECTS) objects classes headers processed \
		libulibgcj.a libgcj-$(gcc_version).jar hello core-test javadoc

.PHONY: javadoc
javadoc: $(processed_java_sources)
	javadoc -sourcepath processed java.lang java.util java.io \
		java.lang.reflect	-d javadoc

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
	$(GCJ) $(classflags) -C -w $(<)

libgc.a: $(GCLIBS)
	cp $(<) $(@)

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

test15: \
		objects/Test15.o \
		libulibgcj.a \
		$(GCLIBS)
	$(GCJ) -L. $(linkflags) --main=Test15 $(^) -o $(@)
