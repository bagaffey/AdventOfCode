#pragma once

#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif

#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif



#if !COMPILER_MSVC && !COMPILER_LLVM
#if defined(__clang__)
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#elif defined(_MSC_VER)
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
#error Unknown compiler
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

#define UMWFromPointer(Pointer) ((umw)(Pointer))
#define PointerFromUMW(type, Value) (type *)(Value)

//#define U32FromPointer(Pointer) ((u32)(mem_index)(Pointer))
//#define PointerFromU32(type, Value) (type *)((mem_index)Value)

#define OffsetOf(type, Member) (umw)&(((type *)0)->Member)
#define ContainerOf(ptr, type, member) ((type *)((char *)(ptr) - OffsetOf(type, Member)))

#define FILE_AND_LINE__(A, B) A "|" #B
#define FILE_AND_LINE_(A, B) FILE_AND_LINE__(A, B)
#define FILE_AND_LINE FILE_AND_LINE_(__FILE__, __LINE__)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct buffer
{
	umw Count;
	u8* Data;
} buffer, string;

#ifdef __cplusplus
}
#endif

typedef union v2
{
	struct
	{
		real32 X, Y;
	};
	struct
	{
		real32 U, V;
	};
	struct
	{
		real32 Width, Height;
	};
} v2;

typedef union v2u
{
	struct
	{
		u32 X, Y;
	};
	struct 
	{
		u32 Width, Height;
	};
	u32 E[2];
} v2u;

typedef union v2s
{
	struct
	{
		s32 X, Y;
	};
	s32 E[2];
} v2s;

typedef union v3
{
	struct
	{
		real32 X, Y, Z;
	};
	struct
	{
		real32 U, V, __;
	};
	struct
	{
		real32 R, G, B;
	};
	struct
	{
		v2 XY;
		real32 Ignored0_;
	};
	struct
	{
		real32 Ignored1_;
		v2 YZ;
	};
	struct
	{
		v2 UV;
		real32 Ignored2_;
	};
	struct
	{
		real32 Ignored3_;
		v2 V__;
	};
	f32 E[3];
} v3;

typedef union v3s
{
	struct
	{
		s32 x;
		s32 y;
		s32 z;
	};
	s32 E[3];
} v3s;

typedef union v4
{
	struct
	{
		union
		{
			v3 XYZ;
			struct
			{
				real32 X, Y, Z;
			};
		};

		real32 W;

		struct
		{
			union
			{
				v3 RGB;
				struct
				{
					real32 R, G, B;
				};
			};

			real32 A;
		};

		struct
		{
			v2 XY;
			real32 Ignored0_;
			real32 Ignored1_;
		};

		struct
		{
			real32 Ignored2_;
			v2 YZ;
			real32 Ignored3_;
		};

		struct
		{
			real32 Ignored4_;
			real32 Ignored5_;
			v2 ZW;
		};
		f32 E[4];
	};
} v4;

typedef struct m4x4
{
	/* E[ROW][COLUMN] */
	f32 E[4][4];
} m4x4;

typedef struct m4x4_inv
{
	m4x4 Forward;
	m4x4 Inverse;
} m4x4_inv;

typedef struct rectangle2i
{
	v2s Min;
	v2s Max;
} rectangle2i;

typedef struct rectangle2
{
	v2 Min;
	v2 Max;
} rectangle2;

typedef struct rectangle3
{
	v3 Min;
	v3 Max;
} rectangle3;

typedef struct ticket_mutex
{
	u64 volatile Ticket;
	u64 volatile Serving;
} ticket_mutex;

#define Pi32 3.14159265359f
#define Tau32 6.28318530717958647692f

#define CTAssert(Expr) static_assert(Expr, "Assertion failed: " #Expr)

#define func static

#if !defined(internal)
#define internal static
#endif
#define local_persist static
#define global static
#define TEMPORARY 

#if PROJ_INTERNAL
#define NotImplemented Assert(!"NotImplemented")
#else
#define NotImplemented ItIsNotImplemented
#endif

#if PROJ_SLOW
#define Assert(Expression) if(!(Expression)) {*(volatile int *)0 = 0;}
#else
#define Assert(Expression)
#endif

#define InvlaidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

#define Terabytes(Value) (Gigabytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Kilobytes(Value) ((Value)*1024LL)

#define AlignPow2(Value, Alignment) (((Value) + ((Alignment) - 1)) & ~(((Value) - (Value)) + (Alignment) - 1))
#define Align4(Value) (((Value) + 3) & ~3)
#define Align8(Value) (((Value) + 7) & ~7)
#define Align16(Value) (((Value) + 15) & ~15)

inline xbool32 IsPow2(u32 Value)
{
	xbool32 Result = ((Value != 0) && ((Value & (Value - 1)) == 0));
	return(Result);
}

#if COMPILER_MSVC
#define CompletePreviousReadsBeforeFutureReads _ReadBarrier()
#define CompletePreviousWritesBeforeFutureWrites _WriteBarrier()
inline uint32 AtomicCompareExchangeUInt32(uint32 volatile* Value, uint32 New, uint32 Expected)
{
	uint32 Result = _InterlockedCompareExchange((long volatile*)Value, New, Expected);
	return(Result);
}

inline uint64 AtomicExchangeU64(uint64 volatile* Value, uint64 New)
{
	uint64 Result = _InterlockedExchange64((__int64 volatile*)Value, New);
	return(Result);
}

inline uint64 AtomicAddU64(uint64 volatile* Value, uint64 Addend)
{
	uint64 Result = _InterlockedExchangeAdd64((__int64 volatile*)Value, Addend);
	return(Result);
}

#elif COMPILER_LLVM
#define CompletePreviousReadsBeforeFutureReads asm volatile("" ::: "memory")
#define CompletePreviousWritesBeforeFutureWrites asm volatile("" ::: "memory")
inline uint32 AtomicCompareExchangeUInt32(uint32 volatile* Value, uint32 New, uint32 Expected)
{
	uint32 Result = __sync_val_compare_and_swap(Value, Expected, New);

	return(Result);
}
inline uint64 AtomicExchangeU64(uint64 volatile* Value, uint64 New)
{
	uint64 Result = __sync_lock_test_and_set(Value, New);
	return(Result);
}
inline uint64 AtomicAddU64(uint64 volatile* Value, uint64 Addend)
{
	uint64 Result = __sync_fetch_and_add(Value, Addend);

	return(Result);
}
#endif

inline void
BeginTicketMutex(ticket_mutex* Mutex)
{
	u64 Ticket = AtomicAddU64(&Mutex->Ticket, 1);
	while (Ticket != Mutex->Serving) { _mm_pause(); }
}

inline void
EndTicketMutex(ticket_mutex* Mutex)
{
	AtomicAddU64(&Mutex->Serving, 1);
}

inline u32
SafeTruncateToU32(u64 Value)
{
	Assert(Value <= U32Max);
	u32 Result = (u32)Value;
	return(Result);
}

inline u16
SafeTruncateToU16(u32 Value)
{
	Assert(Value <= U16Max);
	u16 Result = (u16)Value;
	return(Result);
}

inline u8
SafeTruncateToU8(u64 Value)
{
	Assert(Value <= U8Max);
	u8 Result = (u8)Value;
	return(Result);
}

inline u32
StringLength(char* String)
{
	u32 Count = 0;
	if (String)
	{
		while (*String++)
		{
			++Count;
		}
	}
	return(Count);
}
