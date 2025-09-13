typedef struct adler_32
{
    u32 S1;
    u32 S2;
} adler_32;

#define StringLiteralZ(Z) {sizeof(Z) - 1, (u8 *)(Z)}
#define StringZFunc(Z) BundleString(sizeof(Z) - 1, (Z))

#define CopyArray(Count, Source, Dest) Copy((Count)*sizeof(*(Source)), (Source), (Dest))
internal void*
Copy(mem_index Size, void* SourceInit, void* DestInit)
{
    u8* Source = (u8*)SourceInit;
    u8* Dest = (u8*)DestInit;
    while (Size--) { *Dest++ = *Source++; }

    return(DestInit);
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