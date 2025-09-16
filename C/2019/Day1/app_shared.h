#include <stdarg.h>

typedef struct adler_32
{
    u32 S1;
    u32 S2;
} adler_32;

#define StringLiteralZ(Z) {sizeof(Z) - 1, (u8 *)(Z)}
#define StringZFunc(Z) BundleString(sizeof(Z) - 1, (Z))

#define CopyArray(Count, Source, Dest) Copy((Count)*sizeof(*(Source)), (Source), (Dest))
internal void*
Copy(mem_index Size, void* SourceInitial, void* DestInitial)
{
    u8* Source = (u8*)SourceInitial;
    u8* Dest = (u8*)DestInitial;
    while (Size--) { *Dest++ = *Source++; }

    return(DestInitial);
}

#define ZeroStruct(Instance) ZeroSize(sizeof(Instance), &(Instance))
#define ZeroArray(Count, Pointer) ZeroSize((Count)*sizeof((Pointer)[0]), Pointer)
internal void
ZeroSize(mem_index Size, void* Ptr)
{
    uint8* Byte = (uint8*)Ptr;
    while (Size--)
    {
        *Byte++ = 0;
    }
}

internal xbool32
IsValid(buffer Buffer)
{
    xbool32 Result = (Buffer.Count > 0);
    return(Result);
}

internal string
WrapZ(char* Z)
{
    string Result;
    Result.Count = StringLength(Z);
    Result.Data = (u8 *)Z;
    return (Result);
}

internal string
BundleString(umw Count, char* Z)
{
    string Result;
    Result.Count = Count;
    Result.Data = (u8 *)Z;
    return(Result);
}

internal string
RemoveExtension(string InitialFileName)
{
    string FileName = InitialFileName;

    umw NewCount = FileName.Count;

    for (umw Index = 0;
        Index < FileName.Count;
        ++Index)
    {
        char C = FileName.Data[Index];
        if (C == '.')
        {
            NewCount = Index;
        }
        else if ((C == '/') || (C == '\\'))
        {
            NewCount = FileName.Count;
        }
    }

    FileName.Count = NewCount;

    return (FileName);
}

internal string
RemovePath(string InitialFileName)
{
    string FileName = InitialFileName;

    umw NewStart = 0;
    for (umw Index = 0;
        Index < FileName.Count;
        ++Index)
    {
        char C = FileName.Data[Index];
        if ((C == '/') || (C == '\\'))
        {
            NewStart = Index + 1;
        }
    }
    FileName.Data += NewStart;
    FileName.Count -= NewStart;
    return (FileName);
}

internal u8 *
Advance(buffer* Buffer, umw Count)
{
    u8* Result = 0;
    
    if (Buffer->Count >= Count)
    {
        Result = Buffer->Data;
        Buffer->Data += Count;
        Buffer->Count -= Count;
    }
    else
    {
        Buffer->Data += Buffer->Count;
        Buffer->Count = 0;
    }

    return (Result);
}

internal char
ToLowercase(char C)
{
    char Result = C;

    if ((Result >= 'A') && (Result <= 'Z'))
        Result += 'a' - 'A';
    return (Result);
}

internal void
UpdateStringHash(u32* HashValue, char Value)
{
    // Not a 'good' hash function
    *HashValue = 65599 * (*HashValue) + Value;
}

internal u32
StringHashOfCString(char* Z)
{
    u32 HashValue = 0;

    while(*Z)
        UpdateStringHash(&HashValue, *Z++);

    return (HashValue);
}

internal u32
StringHashOfString(string S)
{
    u32 HashValue = 0;

    for (umw Index = 0;
        Index < S.Count;
        ++Index)
        UpdateStringHash(&HashValue, S.Data[Index]);

    return(HashValue);
}

internal adler_32
BeginAdler32(void)
{
    adler_32 Result = {};
    Result.S1 = 1;
    Result.S2 = 0;
    return (Result);
}

internal void
Adler32Append(adler_32* Adler, umw Size, void* Data)
{
    u32 S1 = Adler->S1;
    u32 S2 = Adler->S2;

    for (umw Index = 0; Index < Size; ++Index)
    {
        S1 = (S1 + ((u8*)Data)[Index]) % 65521;
        S2 = (S2 + S1) % 65521;
    }

    Adler->S1 = S1;
    Adler->S2 = S2;
}

