
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
};

inline arena_push_params
DefaultArenaParams(void)
{
	arena_push_params Params;
	Params.Flags = ArenaFlag_ClearToZero;
	Params.Alignment = 4;
	return(Parms);
}
