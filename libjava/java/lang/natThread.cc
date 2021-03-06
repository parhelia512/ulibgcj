// natThread.cc - Native part of Thread class.

/* Copyright (C) 1998, 1999, 2000, 2001, 2002, 2005, 2006  Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

#include <config.h>

#include <stdlib.h>

#include <gcj/cni.h>
#include <jvm.h>
#include <java-threads.h>

#include <gnu/gcj/RawDataManaged.h>
#include <java/lang/Thread.h>
#ifndef JV_ULIBGCJ
#include <java/lang/Thread$State.h>
#include <java/lang/Thread$UncaughtExceptionHandler.h>
#include <java/lang/ThreadGroup.h>
#endif// not JV_ULIBGCJ
#include <java/lang/IllegalArgumentException.h>
#include <java/lang/IllegalThreadStateException.h>
#ifndef JV_ULIBGCJ
#include <java/lang/InterruptedException.h>
#endif// not JV_ULIBGCJ
#include <java/lang/NullPointerException.h>

#ifndef JV_ULIBGCJ
#include <jni.h>

#ifdef ENABLE_JVMPI
#include <jvmpi.h>
#endif
#endif// not JV_ULIBGCJ



static void finalize_native (jobject ptr);

// This is called from the constructor to initialize the native side
// of the Thread.
void
java::lang::Thread::initialize_native (void)
{
  natThread *nt = (natThread *) _Jv_AllocBytes (sizeof (natThread));
  
  state = JV_NEW;

  data = (gnu::gcj::RawDataManaged *) nt;
  
  // Register a finalizer to clean up the native thread resources.
  _Jv_RegisterFinalizer (data, finalize_native);

  _Jv_MutexInit (&nt->join_mutex);
  _Jv_CondInit (&nt->join_cond);

#ifndef JV_ULIBGCJ
  pthread_mutex_init (&nt->park_mutex, NULL);
  pthread_cond_init (&nt->park_cond, NULL);
#endif// not JV_ULIBGCJ

  nt->thread = _Jv_ThreadInitData (this);
#ifndef JV_ULIBGCJ
  // FIXME: if JNI_ENV is set we will want to free it.  It is
  // malloc()d.
  nt->jni_env = NULL;
#endif// not JV_ULIBGCJ
}

static void
finalize_native (jobject ptr)
{
  natThread *nt = (natThread *) ptr;
  _Jv_ThreadDestroyData (nt->thread);
#ifdef _Jv_HaveCondDestroy
  _Jv_CondDestroy (&nt->join_cond);
#endif
#ifdef _Jv_HaveMutexDestroy
  _Jv_MutexDestroy (&nt->join_mutex);
#endif
#ifndef JV_ULIBGCJ
  _Jv_FreeJNIEnv((JNIEnv*)nt->jni_env);

  pthread_mutex_destroy (&nt->park_mutex);
  pthread_cond_destroy (&nt->park_cond);
#endif// not JV_ULIBGCJ
}

#ifndef JV_ULIBGCJ
jint
java::lang::Thread::countStackFrames (void)
{
  // NOTE: This is deprecated in JDK 1.2.

  // Old applets still call this method.  Rather than throwing
  // UnsupportedOperationException we simply fail silently.

  return 0;
}
#endif// not JV_ULIBGCJ

java::lang::Thread *
java::lang::Thread::currentThread (void)
{
  return _Jv_ThreadCurrent ();
}

#ifndef JV_ULIBGCJ
jboolean
java::lang::Thread::holdsLock (jobject obj)
{
  if (!obj)
    throw new NullPointerException;
  return !_Jv_ObjectCheckMonitor (obj);
}

void
java::lang::Thread::interrupt (void)
{
  checkAccess ();

  // If a thread is in state ALIVE, we atomically set it to state
  // SIGNALED and send it a signal.  Once we've sent it the signal, we
  // set its state back to ALIVE.
  if (__sync_bool_compare_and_swap 
      (&alive_flag, Thread::THREAD_ALIVE, Thread::THREAD_SIGNALED))
    {
      natThread *nt = (natThread *) data;

      _Jv_ThreadInterrupt (nt->thread);
      __sync_bool_compare_and_swap 
	(&alive_flag, THREAD_SIGNALED, Thread::THREAD_ALIVE);

      // Even though we've interrupted this thread, it might still be
      // parked.
      if (__sync_bool_compare_and_swap 
	  (&parkPermit, Thread::THREAD_PARK_PARKED, Thread::THREAD_PARK_RUNNING))
	{
	  pthread_mutex_lock (&nt->park_mutex);
	  pthread_cond_signal (&nt->park_cond);
	  pthread_mutex_unlock (&nt->park_mutex);
	}
    }
}

void
java::lang::Thread::join (jlong millis, jint nanos)
{
  if (millis < 0 || nanos < 0 || nanos > 999999)
    throw new IllegalArgumentException;

  Thread *current = currentThread ();

  // Here `NT' is the native structure for the thread we are trying to join.
  natThread *nt = (natThread *) data;

  // Now wait for: (1) an interrupt, (2) the thread to exit, or (3)
  // the timeout to occur. 
  _Jv_MutexLock (&nt->join_mutex);
  if (! isAlive ())
    {
      _Jv_MutexUnlock (&nt->join_mutex);
      return;
    }
  _Jv_CondWait (&nt->join_cond, &nt->join_mutex, millis, nanos);
  _Jv_MutexUnlock (&nt->join_mutex);

  if (current->isInterrupted (true))
    throw new InterruptedException;
}

void
java::lang::Thread::resume (void)
{
  checkAccess ();

  // Old applets still call this method.  Rather than throwing
  // UnsupportedOperationException we simply fail silently.
}
#endif// not JV_ULIBGCJ

void
java::lang::Thread::setPriority (jint newPriority)
{
#ifndef JV_ULIBGCJ
  checkAccess ();
#endif// not JV_ULIBGCJ
  if (newPriority < MIN_PRIORITY || newPriority > MAX_PRIORITY)
    throw new IllegalArgumentException;

#ifndef JV_ULIBGCJ
  jint gmax = group->getMaxPriority();
  if (newPriority > gmax)
    newPriority = gmax;
#endif// not JV_ULIBGCJ

  priority = newPriority;
  natThread *nt = (natThread *) data;
  _Jv_ThreadSetPriority (nt->thread, priority);
}

void
java::lang::Thread::sleep (jlong millis, jint nanos)
{
  if (millis < 0 || nanos < 0 || nanos > 999999)
    throw new IllegalArgumentException;

  if (millis == 0 && nanos == 0)
    ++nanos;

  Thread *current = currentThread ();

  // We use a condition variable to implement sleeping so that an
  // interrupt can wake us up.

  // TODO: no need to do this when JV_ULIBGCJ is defined, since thread
  // interruption is not supported.
  natThread *nt = (natThread *) current->data;
  _Jv_MutexLock (&nt->join_mutex);
  _Jv_CondWait (&nt->join_cond, &nt->join_mutex, millis, nanos);
  _Jv_MutexUnlock (&nt->join_mutex);

#ifndef JV_ULIBGCJ
  if (current->isInterrupted (true))
    throw new InterruptedException;
#endif// not JV_ULIBGCJ
}

void
java::lang::Thread::finish_ ()
{
  parkPermit = THREAD_PARK_DEAD;
  __sync_synchronize();
  natThread *nt = (natThread *) data;
  
#ifndef JV_ULIBGCJ
  group->removeThread (this);

#ifdef ENABLE_JVMPI  
  if (_Jv_JVMPI_Notify_THREAD_END)
    {
      JVMPI_Event event;

      event.event_type = JVMPI_EVENT_THREAD_END;
      event.env_id = _Jv_GetCurrentJNIEnv ();

      _Jv_DisableGC ();
      (*_Jv_JVMPI_Notify_THREAD_END) (&event);
      _Jv_EnableGC ();
    }
#endif
#endif// not JV_ULIBGCJ

  // If a method cache was created, free it.
  _Jv_FreeMethodCache();

  // Clear out thread locals.
  locals = NULL;

#ifndef JV_ULIBGCJ
  // Signal any threads that are waiting to join() us.
  _Jv_MutexLock (&nt->join_mutex);
#endif// not JV_ULIBGCJ

  {
    JvSynchronize sync (this);
    alive_flag = THREAD_DEAD;
    state = JV_TERMINATED;
  }

#ifndef JV_ULIBGCJ
  _Jv_CondNotifyAll (&nt->join_cond, &nt->join_mutex);
  _Jv_MutexUnlock (&nt->join_mutex); 
#endif// not JV_ULIBGCJ 
}

// Run once at thread startup, either when thread is attached or when 
// _Jv_ThreadRun is called.
static void
_Jv_NotifyThreadStart (java::lang::Thread* thread)
{
#ifdef ENABLE_JVMPI
      if (_Jv_JVMPI_Notify_THREAD_START)
	{
	  JVMPI_Event event;
	  
	  jstring thread_name = thread->getName ();
	  jstring group_name = NULL, parent_name = NULL;
	  java::lang::ThreadGroup *group = thread->getThreadGroup ();

	  if (group)
	    {
	      group_name = group->getName ();
	      group = group->getParent ();
	      
	      if (group)
		parent_name = group->getName ();
	    }
	  
	  int thread_len = thread_name ? JvGetStringUTFLength (thread_name) : 0;
	  int group_len = group_name ? JvGetStringUTFLength (group_name) : 0;
	  int parent_len = parent_name ? JvGetStringUTFLength (parent_name) : 0;
	  
	  char thread_chars[thread_len + 1];
	  char group_chars[group_len + 1];
	  char parent_chars[parent_len + 1];
	  
	  if (thread_name)
	    JvGetStringUTFRegion (thread_name, 0, 
				  thread_name->length(), thread_chars);
	  if (group_name)
	    JvGetStringUTFRegion (group_name, 0, 
				  group_name->length(), group_chars);
	  if (parent_name)
	    JvGetStringUTFRegion (parent_name, 0, 
				  parent_name->length(), parent_chars);
	  
	  thread_chars[thread_len] = '\0';
	  group_chars[group_len] = '\0';
	  parent_chars[parent_len] = '\0';
	  
	  event.event_type = JVMPI_EVENT_THREAD_START;
	  event.env_id = NULL;
	  event.u.thread_start.thread_name = thread_chars;
	  event.u.thread_start.group_name = group_chars;
	  event.u.thread_start.parent_name = parent_chars;
	  event.u.thread_start.thread_id = (jobjectID) thread;
	  event.u.thread_start.thread_env_id = _Jv_GetCurrentJNIEnv ();
	  
	  _Jv_DisableGC ();
	  (*_Jv_JVMPI_Notify_THREAD_START) (&event);
	  _Jv_EnableGC ();
	}
#endif
}

void
_Jv_ThreadRun (java::lang::Thread* thread)
{
  try
    {
      _Jv_NotifyThreadStart (thread);
      thread->run ();
    }
  catch (java::lang::Throwable *t)
    {
#ifndef JV_ULIBGCJ
      // Uncaught exceptions are forwarded to the ThreadGroup.  If
      // this results in an uncaught exception, that is ignored.
      try
	{
	  thread->getUncaughtExceptionHandler()->uncaughtException (thread, t);
	}
      catch (java::lang::Throwable *f)
	{
	  // Nothing.
	}
#endif// not JV_ULIBGCJ
    }

  thread->finish_ ();
}

void
java::lang::Thread::start (void)
{
  JvSynchronize sync (this);

  // Its illegal to re-start() a thread, even if its dead.
  if (!startable_flag)
    throw new IllegalThreadStateException;

  alive_flag = THREAD_ALIVE;
  startable_flag = false;
  state = JV_RUNNABLE;
  natThread *nt = (natThread *) data;
  _Jv_ThreadStart (this, nt->thread, (_Jv_ThreadStartFunc *) &_Jv_ThreadRun);
}

#ifndef JV_ULIBGCJ
void
java::lang::Thread::stop (java::lang::Throwable *)
{
  checkAccess ();

  // Old applets still call this method.  Rather than throwing
  // UnsupportedOperationException we simply fail silently.
}

void
java::lang::Thread::suspend (void)
{
  checkAccess ();

  // Old applets still call this method.  Rather than throwing
  // UnsupportedOperationException we simply fail silently.
}
#endif// not JV_ULIBGCJ

static int nextThreadNumber = 0;

jstring
java::lang::Thread::gen_name (void)
{
  jint i;
  jclass sync = &java::lang::Thread::class$;
  {
    JvSynchronize dummy(sync); 
    i = ++nextThreadNumber;
  }

  // Use an array large enough for "-2147483648"; i.e. 11 chars, + "Thread-".
  jchar buffer[7+11];
  jchar *bufend = (jchar *) ((char *) buffer + sizeof(buffer));
  i = _Jv_FormatInt (bufend, i);
  jchar *ptr = bufend - i;
  // Prepend "Thread-".
  *--ptr = '-';
  *--ptr = 'd';
  *--ptr = 'a';
  *--ptr = 'e';
  *--ptr = 'r';
  *--ptr = 'h';
  *--ptr = 'T';
  return JvNewString (ptr, bufend - ptr);
}

void
java::lang::Thread::yield (void)
{
  _Jv_ThreadYield ();
}

#ifndef JV_ULIBGCJ
::java::lang::Thread$State *
java::lang::Thread::getState()
{
  _Jv_InitClass(&::java::lang::Thread$State::class$);

  switch (state)
    {
    case JV_BLOCKED:
      return ::java::lang::Thread$State::BLOCKED;
    case JV_NEW:
      return ::java::lang::Thread$State::NEW;

    case JV_RUNNABLE:
      return ::java::lang::Thread$State::RUNNABLE;
    case JV_TERMINATED:
      return ::java::lang::Thread$State::TERMINATED;
    case JV_TIMED_WAITING:
      return ::java::lang::Thread$State::TIMED_WAITING;
    case JV_WAITING:
      return ::java::lang::Thread$State::WAITING;
    }

  // We don't really need a default, but this makes the compiler
  // happy.
  return ::java::lang::Thread$State::RUNNABLE;
}

_Jv_GetCurrentJNIEnv ()
{
  java::lang::Thread *t = _Jv_ThreadCurrent ();
  if (t == NULL)
    return NULL;
  return (JNIEnv *)((natThread *) t->data)->jni_env;
}

void
_Jv_SetCurrentJNIEnv (JNIEnv *env)
{
  java::lang::Thread *t = _Jv_ThreadCurrent ();
  JvAssert (t != NULL);
  ((natThread *) t->data)->jni_env = env;
}
#endif// not JV_ULIBGCJ

// Attach the current native thread to an existing (but unstarted) Thread 
// object. Does not register thread with the garbage collector.
// Returns -1 on failure, 0 upon success.
jint
_Jv_AttachCurrentThread(java::lang::Thread* thread)
{
  JvSynchronize sync (thread);
  if (thread == NULL || thread->startable_flag == false)
    return -1;
  thread->startable_flag = false;
  thread->alive_flag = ::java::lang::Thread::THREAD_ALIVE;
  thread->state = JV_RUNNABLE;
  natThread *nt = (natThread *) thread->data;
  _Jv_ThreadRegister (nt->thread);
  return 0;
}

java::lang::Thread*
_Jv_AttachCurrentThread(jstring name, java::lang::ThreadGroup* group)
{
  // Register thread with GC before attempting any allocations.
  _Jv_GCAttachThread ();
  java::lang::Thread *thread = _Jv_ThreadCurrent ();
  if (thread != NULL)
    return thread;
  if (name == NULL)
    name = java::lang::Thread::gen_name ();
#ifdef JV_ULIBGCJ
  thread = new java::lang::Thread (NULL, NULL, name);
#else
  thread = new java::lang::Thread (NULL, group, NULL, name);
#endif
  _Jv_AttachCurrentThread (thread);
  _Jv_NotifyThreadStart (thread);
  return thread;
}

#ifndef JV_ULIBGCJ
java::lang::Thread*
_Jv_AttachCurrentThreadAsDaemon(jstring name, java::lang::ThreadGroup* group)
{
  java::lang::Thread *thread = _Jv_ThreadCurrent ();
  if (thread != NULL)
    return thread;
  if (name == NULL)
    name = java::lang::Thread::gen_name ();
  thread = new java::lang::Thread (NULL, group, NULL, name);
  thread->setDaemon (true);
  _Jv_AttachCurrentThread (thread);
  _Jv_NotifyThreadStart (thread);
  return thread;
}
#endif// not JV_ULIBGCJ

jint
_Jv_DetachCurrentThread (void)
{
  java::lang::Thread *t = _Jv_ThreadCurrent ();
  if (t == NULL)
    return -1;

  _Jv_ThreadUnRegister ();
  _Jv_GCDetachThread ();
  // Release the monitors.
  t->finish_ ();

  return 0;
}
