/* $Id: kTypes.h 3610 2007-10-29 03:33:14Z bird $ */
/** @file
 *
 * kTypes - Typedefs And Related Constants And Macros.
 *
 * Copyright (c) 2007 knut st. osmundsen <bird-src-spam@anduin.net>
 *
 *
 * This file is part of kStuff.
 *
 * kStuff is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * kStuff is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with kStuff; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef ___k_kTypes_h___
#define ___k_kTypes_h___

#include <k/kDefs.h>

/** @defgroup grp_kTypes    kTypes - Typedefs And Related Constants And Macros
 * @{
 */

/** @typedef KI64
 * 64-bit signed integer. */
/** @typedef KU64
 * 64-bit unsigned integer. */
/** @def KI64_C
 * 64-bit signed integer constant.
 * @param c         The constant value. */
/** @def KU64_C
 * 64-bit unsigned integer constant.
 * @param c         The constant value. */
/** @def KI64_PRI
 * 64-bit signed integer printf format. */
/** @def KU64_PRI
 * 64-bit unsigned integer printf format. */
/** @def KX64_PRI
 * 64-bit signed and unsigned integer hexadecimal printf format. */

/** @typedef KI32
 * 32-bit signed integer. */
/** @typedef KU32
 * 32-bit unsigned integer. */
/** @def KI32_C
 * 32-bit signed integer constant.
 * @param c         The constant value. */
/** @def KU32_C
 * 32-bit unsigned integer constant.
 * @param c         The constant value. */
/** @def KI32_PRI
 * 32-bit signed integer printf format. */
/** @def KU32_PRI
 * 32-bit unsigned integer printf format. */
/** @def KX32_PRI
 * 32-bit signed and unsigned integer hexadecimal printf format. */

/** @typedef KI16
 * 16-bit signed integer. */
/** @typedef KU16
 * 16-bit unsigned integer. */
/** @def KI16_C
 * 16-bit signed integer constant.
 * @param c         The value. */
/** @def KU16_C
 * 16-bit unsigned integer constant.
 * @param c         The value. */
/** @def KI16_PRI
 * 16-bit signed integer printf format. */
/** @def KU16_PRI
 * 16-bit unsigned integer printf format. */
/** @def KX16_PRI
 * 16-bit signed and unsigned integer hexadecimal printf format. */

/** @typedef KI8
 * 8-bit signed integer. */
/** @typedef KU8
 * 8-bit unsigned integer. */
/** @def KI8_C
 * 8-bit signed integer constant.
 * @param c         The constant value. */
/** @def KU8_C
 * 8-bit unsigned integer constant.
 * @param c         The constant value. */
/** @def KI8_PRI
 * 8-bit signed integer printf format. */
/** @def KU8_PRI
 * 8-bit unsigned integer printf format. */
/** @def KX8_PRI
 * 8-bit signed and unsigned integer hexadecimal printf format. */

/** @typedef KSIZE
 * Memory size type; unsigned integer. */
/** @typedef KSSIZE
 * Memory size type; signed integer. */
/** @def KSIZE_C
 * Memory size constant.
 * @param c         The constant value. */
/** @def KSSIZE_C
 * Memory size constant.
 * @param c         The constant value. */
/** @def KSIZE_MAX
 * Memory size max constant.*/
/** @def KSSIZE_MAX
 * Memory size max constant.*/
/** @def KSSIZE_MIN
 * Memory size min constant.*/
/** @def KSIZE_PRI
 * Memory size printf format. */
/** @def KSSIZE_PRI
 * Memory size printf format. */

/** @typedef KIPTR
 * Signed integer type capable of containing a pointer value.  */
/** @typedef KUPTR
 * Unsigned integer type capable of containing a pointer value.  */
/** @def KIPTR_C
 * Signed pointer constant.
 * @param c         The constant value. */
/** @def KUPTR_C
 * Unsigned pointer constant.
 * @param c         The constant value. */
/** @def KIPTR_MAX
 * Signed pointer max constant.*/
/** @def KIPTR_MIN
 * Signed pointer min constant.*/
/** @def KUPTR_MAX
 * Unsigned pointer max constant.*/
/** @def KIPTR_PRI
 * Signed pointer printf format. */
/** @def KUPTR_PRI
 * Unsigned pointer printf format. */


#if K_ARCH_BITS == 32
  /* ASSUMES int == long == 32-bit, short == 16-bit, char == 8-bit. */
