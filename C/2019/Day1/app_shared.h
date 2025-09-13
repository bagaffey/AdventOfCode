typedef struct adler_32
{
    u32 S1;
    u32 S2;
} adler_32;

#define StringZ(Z) {sizeof(Z) - 1, (u8 *)(Z)}
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

