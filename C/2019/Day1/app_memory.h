
typedef struct memory_arena
{
	platform_memory_block* CurrentBlock;
	umw MinimumBlockSize;

	u64 AllocationFlags;
	s32 TempCount;
} memory_arena;

typedef struct temporary_memory
{
	memory_arena* Arena;
	platform_memory_block* Block;
	umw Used;
} temporary_memory;

inline void 
SetMinimumBlockSize(memory_arena* Arena, mem_index MinimumBlockSize)
{
	Arena->MinimumBlockSize = MinimumBlockSize;
}

inline mem_index
GetAlignmentOffset(memory_arena* Arena, mem_index Alignment)
{
	mem_index AlignmentOffset = 0;

	mem_index ResultPointer = (mem_index)Arena->CurrentBlock->Base +
		Arena->CurrentBlock->Used;
	mem_index AlignmentMask = Alignment - 1;
	if (ResultPointer & AlignmentMask)
	{
		AlignmentOffset = Alignment - (ResultPointer & AlignmentMask);
	}

	return(AlignmentOffset);
}

enum arena_push_flag
{
	ArenaFlag_ClearToZero = 0x1,
};

typedef struct arena_push_params
{
	u32 Flags;
	u32 Alignment;
} arena_push_params;

inline arena_push_params
DefaultArenaParams(void)
{
	arena_push_params Params;
	Params.Flags = ArenaFlag_ClearToZero;
	Params.Alignment = 4;
	return(Params);
}

inline arena_push_params
AlignNoClear(u32 Alignment)
{
	arena_push_params Params = DefaultArenaParams();
	Params.Flags &= ~ArenaFlag_ClearToZero;
	Params.Alignment = Alignment;
	return(Params);
}

inline arena_push_params
Align(u32 Alignment, bool32 Clear)
{
	arena_push_params Params = DefaultArenaParams();
	if (Clear)
		Params.Flags |= ArenaFlag_ClearToZero;
	else
		Params.Flags &= ~ArenaFlag_ClearToZero;
	Params.Alignment = Alignment;
	return(Params);
}

inline arena_push_params
NoClear(void)
{
	arena_push_params Params = DefaultArenaParams();
	Params.Flags &= ~ArenaFlag_ClearToZero;
	return(Params);
}

typedef struct arena_bootstrap_params
{
	u64 AllocationFlags;
	umw MinimumBlockSize;
} arena_bootstrap_params;

inline arena_bootstrap_params
DefaultBootstrapParams(void)
{
	arena_bootstrap_params Params = {};
	return(Params);
}

inline arena_bootstrap_params
NonRestoredArena(void)
{
	arena_bootstrap_params Params = DefaultBootstrapParams();
	Params.AllocationFlags = PlatformMemory_NotRestored;
	return(Params);
}

#if PROJ_INTERNAL
#define DEBUG_MEMORY_LANE(Name) DEBUG_NAME_(__FILE__, __LINE__, __COUNTER__),
#define INTERNAL_MEMORY_PARAM char *GUID,
#define INTERNAL_MEMORY_PASS GUID,
#else
#define DEBUG_MEMORY_NAME(Name)
#define INTERNAL_MEMORY_PARAM
#define INTERNAL_MEMORY_PASS
#endif

#define PushStruct(Arena, type, ...) (type *)PushSize_(DEBUG_MEMORY_NAME("PushStruct") Arena, sizeof(type), ## __VA_ARGS__)
#define PushArray(Arena, Count, type, ...) (type *)PushSize_(DEBUG_MEMORY_NAME("PushArray") Arena, (Count)*sizeof(type), ## __VA_ARGS__)
#define PushSize(Arena, Size, ...) PushSize_(DEBUG_MEMORY_NAME("PushSize") Arena, Size, ## __VA_ARGS__)
#define PushCopy(...) PushCopy_(DEBUG_MEMORY_NAME("PushCopy") __VA_ARGS__)
#define PushStringZ(...) PushStringZ_(DEBUG_MEMORY_NAME("PushStringZ") __VA_ARGS__)
#define PushString(...) PushString_(DEBUG_MEMORY_NAME("PushString") __VA_ARGS__)
#define PushBuffer(...) PushBuffer_(DEBUG_MEMORY_NAME("PushBuffer") __VA_ARGS__)
#define PushAndNullTerminate(...) PushAndNullTerminate_(DEBUG_MEMORY_NAME("PushAndNullTerminate") __VA_ARGS__)
#define BootstrapPushStruct(type, Member, ...) (type *)BootstrapPushSize_(DEBUG_MEMORY_NAME("BootstrapPushSize") sizeof(type), OffsetOf(type, Member), ## __VA_ARGS__)

inline mem_index
GetEffectiveSizeFor(memory_arena* Arena, mem_index SizeInit, arena_push_params Params)
{
	mem_index Size = SizeInit;

	mem_index AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
	Size += AlignmentOffset;
	return(Size);
}

inline mem_index
GetEffectiveSizeForDefault(memory_arena* Arena, mem_index SizeInit)
{
	// C23 does not support function argument defaults
	return GetEffectiveSizeFor(Arena, SizeInit, DefaultArenaParams());
}

internal inline void *
PushSize_(INTERNAL_MEMORY_PARAM
	memory_arena* Arena, mem_index SizeInit, arena_push_params Params)
{
	void* Result = 0;

	Assert(Params.Alignment <= 128);
	Assert(IsPow2(Params.Alignment));

	mem_index Size = 0;
	if (Arena->CurrentBlock)
		Size = GetEffectiveSizeFor(Arena, SizeInit, Params);

	if (!Arena->CurrentBlock ||
		((Arena->CurrentBlock->Used + Size) > Arena->CurrentBlock->Size))
	{
		Size = SizeInit; // Base automatically aligns
		if (Arena->AllocationFlags & (PlatformMemory_OverflowCheck |
			PlatformMemory_UnderflowCheck))
		{
			Arena->MinimumBlockSize = 0;
			Size = AlignPow2(Size, Params.Alignment);
		}
		else if (!Arena->MinimumBlockSize)
			Arena->MinimumBlockSize = 1024 * 1024;

		mem_index BlockSize = Maximum(Size, Arena->MinimumBlockSize);

		platform_memory_block* NewBlock =
			Platform.AllocateMemory(BlockSize, Arena->AllocationFlags);
		NewBlock->ArenaPrev = Arena->CurrentBlock;
		Arena->CurrentBlock = NewBlock;
		DEBUG_RECORD_BLOCK_ALLOCATION(Arena->CurrentBlock);
	}

	Assert((Arena->CurrentBlock->Used + Size) <= Arena->CurrentBlock->Size);

	mem_index AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
	umw OffsetInBlock = Arena->CurrentBlock->Used + AlignmentOffset;
	Result = Arena->CurrentBlock->Base + OffsetInBlock;
	Arena->CurrentBlock->Used += Size;

	Assert(Size >= SizeInit);

	Assert(Arena->CurrentBlock->Used <= Arena->CurrentBlock->Size);

	if (Params.Flags & ArenaFlag_ClearToZero)
		ZeroSize(SizeInit, Result);

	DEBUG_RECORD_ALLOCATION(Arena->CurrentBlock, GUID, Size, SizeInit, OffsetInBlock);

	return (Result);
}