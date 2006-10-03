// FinalizerThread.java -- Thread in which finalizers are run.

/* Copyright (C) 2001, 2004  Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

package gnu.gcj.runtime;

/**
 * @author Tom Tromey <tromey@redhat.com>
 * @date October 3, 2001
 */
public final class FinalizerThread
/*#if not ULIBGCJ*/
  extends Thread
/*#endif*/
{
/*#if not ULIBGCJ*/
  private static boolean finalizer_ready;
/*#endif*/

  public FinalizerThread ()
  {
/*#if not ULIBGCJ*/
    super ("LibgcjInternalFinalizerThread");
    setDaemon (true);
    finalizer_ready = false;
    init();
/*#endif*/
  }

/*#if not ULIBGCJ*/
  private native void init();
/*#endif*/
  static native void finalizerReady();
/*#if not ULIBGCJ*/
  public native void run();
/*#endif*/
}
