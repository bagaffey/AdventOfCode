#pragma once

#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif

#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif

#if !COMPILER_MSVC && !COMPILER_LLVM
#if _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#endif
#endif

#if COMPILER_MSVC
#include <intrin.h>
#elif COMPILER_LLVM
#include <x86intrin.h>
#else
#error No low-level compiler intrinsics for this compiler?
#endif

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

typedef double real64;
typedef float real32;

typedef real32 r32;
typedef real64 r64;
typedef real32 f32;
typedef real64 f64;

// This varies wildly between compilers
typedef long double realmax;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef uint8 ubool8;
typedef uint16 ubool16;
// Some APIs use unsigned bools
typedef uint32 ubool32; 

typedef uint8 u8;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int8 bool8;
typedef int16 bool16;
// Win32 and legacy C code used signed ints for bools
typedef int32 bool32;

typedef int8 s8;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;

typedef size_t mem_index;

typedef uintptr_t uintptr;
typedef intptr_t intptr;

typedef uintptr_t umw;
typedef intptr_t smw;

typedef bool32 xbool32;
typedef u32 xu32;

#define U8Max 255
#define U16Max 65535
#define S32Min ( (int32) 0x80000000 )
#define S32Max ( (int32) 0x7fffffff )
#define U32Min 0
#define U32Max ( (uint32) - 1 )
#define U64Max ( (uint64) - 1 )
#define F32Max FLT_MAX
#define F32Min -FLT_MAX

struct buffer
{
	umw Count;
	u8* Data;
};
typedef buffer string;

#define Pi32 3.14159265359f
#define Tau32 6.28318530717958647692f