# ifdef _MSC_VER
typedef signed __int64          KI64;
typedef unsigned __int64        KU64;
#define KI64_PRI                "I64d"
#define KU64_PRI                "I64u"
#define KX64_PRI                "I64x"
# else
typedef signed long long int    KI64;
typedef unsigned long long int  KU64;
#define KI64_PRI                "lld"
#define KU64_PRI                "llu"
#define KX64_PRI                "llx"
# endif
typedef signed int              KI32;
typedef unsigned int            KU32;
typedef signed short int        KI16;
typedef unsigned short int      KU16;
typedef signed char             KI8;
typedef unsigned char           KU8;
#define KI64_C(c)               (c ## LL)
#define KU64_C(c)               (c ## ULL)
#define KI32_C(c)               (c)
#define KU32_C(c)               (c)
#define KI16_C(c)               (c)
#define KU16_C(c)               (c)
#define KI8_C(c)                (c)
#define KU8_C(c)                (c)

#define KI32_PRI                "d"
#define KU32_PRI                "u"
#define KX32_PRI                "x"
#define KI16_PRI                "d"
#define KU16_PRI                "u"
#define KX16_PRI                "x"
#define KI8_PRI                 "d"
#define KU8_PRI                 "u"
#define KX8_PRI                 "x"

typedef KI32                    KSSIZE;
#define KSSIZE(c)               KI32_C(c)
#define KSSIZE_MAX              KI32_MAX
#define KSSIZE_MIN              KI32_MIN
#define KSSIZE_PRI              KX32_PRI

typedef KU32                    KSIZE;
#define KSIZE_C(c)              KU32_C(c)
#define KSIZE_MAX               KU32_MAX
#define KSIZE_PRI               KX32_PRI
#define KIPTR_C(c)              KI32_C(c)

typedef KI32                    KIPTR;
#define KIPTR_MAX               KI32_MAX
#define KIPTR_MIN               KI32_MIN
#define KIPTR_PRI               KX32_PRI

typedef KU32                    KUPTR;
#define KUPTR_C(c)              KU32_C(c)
#define KUPTR_MAX               KU32_MAX
#define KUPTR_PRI               KX32_PRI


#elif K_ARCH_BITS == 64

# if K_OS == K_OS_WINDOWS
#  if _MSC_VER
typedef signed __int64          KI64;
typedef unsigned __int64        KU64;
#   define KI64_PRI             "I64d"
#   define KU64_PRI             "I64u"
#   define KX64_PRI             "I64x"
#  else
typedef signed long long int    KI64;
typedef unsigned long long int  KU64;
#   define KI64_PRI              "lld"
#   define KU64_PRI              "llu"
#   define KX64_PRI              "llx"
#  endif
#  define KI64_C(c)             (c ## LL)
#  define KU64_C(c)             (c ## ULL)
# else
typedef signed long int         KI64;
typedef unsigned long int       KU64;
#  define KI64_C(c)             (c ## L)
#  define KU64_C(c)             (c ## UL)
#  define KI64_PRI              "ld"
#  define KU64_PRI              "lu"
#  define KX64_PRI              "lx"
# endif
typedef signed int              KI32;
typedef unsigned int            KU32;
typedef signed short            KI16;
typedef unsigned short          KU16;
typedef signed char             KI8;
typedef unsigned char           KU8;
#define KI32_C(c)               (c)
#define KU32_C(c)               (c)
#define KI16_C(c)               (c)
#define KU16_C(c)               (c)
#define KI8_C(c)                (c)
#define KU8_C(c)                (c)

#define KI32_PRI                "d"
#define KU32_PRI                "u"
#define KX32_PRI                "x"
#define KI16_PRI                "d"
#define KU16_PRI                "u"
#define KX16_PRI                "x"
#define KI8_PRI                 "d"
#define KU8_PRI                 "u"
#define KX8_PRI                 "x"

typedef KI64                    KSSIZE;
#define KSSIZE(c)               KI64_C(c)
#define KSSIZE_MAX              KI64_MAX
#define KSSIZE_MIN              KI64_MIN
#define KSSIZE_PRI              KX64_PRI

typedef KU64                    KSIZE;
#define KSIZE_C(c)              KU64_C(c)
#define KSIZE_MAX               KU64_MAX
#define KSIZE_PRI               KX64_PRI

typedef KI64                    KIPTR;
#define KIPTR_C(c)              KI64_C(c)
#define KIPTR_MAX               KI64_MAX
#define KIPTR_MIN               KI64_MIN
#define KIPTR_PRI               KX64_PRI

