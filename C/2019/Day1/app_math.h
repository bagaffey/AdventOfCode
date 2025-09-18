#define SQUARE(a) ((a)*(a))
#define CUBE(a) ((a)*(a)*(a))

inline v2u
V2U(u32 X, u32 Y)
{
    v2u Result = { {X, Y} };
    return (Result);
}

inline v2s
V2S(s32 X, s32 Y)
{
    v2s Result = { {X, Y} };
    return (Result);
}

#define V2i(X,Y) _Generic(((X)+(Y)), \
    int32    : V2is, \
    uint32   : V2iu \
)(X,Y)

inline v2
V2is(int32 X, int32 Y)
{
    v2 Result = { { (real32)X, (real32)Y } };
    return (Result);
}

inline v2
V2iu(uint32 X, uint32 Y)
{
    v2 Result = { { (real32)X, (real32)Y } };

    return (Result);
}

inline v2
V2(real32 X, real32 Y)
{
    v2 Result;

    Result.X = X;
    Result.Y = Y;

    return (Result);
}

inline v2
V2sToV2(v2s A)
{
    v2 Result;

    Result.X = (f32)A.X;
    Result.Y = (f32)A.Y;

    return (Result);
}

inline v2
SignedV2From(v2u Source)
{
    v2 Result = { { (f32)Source.X, (f32)Source.Y } };
    return (Result);
}

inline v3
V3(real32 X, real32 Y, real32 Z)
{
    v3 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;

    return (Result);
}

inline v3
V2ToV3(v2 XY, real32 Z)
{
    v3 Result;

    Result.X = XY.X;
    Result.Y = XY.Y;
    Result.Z = Z;

    return (Result);
}

inline v4
V4(real32 X, real32 Y, real32 Z, real32 W)
{
    v4 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;

    return (Result);
}

inline v4
V3ToV4(v3 XYZ, real32 W)
{
    v4 Result;

    Result.XYZ = XYZ;
    Result.W = W;

    return (Result);
}

// Scalar funcs

inline real32
Square(real32 A)
{
    real32 Result = A * A;

    return (Result);
}

inline r32
Sin01(r32 t)
{
    r32 Result = Sin(Pi32 * t);

    return (Result);
}

inline r32
Triangle01(r32 t)
{
    r32 Result = 2.0f * t;
    if (Result > 1.0f)
        Result = 2.0f - Result;

    return (Result);
}

inline real32
Lerp(real32 A, real32 t, real32 B)
{
    real32 Result = (1.0f - t) * A + t * B;

    return (Result);
}

inline s32
S32BinormalLerp(s32 A, f32 tBinormal, s32 B)
{
    f32 t = 0.5f + (0.5f * tBinormal);
    f32 fResult = Lerp((f32)A, t, (f32)B);
    s32 Result = RoundReal32ToInt32(fResult);

    return (Result);
}

inline s32
ClampS32(s32 Min, s32 Value, s32 Max)
{
    s32 Result = Value;

    if (Result < Min)
        Result = Min;
    else if (Result > Max)
        Result = Max;

    return (Result);
}

inline real32
ClampReal32(real32 Min, real32 Value, real32 Max)
{
    real32 Result = Value;

    if (Result < Min)
        Result = Min;
    else if (Result > Max)
        Result = Max;

    return (Result);
}

inline real32
Clamp01(real32 Value)
{
    real32 Result = ClampReal32(0.0f, Value, 1.0f);

    return (Result);
}

inline real32
Clamp01MapToRange(real32 Min, real32 t, real32 Max)
{
    real32 Result = 0.0f;

    real32 Range = Max - Min;

    if (Range != 0.0f)
        Result = Clamp01((t - Min) / Range);

    return (Result);
}

inline real32
ClampBinormalMapToRange(real32 Min, real32 t, real32 Max)
{
    real32 Result = -1.0f + 2.0f * Clamp01MapToRange(Min, t, Max);
    return (Result);
}

inline r32
ClampAboveZero(r32 Value)
{
    r32 Result = (Value < 0) ? 0.0f : Value;
    return (Result);
}

inline real32
Real32SafeRatioN(real32 Numerator, real32 Divisor, real32 N)
{
    real32 Result = N;

    if (Divisor != 0.0f)
        Result = Numerator / Divisor;

    return (Result);
}

inline real32
Real32SafeRatio0(real32 Numerator, real32 Divisor)
{
    real32 Result = Real32SafeRatioN(Numerator, Divisor, 0.0f);

    return (Result);
}

inline real32
Real32SafeRatio1(real32 Numerator, real32 Divisor)
{
    real32 Result = Real32SafeRatioN(Numerator, Divisor, 1.0f);

    return (Result);
}

inline f64
F64SafeRatioN(f64 Numerator, f64 Divisor, f64 N)
{
    f64 Result = N;

    if (Divisor != 0.0f)
        Result = Numerator / Divisor;

    return (Result);
}

inline f64
F64SafeRatio0(f64 Numerator, f64 Divisor)
{
    f64 Result = F64SafeRatioN(Numerator, Divisor, 0.0);

    return (Result);
}

//
// V2 unsigned and unsigned functions
//

func v2u
V2uAddV2u(v2u A, v2u B)
{
    v2u Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;

    return (Result);
}

func v2s
V2sAddV2s(v2s A, v2s B)
{
    v2s Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;

    return (Result);
}

func inline v2s
V2sAddV2sAssign(v2s* A, v2s B)
{
    *A = V2sAddV2s(*A, B);

    return (*A);
}

//
// V2 Functions
//

inline v2
V2Perp(v2 A)
{
    v2 Result = { { -A.Y, A.X } };
    return (Result);
}

inline v2
Real32MultiplyV2(real32 A, v2 B)
{
    v2 Result;

    Result.X = A * B.X;
    Result.Y = A * B.Y;

    return (Result);
}

inline v2s
S32MultiplyV2s(s32 A, v2s B)
{
    v2s Result;

    Result.X = A * B.X;
    Result.Y = A * B.Y;

    return (Result);
}

inline v2
V2MultiplyReal32(v2 B, real32 A)
{
    v2 Result = Real32MultiplyV2(A, B);

    return (Result);
}

inline v2
V2MultiplyReal32Assign(v2 *B, real32 A)
{
    *B = Real32MultiplyV2(A, *B);

    return (*B);
}