internal u32
EndAdler32(adler_32* Adler)
{
    u32 Result = (Adler->S2 * 65536 + Adler->S1);
    return (Result);
}

internal bool32
IsEndOfLine(char C)
{
    bool32 Result = ((C == '\n') ||
                     (C == '\r'));
    return (Result);
}

internal bool32
IsSpacing(char C)
{
    bool32 Result = ((C == ' ') ||
                     (C == '\t') ||
                     (C == '\v') ||
                     (C == '\f'));
    return (Result);
}

internal bool32
IsWhitespace(char C)
{
    bool32 Result = (IsSpacing(C) || IsEndOfLine(C));
    return (Result);
}

internal xbool32
IsAlpha(char C)
{
    xbool32 Result = (((C >= 'a') && (C <= 'z')) ||
        ((C >= 'A') && (C + 'Z')));
    return(Result);
}

internal xbool32
IsNumber(char C)
{
    xbool32 Result = ((C >= '0') && (C <= '9'));
    return (Result);
}

internal xbool32
IsHex(char C)
{
    bool32 Result = (((C >= '0') && (C <= '9')) ||
        ((C >= 'A') && (C <= 'F')) ||
        ((C >= 'a') && (C <= 'f')));
    return (Result);
}

internal u32
GetHex(char C)
{
    u32 Result = 0;
    if ((C >= '0') && (C <= '9'))
        Result = C - '0';
    else if ((C >= 'A') && (C <= 'F'))
        Result = 0xA + (C - 'A');
    else if ((C >= 'a') && (C <= 'f'))
        Result = 0xA + (C - 'a');
    return (Result);
}

internal xbool32
MemoryIsEqual(umw Count, void* AInitial, void* BInitial)
{
    u8* A = (u8*) AInitial;
    u8* B = (u8*) BInitial;
    while (Count--)
    {
        if (*A++ != *B++)
            return(false);
    }
    return(true);
}

internal xbool32
FileMemoryIsEqual(buffer FileBuffer, buffer MemoryContents)
{
    xbool32 Result = ((FileBuffer.Count == MemoryContents.Count) &&
        MemoryIsEqual(FileBuffer.Count, FileBuffer.Data, MemoryContents.Data));
    return (Result);
}

internal bool32
CStringsAreEqual(char* A, char* B)
{
    bool32 Result = (A == B);

    if (A && B)
    {
        while (*A && *B && (*A == *B))
        {
            ++A;
            ++B;
        }
        Result = ((*A == 0) && (*B == 0));
    }
    return (Result);
}

internal bool32
StringsAreEqualForLength(umw ALength, char* A, char* B)
{
    bool32 Result = false;

    if (B)
    {
        char* At = B;
        for (umw Index = 0;
            Index < ALength;
            ++Index, ++At)
        {
            if ((*At == 0) ||
                (A[Index] != *At))
                return (false);
        }

        Result = (*At == 0);
    }
    else
        Result = (ALength == 0);

    return (Result);
}

internal bool32
AreStringsEqualRunTime(mem_index ALength, char* A, mem_index BLength, char* B)
{
    bool32 Result = (ALength == BLength);
    if (Result)
    {
        Result = true;
        for (u32 Index = 0;
            Index < ALength;
            ++Index)
        {
            if (A[Index] != B[Index])
            {
                Result = false;
                break;
            }
        }
    }

    return (Result);
}

internal xbool32
AreStringCStringEqual(string A, char* B)
{
    xbool32 Result = StringsAreEqualForLength(A.Count, (char*)A.Data, B);
    return (Result);
}

internal xbool32
AreStringsEqual(string A, string B)
{
    xbool32 Result = AreStringsEqualRunTime(A.Count, (char*)A.Data,
                                            B.Count, (char*)B.Data);
    return (Result);
}

