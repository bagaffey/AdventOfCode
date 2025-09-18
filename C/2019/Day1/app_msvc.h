// Use this to avoid linking with the C runtime library when compiling with MSVC.
int _fltused = 0x9875; // This sets a floating point global symbol that MSVC uses when FP is used in a program.

// This sets our own implementation of memset which at times gets called by the CRT for basically initialization steps.
#pragma function(memset)
void* memset(void* _Dst, int _Val, size_t _Size)
{
    unsigned char Val = *(unsigned char*)&_Val;
    unsigned char* At = (unsigned char*)_Dst;
    while (_Size--)
    {
        *At++ = Val;
    }
    return(_Dst);
}