inline v2
V2Negated(v2 A)
{
    v2 Result;

    Result.X = -A.X;
    Result.Y = -A.Y;

    return (Result);
}

inline v2
V2AddV2(v2 A, v2 B)
{
    v2 Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;

    return (Result);
}

inline v2
V2AddV2Assign(v2 *A, v2 B)
{
    *A = V2AddV2(*A, B);

    return (*A);
}

inline v2
V2MinusV2(v2 A, v2 B)
{
    v2 Result;

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;

    return (Result);
}

inline v2
V2MinusV2Assign(v2 *A, v2 B)
{
    *A = V2MinusV2(*A, B);

    return (*A);
}

inline v2
V2HadamardV2(v2 A, v2 B)
{
    v2 Result = { { A.X * B.X, A.Y * B.Y } };

    return (Result);
}

inline v2s
V2sHadamardV2s(v2s A, v2s B)
{
    v2s Result = { { A.X * B.X, A.Y * B.Y } };

    return (Result);
}

inline real32
V2InnerV2(v2 A, v2 B)
{
    real32 Result = A.X * B.X + A.Y * B.Y;

    return (Result);
}

inline real32
V2LengthSq(v2 A)
{
    real32 Result = V2InnerV2(A, A);

    return (Result);
}

inline real32
V2Length(v2 A)
{
    real32 Result = SquareRoot(V2LengthSq(A));
    return (Result);
}

inline v2
V2Clamp01(v2 Value)
{
    v2 Result;

    Result.X = Clamp01(Value.X);
    Result.Y = Clamp01(Value.Y);

    return (Result);
}

inline v2
V2Arm2(r32 Angle)
{
    v2 Result = { { Cos(Angle), Sin(Angle) } };

    return (Result);
}

//
// V3 functions
//

inline v3
V3MultiplyReal32(real32 A, v3 B)
{
    v3 Result;

    Result.X = A * B.X;
    Result.Y = A * B.Y;
    Result.Z = A * B.Z;

    return (Result);
}

inline v3
V3MultiplyReal32Assign(v3 *B, real32 A)
{
    *B = V3MultiplyReal32(A, *B);

    return (*B);
}

inline v3
V3DivideByReal32(v3 B, real32 A)
{
    v3 Result = V3MultiplyReal32((1.0f / A), B);

    return (Result);
}

inline v3
F32DivideByV3(f32 B, v3 A)
{
    v3 Result =
    { 
        {
            B / A.X,
            B / A.Y,
            B / A.Z,
        }
    };

    return (Result);
}

inline v3
V3DivideReal32Assign(v3 *B, real32 A)
{
    *B = V3DivideByReal32(*B, A);

    return (*B);
}

inline v3
V3Negated(v3 A)
{
    v3 Result;

    Result.X = -A.X;
    Result.Y = -A.Y;
    Result.Z = -A.Z;

    return (Result);
}

inline v3
V3AddV3(v3 A, v3 B)
{
    v3 Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;

    return (Result);
}

inline v3
V3AddV3Assign(v3 *A, v3 B)
{
    *A = V3AddV3(*A, B);

    return (*A);
}

inline v3
V3SubtractV3(v3 A, v3 B)
{
    v3 Result;

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;

    return (Result);
}

inline v3
V3SubtractV3Assign(v3 *A, v3 B)
{
    *A = V3SubtractV3(*A, B);

    return (*A);
}

inline v3
V3MultiplyV3(v3 A, v3 B)
{
    v3 Result = { { A.X * B.X, A.Y * B.Y, A.Z * B.Z } };

    return (Result);
}

inline v3
V3DivideV3(v3 A, v3 B)
{
    v3 Result = { { A.X / B.X, A.Y / B.Y, A.Z / B.Z } };

    return (Result);
}

inline v3s
V3sMultiplyV3s(v3s A, v3s B)
{
    v3s Result = { { A.X * B.X, A.Y * B.Y, A.Z * B.Z } };

    return (Result);
}

inline real32
V3InnerV3(v3 A, v3 B)
{
    real32 Result = A.X * B.X + A.Y * B.Y + A.Z * B.Z;

    return (Result);
}

inline v3
V3CrossV3(v3 A, v3 B)
{
    v3 Result;

    Result.X = A.Y * B.Z - A.Z * B.Y;
    Result.Y = A.Z * B.X - A.X * B.Z;
    Result.Z = A.X * B.Y - A.Y * B.X;

    return (Result);
}

inline real32
V3LengthSq(v3 A)
{
    real32 Result = V3InnerV3(A, A);

    return (Result);
}

inline real32
V3Length(v3 A)
{
    real32 Result = SquareRoot(V3LengthSq(A));
    return (Result);
}

inline v3
V3Normalize(v3 A)
{
    v3 Result = V3MultiplyReal32((1.0f / V3Length(A)), A);

    return (Result);
}

inline v3
V3NOZ(v3 A)
{
    v3 Result = {};

    r32 LenSq = V3LengthSq(A);
    if (LenSq > Square(0.0001f))
        Result = V3MultiplyReal32((1.0f / SquareRoot(LenSq)), A);

    return (Result);
}

internal v3
V3Floor(v3 Value)
{
    v3 Result = { { Floor(Value.X), Floor(Value.Y), Floor(Value.Z) } };
    return (Result);
}

internal v3
V3Round(v3 Value)
{
    v3 Result = { { Round(Value.X), Round(Value.Y), Round(Value.Z) } };
    return (Result);
}

internal v3
V3Clamp01(v3 Value)
{
    v3 Result;

    Result.X = Clamp01(Value.X);
    Result.Y = Clamp01(Value.Y);
    Result.Z = Clamp01(Value.Z);

    return (Result);
}

inline v3
V3Lerp(v3 A, real32 t, v3 B)
{
    v3 Result = 
        V3AddV3(
            V3MultiplyReal32((1.0f - t), A),
            V3MultiplyReal32(t, B));

    return (Result);
}

inline v3
V3Min(v3 A, v3 B)
{
	v3 Result =
	{
        {
            Minimum(A.X, B.X),
            Minimum(A.Y, B.Y),
            Minimum(A.Z, B.Z),
        }
    };

	return (Result);
}

inline v3
V3Max(v3 A, v3 B)
{
    v3 Result =
    {
        {
            Maximum(A.X, B.X),
            Maximum(A.Y, B.Y),
            Maximum(A.Z, B.Z),
        }
    };

    return (Result);
}

