#include "basetypes.h"

#define counted_pointer(IGNORED)

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
    typedef struct bitmap_id
    {
        u32 Value;
    } bitmap_id;

    typedef struct sound_id
    {
        u32 Value;
    } sound_id;

    typedef struct font_id
    {
        u32 Value;
    } font_id;
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

    extern struct app_memory* DebugGlobalMemory;
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

    struct platform_memory_block
    {
        u64 Flags;
        u64 Size;
        u8* Base;
        umw Used;
        platform_memory_block *ArenaPrev;
    };

#define PLATFORM_GET_ALL_FILE_OF_TYPE_BEGIN(name) platform_file_group name(platform_file_type Type)
    typedef PLATFORM_GET_ALL_FILE_OF_TYPE_BEGIN(platform_get_all_files_of_type_begin);

#define PLATFORM_GET_ALL_FILE_OF_TYPE_END(name) void name(platform_file_group *FileGroup)
    typedef PLATFORM_GET_ALL_FILE_OF_TYPE_END(platform_get_all_files_of_type_end);

    enum platform_open_file_mode_flags
    {
        OpenFile_Read = 0x1,
        OpenFile_Write = 0x2,
    };

#define PLATFORM_OPEN_FILE(name) platform_file_handle name(platform_file_group *FileGroup, platform_file_info *Info, u32 ModeFlags)
    typedef PLATFORM_OPEN_FILE(platform_open_file);

#define PLATFORM_SET_FILE_SIZE(name) void name(platform_file_handle *Handle, u64 Size)
    typedef PLATFORM_SET_FILE_SIZE(platform_set_file_size);

#define PLATFORM_GET_FILE_BY_PATH(name) platform_file_info *name(platform_file_group *FileGroup, char *Path, u32 ModeFlags)
    typedef PLATFORM_GET_FILE_BY_PATH(platform_get_file_by_path);

#define PLATFORM_READ_DATA_FROM_FILE(name) void name(platform_file_handle *Handle, u64 Offset, u64 Size, void *Dest)
    typedef PLATFORM_READ_DATA_FROM_FILE(platform_read_data_from_file);

#define PLATFORM_WRITE_DATA_TO_FILE(name) void name(platform_file_handle *Handle, u64 Offset, u64 Size, void *Source)
    typedef PLATFORM_WRITE_DATA_TO_FILE(platform_write_data_to_file);

#define PLATFORM_ATOMIC_REPLACE_FILE_CONTENTS(name) bool32 name(platform_file_info *Info, u64 Size, void *Source)
    typedef PLATFORM_ATOMIC_REPLACE_FILE_CONTENTS(platform_atomic_replace_file_contents);

#define PLATFORM_FILE_ERROR(name) void name(platform_file_handle *Handle, char *Message)
    typedef PLATFORM_FILE_ERROR(platform_file_error);

#define PLATFORM_CLOSE_FILE(name) void name(platform_file_handle *Handle)
    typedef PLATFORM_CLOSE_FILE(platform_close_file);

#define PlatformNoFileErrors(Handle) ((Handle)->NoErrors)

    typedef struct platform_work_queue platform_work_queue;

#define PLATFORM_WORK_QUEUE_CALLBACK(name) void name(platform_work_queue *Queue, void *Data)
    typedef PLATFORM_WORK_QUEUE_CALLBACK(platform_work_queue_callback);

#define PLATFORM_ALLOCATE_MEMORY(name) platform_memory_block *name(mem_index Size, u64 Flags)
    typedef PLATFORM_ALLOCATE_MEMORY(platform_allocate_memory);

#define PLATFORM_DEALLOCATE_MEMORY(name) void name(platform_memory_block *Block)
    typedef PLATFORM_DEALLOCATE_MEMORY(platform_deallocate_memory);

    typedef void platform_add_entry(platform_work_queue* Queue, platform_work_queue_callback* Callback, void* Data);
    typedef void platform_complete_all_work(platform_work_queue* Queue);


    typedef enum platform_error_type
    {
        PlatformError_Fatal,
        PlatformError_Nonfatal,
    } platform_error_type;

#define PLATFORM_ERROR_MESSAGE(name) void name(platform_error_type Type, char *Message)
    typedef PLATFORM_ERROR_MESSAGE(platform_error_message);

    typedef struct platform_api
    {
        platform_add_entry* AddEntry;
        platform_complete_all_work* CompleteAllWork;

        platform_get_all_files_of_type_begin* GetAllFilesOfTypeBegin;
        platform_get_all_files_of_type_end* GetAllFilesOfTypeEnd;
        platform_get_file_by_path* GetFileByPath;
        platform_open_file* OpenFile;
        platform_set_file_size* SetFileSize;
        platform_read_data_from_file* ReadDataFromFile;
        platform_write_data_to_file* WriteDataToFile;
        platform_atomic_replace_file_contents* AtomicReplaceFileContents;
        platform_file_error* FileError;
        platform_close_file* CloseFile;

        platform_allocate_memory* AllocateMemory;
        platform_deallocate_memory* DeallocateMemory;

        platform_error_message* ErrorMessage;

#if PROJ_INTERNAL
        dbg_platform_execute_system_command* DEBUGExecuteSystemCommand;
        dbg_platform_get_process_state* DEBUGGetProcessState;
        dbg_platform_get_memory_stats* DEBUGGetMemoryStats;
#endif

    } platform_api;
    extern platform_api Platform;

    typedef struct app_memory
    {
        struct app_state* AppState;

#if PROJ_INTERNAL
        struct debug_table* DebugTable;
        struct debug_state* DebugState;
#endif

        platform_work_queue* HighPriorityQueue;
        platform_work_queue* LowPriorityQueue;
        struct renderer_texture_queue* TextureQueue;

        bool32 ExecutableReloaded;
        platform_api PlatformAPI;
    } app_memory;

    typedef struct app_render_commands app_render_commands;
    typedef struct app_render_settings app_render_settings;
#define APP_UPDATE_AND_RENDER(name) void name(app_memory *Memory, app_input *Input, app_render_commands *RenderCommands)
    typedef APP_UPDATE_AND_RENDER(app_update_and_render);

// This function has to be very fast. It cannot be longer than a ms.
// Need to measure func's performance
#define APP_GET_SOUND_SAMPLES(name) void name(app_memory *Memory, app_sound_output_buffer *SoundBuffer)
    typedef APP_GET_SOUND_SAMPLES(app_get_sound_samples);