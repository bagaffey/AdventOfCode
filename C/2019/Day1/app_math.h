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