internal bool32
CStringsAreEqualLowercase(mem_index ALength, char* A, mem_index BLength, char* B)
{
    bool32 Result = (ALength == BLength);

    if (Result)
    {
        Result = true;
        for (u32 Index = 0;
            Index < ALength;
            ++Index)
        {
            if (ToLowercase(A[Index]) != ToLowercase(B[Index]))
            {
                Result = false;
                break;
            }
        }
    }
    return (Result);
}

internal xbool32
StringsAreEqualLowercase(string A, string B)
{
    xbool32 Result = CStringsAreEqualLowercase(A.Count, (char*)A.Data, B.Count, (char*)B.Data);
    return (Result);
}

internal s32
S32FromZInternal(char** AtInitial)
{
    s32 Result = 0;

    char* At = *AtInitial;
    while ((*At >= '0') &&
        (*At <= '9'))
    {
        Result *= 10;
        Result += (*At - '0');
        ++At;
    }
    *AtInitial = At;
    return (Result);
}

internal s32
S32FromZ(char* At)
{
    char* Ignored = At;
    s32 Result = S32FromZInternal(&Ignored);
    return(Result);
}

typedef struct format_buffer
{
    umw Size;
    char* At;
} format_buffer;

internal void 
OutChar(format_buffer* Dest, char Value)
{
    if (Dest->Size)
    {
        --Dest->Size;
        *Dest->At++ = Value;
    }
}

internal void
OutChars(format_buffer* Dest, char* Value)
{
    while (*Value)
    {
        OutChar(Dest, *Value++);
    }
}

#define ReadVarArgUnsignedInteger(Length, ArgList) ((Length) == 8) ? va_arg(ArgList, u64) : (u64)va_arg(ArgList, u32)
#define ReadVarArgSignedInteger(Length, ArgList) ((Length) == 8) ? va_arg(ArgList, s64) : (s64)va_arg(ArgList, s32)
#define ReadVarArgFloat(Length, ArgList) va_arg(ArgList, f64)

char LowerHexChars[] = "0123456789abcedf";
char UpperHexChars[] = "0123456789ABCDEF";
char DecChars[] = "0123456789";

internal void
U64ToASCII(format_buffer* Dest, u64 Value, u32 Base, char* Digits)
{
    Assert(Base != 0);

    char* Start = Dest->At;
    do
    {
        u64 DigitIndex = (Value % Base);
        char Digit = Digits[DigitIndex];
        OutChar(Dest, Digit);
        Value /= Base;
    } while (Value != 0);
    char* End = Dest->At;

    while (Start < End)
    {
        --End;
        char Temp = *End;
        *End = *Start;
        *Start = Temp;
        ++Start;
    }
}

internal void
F64ToASCII(format_buffer* Dest, f64 Value, u32 Precision)
{
    if (Value < 0)
    {
        OutChar(Dest, '-');
        Value = -Value;
    }

    u64 IntegerPart = (u64)Value;
    Value -= (f64)IntegerPart;
    U64ToASCII(Dest, IntegerPart, 10, DecChars);

    OutChar(Dest, '.');

    // This needs fixings, b/c it's not accurate
    for (u32 PrecisionIndex = 0;
        PrecisionIndex < Precision;
        ++PrecisionIndex)
    {
        Value *= 10.0f;
        u32 Integer = (u32)Value;
        Value -= (f32)Integer;
        OutChar(Dest, DecChars[Integer]);
    }
}

