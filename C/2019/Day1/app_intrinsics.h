#include "basetypes.h"
//#include "math.h"

#define Maximum(A, B) ((A > B) ? (A) : (B))
#define Minimum(A, B) ((A < B) ? (A) : (B))

#define SignOf(X) _Generic((X), \
int32: SignOfInt32, \
f32: SignOfF32 \
)(X)

inline int32
SignOfInt32(int32 Value)
{
	int32 Result = (Value >= 0) ? 1 : -1;
	return(Result);
}

inline f32
SignOfF32(f32 Value)
{
	u32 MaskU32 = (u32)(1 << 31);
	__m128 Mask = _mm_set_ss(*(float*)&MaskU32);

	__m128 One = _mm_set_ss(1.0f);
	__m128 SignBit = _mm_and_ps(_mm_set_ss(Value), Mask);
	__m128 Combined = _mm_or_ps(One, SignBit);

	f32 Result = _mm_cvtss_f32(Combined);

	return(Result);
}

inline f32
SquareRoot(f32 Real32)
{
	f32 Result = _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(Real32)));
	return(Result);
}

inline f32
ReciprocalSquareRoot(f32 Real32)
{
	f32 Result = (1.0f / SquareRoot(Real32));
	return(Result);
}

#define RotateLeft(X) _Generic((X), \
uint32: RotateLeftU32, \
u64: RotateLeftU64 \
)(X)

inline uint32
RotateLeftU32(uint32 Value, int32 Amount)
{
#if COMPILER_MSVC
	// MSVC intrinsic
	return _rotl(Value, Amount);

#elif COMPILER_LLVM || COMPILER_GCC
#if (__has_builtin(__builtin_rotateleft32))
	// Clang/GCC built-in (very efficient)
	return __builtin_rotateleft32(Value, Amount);
#else
	// Portable fallback that will always work.
	Amount &= 31;
	return (Value << Amount) | (Value >> (32 - Amount));
#endif
#else
	// Ultimate fallback if some exotic compiler
	Amount &= 31;
	return (Value << Amount) | (Value >> (32 - Amount));
#endif
}

inline u64
RotateLeftU64(u64 Value, s32 Amount)
{
#if COMPILER_MSVC
	u64 Result = _rotl64(Value, Amount);
#elif COMPILER_LLVM || COMPILER_GCC
#if (__has_builtin(__builtin_rotateleft64))
	// Clang/GCC built-in (very efficient)
	u64 Result = __builtin_rotateleft64(Value, Amount);
#else
	// Portable fallback that will always work.
	Amount &= 63;
	u64 Result = (Value << Amount) | (Value >> (64 - Amount));
#endif
#else
	// Ultimate fallback if some exotic compiler
	Amount &= 63;
	u64 Result = (Value << Amount) | (Value >> (64 - Amount));
#endif
	return(Result);
}

inline uint32
RotateRight(uint32 Value, int32 Amount)
{
#if COMPILER_MSVC
	uint32 Result = _rotr(Value, Amount);
#elif COMPILER_LLVM || COMPILER_GCC
#if (__has_builtin(__builtin_rotateright32))
	uint32 Result = __builtin_rotateright32(Value, Amount);
#else
	// Portable fallback that will always work.
	Amount &= 31;
	uint32 Result = (Value << Amount) | (Value >> (32 - Amount));
#endif
#else
	// Ultimate fallback if some exotic compiler
	Amount &= 31;
	uint32 Result = (Value << Amount) | (Value >> (32 - Amount));
#endif
	return(Result);
}

inline s32
RoundReal32ToInt32(f32 Real32)
{
	s32 Result = _mm_cvtss_si32(_mm_set_ss(Real32));
	return(Result);
}

inline u32
RoundReal32ToUInt32(real32 Real32)
{
	u32 Result = (u32)_mm_cvtss_si32(_mm_set_ss(Real32));
	return(Result);
}

inline int32
FloorReal32ToInt32(real32 Real32)
{
	int32 Result = _mm_cvtss_si32(_mm_floor_ss(_mm_setzero_ps(), _mm_set_ss(Real32)));
	return(Result);
}

inline f32
Round(f32 Real32)
{
	f32 Result = _mm_cvtss_f32(_mm_round_ss(_mm_setzero_ps(), _mm_set_ss(Real32),
		(_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)));
	return(Result);
}

inline f32
Floor(f32 Real32)
{
	f32 Result = _mm_cvtss_f32(_mm_floor_ss(_mm_setzero_ps(), _mm_set_ss(Real32)));
	return(Result);
}

inline int32
CeilReal32ToInt32(real32 Real32)
{
	int32 Result = _mm_cvtss_si32(_mm_ceil_ss(_mm_setzero_ps(), _mm_set_ss(Real32)));
	return(Result);
}