//
// V4 functions
//


inline v4
V4MultiplyByReal32(v4 B, real32 A)
{
    v4 Result;

    Result.X = A * B.X;
    Result.Y = A * B.Y;
    Result.Z = A * B.Z;
    Result.W = A * B.W;

    return (Result);
}

inline v4
V4AssignMultiplyByReal32(v4 *B, real32 A)
{
    *B = V4MultiplyByReal32(*B, A);

    return (*B);
}

inline v4
V4Negated(v4 A)
{
    v4 Result;

    Result.X = -A.X;
    Result.Y = -A.Y;
    Result.Z = -A.Z;
    Result.W = -A.W;

    return (Result);
}

inline v4
V4AddV4(v4 A, v4 B)
{
    v4 Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    Result.W = A.W + B.W;

    return (Result);
}

inline v4
V4AddV4Assign(v4 *A, v4 B)
{
    *A = V4AddV4(*A, B);

    return (*A);
}

inline v4
V4SubtractV4(v4 A, v4 B)
{
    v4 Result;

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    Result.W = A.W - B.W;

    return (Result);
}

inline v4
V4SubtractV4Assign(v4 *A, v4 B)
{
    *A = V4SubtractV4(*A, B);

    return (*A);
}

inline v4
V4HadamardV4(v4 A, v4 B)
{
    v4 Result; //  = { { A.X * B.X, A.Y * B.Y, A.Z * B.Z, A.W * B.W } };

    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    Result.W = A.W * B.W;

    return (Result);
}

inline real32
V4InnerV4(v4 A, v4 B)
{
    real32 Result = A.X * B.X + A.Y * B.Y + A.Z * B.Z + A.W * B.W;

    return (Result);
}

inline real32
V4LengthSq(v4 A)
{
    real32 Result = V4InnerV4(A, A);

    return (Result);
}

inline real32
V4Length(v4 A)
{
    real32 Result = SquareRoot(V4LengthSq(A));
    return (Result);
}

inline v4
V4Clamp01(v4 Value)
{
    v4 Result;

    Result.X = Clamp01(Value.X);
    Result.Y = Clamp01(Value.Y);
    Result.Z = Clamp01(Value.Z);
    Result.W = Clamp01(Value.W);

    return (Result);
}

inline v4
V4Lerp(v4 A, real32 t, v4 B)
{
    v4 Result = 
        V4AddV4(
            V4MultiplyByReal32(A, (1.0f - t)), 
            V4MultiplyByReal32(B, t)
        );

    return (Result);
}

inline v4
V4Min(v4 A, v4 B)
{
    v4 Result;
    //{ 
    //    {
    //        Minimum(A.X, B.X),
    //        Minimum(A.Y, B.Y),
    //        Minimum(A.Z, B.Z),
    //        Minimum(A.W, B.W),
    //    }
    //};

    Result.X = Minimum(A.X, B.X);
    Result.Y = Minimum(A.Y, B.Y);
    Result.Z = Minimum(A.Z, B.Z);
    Result.W = Minimum(A.W, B.W);

    return (Result);
}

inline v4
V4Max(v4 A, v4 B)
{
    v4 Result;
    //{ 
    //    {
    //        Maximum(A.X, B.X),
    //        Maximum(A.Y, B.Y),
    //        Maximum(A.Z, B.Z),
    //        Maximum(A.W, B.W),
    //    } 
    //};
    
    Result.X = Maximum(A.X, B.X);
    Result.Y = Maximum(A.Y, B.Y);
    Result.Z = Maximum(A.Z, B.Z);
    Result.W = Maximum(A.W, B.W);

    return (Result);
}

//
// Rectangle2
//

inline rectangle2
InvertedInfinityRectangle2(void)
{
    rectangle2 Result;

    Result.Min.X = Result.Min.Y = F32Max;
    Result.Max.X = Result.Max.Y = -F32Max;

    return (Result);
}

inline rectangle2
Rect2Union(rectangle2 A, rectangle2 B)
{
    rectangle2 Result;

    Result.Min.X = (A.Min.X < B.Min.X) ? A.Min.X : B.Min.X;
    Result.Min.Y = (A.Min.Y < B.Min.Y) ? A.Min.Y : B.Min.Y;
    Result.Max.X = (A.Max.X > B.Max.X) ? A.Max.X : B.Max.X;
    Result.Max.Y = (A.Max.Y > B.Max.Y) ? A.Max.Y : B.Max.Y;

    return (Result);
}

inline v2
V2GetMinCorner(rectangle2 Rect)
{
    v2 Result = Rect.Min;
    return (Result);
}

inline v2
V2GetMaxCorner(rectangle2 Rect)
{
    v2 Result = Rect.Max;
    return (Result);
}

inline v2
V2GetDim(rectangle2 Rect)
{
    v2 Result = V2MinusV2(Rect.Max, Rect.Min);
    return (Result);
}

inline rectangle2
V2Rectangle2From(rectangle2i Source)
{
    rectangle2 Result;
    //{
    //    (f32)Source.Min.X,
    //    (f32)Source.Min.Y,
    //    (f32)Source.Max.X,
    //    (f32)Source.Max.Y,
    //};

    Result.Min.X = (f32)Source.Min.X;
    Result.Min.Y = (f32) Source.Min.Y;
    Result.Max.X = (f32) Source.Max.X;
    Result.Max.Y = (f32) Source.Max.Y;

    return (Result);
}

inline v2
V2Rect2GetCenter(rectangle2 Rect)
{
    v2 Result = Real32MultiplyV2(0.5f, (V2AddV2(Rect.Min, Rect.Max)));
    return (Result);
}

inline rectangle2
V2Rect2MinMax(v2 Min, v2 Max)
{
    rectangle2 Result;

    Result.Min = Min;
    Result.Max = Max;

    return (Result);
}

inline rectangle2
V2Rect2MinDim(v2 Min, v2 Dim)
{
    rectangle2 Result;

    Result.Min = Min;
    Result.Max = V2AddV2(Min, Dim);

    return (Result);
}

inline rectangle2
V2Rect2CenterHalfDim(v2 Center, v2 HalfDim)
{
    rectangle2 Result;

    Result.Min = V2MinusV2(Center, HalfDim);
    Result.Max = V2AddV2(Center, HalfDim);

    return (Result);
}

