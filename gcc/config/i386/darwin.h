/* Target definitions for x86 running Darwin.
   Copyright (C) 2001, 2002, 2004, 2005 Free Software Foundation, Inc.
   Contributed by Apple Computer Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

/* Enable Mach-O bits in generic x86 code.  */
#undef TARGET_MACHO
#define TARGET_MACHO 1

#define TARGET_VERSION fprintf (stderr, " (i686 Darwin)");

#undef TARGET_FPMATH_DEFAULT
#define TARGET_FPMATH_DEFAULT (TARGET_SSE ? FPMATH_SSE : FPMATH_387)

#define TARGET_OS_CPP_BUILTINS()                \
  do                                            \
    {                                           \
      builtin_define ("__i386__");              \
      builtin_define ("__LITTLE_ENDIAN__");     \
      darwin_cpp_builtins (pfile);		\
    }                                           \
  while (0)

#undef FORCE_PREFERRED_STACK_BOUNDARY_IN_MAIN
#define FORCE_PREFERRED_STACK_BOUNDARY_IN_MAIN (0)

/* We want -fPIC by default, unless we're using -static to compile for
   the kernel or some such.  */

#undef CC1_SPEC
#define CC1_SPEC "%{!static:-fPIC}\
  %{g: %{!fno-eliminate-unused-debug-symbols: -feliminate-unused-debug-symbols }}"

#undef ASM_SPEC
#define ASM_SPEC "-arch i386 -force_cpusubtype_ALL"

#undef SUBTARGET_EXTRA_SPECS
#define SUBTARGET_EXTRA_SPECS					\
  { "darwin_arch", "i386" },					\
  { "darwin_crt2", "" },					\
  { "darwin_subarch", "i386" },

/* Use the following macro for any Darwin/x86-specific command-line option
   translation.  */
#define SUBTARGET_OPTION_TRANSLATE_TABLE \
  { "", "" }

/* The Darwin assembler mostly follows AT&T syntax.  */
#undef ASSEMBLER_DIALECT
#define ASSEMBLER_DIALECT ASM_ATT

/* Define macro used to output shift-double opcodes when the shift
   count is in %cl.  Some assemblers require %cl as an argument;
   some don't.  This macro controls what to do: by default, don't
   print %cl.  */

#define SHIFT_DOUBLE_OMITS_COUNT 0

extern void darwin_x86_file_end (void);
#undef TARGET_ASM_FILE_END
#define TARGET_ASM_FILE_END darwin_x86_file_end

/* Define the syntax of pseudo-ops, labels and comments.  */

/* String containing the assembler's comment-starter.  */

#define ASM_COMMENT_START "#"

/* By default, target has a 80387, uses IEEE compatible arithmetic,
   and returns float values in the 387.  */

#define TARGET_SUBTARGET_DEFAULT (MASK_80387 | MASK_IEEE_FP | MASK_FLOAT_RETURNS | MASK_128BIT_LONG_DOUBLE | MASK_ALIGN_DOUBLE)

/* For now, disable dynamic-no-pic.  We'll need to go through i386.c
   with a fine-tooth comb looking for refs to flag_pic!  */
#define MASK_MACHO_DYNAMIC_NO_PIC 0
#define TARGET_DYNAMIC_NO_PIC	  (target_flags & MASK_MACHO_DYNAMIC_NO_PIC)

#undef GOT_SYMBOL_NAME
#define GOT_SYMBOL_NAME (machopic_function_base_name ())

/* Define the syntax of pseudo-ops, labels and comments.  */

#define LPREFIX "L"

/* These are used by -fbranch-probabilities */
#define HOT_TEXT_SECTION_NAME "__TEXT,__text,regular,pure_instructions"
#define UNLIKELY_EXECUTED_TEXT_SECTION_NAME \
                              "__TEXT,__unlikely,regular,pure_instructions"

/* Assembler pseudos to introduce constants of various size.  */

#define ASM_BYTE_OP "\t.byte\t"
#define ASM_SHORT "\t.word\t"
#define ASM_LONG "\t.long\t"
/* Darwin as doesn't do ".quad".  */

#define SUBTARGET_ENCODE_SECTION_INFO  darwin_encode_section_info

