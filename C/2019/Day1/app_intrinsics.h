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