inline rectangle2
V2AddRadiusTo(rectangle2 A, v2 Radius)
{
    rectangle2 Result;
    Result.Min = V2MinusV2(A.Min, Radius);
    Result.Max = V2AddV2(A.Max, Radius);

    return (Result);
}

inline rectangle2
Rect2Offset(rectangle2 A, v2 Offset)
{
    rectangle2 Result;

    Result.Min = V2AddV2(A.Min, Offset);
    Result.Max = V2AddV2(A.Max, Offset);

    return (Result);
}

inline rectangle2
Rect2CenterDim(v2 Center, v2 Dim)
{
    rectangle2 Result = V2Rect2CenterHalfDim(Center, Real32MultiplyV2(0.5f, Dim));
    return (Result);
}

inline bool32
IsInRectangle2(rectangle2 Rectangle, v2 Test)
{
    bool32 Result = ((Test.X >= Rectangle.Min.X) &&
        (Test.Y >= Rectangle.Min.Y) &&
        (Test.X < Rectangle.Max.X) &&
        (Test.Y < Rectangle.Max.Y));

    return (Result);
}

inline bool32
Rectangle2Intersect(rectangle2 A, rectangle2 B)
{
    bool32 Result = !((B.Max.X <= A.Min.X) ||
        (B.Min.X >= A.Max.X) ||
        (B.Max.Y <= A.Min.Y) ||
        (B.Min.Y >= A.Max.Y));
    return (Result);
}

inline v2
V2GetBarycentric(rectangle2 A, v2 P)
{
    v2 Result;

    Result.X = F64SafeRatio0(P.X - A.Min.X, A.Max.X - A.Min.X);
    Result.Y = F64SafeRatio0(P.Y - A.Min.Y, A.Max.Y - A.Min.Y);

    return (Result);
}

inline r32
GetArea(rectangle2 A)
{
    v2 Dim = V2GetDim(A);
    r32 Result = Dim.X * Dim.Y;
    return (Result);
}

//
// Rectangle3
//

inline rectangle3
InvertedInfinityRectangle3(void)
{
    rectangle3 Result;

    Result.Min.X = Result.Min.Y = Result.Min.Z = F32Max;
    Result.Max.X = Result.Max.Y = Result.Max.Z = -F32Max;

    return (Result);
}

inline v3
V3GetMinCorner(rectangle3 Rect)
{
    v3 Result = Rect.Min;
    return (Result);
}

inline v3
V3GetMaxCorner(rectangle3 Rect)
{
    v3 Result = Rect.Max;
    return (Result);
}

inline v3
V3GetDim(rectangle3 Rect)
{
    v3 Result = V3SubtractV3(Rect.Max, Rect.Min);
    return (Result);
}

inline v3
V3GetRadius(rectangle3 Rect)
{
    v3 Result = 
        V3MultiplyReal32(0.5f, 
            V3SubtractV3(Rect.Max, Rect.Min));
    return (Result);
}

inline v3
GetCenter(rectangle3 Rect)
{
    v3 Result = V3MultiplyReal32(0.5f, V3AddV3(Rect.Min, Rect.Max));
        // (Rect.Min + Rect.Max);
    return (Result);
}

inline rectangle3
V3RectMinMax(v3 Min, v3 Max)
{
    rectangle3 Result;

    Result.Min = Min;
    Result.Max = Max;

    return (Result);
}

inline rectangle3
V3RectMinDim(v3 Min, v3 Dim)
{
    rectangle3 Result;

    Result.Min = Min;
    Result.Max = V3AddV3(Min, Dim);

    return (Result);
}

inline rectangle3
Rect3CenterHalfDim(v3 Center, v3 HalfDim)
{
    rectangle3 Result;

    Result.Min = V3SubtractV3(Center, HalfDim);
    Result.Max = V3AddV3(Center, HalfDim);

    return (Result);
}

inline rectangle3
Rect3AddRadiusTo(rectangle3 A, v3 Radius)
{
    rectangle3 Result;

    Result.Min = V3SubtractV3(A.Min, Radius);
    Result.Max = V3AddV3(A.Max, Radius);

    return (Result);
}

inline rectangle3
Rect3Offset(rectangle3 A, v3 Offset)
{
    rectangle3 Result;

    Result.Min = V3AddV3(A.Min, Offset);
    Result.Max = V3AddV3(A.Max, Offset);

    return (Result);
}

inline rectangle3
RectCenterDim(v3 Center, v3 Dim)
{
    rectangle3 Result = Rect3CenterHalfDim(Center, V3MultiplyReal32(0.5f, Dim));
    return (Result);
}

inline bool32
IsInRectangle(rectangle3 Rectangle, v3 Test)
{
    bool32 Result = ((Test.X >= Rectangle.Min.X) &&
        (Test.Y >= Rectangle.Min.Y) &&
        (Test.Z >= Rectangle.Min.Z) &&
        (Test.X < Rectangle.Max.X) &&
        (Test.Y < Rectangle.Max.Y) &&
        (Test.Z < Rectangle.Max.Z));

    return (Result);
}

inline xbool32
IsInRectangleCenterHalfDim(v3 P, v3 Radius, v3 Test)
{
    v3 Rel = V3SubtractV3(Test, P);
    xbool32 Result = ((AbsoluteValue(Rel.X) <= Radius.X) &&
        (AbsoluteValue(Rel.Y) <= Radius.Y) &&
        (AbsoluteValue(Rel.Z) <= Radius.Z));

    return (Result);
}

inline bool32
RectanglesIntersect(rectangle3 A, rectangle3 B)
{
    bool32 Result = !((B.Max.X <= A.Min.X) ||
        (B.Min.X >= A.Max.X) ||
        (B.Max.Y <= A.Min.Y) ||
        (B.Min.Y >= A.Max.Y) ||
        (B.Max.Z <= A.Min.Z) ||
        (B.Min.Z >= A.Max.Z));
    return (Result);
}

inline v3
V3GetBarycentric(rectangle3 A, v3 P)
{
    v3 Result;
    
    Result.X = Real32SafeRatio0(P.X - A.Min.X, A.Max.X - A.Min.X);
    Result.Y = Real32SafeRatio0(P.Y - A.Min.Y, A.Max.Y - A.Min.Y);
    Result.Z = Real32SafeRatio0(P.Z - A.Min.Z, A.Max.Z - A.Min.Z);

    return (Result);
}

