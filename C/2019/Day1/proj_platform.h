#define counted_pointer(IGNORED)

#ifdef __cplusplus
extern "C" {
#endif

#include "basetypes.h"

#pragma pack(push, 1)
    struct bitmap_id
    {
        u32 Value;
    };

    struct sound_id
    {
        u32 Value;
    };

    struct font_id
    {
        u32 Value;
    };
#pragma pack(pop)

    /*
     * These are used for dbgging and ineract with platform layer services
     */
#if PROJ_INTERNAL
    typedef struct dbg_read_file_result
    {
        uint32 ContentsSize;
        void* Contents;
    } dbg_read_file_result;

    typedef struct dbg_executing_process
    {
        u64 OSHandle;
    } dbg_executing_process;

    typedef struct dbg_process_state
    {
        bool32 StartedSuccessfully;
        bool32 IsRunning;
        s32 ReturnCode;
    } dbg_process_state;

    typedef struct dbg_platform_memory_stats
    {
        umw BlockCount;
        umw TotalSize;
        umw TotalUsed;
    } dbg_platform_memory_stats;

#define DEBUG_PLATFORM_EXECUTE_SYSTEM_COMMAND(name) dbg_executing_process name(char *Path, char *Command, char *CommandLine)
    typedef DEBUG_PLATFORM_EXECUTE_SYSTEM_COMMAND(dbg_platform_execute_system_command);

#define DEBUG_PLATFORM_GET_MEMORY_STATS(name) dbg_platform_memory_stats name(void)
    typedef DEBUG_PLATFORM_GET_MEMORY_STATS(dbg_platform_get_memory_stats);

#define DEBUG_PLATFORM_GET_PROCESS_STATE(name) dbg_process_state name(dbg_executing_process Process)
    typedef DEBUG_PLATFORM_GET_PROCESS_STATE(dbg_platform_get_process_state);

    extern struct game_memory* DebugGlobalMemory;
#endif

    /*
     *  Provide this to the platform layer
     */
    struct manual_sort_key
    {
        u16 AlwaysInFrontOf;
        u16 AlwaysBehind;
    };

#define BITMAP_BYTES_PER_PIXEL 4
    typedef struct app_offscreen_buffer
    {
        void* Memory;
        int Width;
        int Height;
        int Pitch;
    } app_offscreen_buffer;

    typedef struct app_sound_output_buffer
    {
        int SamplesPerSecond;
        int SampleCount;
    } app_sound_output_buffer;

    typedef struct app_button_state
    {
        int HalfTransitionCount;
        bool32 EndedDown;
    } app_button_state;

    enum app_input_mouse_button
    {
        PlatformMouseButton_Left,
        PlatformMouseButton_Middle,
        PlatformMouseButton_Right,
        PlatformMouseButton_Extended0,
        PlatformMouseButton_Extended1,
        PlatformMouseButton_Count,
    };

    typedef struct app_input
    {
        r32 dtForFrame;
        u32 Entropy;

        // Signals back to platform layer
        bool32 QuitRequested;
        bool32 EntropyRequested;

        // For debugging purposes
        app_button_state MouseButton[PlatformMouseButton_Count];
        v3 ClipSpaceMouseP;
        bool32 ShiftDown, AltDown, ControlDown;
        bool32 FKeyPressed[13]; // 1 for F! etc., 0 is not used.
    } app_input;

    inline bool32 WasPressed(app_button_state State)
    {
        bool32 Result = ((State.HalfTransitionCount > 1) ||
                        ((State.HalfTransitionCount == 1) && (State.EndedDown)));
        return(Result);
    }

    inline bool32 IsDown(app_button_state State)
    {
        bool32 Result = (State.EndedDown);
        return(Result);
    }

    typedef struct platform_file_handle
    {
        bool32 NoErrors;
        void* Platform;
    } platform_file_handle;

    typedef struct platform_file_info platform_file_info;

    typedef struct platform_file_info
    {
        platform_file_info* Next;
        u64 FileDate; // 64-bit number that is the date to the platform, but doesn't have to be understood by app.
        u64 FileSize;
        char* BaseName;
        void* Platform;
    } platform_file_info;

    typedef struct platform_file_group
    {
        u32 FileCount;
        platform_file_info* FirstFileInfo;
        void* Platform;
    } platform_file_group;

    typedef enum platform_file_type
    {
        PlatformFileType_Dump,
        PlatformFileType_Count,
    } platform_file_type;

    enum platform_memory_block_flags
    {
        PlatformMemory_NotRestored = 0x1,
        PlatformMemory_OverflowCheck = 0x2,
        PlatformMemory_UnderflowCheck = 0x4,
    };

    typedef struct platform_memory_block platform_memory_block;

    typedef struct platform_memory_block
    {
        u64 Flags;
        u64 Size;
        u8* Base;
        umw Used;
        platform_memory_block *ArenaPrev;
    } platform_memory_block;

#define PLATFORM_GET_ALL_FILE_OF_TYPE_BEGIN(name) platform_file_group name(platform_file_type Type)
    typedef PLATFORM_GET_ALL_FILE_OF_TYPE_BEGIN(platform_get_all_files_of_type_begin);