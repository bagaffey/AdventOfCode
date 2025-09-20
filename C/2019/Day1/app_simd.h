typedef struct f32_4x
{
    union
    {
        __m128 P;
        f32 E[4];
        u32 U32[4];
    };
} f32_4x;

typedef struct v3_4x
{
    union
    {
        struct
        {
            union
            {
                f32_4x x;
                f32_4x r;
            };

            union
            {
                f32_4x y;
                f32_4x g;
            };

            union
            {
                f32_4x z;
                f32_4x b;
            };
        };

        f32_4x E[3];
    };
} v3_4x;

typedef struct v4_4x
{
    union
    {
        struct
        {
            union
            {
                f32_4x x;
                f32_4x r;
            };

            union
            {
                f32_4x y;
                f32_4x g;
            };

            union
            {
                f32_4x z;
                f32_4x b;
            };

            union
            {
                f32_4x w;
                f32_4x a;
            };
        };

        f32_4x E[4];
    };
} v4_4x;

#define mmSquare(a) _mm_mul_ps(a, a)
#define M(a, i) ((float *)&(a))[i]
#define Mi(a, i) ((uint32 *)&(a))[i]

//
// f32_4x functions
//

inline f32_4x
LoadF32_4X(void* Address)
{
    f32_4x Result;

    Result.P = _mm_loadu_ps((f32*)Address);

    return (Result);
}

inline void
StoreF32_4X_Addr(f32_4x A, void* Address)
{
    _mm_storeu_ps((f32*)Address, A.P);
}

inline f32_4x
F32_4x(f32 EAll)
{
    f32_4x Result;

    Result.P = _mm_set1_ps(EAll);

    return (Result);
}

inline f32_4x
U32_4x_1xU32(u32 EAll)
{
    f32_4x Result;

    Result.P = _mm_set1_ps(*(float*)&EAll);

    return (Result);
}

inline f32_4x
U32_4x(u32 E0, u32 E1, u32 E2, u32 E3)
{
    f32_4x Result;

    Result.P = _mm_setr_ps(*(float*)&E0,
        *(float*)&E1,
        *(float*)&E2,
        *(float*)&E3);

    return (Result);
}

inline f32_4x
U32ToF32(f32_4x A)
{
    f32_4x Result;

    Result.P = _mm_cvtepi32_ps(_mm_castps_si128(A.P));

    return (Result);
}

inline f32_4x
F32_4x_byF32(f32 E0, f32 E1, f32 E2, f32 E3)
{
    f32_4x Result;

    Result.P = _mm_setr_ps(E0, E1, E2, E3);

    return (Result);
}

inline f32_4x
F32_4x_m128(__m128 EAll)
{
    f32_4x Result;

    Result.P = EAll;

    return (Result);
}

inline f32_4x
ZeroF32_4x(void)
{
    f32_4x Result;

    Result.P = _mm_setzero_ps();

    return (Result);
}

inline f32_4x
f32_4x_add_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_add_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_minus_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_sub_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_negate(f32_4x A)
{
    f32_4x Result;

    Result = f32_4x_minus_f32_4x(ZeroF32_4x(), A);

    return (Result);
};

inline f32_4x
f32_4x_multiply_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_mul_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_xor_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_xor_ps(A.P, B.P);

    return (Result);
}

#define ShiftLeft4X(A, Imm) F32_4x_m128(_mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(A.P), Imm)))
#define ShiftRight4X(A, Imm) F32_4x_m128(_mm_castsi128_ps(_mm_srli_epi32( _mm_castps_si128(A.P), Imm)))

inline f32_4x
f32_4x_divide_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_div_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_xor_assign_f32_4x(f32_4x *A, f32_4x B)
{
    *A = f32_4x_xor_f32_4x(*A, B);

    return (*A);
}

inline f32_4x
f32_4x_add_assign_f32_4x(f32_4x *A, f32_4x B)
{
    *A = f32_4x_add_f32_4x(*A, B);

    return (*A);
}

inline f32_4x
f32_4x_minus_assign_f32_4x(f32_4x *A, f32_4x B)
{
    *A = f32_4x_minus_f32_4x(*A, B);
    return (*A);
}

inline f32_4x
f32_4x_multiply_assign_f32_4x(f32_4x *A, f32_4x B)
{
    *A = f32_4x_multiply_f32_4x (*A, B);
    return (*A);
}