typedef KU64                    KUPTR;
#define KUPTR_C(c)              KU64_C(c)
#define KUPTR_MAX               KU64_MAX
#define KUPTR_PRI               KX64_PRI

#else
# error "Port Me"
#endif


/** Min KI8 value. */
#define KI8_MIN                 (KI8_C(-0x7f) - 1)
/** Min KI16 value. */
#define KI16_MIN                (KI16_C(-0x7fff) - 1)
/** Min KI32 value. */
#define KI32_MIN                (KI32_C(-0x7fffffff) - 1)
/** Min KI64 value. */
#define KI64_MIN                (KI64_C(-0x7fffffffffffffff) - 1)
/** Max KI8 value. */
#define KI8_MAX                 KI8_C(0x7f)
/** Max KI16 value. */
#define KI16_MAX                KI16_C(0x7fff)
/** Max KI32 value. */
#define KI32_MAX                KI32_C(0x7fffffff)
/** Max KI64 value. */
#define KI64_MAX                KI64_C(0x7fffffffffffffff)
/** Max KU8 value. */
#define KU8_MAX                 KU8_C(0xff)
/** Max KU16 value. */
#define KU16_MAX                KU16_C(0xffff)
/** Max KU32 value. */
#define KU32_MAX                KU32_C(0xffffffff)
/** Max KU64 value. */
#define KU64_MAX                KU64_C(0xffffffffffffffff)

/** File offset. */
typedef KI64                    KFOFF;
/** Pointer a file offset. */
typedef KFOFF                  *PFOFF;
/** Pointer a const file offset. */
typedef KFOFF                  *PCFOFF;
/** The min value for the KFOFF type. */
#define KFOFF_MIN               KI64_MIN
/** The max value for the KFOFF type. */
#define KFOFF_MAX               KI64_MAX
/** File offset contstant.
 * @param c         The constant value. */
#define KFOFF_C(c)              KI64_C(c)
/** File offset printf format. */
#define KFOFF_PRI               KI64_PRI


/**
 * Memory Protection.
 */
typedef enum KPROT
{
    /** The usual invalid 0. */
    KPROT_INVALID = 0,
    /** No access (page not present). */
    KPROT_NOACCESS,
    /** Read only. */
    KPROT_READONLY,
    /** Read & write. */
    KPROT_READWRITE,
    /** Read & copy on write. */
    KPROT_WRITECOPY,
    /** Execute only. */
    KPROT_EXECUTE,
    /** Execute & read. */
    KPROT_EXECUTE_READ,
    /** Execute, read & write. */
    KPROT_EXECUTE_READWRITE,
    /** Execute, read & copy on write. */
    KPROT_EXECUTE_WRITECOPY,
    /** The usual end value. (exclusive) */
    KPROT_END,
    /** Blow the type up to 32-bits. */
    KPROT_32BIT_HACK = 0x7fffffff
} KPROT;
/** Pointer to a memory protection enum. */
typedef KPROT                  *PKPROT;
/** Pointer to a const memory protection enum. */
typedef KPROT const            *PCKPROT;

/** Boolean.
 * This can be used as a tri-state type, but then you *must* do == checks. */
typedef KI8                     KBOOL;
/** Pointer to a boolean value. */
typedef KBOOL                  *PKBOOL;
/** Pointer to a const boolean value. */
typedef KBOOL const            *PCKBOOL;
/** Maxium value the KBOOL type can hold (officially). */
#define KBOOL_MIN               KI8_C(-1)
/** Maxium value the KBOOL type can hold (officially). */
#define KBOOL_MAX               KI8_C(1)
/** The KBOOL printf format. */
#define KBOOL_PRI               KU8_PRI
/** Boolean true constant. */
#define K_TRUE                  KI8_C(1)
/** Boolean false constant. */
#define K_FALSE                 KI8_C(0)
/** Boolean unknown constant (the third state). */
#define K_UNKNOWN               KI8_C(-1)



/** @name   Forward Declarations / Handle Types.
 * @{ */

/** Pointer to a file provider instance. */
typedef struct KRDR *PKRDR;
/** Pointer to a file provider instance pointer. */
typedef struct KRDR **PPKRDR;

/** Pointer to a loader segment. */
typedef struct KLDRSEG *PKLDRSEG;
/** Pointer to a loader segment. */
typedef const struct KLDRSEG *PCKLDRSEG;

/** @} */

/** @} */

#endif