inline v3
PointFromUVW(rectangle3 A, v3 UVW)
{
    v3 Result;

    Result.X = Lerp(A.Min.X, UVW.X, A.Max.X);
    Result.Y = Lerp(A.Min.Y, UVW.Y, A.Max.Y);
    Result.Z = Lerp(A.Min.Z, UVW.Z, A.Max.Z);

    return (Result);
}

inline rectangle2
ToRectangleXY(rectangle3 A)
{
    rectangle2 Result;

    Result.Min = A.Min.XY;
    Result.Max = A.Max.XY;

    return (Result);
}

inline rectangle3
Rect3Union(rectangle3 A, rectangle3 B)
{
    rectangle3 Result;

    Result.Min.X = (A.Min.X < B.Min.X) ? A.Min.X : B.Min.X;
    Result.Min.Y = (A.Min.Y < B.Min.Y) ? A.Min.Y : B.Min.Y;
    Result.Min.Z = (A.Min.Z < B.Min.Z) ? A.Min.Z : B.Min.Z;

    Result.Max.X = (A.Max.X > B.Max.X) ? A.Max.X : B.Max.X;
    Result.Max.Y = (A.Max.Y > B.Max.Y) ? A.Max.Y : B.Max.Y;
    Result.Max.Z = (A.Max.Z > B.Max.Z) ? A.Max.Z : B.Max.Z;

    return (Result);
}

inline rectangle3
Rect3Intersect(rectangle3 A, rectangle3 B)
{
    rectangle3 Result;

    Result.Min.X = (A.Min.X < B.Min.X) ? B.Min.X : A.Min.X;
    Result.Min.Y = (A.Min.Y < B.Min.Y) ? B.Min.Y : A.Min.Y;
    Result.Min.Z = (A.Min.Z < B.Min.Z) ? B.Min.Z : A.Min.Z;

    Result.Max.X = (A.Max.X > B.Max.X) ? B.Max.X : A.Max.X;
    Result.Max.Y = (A.Max.Y > B.Max.Y) ? B.Max.Y : A.Max.Y;
    Result.Max.Z = (A.Max.Z > B.Max.Z) ? B.Max.Z : A.Max.Z;

    return (Result);
}

internal v3
GetMinZCenterP(rectangle3 R)
{
    v3 Result = GetCenter(R);
    Result.Z = V3GetMinCorner(R).Z;
    return (Result);
}

internal v3
GetMaxZCenterP(rectangle3 R)
{
    v3 Result = GetCenter(R);
    Result.Z = V3GetMaxCorner(R).Z;
    return (Result);
}

internal rectangle3
MakeRelative(rectangle3 R, v3 P)
{
    rectangle3 Result = Rect3Offset(R, V3Negated(P));
    return (Result);
}

inline s32
GetWidth(rectangle2i A)
{
    s32 Result = A.Max.X - A.Min.X;
    return (Result);
}

inline s32
GetHeight(rectangle2i A)
{
    s32 Result = A.Max.Y - A.Min.Y;
    return (Result);
}

inline rectangle2i
Intersect(rectangle2i A, rectangle2i B)
{
    rectangle2i Result;

    Result.Min.X = (A.Min.X < B.Min.X) ? B.Min.X : A.Min.X;
    Result.Min.Y = (A.Min.Y < B.Min.Y) ? B.Min.Y : A.Min.Y;
    Result.Max.X = (A.Max.X > B.Max.X) ? B.Max.X : A.Max.X;
    Result.Max.Y = (A.Max.Y > B.Max.Y) ? B.Max.Y : A.Max.Y;

    return (Result);
}

inline rectangle2i
Union(rectangle2i A, rectangle2i B)
{
    rectangle2i Result;

    Result.Min.X = (A.Min.X < B.Min.X) ? A.Min.X : B.Min.X;
    Result.Min.Y = (A.Min.Y < B.Min.Y) ? A.Min.Y : B.Min.Y;
    Result.Max.X = (A.Max.X > B.Max.X) ? A.Max.X : B.Max.X;
    Result.Max.Y = (A.Max.Y > B.Max.Y) ? A.Max.Y : B.Max.Y;

    return (Result);
}

inline int32
GetClampedRectArea(rectangle2i A)
{
    int32 Width = (A.Max.X - A.Min.X);
    int32 Height = (A.Max.Y - A.Min.Y);
    int32 Result = 0;
    if ((Width > 0) && (Height > 0))
    {
        Result = Width * Height;
    }

    return (Result);
}

inline bool32
Rect2HasArea(rectangle2i A)
{
    bool32 Result = ((A.Min.X < A.Max.X) && (A.Min.Y < A.Max.Y));

    return (Result);
}

internal bool32 
Rect2IsOnEdge(rectangle2i Rect, v2s TileIndex)
{
    bool32 Result = ((TileIndex.X == Rect.Min.X) ||
        (TileIndex.X == (Rect.Max.X - 1)) ||
        (TileIndex.Y == Rect.Min.Y) ||
        (TileIndex.Y == (Rect.Max.Y - 1)));
    return (Result);
}

inline xbool32
Rect3HasArea(rectangle3 A)
{
    xbool32 Result = ((A.Min.X < A.Max.X) && (A.Min.Y < A.Max.Y) && (A.Min.Z < A.Max.Z));

    return (Result);
}

inline rectangle2i
InvertedInfinityRectangle2i(void)
{
    rectangle2i Result;

    Result.Min.X = Result.Min.Y = INT_MAX;
    Result.Max.X = Result.Max.Y = -INT_MAX;

    return (Result);
}

inline rectangle2i
Offset(rectangle2i A, s32 X, s32 Y)
{
    rectangle2i Result = A;

    Result.Min.X += X;
    Result.Max.X += X;
    Result.Min.Y += Y;
    Result.Max.Y += Y;

    return (Result);
}

inline rectangle2i
RectMinMax(s32 MinX, s32 MinY, s32 MaxX, s32 MaxY)
{
    rectangle2i Result;

    Result.Min.X = MinX;
    Result.Min.Y = MinY;
    Result.Max.X = MaxX;
    Result.Max.Y = MaxY;

    return (Result);
}