#undef ASM_OUTPUT_ALIGN
#define ASM_OUTPUT_ALIGN(FILE,LOG)	\
 do { if ((LOG) != 0)			\
        {				\
          if (in_section == text_section) \
            fprintf (FILE, "\t%s %d,0x90\n", ALIGN_ASM_OP, (LOG)); \
          else				\
            fprintf (FILE, "\t%s %d\n", ALIGN_ASM_OP, (LOG)); \
        }				\
    } while (0)

/* This says how to output an assembler line
   to define a global common symbol.  */

#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".comm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ",%lu\n", (unsigned long)(ROUNDED)))

/* This says how to output an assembler line
   to define a local common symbol.  */

#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".lcomm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ","HOST_WIDE_INT_PRINT_UNSIGNED"\n", (ROUNDED)))

/* Darwin profiling -- call mcount.  */
#undef FUNCTION_PROFILER
#define FUNCTION_PROFILER(FILE, LABELNO)				\
    do {								\
      if (MACHOPIC_INDIRECT)						\
	{								\
	  const char *name = machopic_mcount_stub_name ();		\
	  fprintf (FILE, "\tcall %s\n", name+1);  /*  skip '&'  */	\
	  machopic_validate_stub_or_non_lazy_ptr (name);		\
	}								\
      else fprintf (FILE, "\tcall mcount\n");				\
    } while (0)

/* Darwin uses the standard DWARF register numbers but the default
   register numbers for STABS.  Fortunately for 64-bit code the
   default and the standard are the same.  */
#undef DBX_REGISTER_NUMBER
#define DBX_REGISTER_NUMBER(n) 					\
  (TARGET_64BIT ? dbx64_register_map[n]				\
   : write_symbols == DWARF2_DEBUG ? svr4_dbx_register_map[n]	\
   : dbx_register_map[n])

/* Unfortunately, the 32-bit EH information also doesn't use the standard
   DWARF register numbers.  */
#define DWARF2_FRAME_REG_OUT(n, for_eh)					\
  (! (for_eh) || write_symbols != DWARF2_DEBUG || TARGET_64BIT ? (n)	\
   : (n) == 5 ? 4							\
   : (n) == 4 ? 5							\
   : (n) >= 11 && (n) <= 18 ? (n) + 1					\
   : (n))

/* Attempt to turn on execute permission for the stack.  This may be
    used by INITIALIZE_TRAMPOLINE of the target needs it (that is,
    if the target machine can change execute permissions on a page).

    There is no way to query the execute permission of the stack, so
    we always issue the mprotect() call.

    Note that we go out of our way to use namespace-non-invasive calls
    here.  Unfortunately, there is no libc-internal name for mprotect().

    Also note that no errors should be emitted by this code; it is
    considered dangerous for library calls to send messages to
    stdout/stderr.  */

#define ENABLE_EXECUTE_STACK                                            \
extern void __enable_execute_stack (void *);                            \
void                                                                    \
__enable_execute_stack (void *addr)                                     \
{                                                                       \
   extern int mprotect (void *, size_t, int);                           \
   extern int __sysctl (int *, unsigned int, void *, size_t *,          \
                       void *, size_t);                                 \
                                                                        \
   static int size;                                                     \
   static long mask;                                                    \
                                                                        \
   char *page, *end;                                                    \
                                                                        \
   if (size == 0)                                                       \
     {                                                                  \
       int mib[2];                                                      \
       size_t len;                                                      \
                                                                        \
       mib[0] = 6; /* CTL_HW */                                         \
       mib[1] = 7; /* HW_PAGESIZE */                                    \
       len = sizeof (size);                                             \
       (void) __sysctl (mib, 2, &size, &len, NULL, 0);                  \
       mask = ~((long) size - 1);                                       \
     }                                                                  \
                                                                        \
   page = (char *) (((long) addr) & mask);                              \
   end  = (char *) ((((long) (addr + TRAMPOLINE_SIZE)) & mask) + size); \
                                                                        \
   /* 7 == PROT_READ | PROT_WRITE | PROT_EXEC */                        \
   (void) mprotect (page, end - page, 7);                               \
}

#undef REGISTER_TARGET_PRAGMAS
#define REGISTER_TARGET_PRAGMAS() DARWIN_REGISTER_TARGET_PRAGMAS()

#undef TARGET_SET_DEFAULT_TYPE_ATTRIBUTES
#define TARGET_SET_DEFAULT_TYPE_ATTRIBUTES darwin_set_default_type_attributes
