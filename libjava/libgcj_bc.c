/* libgcj_bc.c  */

/* Copyright (C) 2006 Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

/* This file is used to build libgcj_bc.so, a 'fake' library that is
   used at link time only. It ensures that binaries built with the
   BC-ABI link against a constant SONAME. This way, BC-ABI binaries
   continue to work if the SONAME underlying libgcj.so changes.  */

#include <stdlib.h>
#include <stdio.h>

static void print_wrong_lib_msg ()
{
  fprintf (stderr, "libgcj error: \
This is libgcj_bc.so, a fake library used only for linking.\n\
Please create a symlink from libgcj_bc.so.1 to the real libgcj.so.\n");
  exit (1);
}

/* Functions called from code generated by gcj.  */

void __gcj_personality_v0 () {}
void _Jv_AllocObject () {}
void _Jv_AllocObjectNoFinalizer () {}
void _Jv_InitClass () {}
void _Jv_ResolvePoolEntry () {}
void _Jv_Throw () {}
void _Jv_MonitorEnter () {}
void _Jv_NewPrimArray () {}
void _Jv_NewObjectArray () {}
void _Jv_NewMultiArray () {}
void _Jv_ThrowBadArrayIndex () {}
void _Jv_ThrowNullPointerException () {}
void _Jv_ThrowAbstractMethodError () {}
void _Jv_ThrowNoSuchFieldError () {}
void _Jv_CheckCast () {}
void _Jv_IsInstanceOf () {}
void _Jv_CheckArrayStore () {}
void _Jv_LookupInterfaceMethodIdx () {}

void _Jv_RegisterClasses () 
{
  print_wrong_lib_msg ();
}

void _Jv_RegisterNewClasses () 
{
  print_wrong_lib_msg ();
}

/* Symbols used by jvgenmain (-fmain).  */

void JvRunMain () {}
const char **_Jv_Compiler_Properties;

/* Functions used by -fjni.  */

void _Jv_LookupJNIMethod () {}
void _Jv_GetJNIEnvNewFrame () {}
void _Jv_UnwrapJNIweakReference () {}


/* Checked divide (-fuse-divide-subroutine).  */

void _Jv_divI () {}
void _Jv_remI () {}
void _Jv_divJ () {}
void _Jv_remJ () {}


/* CNI Functions.  */

void _Jv_AllocBytes () {}
void _Jv_AllocString () {}
void _Jv_NewString () {}
void _Jv_NewStringLatin1 () {}
void _Jv_GetStringChars () {}
void _Jv_GetStringUTFLength () {}
void _Jv_GetStringUTFRegion () {}
void _Jv_NewStringUTF () {}
void _Jv_Malloc () {}
void _Jv_Realloc () {}
void _Jv_Free () {}
void _Jv_CreateJavaVM () {}
void _Jv_AttachCurrentThread () {}
void _Jv_AttachCurrentThreadAsDaemon () {}
void _Jv_DetachCurrentThread () {}