inline rectangle2i
RectMinDim(s32 MinX, s32 MinY, s32 DimX, s32 DimY)
{
    rectangle2i Result;

    Result.Min.X = MinX;
    Result.Min.Y = MinY;
    Result.Max.X = MinX + DimX;
    Result.Max.Y = MinY + DimY;

    return (Result);
}

inline v4
sRGBLinearizeWithV4(v4 C)
{
    v4 Result;

    Result.R = Square(C.R);
    Result.G = Square(C.G);
    Result.B = Square(C.B);
    Result.A = C.A;

    return (Result);
}

inline v4
LinearTosRGB(v4 C)
{
    v4 Result;

    Result.R = SquareRoot(C.R);
    Result.G = SquareRoot(C.G);
    Result.B = SquareRoot(C.B);
    Result.A = C.A;

    return (Result);
}

inline v4
sRGBLinearize(f32 R, f32 G, f32 B, f32 A)
{
    v4 Input = { }; // { R, G, B, A };
    Input.R = R;
    Input.G = G;
    Input.B = B;
    Input.A = A;
    v4 Result = sRGBLinearizeWithV4(Input);
    return (Result);
}

inline v4
SRGB255ToLinear1(v4 C)
{
    v4 Result;

    real32 Inv255 = 1.0f / 255.0f;

    Result.R = Square(Inv255 * C.R);
    Result.G = Square(Inv255 * C.G);
    Result.B = Square(Inv255 * C.B);
    Result.A = Inv255 * C.A;

    return (Result);
}

inline v4
Linear1ToSRGB255(v4 C)
{
    v4 Result;

    real32 One255 = 255.0f;

    Result.R = One255 * SquareRoot(C.R);
    Result.G = One255 * SquareRoot(C.G);
    Result.B = One255 * SquareRoot(C.B);
    Result.A = One255 * C.A;

    return (Result);
}

internal m4x4
M4x4_Multiply(m4x4 A, m4x4 B)
{
    // Not optimal

    m4x4 R = {};

    for (int r = 0; r <= 3; ++r) // Rows (of A)
    {
        for (int c = 0; c <= 3; ++c) // Column (of B)
        {
            for (int i = 0; i <= 3; ++i) // Columns of A, rows of B!
            {
                R.E[r][c] += A.E[r][i] * B.E[i][c];
            }
        }
    }

    return (R);
}

internal v4
Transform(m4x4 A, v4 P)
{
    // This is not optimal.

    v4 R;

    R.X = P.X * A.E[0][0] + P.Y * A.E[0][1] + P.Z * A.E[0][2] + P.W * A.E[0][3];
    R.Y = P.X * A.E[1][0] + P.Y * A.E[1][1] + P.Z * A.E[1][2] + P.W * A.E[1][3];
    R.Z = P.X * A.E[2][0] + P.Y * A.E[2][1] + P.Z * A.E[2][2] + P.W * A.E[2][3];
    R.W = P.X * A.E[3][0] + P.Y * A.E[3][1] + P.Z * A.E[3][2] + P.W * A.E[3][3];

    return (R);
}

func inline v3
M4x4_MultiplyBy_V3(m4x4 A, v3 P)
{
    v3 R = Transform(A, V3ToV4(P, 1.0f)).XYZ;
    return (R);
}

func inline v4
M4x4_MultiplyBy_V4(m4x4 A, v4 P)
{
    v4 R = Transform(A, P);
    return (R);
}

inline m4x4
Identity(void)
{
    m4x4 R =
    {
        {{1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}},
    };

    return (R);
}

inline m4x4
XRotation(f32 Angle)
{
    f32 c = Cos(Angle);
    f32 s = Sin(Angle);

    m4x4 R =
    {
        {{1, 0, 0, 0},
            {0, c,-s, 0},
            {0, s, c, 0},
            {0, 0, 0, 1}},
    };

    return (R);
}

inline m4x4
YRotation(f32 Angle)
{
    f32 c = Cos(Angle);
    f32 s = Sin(Angle);

    m4x4 R =
    {
        {{ c, 0, s, 0},
            { 0, 1, 0, 0},
            {-s, 0, c, 0},
            { 0, 0, 0, 1}},
    };

    return (R);
}

inline m4x4
ZRotation(f32 Angle)
{
    f32 c = Cos(Angle);
    f32 s = Sin(Angle);

    m4x4 R =
    {
        {{c,-s, 0, 0},
            {s, c, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}},
    };

    return (R);
}

inline m4x4
Translation(v3 T)
{
    m4x4 R =
    {
        {{1, 0, 0, T.X},
            {0, 1, 0, T.Y},
            {0, 0, 1, T.Z},
            {0, 0, 0, 1}},
    };

    return (R);
}

inline m4x4
Transpose(m4x4 A)
{
    m4x4 R;

    for (int j = 0; j <= 3; ++j)
    {
        for (int i = 0; i <= 3; ++i)
        {
            R.E[j][i] = A.E[i][j];
        }
    }

    return (R);
}

internal m4x4
Columns3x3(v3 X, v3 Y, v3 Z)
{
    m4x4 R =
    {
        {{X.X, Y.X, Z.X, 0},
            {X.Y, Y.Y, Z.Y, 0},
            {X.Z, Y.Z, Z.Z, 0},
            {  0,   0,   0, 1}}
    };

    return (R);
}

internal m4x4
Rows3x3(v3 X, v3 Y, v3 Z)
{
    m4x4 R =
    {
        {{X.X, X.Y, X.Z, 0},
            {Y.X, Y.Y, Y.Z, 0},
            {Z.X, Z.Y, Z.Z, 0},
            {  0,   0,   0, 1}}
    };

    return (R);
}

internal m4x4
Translate(m4x4 A, v3 T)
{
    m4x4 R = A;

    R.E[0][3] += T.X;
    R.E[1][3] += T.Y;
    R.E[2][3] += T.Z;

    return (R);
}

inline v3
GetColumn(m4x4 A, u32 C)
{
    v3 Result = { }; // { A.E[0][C], A.E[1][C], A.E[2][C] };

    Result.X = A.E[0][C];
    Result.Y = A.E[1][C];
    Result.Z = A.E[2][C];

    return (Result);
}

inline v3
GetRow(m4x4 A, u32 R)
{
    v3 Result = { }; // { A.E[R][0], A.E[R][1], A.E[R][2] };

    Result.X = A.E[R][0];
    Result.Y = A.E[R][1];
    Result.Z = A.E[R][2];

    return (Result);
}

