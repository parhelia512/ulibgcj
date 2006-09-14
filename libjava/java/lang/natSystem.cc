// natSystem.cc - Native code implementing System class.

/* Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2006 Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

#include <config.h>
#include <platform.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <gcj/cni.h>
#include <jvm.h>
#include <java/lang/System.h>
#ifdef JV_ULIBGCJ
#include <java/lang/System$Output.h>
#include <java/lang/System$Input.h>
#include <java/io/InputStream.h>
#include <java/io/OutputStream.h>
#include <java/io/IOException.h>
#include <java/io/EOFException.h>
#endif//JV_ULIBGCJ
#include <java/lang/Class.h>
#include <java/lang/ArrayStoreException.h>
#include <java/lang/ArrayIndexOutOfBoundsException.h>
#include <java/lang/NullPointerException.h>
#ifndef JV_ULIBGCJ
#include <java/io/PrintStream.h>
#include <java/io/InputStream.h>
#endif//JV_ULIBGCJ



#ifndef JV_ULIBGCJ
void
java::lang::System::setErr0 (java::io::PrintStream *newErr)
{
  err = newErr;
}

void
java::lang::System::setIn0 (java::io::InputStream *newIn)
{
  in = newIn;
}

void
java::lang::System::setOut0 (java::io::PrintStream *newOut)
{
  out = newOut;
}
#endif//JV_ULIBGCJ

void
java::lang::System::arraycopy (jobject src, jint src_offset,
			       jobject dst, jint dst_offset,
			       jint count)
{
  if (! src || ! dst)
    throw new NullPointerException;

  jclass src_c = src->getClass();
  jclass dst_c = dst->getClass();
  jclass src_comp = src_c->getComponentType();
  jclass dst_comp = dst_c->getComponentType();

  if (! src_c->isArray() || ! dst_c->isArray()
      || src_comp->isPrimitive() != dst_comp->isPrimitive()
      || (src_comp->isPrimitive() && src_comp != dst_comp))
    throw new ArrayStoreException;

  __JArray *src_a = (__JArray *) src;
  __JArray *dst_a = (__JArray *) dst;
  if (src_offset < 0 || dst_offset < 0 || count < 0
      || (unsigned jint) src_offset > (unsigned jint) src_a->length
      || (unsigned jint) (src_offset + count) > (unsigned jint) src_a->length
      || (unsigned jint) dst_offset > (unsigned jint) dst_a->length
      || (unsigned jint) (dst_offset + count) > (unsigned jint) dst_a->length)
    throw new ArrayIndexOutOfBoundsException;

  // Do-nothing cases.
  if ((src == dst && src_offset == dst_offset)
      || ! count)
    return;

  // If both are primitive, we can optimize trivially.  If DST
  // components are always assignable from SRC components, then we
  // will never need to raise an error, and thus can do the
  // optimization.  If source and destinations are the same, then we
  // know that the assignability premise always holds.
  const bool prim = src_comp->isPrimitive();
  if (prim || dst_comp->isAssignableFrom(src_comp) || src == dst)
    {
      const size_t size = (prim ? src_comp->size()
			   : sizeof elements((jobjectArray)src)[0]);

      char *src_elts = _Jv_GetArrayElementFromElementType (src, src_comp);
      src_elts += size * src_offset;

      char *dst_elts = _Jv_GetArrayElementFromElementType (dst, dst_comp);
      dst_elts += size * dst_offset;

#if HAVE_MEMMOVE
      // We don't bother trying memcpy.  It can't be worth the cost of
      // the check.
      // Don't cast to (void*), as memmove may expect (char*)
      memmove (dst_elts, src_elts, count * size);
#else
      bcopy (src_elts, dst_elts, count * size);
#endif
    }
  else
    {
      jobject *src_elts = elements ((jobjectArray) src_a) + src_offset;
      jobject *dst_elts = elements ((jobjectArray) dst_a) + dst_offset;

      for (int i = 0; i < count; ++i)
	{
	  if (*src_elts
	      && ! dst_comp->isAssignableFrom((*src_elts)->getClass()))
	    throw new ArrayStoreException;
	  *dst_elts++ = *src_elts++;
	}
    }
}

jlong
java::lang::System::currentTimeMillis (void)
{
  return _Jv_platform_gettimeofday ();
}

jlong
java::lang::System::nanoTime ()
{
  return _Jv_platform_nanotime ();
}

jint
java::lang::System::identityHashCode (jobject obj)
{
  return _Jv_HashCode (obj);
}

#ifndef JV_ULIBGCJ
jstring
java::lang::System::getenv0 (jstring name)
{
  jint len = _Jv_GetStringUTFLength (name);
  char buf[len + 1];
  jsize total = JvGetStringUTFRegion (name, 0, name->length(), buf);
  buf[total] = '\0';
  const char *value = ::getenv (buf);
  if (value == NULL)
    return NULL;
  return JvNewStringUTF (value);
}
#endif//JV_ULIBGCJ

#ifdef JV_ULIBGCJ
inline void
doWrite(int fd, const void* p, int c)
{
  int r = JV_ULIBGCJ_WRITE(fd, p, c);
  if (r == -1) {
    throw new java::io::IOException;    
  }
}

inline void
doClose(int fd)
{
  int r = JV_ULIBGCJ_CLOSE(fd);
  if (r == -1) {
    throw new java::io::IOException;    
  }
}

inline int
doRead(int fd, void* p, int c)
{
  return JV_ULIBGCJ_READ(fd, p, c);
}

void
java::lang::System$Output::write (jbyteArray b, jint offset, jint length)
{
  doWrite(fd, elements(b) + offset, length);
}

void
java::lang::System$Output::write (jint v)
{
  char c = v;
  doWrite(fd, &c, 1);
}

void
java::lang::System$Output::flush ()
{
  // do nothing
}

void
java::lang::System$Output::close ()
{
  if (not closed) {
    doClose(fd);
    closed = true;
  }
}

java::io::OutputStream*
java::lang::System$Output::standardOut ()
{
  return new java::lang::System$Output(1);
}

java::io::OutputStream*
java::lang::System$Output::standardErr ()
{
  return new java::lang::System$Output(2);
}

jint
java::lang::System$Input::read (jbyteArray b, jint offset, jint length)
{
  int r = doRead(fd, elements(b) + offset, length);
  if (r == -1) {
    throw new java::io::IOException;    
  } else if (r == 0) {
    r = -1;
  }
  return r;
}

jint
java::lang::System$Input::read ()
{
  char c;
  int r = doRead(fd, &c, 1);
  if (r == 0) {
    return -1;
  } else if (r == -1) {
    throw new java::io::IOException;
  }
  return r;
}

void
java::lang::System$Input::close ()
{
  if (not closed) {
    doClose(fd);
    closed = true;
  }
}

java::io::InputStream*
java::lang::System$Input::standardIn ()
{
  return new java::lang::System$Input(0);
}

void
java::lang::System::exit (jint status)
{
  ::exit(status);
}
#endif//JV_ULIBGCJ