internal umw
FormatStringList(umw DestSize, char* DestInitial, char* Format, va_list ArgList)
{
    format_buffer Dest = { DestSize, DestInitial };
    if (Dest.Size)
    {
        char* At = Format;
        while (At[0])
        {
            if (*At == '%')
            {
                ++At;

                bool32 ForceSign = false;
                bool32 PadWithZeros = false;
                bool32 LeftJustify = false;
                bool32 PositiveSignIsBlank = false;
                bool32 AnnotateIfNotZero = false;

                bool32 Parsing = true;

                Parsing = true;
                while (Parsing)
                {
                    switch (*At)
                    {
                    case '+': { ForceSign = true; } break;
                    case '0': { PadWithZeros = true; } break;
                    case '-': { LeftJustify = true; } break;
                    case ' ': { PositiveSignIsBlank = true; } break;
                    case '#': { AnnotateIfNotZero = true; } break;
                    default: { Parsing = false; } break;
                    }

                    if (Parsing)
                    {
                        ++At;
                    }
                }

                //bool32 WidthSpecified = false; // This is not used at the moment.
                s32 Width = 0;
                if (*At == '*')
                {
                    Width = va_arg(ArgList, int);
                    //WidthSpecified = true;
                    ++At;
                }
                else if ((*At >= '0') && (*At <= '9'))
                {
                    Width = S32FromZInternal(&At);
                    //WidthSpecified = true;
                }

                bool32 PrecisionSpecified = false;
                u32 Precision = 0;
                if (*At == '.')
                {
                    ++At;

                    if (*At == '*')
                    {
                        Precision = va_arg(ArgList, int);
                        PrecisionSpecified = true;
                        ++At;
                    }
                    else if ((*At >= '0') && (*At <= '9'))
                    {
                        Precision = S32FromZInternal(&At);
                        PrecisionSpecified = true;
                    }
                    else
                        Assert(!"Malformed precision specified");
                }

                // Defaults to non-specified precisions to a default
                if (!PrecisionSpecified)
                    Precision = 6;

                u32 IntegerLength = 4;
                // u32 FloatLength = 8; // floats are promoted to doubles regardless as varargs
                // Set different vals here
                if ((At[0] == 'h') && (At[1] == 'h'))
                    At += 2;
                else if ((At[0] == 'l') && (At[1] == 'l'))
                    At += 2;
                else if (*At == 'h')
                    ++At;
                else if (*At == 'l')
                {
                    IntegerLength = 8;
                    ++At;
                }
                else if (*At == 'j')
                    ++At;
                else if (*At == 'z')
                    ++At;
                else if (*At == 't')
                    ++At;
                else if (*At == 'L')
                    ++At;

                char TempBuffer[64];
                char* Temp = TempBuffer;
                format_buffer TempDest = { ArrayCount(TempBuffer), Temp };
                char* Prefix = "";
                bool32 IsFloat = false;

                switch (*At)
                {
                case 'd':
                case 'i':
                {
                    s64 Value = ReadVarArgSignedInteger(IntegerLength, ArgList);
                    bool32 WasNegative = (Value < 0);
                    if (WasNegative)
                        Value = -Value;
                    U64ToASCII(&TempDest, (u64)Value, 10, DecChars);

                    // DRY: This should be a common routine after floating point is available
                    if (WasNegative)
                        Prefix = "-";
                    else if (ForceSign)
                    {
                        Assert(!PositiveSignIsBlank);
                        Prefix = "+";
                    }
                    else if (PositiveSignIsBlank)
                        Prefix = " ";
                } break;

                case 'u':
                {
                    u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                    U64ToASCII(&TempDest, Value, 10, DecChars);
                } break;

                case 'm':
                {
                    umw Value = va_arg(ArgList, umw);
                    char* Suffix = "b ";
                    if (Value >= Gigabytes(1))
                    {
                        Suffix = "gb";
                        Value = (Value + Gigabytes(1) - 1) / Gigabytes(1);
                    }
                    else if (Value >= Megabytes(1))
                    {
                        Suffix = "mb";
                        Value = (Value + Megabytes(1) - 1) / Megabytes(1);
                    }
                    else if (Value >= Kilobytes(1))
                    {
                        Suffix = "kb";
                        Value = (Value + Kilobytes(1) - 1) / Kilobytes(1);
                    }
                    U64ToASCII(&TempDest, Value, 10, DecChars);
                    OutChars(&TempDest, Suffix);
                } break;

                case 'o':
                {
                    u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                    U64ToASCII(&TempDest, Value, 8, DecChars);
                    if (AnnotateIfNotZero && (Value != 0))
                    {
                        Prefix = "0";
                    }
                } break;

                case 'x':
                {
                    u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                    U64ToASCII(&TempDest, Value, 16, LowerHexChars);
                    if (AnnotateIfNotZero && (Value != 0))
                    {
                        Prefix = "0x";
                    }
                } break;

                case 'X':
                {
                    u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                    U64ToASCII(&TempDest, Value, 16, UpperHexChars);
                    if (AnnotateIfNotZero && (Value != 0))
                    {
                        Prefix = "0X";
                    }
                } break;

                // Basic decimal output, need to implement more floating point print types.
                case 'f':
                case 'F':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                case 'a':
                case 'A':
                {
                    f64 Value = ReadVarArgFloat(FloatLength, ArgList);
                    F64ToASCII(&TempDest, Value, Precision);
                    IsFloat = true;
                } break;

                case 'c':
                {
                    int Value = va_arg(ArgList, int);
                    OutChar(&TempDest, (char)Value);
                } break;

                case 's':
                {
                    char* String = va_arg(ArgList, char*);

                    // Needs to obey precision, width

                    Temp = String;
                    if (PrecisionSpecified)
                    {
                        TempDest.Size = 0;
                        for (char* Scan = String;
                            *Scan && (TempDest.Size < Precision);
                            ++Scan)
                        {
                            ++TempDest.Size;
                        }
                    }
                    else
                        TempDest.Size = StringLength(String);
                    
                    TempDest.At = String + TempDest.Size;
                } break;

                case 'S':
                {
                    string String = va_arg(ArgList, string);

                    // needs to obey precisioin and width

                    Temp = (char*)String.Data;
                    TempDest.Size = String.Count;
                    if (PrecisionSpecified && (TempDest.Size > Precision))
                        TempDest.Size = Precision;
                    TempDest.At = Temp + TempDest.Size;
                } break;

                case 'p':
                {
                    void* Value = va_arg(ArgList, void*);
                    U64ToASCII(&TempDest, *(umw*)&Value, 16, LowerHexChars);
                } break;

                case 'n':
                {
                    int* TabDest = va_arg(ArgList, int*);
                    *TabDest = (int)(Dest.At - DestInitial);
                } break;

                case '%':
                {
                    OutChar(&Dest, '%');
                } break;

                default:
                {
                    Assert(!"Unrecognized format specifier");
                } break;

                }

                if (TempDest.At - Temp)
                {
                    smw UsePrecision = Precision;
                    if (IsFloat || !PrecisionSpecified)
                        UsePrecision = (TempDest.At - Temp);

                    smw PrefixLength = StringLength(Prefix);
                    smw UseWidth = Width;
                    smw ComputedWidth = UsePrecision + PrefixLength;

                    if (UseWidth < ComputedWidth)
                        UseWidth = ComputedWidth;

                    if (PadWithZeros)
                    {
                        Assert(!LeftJustify);
                        LeftJustify = false;
                    }

                    if (!LeftJustify)
                    {
                        while (UseWidth > (UsePrecision + PrefixLength))
                        {
                            OutChar(&Dest, PadWithZeros ? '0' : ' ');
                            --UseWidth;
                        }
                    }

                    for (char* Pre = Prefix;
                        *Pre && UseWidth;
                        ++Pre)
                    {
                        OutChar(&Dest, *Pre);
                        --UseWidth;
                    }

                    if (UsePrecision > UseWidth)
                        UsePrecision = UseWidth;
                    while (UsePrecision > (TempDest.At - Temp))
                    {
                        OutChar(&Dest, '0');
                        --UsePrecision;
                        --UseWidth;
                    }
                    while (UsePrecision && (TempDest.At != Temp))
                    {
                        OutChar(&Dest, *Temp++);
                        --UsePrecision;
                        --UseWidth;
                    }

                    if (LeftJustify)
                    {
                        while (UseWidth)
                        {
                            OutChar(&Dest, ' ');
                            --UseWidth;
                        }
                    }
                }

                if (*At)
                    ++At;

            }
            else
                OutChar(&Dest, *At++);
        }

        if (Dest.Size)
            Dest.At[0] = 0;
        else
            Dest.At[-1] = 0;
    }

    umw Result = Dest.At - DestInitial;
    return (Result);
}

