## Process this file with automake to produce Makefile.in.

AUTOMAKE_OPTIONS = foreign subdir-objects

ACLOCAL_AMFLAGS = -I ../.. -I ../../config

SUFFIXES = .class .java .h

srcdir = @srcdir@
VPATH = @srcdir@/..:@srcdir@/../classpath

AM_CPPFLAGS = -I$(srcdir)/.. -Iinclude -I$(srcdir)/../include \
	-I$(srcdir)/../classpath/include $(GCINCS) $(THREADINCS)

ulibgcjdir = $(libdir)/ulibgcj
ulibgcj_LIBRARIES = libulibgcj.a

java_sources = \
gnu/gcj/RawData.java \
gnu/gcj/RawDataManaged.java \
gnu/gcj/runtime/NameFinder.java \
gnu/gcj/runtime/StringBuffer.java \
java/lang/AbstractMethodError.java \
java/lang/ArithmeticException.java \
java/lang/ArrayIndexOutOfBoundsException.java \
java/lang/ArrayStoreException.java \
java/lang/Boolean.java \
java/lang/Byte.java \
java/lang/Character.java \
java/lang/ClassCastException.java \
java/lang/Class.java \
java/lang/ClassLoader.java \
java/lang/ClassNotFoundException.java \
java/lang/CloneNotSupportedException.java \
java/lang/Comparable.java \
java/lang/Double.java \
java/lang/Error.java \
java/lang/ExceptionInInitializerError.java \
java/lang/Exception.java \
java/lang/Float.java \
java/lang/IllegalAccessError.java \
java/lang/IllegalArgumentException.java \
java/lang/IllegalMonitorStateException.java \
java/lang/IllegalStateException.java \
java/lang/IllegalThreadStateException.java \
java/lang/IncompatibleClassChangeError.java \
java/lang/IndexOutOfBoundsException.java \
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
java/lang/NumberFormatException.java \
java/lang/Number.java \
java/lang/Object.java \
java/lang/OutOfMemoryError.java \
java/lang/reflect/Modifier.java \
java/lang/Runnable.java \
java/lang/RuntimeException.java \
java/lang/Short.java \
java/lang/StackTraceElement.java \
java/lang/StringBuffer.java \
java/lang/StringIndexOutOfBoundsException.java \
java/lang/String.java \
java/lang/System.java \
java/lang/Thread.java \
java/lang/Throwable.java \
java/lang/VerifyError.java \
java/lang/VirtualMachineError.java \
java/lang/VMClassLoader.java \
java/lang/VMThrowable.java

libulibgcj_a_SOURCES = \
prims.cc \
jni.cc \
exception.cc \
stacktrace.cc \
link.cc \
defineclass.cc \
java/lang/natCharacter.cc \
java/lang/natClass.cc \
java/lang/natClassLoader.cc \
java/lang/natConcreteProcess.cc \
java/lang/natDouble.cc \
java/lang/natFloat.cc \
java/lang/natMath.cc \
java/lang/natObject.cc \
java/lang/natRuntime.cc	\
java/lang/natString.cc \
java/lang/natStringBuffer.cc \
java/lang/natStringBuilder.cc \
java/lang/natSystem.cc \
java/lang/natThread.cc \
java/lang/natVMClassLoader.cc \
java/lang/natVMSecurityManager.cc \
java/lang/natVMThrowable.cc \
java/lang/ref/natReference.cc \
java/lang/reflect/natArray.cc \
java/lang/reflect/natConstructor.cc \
java/lang/reflect/natField.cc \
java/lang/reflect/natMethod.cc \
$(java_sources)

omitted_headers = \
java/lang/ClassLoader.h \
java/lang/Thread.h \
java/lang/String.h \
java/lang/reflect/Constructor.h \
java/lang/reflect/Field.h \
java/lang/reflect/Method.h \
java/lang/reflect/Proxy.h \
gnu/gcj/runtimse/ExtensionClassLoader.h

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

java_headers = $(filter-out $(omitted_headers),$(java_sources:.java=.h))

$(java_headers): %.h: %.class
	@mkdir -p $(dir $(@))
	$(GCJH) -d . -classpath '' -bootclasspath processed $(@:.class=)

$(libulibgcj_a_OBJECTS): $(java_headers)

$(classes): %.class: processed/%.java $(processed_java_sources)
	@mkdir -p $(dir $(@))
	$(GCJ) -d . --classpath '' --bootclasspath processed -C $(<)
