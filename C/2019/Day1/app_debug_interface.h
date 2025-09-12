
typedef struct dbg_table dbg_table;
#define DEBUG_APP_FRAME_END(name) void name(app_memory *Memory, app_input *Input, app_render_commands *RenderCommands)
typedef DEBUG_APP_FRAME_END(dbg_app_frame_end);

#if PROJ_INTERNAL
enum debug_type
{
	DebugType_Unknown,
	
	DebugType_Name,

	DebugType_FrameMarker,
	DebugType_BeginBlock,
	DebugType_EndBlock,

	DebugType_OpenDataBlock,
	DebugType_CloseDataBlock,
	DebugType_SetHUD,

	DebugType_ArenaSetName,
	DebugType_ArenaBlockFree,
	DebugType_ArenaBlockTruncate,
	DebugType_ArenaBlockAllocate,
	DebugType_ArenaAllocate,

	DebugType_string_ptr,
	DebugType_b32,
	DebugType_r32,
	DebugType_u32,
	DebugType_umm,
	DebugType_s32,
	DebugType_v2,
	DebugType_v3,
	DebugType_v4,
	DebugType_rectangle2,
	DebugType_rectangle3,
	DebugType_bitmap_id,
	DebugType_sound_id,
	DebugType_font_id,

	DebugType_ThreadIntervalGraph,
	DebugType_FrameBarGraph,
	DebugType_LastFrameInfo,
	DebugType_FrameSlider,
	DebugType_TopClocksList,
	DebugType_FunctionSummary,
	DebugType_MemoryByArena,
	DebugType_MemoryByFrame,
	DebugType_MemoryBySize,
};

typedef struct dbg_memory_block_op
{
	platform_memory_block* ArenaLookupBlock;
	platform_memory_block* Block;
	umw AllocatedSize;
} dbg_memory_block_op;

typedef struct dbg_memory_op
{
	platform_memory_block* Block;
	umw AllocatedSize;
	umw OffsetInBlock;
} dbg_memory_op;

typedef struct dbg_event dbg_event;

struct dbg_event
{
	u64 Clock;
	char* GUID;
	u16 ThreadID;
	u16 CoreIndex;
	u8 Type;
	char* Name;

	union
	{
		dev_id DebugID;
		dbg_event* Value_debug_event;

		char* Value_string_ptr;
		bool32 Value_b32;
		s32 Value_s32;
		u32 Value_u32;
		umw Value_umm;
		r32 Value_r32;
		v2 Value_v2;
		v3 Value_v3;
		v4 Value_v4;
		rectangle2 Value_rectangle2;
		rectangle3 Value_rectangle3;
		bitmap_id Value_bitmap_id;
		sound_id Value_sound_id;
		font_id Value_font_id;
		dbg_memory_op Value_debug_memory_op;
		dbg_memory_block_op Value_debug_memory_block_op;
	};
};

struct dbg_table
{
	dbg_event EditEvent;
	v2 MouseP;
	u32 RecordIncrement;

	// Currently does not ensure final debug
	// records written to event array complete
	// output before swapping of event array index.
	u32 CurrentEventArrayIndex;
	// Could be a u32 atomic since only 1 bit is needed to store 
	// which array we're using
	u64 volatile EventArrayIndex_EventIndex;
	dbg_event Events[2][16 * 65536];
};

extern dbg_table* GlobalDebugTable;

#define DEBUG_NAME__(A, B, C) A "|" #B "|" #C
#define DEBUG_NAME_(A, B, C) DEBUG_NAME__(A, B, C)
#define DEBUG_NAME(Name) DEBUG_NAME_(__FILE__, __LINE__, __COUNTER__)

#define DEBUGSetEventRecording(Enabled) (GlobalDebugTable->RecordIncrement = (Enabled) ? 1 : 0)

#define RecordDebugEvent(EventType, GUIDInit, NameInit)           \
u64 ArrayIndex_EventIndex = AtomicAddU64(&GlobalDebugTable->EventArrayIndex_EventIndex, GlobalDebugTable->RecordIncrement); \
u32 EventIndex = ArrayIndex_EventIndex & 0xFFFFFFFF;            \
Assert(EventIndex < ArrayCount(GlobalDebugTable->Events[0]));   \
dbg_event *Event = GlobalDebugTable->Events[ArrayIndex_EventIndex >> 32] + EventIndex; \
Event->Clock = __rdtsc();                       \
Event->Type = (u8)EventType;                                    \
Event->CoreIndex = 0;                                           \
Event->ThreadID = (u16)GetThreadID();                         \
Event->GUID = GUIDInit;                                       \
Event->Name = NameInit

#define FRAME_MARKER(SecondsElapsedInit) \
{RecordDebugEvent(DebugType_FrameMarker, DEBUG_NAME("Frame Marker"), "Frame Marker"); \
    Event->Value_r32 = SecondsElapsedInit;}

#if PROJ_SLOW

#define TIMED_BLOCK__(GUID, Name) timed_block TimedBlock_##Number(GUID, Name)
#define TIMED_BLOCK_(GUID, Name) TIMED_BLOCK__(GUID, Name)
#define TIMED_BLOCK(Name) TIMED_BLOCK_(DEBUG_NAME(Name), Name)
#define TIMED_FUNCTION(...) TIMED_BLOCK_(DEBUG_NAME(__FUNCTION__), (char *)__FUNCTION__)
#define HUD_TIMED_FUNCTION_(Name, ...) GlobalDebugTable->HUDFunction = Name; TIMED_BLOCK_(Name, (char *)__FUNCTION__)
#define HUD_TIMED_FUNCTION(...) HUD_TIMED_FUNCTION_(DEBUG_NAME(__FUNCTION__), ## __VA_ARGS__)

#define BEGIN_BLOCK_(GUID, Name) {RecordDebugEvent(DebugType_BeginBlock, GUID, Name);}
#define END_BLOCK_(GUID, Name) {RecordDebugEvent(DebugType_EndBlock, GUID, Name);}

#define BEGIN_BLOCK(Name) BEGIN_BLOCK_(DEBUG_NAME(Name), Name)
#define END_BLOCK() END_BLOCK_(DEBUG_NAME("END_BLOCK_"), "END_BLOCK_")

//typedef struct timed_block
//{
//	timed_block(char* GUID, char* Name)
//	{
//		BEGIN_BLOCK_(GUID, Name);
//	}
//
//	~timed_block()
//	{
//		END_BLOCK();
//	}
//} timed_block;

#else

#define TIMED_BLOCK(...)
#define TIMED_FUNCTION(...)
#define BEGIN_BLOCK(...)
#define END_BLOCK(...)
#define HUD_TIMED_FUNCTION(...)

#endif

#endif