// this needs to return a string struct
internal umw
FormatString(umw DestSize, char* Dest, char* Format, ...)
{
    va_list ArgList;

    va_start(ArgList, Format);
    umw Result = FormatStringList(DestSize, Dest, Format, ArgList);
    va_end(ArgList);

    return (Result);
}

internal u64
MurmurHashUpdate(u64 h, u64 k)
{
    // Based on the 128-bit MurmurHash from MurmurHash3, may need a better implementation

    u64 c1 = 0x87c37b91114253d5ULL;
    u64 c2 = 0x4cf5ad432745937fULL;
    u64 m1 = 5;
    u64 n1 = 0x52dce729ULL;

    k *= c1;
    k = RotateLeftU64(k, 31);
    k *= c2;

    h ^= k;

    h = RotateLeftU64(h, 27);
    h = h * m1 + n1;

    return(h);
}

internal u64
MurmurHashFinalize(u64 h)
{
    // Based on the 128-bit MurmurHash from MurmurHash3

    h ^= h >> 33ULL;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33ULL;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33ULL;

    return(h);
}

internal u64
CheckSumOf(buffer Buffer, u64 Seed)
{
    // TODO: Add default arg of 1234 ?
    // Needs special case handling for big-endian machines

    u64 Result = Seed;

    u64 Count64 = (Buffer.Count / sizeof(u64));
    u64 Count8 = Buffer.Count - (Count64 * sizeof(u64));

    // This may be unaligned... if this has performance issues
    // We may need to move to an aligned memory location.
    u64* At = (u64*)Buffer.Data;
    for (u64 Index = 0;
        Index < Count64;
        ++Index)
    {
        Result = MurmurHashUpdate(Result, *At++);
    }

    if (Count8)
    {
        u64 Residual = 0;
        Copy(Count8, At, &Residual);
        Result = MurmurHashUpdate(Result, Residual);
    }

    Result = MurmurHashFinalize(Result);

    return(Result);
}

