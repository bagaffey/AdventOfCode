#define SQUARE(a) ((a)*(a))
#define CUBE(a) ((a)*(a)*(a))

inline v2u
V2U(u32 X, u32 Y)
{
    v2u Result = { {X, Y} };
    return(Result);
}

inline v2s
V2S(s32 X, s32 Y)
{
    v2s Result = { {X, Y} };
    return(Result);
}

#define V2i(X,Y) _Generic(((X)+(Y)), \
    int32    : V2is, \
    uint32   : V2iu \
)(X,Y)

inline v2
V2is(int32 X, int32 Y)
{
    v2 Result = { { (real32)X, (real32)Y } };
    return(Result);
}

inline v2
V2iu(uint32 X, uint32 Y)
{
    v2 Result = { { (real32)X, (real32)Y } };

    return(Result);
}

inline v2
Real32sToV2(real32 X, real32 Y)
{
    v2 Result;

    Result.X = X;
    Result.Y = Y;

    return(Result);
}

inline v2
V2sToV2(v2s A)
{
    v2 Result;

    Result.X = (f32)A.X;
    Result.Y = (f32)A.Y;

    return(Result);
}

inline v2
SignedV2From(v2u Source)
{
    v2 Result = { { (f32)Source.X, (f32)Source.Y } };
    return(Result);
}

inline v3
V3(real32 X, real32 Y, real32 Z)
{
    v3 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;

    return(Result);
}

inline v3
V2ToV3(v2 XY, real32 Z)
{
    v3 Result;

    Result.X = XY.X;
    Result.Y = XY.Y;
    Result.Z = Z;

    return(Result);
}

inline v4
V4(real32 X, real32 Y, real32 Z, real32 W)
{
    v4 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;

    return(Result);
}

inline v4
V3ToV4(v3 XYZ, real32 W)
{
    v4 Result;

    Result.XYZ = XYZ;
    Result.W = W;

    return(Result);
}