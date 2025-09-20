
// forward-declaration
struct memory_arena;

typedef struct stream_chunk stream_chunk;
struct stream_chunk
{
	buffer Contents;
	stream_chunk* Next;
#if PROJ_INTERNAL
	char* GUID;
#endif
};

typedef struct stream stream;
struct stream
{
	memory_arena* Memory;
	stream* Errors;
	buffer Contents;

	u32 BitCount;
	u32 BitBuff;
	bool32 Underflowed;

	stream_chunk* First;
	stream_chunk* Last;
};

#if APP_INTERNAL
#define Outf(...) Outf_(DEBUG_VALUE("Outf"), ## __VA_ARGS__)
#else
#define Outf(...) Outf_(__VA_ARGS__)
#endif

internal umw OutfArgList(INTERNAL_MEMORY_PARAM stream* Dest, char* Format, va_list ArgList);
internal umw Outf_(INTERNAL_MEMORY_PARAM stream* Dest, char* Format, ...);

#define OutStruct(Stream, Type) (type *)OutSize(Stream, sizeof(type));
#define OutStructCopy(Stream, Instance) OutCopy(Stream, sizeof(Instance), &(Instance))