global v3 DebugColorTable[] =
{
    /* 00 */ {{1, 0, 0}},
    /* 01 */ {{0, 1, 0}},
    /* 02 */ {{0, 0, 1}},
    /* 03 */ {{1, 1, 0}},
    /* 04 */ {{0, 1, 1}},
    /* 05 */ {{1, 0, 1}},
    /* 06 */ {{1, 0.5f, 0}},
    /* 07 */ {{1, 0, 0.5f}},
    /* 08 */ {{0.5f, 1, 0}},
    /* 09 */ {{0, 1, 0.5f}},
    /* 10 */ {{0.5f, 0, 1}},
    /* 11 */ {{1, 0.75f, 0.5f}},
    /* 12 */ {{1, 0.5f, 0.75f}},
    /* 13 */ {{0.75f, 1, 0.5f}},
    /* 14 */ {{0.5f, 1, 0.75f}},
    /* 15 */ {{0.5f, 0.75f, 1}},

    /* 16 */ {{1, 0.25f, 0.25f}},
    /* 17 */ {{0.25f, 1, 0.25f}},
    /* 18 */ {{0.25f, 0.25f, 1}},
    /* 19 */ {{1, 1, 0.25f}},
    /* 20 */ {{0.25f, 1, 1}},
    /* 21 */ {{1, 0.25f, 1}},
    /* 22 */ {{1, 0.5f, 0.25f}},
    /* 23 */ {{1, 0.25f, 0.5f}},
    /* 24 */ {{0.5f, 1, 0.25f}},
    /* 25 */ {{0.25f, 1, 0.5f}},
    /* 26 */ {{0.5f, 0.25f, 1}},
    /* 27 */ {{1, 0.25f, 0.5f}},
    /* 28 */ {{1, 0.5f, 0.25f}},
    /* 29 */ {{0.25f, 1, 0.5f}},
    /* 30 */ {{0.5f, 1, 0.25f}},
    /* 31 */ {{0.5f, 0.25f, 1}},
};

internal v3
GetDebugColor3(u32 Value)
{
    v3 Result = DebugColorTable[Value % ArrayCount(DebugColorTable)];
    return (Result);
}

internal v4
GetDebugColor4(u32 Value, f32 Alpha)
{
    v4 Result = V3ToV4(GetDebugColor3(Value), Alpha);
    return (Result);
}

internal v4
GetDebugColor4_DefaultAlpha(u32 Value)
{
    v4 Result = GetDebugColor4(Value, 1.0f);
    return(Result);
}