internal v2
RayIntersect2(v2 Pa, v2 ra, v2 Pb, v2 rb)
{
    v2 Result = {};

    /* 
       Pa.x + ta*ra.x = Pb.x + tb*rb.x

       Pa.y + ta*ra.y = Pb.y + tb*rb.y
    */

    f32 d = (rb.X * ra.Y - rb.Y * ra.X);
    if (d != 0.0f)
    {
        f32 ta = ((Pa.X - Pb.X) * rb.Y + (Pb.Y - Pa.Y) * rb.X) / d;
        f32 tb = ((Pa.X - Pb.X) * ra.Y + (Pb.Y - Pa.Y) * ra.X) / d;

        Result = V2(ta, tb);
    }

    return (Result);
}

internal u32
SwapRAndB(u32 C)
{
    u32 Result = ((C & 0xFF00FF00) |
        ((C >> 16) & 0xFF) |
        ((C & 0xFF) << 16));

    return (Result);
}

internal u32
ReplAlpha(u32 C)
{
    u32 Alpha = (C >> 24);
    u32 Result = ((Alpha << 24) |
        (Alpha << 16) |
        (Alpha << 8) |
        (Alpha << 0));

    return (Result);
}

internal u32
MulAlpha(u32 C)
{
    u32 C0 = ((C >> 0) & 0xFF);
    u32 C1 = ((C >> 8) & 0xFF);
    u32 C2 = ((C >> 16) & 0xFF);
    u32 Alpha = (C >> 24);

    // Lossy multiple where you lose a bit.
    C0 = ((C0 * Alpha) >> 8);
    C1 = ((C1 * Alpha) >> 8);
    C2 = ((C2 * Alpha) >> 8);

    u32 Result = ((Alpha << 24) |
        (C2 << 16) |
        (C1 << 8) |
        (C0 << 0));

    return (Result);
}

inline v4
BGRAUnpack4x8(u32 Packed)
{
    v4 Result = { .R = (real32)((Packed >> 16) & 0xFF),
        .G =(real32)((Packed >> 8) & 0xFF),
        .B = (real32)((Packed >> 0) & 0xFF),
        .A = (real32)((Packed >> 24) & 0xFF) };

    return (Result);
}

inline u32
BGRAPack4x8(v4 Unpacked)
{
    u32 Result = ((RoundReal32ToUInt32(Unpacked.A) << 24) |
        (RoundReal32ToUInt32(Unpacked.R) << 16) |
        (RoundReal32ToUInt32(Unpacked.G) << 8) |
        (RoundReal32ToUInt32(Unpacked.B) << 0));

    return (Result);
}

inline u64
BGRAPack4x16(v4 Unpacked)
{
    u64 Result = (((u64)RoundReal32ToUInt32(Unpacked.A) << 48) |
        ((u64)RoundReal32ToUInt32(Unpacked.R) << 32) |
        ((u64)RoundReal32ToUInt32(Unpacked.G) << 16) |
        ((u64)RoundReal32ToUInt32(Unpacked.B) << 0));

    return (Result);
}

inline v4
RGBAUnpack4x8(u32 Packed)
{
    v4 Result = { .R = (real32)((Packed >> 0) & 0xFF),
        .G = (real32)((Packed >> 8) & 0xFF),
        .B = (real32)((Packed >> 16) & 0xFF),
        .A =(real32)((Packed >> 24) & 0xFF) };

    return (Result);
}

inline u32
RGBAPack4x8(v4 Unpacked)
{
    u32 Result = ((RoundReal32ToUInt32(Unpacked.A) << 24) |
        (RoundReal32ToUInt32(Unpacked.B) << 16) |
        (RoundReal32ToUInt32(Unpacked.G) << 8) |
        (RoundReal32ToUInt32(Unpacked.R) << 0));

    return (Result);
}

inline xbool32
IsInRange(f32 Min, f32 Value, f32 Max)
{
    xbool32 Result = ((Min <= Value) &&
        (Value <= Max));

    return (Result);
}

internal rectangle2i
AspectRatioFit(u32 RenderWidth, u32 RenderHeight,
    u32 WindowWidth, u32 WindowHeight)
{
    rectangle2i Result = {};

    if ((RenderWidth > 0) &&
        (RenderHeight > 0) &&
        (WindowWidth > 0) &&
        (WindowHeight > 0))
    {
        r32 OptimalWindowWidth = (r32)WindowHeight * ((r32)RenderWidth / (r32)RenderHeight);
        r32 OptimalWindowHeight = (r32)WindowWidth * ((r32)RenderHeight / (r32)RenderWidth);

        if (OptimalWindowWidth > (r32)WindowWidth)
        {
            // Width-constrained display - top and bottom black bars
            Result.Min.X = 0;
            Result.Max.X = WindowWidth;

            r32 Empty = (r32)WindowHeight - OptimalWindowHeight;
            s32 HalfEmpty = RoundReal32ToInt32(0.5f * Empty);
            s32 UseHeight = RoundReal32ToInt32(OptimalWindowHeight);

            Result.Min.Y = HalfEmpty;
            Result.Max.Y = Result.Min.Y + UseHeight;
        }
        else
        {
            // Height-constrained display - left and right black bars
            Result.Min.Y = 0;
            Result.Max.Y = WindowHeight;

            r32 Empty = (r32)WindowWidth - OptimalWindowWidth;
            s32 HalfEmpty = RoundReal32ToInt32(0.5f * Empty);
            s32 UseWidth = RoundReal32ToInt32(OptimalWindowWidth);

            Result.Min.X = HalfEmpty;
            Result.Max.X = Result.Min.X + UseWidth;
        }
    }

    return (Result);
}

internal v3s
FloorToV3S(v3 A)
{
    v3s Result =
    {
        .X = FloorReal32ToInt32(A.X),
        .Y = FloorReal32ToInt32(A.Y),
        .Z = FloorReal32ToInt32(A.Z),
    };

    return (Result);
}

internal v3s
RoundToV3S(v3 A)
{
    v3s Result =
    {
        .X = RoundReal32ToInt32(A.X),
        .Y = RoundReal32ToInt32(A.Y),
        .Z = RoundReal32ToInt32(A.Z),
    };

    return (Result);
}