inline f32_4x
f32_4x_divide_assign_f32_4x(f32_4x* A, f32_4x B)
{
    *A = f32_4x_divide_f32_4x(*A, B);

    return (*A);
}

inline f32_4x
f32_4x_lessthan_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_cmplt_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_lessthen_equals_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_cmple_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_greaterthan_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_cmpgt_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_greaterthan_equals_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_cmpge_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_equals_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_cmpeq_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_notequals_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_cmpneq_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_AND_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_and_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_OR_f32_4x(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_or_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_AND_assign_f32_4x(f32_4x *A, f32_4x B)
{
    *A = f32_4x_AND_f32_4x(*A, B);

    return (*A);
}

inline f32_4x
f32_4x_OR_assign_f32_4x(f32_4x *A, f32_4x B)
{
    *A = f32_4x_OR_f32_4x(*A, B);

    return (*A);
}

inline f32_4x
f32_4x_AbsoluteValue(f32_4x A)
{
    u32 MaskU32 = (u32)(1 << 31);
    __m128 Mask = _mm_set1_ps(*(float*)&MaskU32);

    f32_4x Result;
    Result.P = _mm_andnot_ps(Mask, A.P);

    return (Result);
}

inline f32_4x
f32_4x_SignBitFrom(f32_4x A)
{
    u32 MaskU32 = (u32)(1 << 31);
    __m128 Mask = _mm_set1_ps(*(float*)&MaskU32);

    f32_4x Result;
    Result.P = _mm_and_ps(A.P, Mask);

    return (Result);
}

inline f32_4x
f32_4x_SignOf(f32_4x Value)
{
    f32_4x Result = f32_4x_OR_f32_4x(F32_4x(1.0f), f32_4x_SignBitFrom(Value) );

    return (Result);
}

inline f32_4x
f32_4x_InverseSignBitFrom(f32_4x A)
{
    u32 MaskU32 = (u32)(1 << 31);
    __m128 Mask = _mm_set1_ps(*(float*)&MaskU32);

    f32_4x Result;
    Result.P = _mm_xor_ps(_mm_and_ps(A.P, Mask), Mask);

    return (Result);
}

inline f32_4x
f32_4x_Min(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_min_ps(A.P, B.P);

    return (Result);
}

inline f32_4x
f32_4x_Max(f32_4x A, f32_4x B)
{
    f32_4x Result;

    Result.P = _mm_max_ps(A.P, B.P);

    return (Result);
}

internal f32_4x
f32_4x_Floor(f32_4x A)
{
    f32_4x Result;

    Result.P = _mm_floor_ps(A.P);

    return (Result);
}

inline xbool32
f32_4x_AnyTrue(f32_4x Comparison)
{
    xbool32 Result = _mm_movemask_ps(Comparison.P);
    return (Result);
}

inline xbool32
f32_4x_AllTrue(f32_4x Comparison)
{
    xbool32 Result = (_mm_movemask_ps(Comparison.P) == 15);
    return (Result);
}

inline xbool32
f32_4x_AllFalse(f32_4x Comparison)
{
    xbool32 Result = (_mm_movemask_ps(Comparison.P) == 0);
    return (Result);
}

inline f32_4x
f32_4x_AndNot(f32_4x A, f32_4x B) // B gets notted
{
    f32_4x Result;

    Result.P = _mm_andnot_ps(B.P, A.P);

    return (Result);
}

inline f32_4x
f32_4x_Select(f32_4x A, f32_4x Mask, f32_4x B)
{
    f32_4x Result;

    // Requires SSE 4.
    Result.P = _mm_or_ps(_mm_andnot_ps(Mask.P, A.P), _mm_and_ps(Mask.P, B.P));

    return (Result);
}

inline f32_4x
f32_4x_ApproxInvSquareRoot(f32_4x A)
{
    f32_4x Result;

    Result.P = _mm_rsqrt_ps(A.P);

    return (Result);
}

inline f32_4x
f32_4x_SquareRoot(f32_4x A)
{
    f32_4x Result;

    Result.P = _mm_sqrt_ps(A.P);

    return (Result);
}

inline f32_4x
f32_4x_ApproxOneOver(f32_4x A)
{
    f32_4x Result;

    Result.P = _mm_rcp_ps(A.P);

    return (Result);
}

internal f32_4x
f32_4x_Clamp(f32_4x MinV, f32_4x A, f32_4x MaxV)
{
    f32_4x Result;

    Result = f32_4x_Min(f32_4x_Max(A, MinV), MaxV);

    return (Result);
}

internal f32_4x
f32_4x_Clamp01(f32_4x A)
{
    f32_4x Result;

    f32_4x One = F32_4x(1);
    Result = f32_4x_Clamp(ZeroF32_4x(), A, One);

    return (Result);
}

//
// v3_4x operations
//

inline v3_4x
f32_multiply_v3_4x(f32 As, v3_4x B)
{
    v3_4x Result;

    f32_4x A = F32_4x(As);

    Result.x = f32_4x_multiply_f32_4x(A, B.x);
    Result.y = f32_4x_multiply_f32_4x(A, B.y);
    Result.z = f32_4x_multiply_f32_4x(A, B.z);

    return (Result);
}

inline v3_4x
v3_4x_divide_v3_4x(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_divide_f32_4x(A.x, B.x);
    Result.y = f32_4x_divide_f32_4x(A.y, B.y);
    Result.z = f32_4x_divide_f32_4x(A.z, B.z);

    return (Result);
}

inline v3_4x
v3_4x_Hadamard_v3_4x(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_multiply_f32_4x(A.x, B.x);
    Result.y = f32_4x_multiply_f32_4x(A.y, B.y);
    Result.z = f32_4x_multiply_f32_4x(A.z, B.z);

    return (Result);
}

inline v3_4x
v3_4x_add_v3_4x(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_add_f32_4x(A.x, B.x);
    Result.y = f32_4x_add_f32_4x(A.y, B.y);
    Result.z = f32_4x_add_f32_4x(A.z, B.z);

    return (Result);
}

inline v3_4x
v3_4x_minus_v3_4x(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_minus_f32_4x(A.x, B.x);
    Result.y = f32_4x_minus_f32_4x(A.y, B.y);
    Result.z = f32_4x_minus_f32_4x(A.z, B.z);

    return (Result);
}

inline v3_4x
f32_4x_multiply_v3_4x(f32_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_multiply_f32_4x(A, B.x);
    Result.y = f32_4x_multiply_f32_4x(A, B.y);
    Result.z = f32_4x_multiply_f32_4x(A, B.z);

    return (Result);
};

inline v3_4x
ZeroV34x(void)
{
    v3_4x Result = {};
    return (Result);
}

inline v3_4x
v3_4x_negated(v3_4x A)
{
    v3_4x Result = v3_4x_minus_v3_4x(ZeroV34x(), A);
    
    return (Result);
}

inline v3_4x
v3_4x_OR_v3_4x(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_OR_f32_4x(A.x, B.x);
    Result.y = f32_4x_OR_f32_4x(A.y, B.y);
    Result.z = f32_4x_OR_f32_4x(A.z, B.z);

    return (Result);
}

inline v3_4x
v3_4x_AND_v3_4x(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_AND_f32_4x(A.x, B.x);
    Result.y = f32_4x_AND_f32_4x(A.y, B.y);
    Result.z = f32_4x_AND_f32_4x(A.z, B.z);

    return (Result);
}

inline v3_4x
v3_4x_add_assign_v3_4x(v3_4x *A, v3_4x B)
{
    A->x = f32_4x_add_f32_4x(A->x, B.x);
    A->y = f32_4x_add_f32_4x(A->y, B.y);
    A->z = f32_4x_add_f32_4x(A->z, B.z);

    return (*A);
}


inline v4_4x
v3_4x_multiply_v3_4x(f32 As, v4_4x B)
{
    v4_4x Result;

    f32_4x A = F32_4x(As);
    Result.x = f32_4x_multiply_f32_4x(A, B.x);
    Result.y = f32_4x_multiply_f32_4x(A, B.y);
    Result.z = f32_4x_multiply_f32_4x(A, B.z);
    Result.w = f32_4x_multiply_f32_4x(A, B.w);

    return (Result);
}

inline v4_4x
v4_4x_add_v4_4x(v4_4x A, v4_4x B)
{
    v4_4x Result;

    Result.x = f32_4x_add_f32_4x(A.x, B.x);
    Result.y = f32_4x_add_f32_4x(A.y, B.y);
    Result.z = f32_4x_add_f32_4x(A.z, B.z);
    Result.w = f32_4x_add_f32_4x(A.w, B.w);

    return (Result);
}

inline v4_4x
v4_4x_add_assign_v4_4x(v4_4x *A, v4_4x B)
{
    f32_4x_add_assign_f32_4x(&A->x, B.x);
    f32_4x_add_assign_f32_4x(&A->y, B.y);
    f32_4x_add_assign_f32_4x(&A->z, B.z);
    f32_4x_add_assign_f32_4x(&A->w, B.w);

    return (*A);
}

inline v3_4x
v3_4x_lessthan_equals_v3_4x(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_lessthen_equals_f32_4x(A.x, B.x);
    Result.y = f32_4x_lessthen_equals_f32_4x(A.y, B.y);
    Result.z = f32_4x_lessthen_equals_f32_4x(A.z, B.z);

    return (Result);
}

inline v3_4x
v3_4x_lessthan_v3_4x(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_lessthan_f32_4x(A.x, B.x);
    Result.y = f32_4x_lessthan_f32_4x(A.y, B.y);
    Result.z = f32_4x_lessthan_f32_4x(A.z, B.z);

    return (Result);
}

inline v3_4x
v3_4x_AbsoluteValue(v3_4x A)
{
    v3_4x Result;

    Result.x = f32_4x_AbsoluteValue(A.x);
    Result.y = f32_4x_AbsoluteValue(A.y);
    Result.z = f32_4x_AbsoluteValue(A.z);

    return (Result);
}

inline v3_4x
v3_4x_Min(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_Min(A.x, B.x);
    Result.y = f32_4x_Min(A.y, B.y);
    Result.z = f32_4x_Min(A.z, B.z);

    return (Result);
}

inline v3_4x
v3_4x_Max(v3_4x A, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_Max(A.x, B.x);
    Result.y = f32_4x_Max(A.y, B.y);
    Result.z = f32_4x_Max(A.z, B.z);

    return (Result);
}

inline xbool32
Any3TrueInAtLeastOneLane(v3_4x Comparison)
{
    xbool32 Result = f32_4x_AnyTrue(
        f32_4x_OR_f32_4x(
            f32_4x_OR_f32_4x(Comparison.x, Comparison.y), 
            Comparison.z));
    return (Result);
}

inline xbool32
All3TrueInAtLeastOneLane(v3_4x Comparison)
{
    xbool32 Result = f32_4x_AnyTrue(
        f32_4x_OR_f32_4x(
            f32_4x_AND_f32_4x(Comparison.x, Comparison.y), 
            Comparison.z));
    return (Result);
}

inline v3_4x
V3_4x(v3 A)
{
    v3_4x Result;

    Result.x = F32_4x(A.X);
    Result.y = F32_4x(A.Y);
    Result.z = F32_4x(A.Z);

    return (Result);
}

inline v3_4x
V3_4x_by_v3(v3 E0, v3 E1, v3 E2, v3 E3)
{
    v3_4x Result;

    Result.x = F32_4x_byF32(E0.X, E1.X, E2.X, E3.X);
    Result.y = F32_4x_byF32(E0.Y, E1.Y, E2.Y, E3.Y);
    Result.z = F32_4x_byF32(E0.Z, E1.Z, E2.Z, E3.Z);

    return (Result);
}

inline v3_4x
V3_4x_by_f32(f32 E0, f32 E1, f32 E2, f32 E3)
{
    v3_4x Result;

    Result.x =
        Result.y =
        Result.z = F32_4x_byF32(E0, E1, E2, E3);

    return (Result);
}

inline v3
GetComponent(v3_4x Combined, u32 Index)
{
    v3 Result =
    {
        .X = Combined.x.E[Index],
        .Y = Combined.y.E[Index],
        .Z = Combined.z.E[Index],
    };
    return (Result);
}

inline v3_4x
v3_4x_Select(v3_4x A, f32_4x Mask, v3_4x B)
{
    v3_4x Result;

    Result.x = f32_4x_Select(A.x, Mask, B.x);
    Result.y = f32_4x_Select(A.y, Mask, B.y);
    Result.z = f32_4x_Select(A.z, Mask, B.z);

    return (Result);
}

inline f32_4x
v3_4x_Inner_v3_4x(v3_4x A, v3_4x B)
{
    f32_4x Result;

    Result = // A.x * B.x + A.y * B.y + A.z * B.z;
        f32_4x_add_f32_4x(
            f32_4x_add_f32_4x(
                f32_4x_multiply_f32_4x(A.x, B.x),
                f32_4x_multiply_f32_4x(A.y, B.y)
            ), f32_4x_multiply_f32_4x(A.z, B.z));

    return (Result);
}

inline f32_4x
v3_4x_LengthSq(v3_4x A)
{
    f32_4x Result = v3_4x_Inner_v3_4x(A, A);

    return (Result);
}

inline v3_4x
v3_4x_ApproxNOZ(v3_4x A)
{
    v3_4x Result = {};

    f32_4x LenSq = v3_4x_LengthSq(A);
    v3_4x Norm =  //(F32_4x(1.0f) * f32_4x_ApproxInvSquareRoot(LenSq)) * A;
    f32_4x_multiply_v3_4x(f32_4x_multiply_f32_4x(F32_4x(1.0f), f32_4x_ApproxInvSquareRoot(LenSq)), A);
    
    f32_4x Mask = (f32_4x_lessthan_f32_4x(LenSq, F32_4x(Square(0.0001f))));
    Result = v3_4x_Select(Result, Mask, Norm);

    return (Result);
}

inline v3_4x
v3_4x_ApproxNOUp(v3_4x A)
{
    v3_4x Result = V3_4x(V3(0.0f, 0.0f, 1.0f));

    f32_4x LenSq = v3_4x_LengthSq(A);
    v3_4x Norm = f32_4x_multiply_v3_4x(f32_4x_multiply_f32_4x(F32_4x(1.0f), f32_4x_ApproxInvSquareRoot(LenSq)), A);
    
    f32_4x Mask = (f32_4x_lessthan_f32_4x(LenSq, F32_4x(Square(0.0001f))));

    Result = v3_4x_Select(Result, Mask, Norm);

    return (Result);
}

inline v3_4x
v3_4x_ApproxOneOver(v3_4x A)
{
    v3_4x Result;

    Result.x = f32_4x_ApproxOneOver(A.x);
    Result.y = f32_4x_ApproxOneOver(A.y);
    Result.z = f32_4x_ApproxOneOver(A.z);

    return (Result);
}

// Shuffle 4x floats within one register efficiently
// Uses pshufd (does not destroy unrelated lanes).
#define ShuffleWithin4x(A, S0, S1, S2, S3) \
    F32_4x_m128(_mm_castsi128_ps( \
        _mm_shuffle_epi32(_mm_castps_si128(A.P), \
            ((S0 << 0) | (S1 << 2) | (S2 << 4) | (S3 << 6)))))

// Shuffle 4x floats from two registers (A and B)
// Uses shufps (general purpose shuffle across registers).
#define Shuffle4x(A, S0, S1, B, S2, S3) \
    F32_4x_m128(_mm_shuffle_ps(A.P, B.P, \
            ((S0 << 0) | (S1 << 2) | (S2 << 4) | (S3 << 6))))

#define Broadcast4x(A, Index) Shuffle4x(A, Index, Index, A, Index, Index)

inline v3_4x
v3_4x_Transpose(v3_4x A)
{
    v3_4x Result;

#define VERIFY_SHUFFLE 0
#if VERIFY_SHUFFLE
    A.x = F32_4x(0.0f, 3.0f, 6.0f, 9.0f);
    A.y = F32_4x(1.0f, 4.0f, 7.0f, 10.0f);
    A.z = F32_4x(2.0f, 5.0f, 8.0f, 11.0f);
#endif

    f32_4x XY = Shuffle4x(A.x, 0, 2, A.y, 0, 2);
    f32_4x YZ = Shuffle4x(A.y, 1, 3, A.z, 1, 3);
    f32_4x XZ = Shuffle4x(A.x, 1, 3, A.z, 0, 2);

    Result.x = Shuffle4x(XY, 0, 2, XZ, 2, 0);
    Result.y = Shuffle4x(YZ, 0, 2, XY, 1, 3);
    Result.z = Shuffle4x(XZ, 3, 1, YZ, 1, 3);

#if VERIFY_SHUFFLE
    f32 Order[] =
    {
        Result.x.E[0], Result.x.E[1], Result.x.E[2], Result.x.E[3],
        Result.y.E[0], Result.y.E[1], Result.y.E[2], Result.y.E[3],
        Result.z.E[0], Result.z.E[1], Result.z.E[2], Result.z.E[3],
    };
#endif

    return (Result);
}

internal f32_4x
PShufB(f32_4x Value, __m128i ShufflePattern)
{
    f32_4x Result;

    Result.P = _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(Value.P), ShufflePattern));

    return (Result);
}

#define ConvertF32(V, I) _mm_cvtss_f32(Shuffle4x(V, I, I, V, I, I).P)
#define ConvertS32(V, I) _mm_extract_epi32(_mm_cvtps_epi32(V.P), I)
