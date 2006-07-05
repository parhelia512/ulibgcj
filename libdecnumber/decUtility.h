/* Utility functions for decimal floating point support via decNumber.
   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by IBM Corporation.  Author Mike Cowlishaw.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */

#ifdef IN_LIBGCC2
#define decDensePackCoeff __decDensePackCoeff
#define decDenseUnpackCoeff __decDenseUnpackCoeff
#endif

extern void decDensePackCoeff (decNumber *, uByte *, Int, Int);
extern void decDenseUnpackCoeff (uByte *, Int, decNumber *, Int, Int);