internal v3s
V3S(s32 X, s32 Y, s32 Z)
{
    v3s Result = { 
        { 
            .X = X,
            .Y = Y,
            .Z = Z 
        } 
    };

    return (Result);
}

internal v3
V3s_to_V3(v3s A)
{
    v3 Result = {
        {
            .X = (f32)A.X,
            .Y = (f32)A.Y,
            .Z = (f32)A.Z,
        }
    };

    return (Result);
}

internal v3s
V3sSubstract(v3s A, v3s B)
{
    v3s Result =
    { 
        {
            .X = A.X - B.X,
            .Y = A.Y - B.Y,
            .Z = A.Z - B.Z,
        }
    };

    return (Result);
}

internal v2s
V2sSubtractV2s(v2s A, v2s B)
{
	v2s Result = {
        {
            .X = A.X - B.X,
            .Y = A.Y - B.Y,
        }
    };

	return (Result);
}

internal v3s
V3sAddV3s(v3s A, v3s B)
{
    v3s Result = {
        {
            .X = A.X + B.X,
            .Y = A.Y + B.Y,
            .Z = A.Z + B.Z,
        }
    };

    return (Result);
}

internal v3s
V3sAssignAddV3s(v3s *A, v3s B)
{
    *A = V3sAddV3s(*A, B);
    return (*A);
}

internal v3s
V3sMultiplyS32(v3s B, s32 A)
{
    v3s Result = {
        {
            .X = A * B.X,
            .Y = A * B.Y,
            .Z = A * B.Z,
        }
    };

    return (Result);
}

internal v3s
V3sDivideByS32(v3s A, s32 B)
{
    v3s Result = {
        {
            .X = A.X / B,
            .Y = A.Y / B,
            .Z = A.Z / B,
        }
    };

    return (Result);
}

internal v3s
Clamp(v3s Min, v3s V, v3s Max)
{
    v3s Result = {
        {
            .X = ClampS32(Min.X, V.X, Max.X),
            .Y = ClampS32(Min.Y, V.Y, Max.Y),
            .Z = ClampS32(Min.Z, V.Z, Max.Z),
        }
    };

    return (Result);
}

internal v3
GetClosestPointInBox(rectangle3 Box, v3 P)
{
    v3 Result = V3Min(Box.Max, V3Max(Box.Min, P));
    return (Result);
}

internal f32
GetDistanceToBoxSq(rectangle3 Box, v3 P)
{
    v3 ClosestP = GetClosestPointInBox(Box, P);
    f32 DistanceSq = V3LengthSq(V3SubtractV3(P, ClosestP));
    return (DistanceSq);
}

internal v2
OctahedralFromUnitVector(v3 V)
{
    f32 OneNorm = AbsoluteValue(V.X) + AbsoluteValue(V.Y) + AbsoluteValue(V.Z);
    v2 Result = Real32MultiplyV2((1.0f / OneNorm), V.XY);

    if (V.Z < 0)
    {
        f32 Ox = SignOf(Result.X) * (1.0f - AbsoluteValue(Result.Y));
        f32 Oy = SignOf(Result.Y) * (1.0f - AbsoluteValue(Result.X));
        Result.X = Ox;
        Result.Y = Oy;
    }

    return (Result);
}

internal v3
UnitVectorFromOctahedral(v2 O)
{
    f32 Ox = O.X;
    f32 Oy = O.Y;
    f32 SumXY = (AbsoluteValue(Ox) + AbsoluteValue(Oy));
    f32 Oz = 1.0f - SumXY;
    if (SumXY > 1)
    {
        Ox = SignOf(O.X) * (1.0f - AbsoluteValue(O.Y));
        Oy = SignOf(O.Y) * (1.0f - AbsoluteValue(O.X));
    }

    v3 Result = V3(Ox, Oy, Oz);
    Result = V3Normalize(Result);

    return (Result);
}

internal v3
DirectionFromTxTy(v2 OxyCoefficient, u32 Tx, u32 Ty)
{
    v2 Oxy = {
        {
            .X = OxyCoefficient.X * (f32)(Tx - 1),
            .Y = OxyCoefficient.Y * (f32)(Ty - 1),
        }
    };
    Oxy = V2MinusV2(Real32MultiplyV2(2.0f, Oxy), V2(1.0f, 1.0f));
    //Oxy += (1.0f / (f32)(LIGHT_COLOR_LOOKUP_SQUARE_DIM - 2))*V2(1,1);
    v3 SampleDir = UnitVectorFromOctahedral(Oxy);

    return (SampleDir);
}

internal v2u
GetOctahedralOffset(v2 OctDimCoefficient, v3 Dir)
{
    v2 UV = OctahedralFromUnitVector(Dir);
    UV = Real32MultiplyV2(0.5f, 
        (V2AddV2(
            V2(1.0f, 1.0f), UV
        ))
    );

    v2 I = V2AddV2(V2HadamardV2(OctDimCoefficient, UV), V2(1.5f, 1.5f));
    v2u Result = {
        {
            (u32)FloorReal32ToInt32(I.X),
            (u32)FloorReal32ToInt32(I.Y),
        }
    };
    return (Result);
}

internal xbool32
V3sAreEqual(v3s A, v3s B)
{
    xbool32 Result = ((A.X == B.X) &&
        (A.Y == B.Y) &&
        (A.Z == B.Z));

    return (Result);
}

func bool32 V2sAreEqual(v2s A, v2s B)
{
    bool32 Result = ((A.X == B.X) &&
        (A.Y == B.Y));
    return (Result);
}

internal xbool32
V2uAreEqual(v2u A, v2u B)
{
    xbool32 Result = ((A.X == B.X) &&
        (A.Y == B.Y));


    return (Result);
}

internal f32 DistanceBetweenLineSegmentAndPointSq(v2 L0, v2 L1, v2 P)
{
    v2 Delta = V2MinusV2(L1, L0);
    v2 RelP = V2MinusV2(P, L0);

    f32 DeltaLen = V2Length(Delta);
    v2 Dir = Real32MultiplyV2((1.0f / DeltaLen), Delta);
    f32 t = ClampReal32(0.0f, V2InnerV2(Dir, RelP), DeltaLen);

    v2 ClosestP = V2AddV2(L0, Real32MultiplyV2(t, Dir));
    f32 Result = V2LengthSq(V2MinusV2(ClosestP, P));

    return (Result);
}
