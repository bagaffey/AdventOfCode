
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

#define