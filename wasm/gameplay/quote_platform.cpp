#pragma once

//#include <stdint.h>
//#include <math.h>

#define Pi 3.14152128f

#if QUOTE_SLOW
#define Assert(Expression) if(!(Expression)) { *(int*)0 = 0; }
#else
#define Assert(Expression) if(Expression){}
#endif

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef bool bool32; // Used for when we want a bool that's not forced to be 0 or 1. IE 0 = false, non-0 = true.

typedef uint32 memory_index;

#define null 0
#define internal static 
#define local_persist static // Good to be able to find these because they suck and should never be used
#define global_variable static // We'll only ever have one translation unit for the whole program so static is program-wide global

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define Kilobytes(x) ((x) * (1024LL))
#define Megabytes(x) ((Kilobytes(x)) * (1024LL))
#define Gigabytes(x) ((Megabytes(x)) * (1024LL))
#define Terabytes(x) ((Gigabytes(x)) * (1024LL))

inline uint32 SafeTrunchateUint64(uint64 Value)
{
    Assert(Value <= 0xFFFFFFFF);
    uint32 result = (uint32)Value;
    return result;
}

bool32 IsValidIndex(int index, int length)
{
    if (index >= 0 && index <= length-1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// when defining an API, write the usage code first.


// things are split up into two "directions", 


// services the os provides to the game
// "I need to look at the system clock"
// 


// 1. Services the game provides to the platform layer
// "what do you want to render"
// "what do you want to play out the sound card"
// "what do you want to do for files"
// Takes timing, controller input, bitmap to use, sound buffer to use

struct read_file_result
{
    uint32 ContentsSize;
    void* Contents;
};

enum ChannelLayout {
    ChannelLayout_ARGB,
    ChannelLayout_ABGR,
};

struct game_offscreen_buffer
{
    void* Memory;
    int Width;
    int Height;
    int BytesPerPixel;
    int Pitch;
    ChannelLayout Layout;
};

typedef struct
{
    int16 left;
    int16 right;
} Sample;
inline Sample ctor_Sample(int16 left, int16 right)
{
    Sample result;
    result.left = left;
    result.right = right;
    return result;
}
#define Sample(left, right) ctor_Sample((left), (right))


struct game_sound_output_buffer
{
    int32 SamplesPerSecond;
    int32 SampleCount;
    Sample* Samples;
};

struct game_button_state
{
    bool32 Held; // Button is being held down
    bool32 Down; // Button changed from down to up
    bool32 Up; // Button changed from up to down
    bool32 LastHeld;
    //int HalfTransistionCount;
};

struct game_int_state
{
    int32 Value;
    int32 LastValue;
    int32 ValueDelta;
};

struct game_float_state
{
    float Value;
    float LastValue;
    float ValueDelta;
};
#define UNION_BUTTON(name) union { game_button_state State##name; struct { bool32 name; bool32 name##Up; bool32 name##Down; }; };
#define UNION_INT(name) union { game_int_state State##name; struct { int32 name; int32 Last##name; int32 name##Delta; }; };
#define UNION_FLOAT(name) union { game_float_state State##name; struct { float name; float Last##name; float name##Delta; }; };


struct game_controller_input
{
    UNION_FLOAT(LeftStickX)
    UNION_FLOAT(LeftStickY)
    UNION_FLOAT(RightStickX)
    UNION_FLOAT(RightStickY)

    union
    {
        game_button_state Buttons[12];
        struct
        {
            UNION_BUTTON(Left)
            UNION_BUTTON(Right)
            UNION_BUTTON(Up)
            UNION_BUTTON(Down)

            UNION_BUTTON(A)
            UNION_BUTTON(B)
            UNION_BUTTON(X)
            UNION_BUTTON(Y)

            UNION_BUTTON(LeftBumper)
            UNION_BUTTON(RightBumper)

            UNION_BUTTON(LeftTrigger)
            UNION_BUTTON(RightTrigger)

            UNION_BUTTON(Start)
            UNION_BUTTON(Back)
        };
    };
};
struct thread_context
{
    int Placeholder;
};

struct thread
{
    bool executing;
    void* function;
};

#define PLATFORM_READ_FILE(name) read_file_result name(thread_context* Thread, const char* Filename)
typedef PLATFORM_READ_FILE(platform_read_file); // a way to define a function with that template

#define PLATFORM_FREE_FILE_MEMORY(name) void name(thread_context* Thread, read_file_result Memory)
typedef PLATFORM_FREE_FILE_MEMORY(platform_free_file_memory);

#define PLATFORM_WRITE_FILE(name) bool32 name(thread_context* Thread, const char* Filename, read_file_result Memory)
typedef PLATFORM_WRITE_FILE(platform_write_file);

#define PLATFORM_READ_IMAGE_INTO_MEMORY(name) void name(void* Destination, const char* File)
typedef PLATFORM_READ_IMAGE_INTO_MEMORY(platform_read_image_into_memory);

#define PLATFORM_READ_SOUND_INTO_MEMORY(name) void name(void* Destination, const char* File)
typedef PLATFORM_READ_SOUND_INTO_MEMORY(platform_read_sound_into_memory);

#define PLATFORM_PRINT(name) void name(const char* string)
typedef PLATFORM_PRINT(platform_print);

#define PLATFORM_TIME(name) int64 name()
typedef PLATFORM_TIME(platform_time);

struct game_memory;

typedef void ThreadCallType(int thread, game_memory* Memory, game_offscreen_buffer* VideoBuffer, game_sound_output_buffer* SoundBuffer);

//extern "C" void PlatformThreadCall(ThreadCallType function, int ThreadIndex);
//extern "C" int dllTest();
//extern "C" __declspec(dllexport) int exeTest();
//extern "C" __declspec(dllexport) int dllTest();

#if WASM
#else
extern "C" __declspec(dllexport) float exe_sqrtf(float n);
extern "C" __declspec(dllexport) float exe_floorf(float n);
extern "C" __declspec(dllexport) float exe_ceilf(float n);
extern "C" __declspec(dllexport) float exe_sinf(float n);
extern "C" __declspec(dllexport) float exe_cosf(float n);
extern "C" __declspec(dllexport) float exe_atan2(float a, float b);
extern "C" __declspec(dllexport) float exe_log10(float n);
extern "C" __declspec(dllexport) float exe_fabsf(float n);
extern "C" __declspec(dllexport) float exe_pow(float a, float b);
extern "C" __declspec(dllexport) float exe_fminf(float a, float b);
extern "C" __declspec(dllexport) float exe_fmaxf(float a, float b);
extern "C" __declspec(dllexport) float exe_roundf(float a);
extern "C" __declspec(dllexport) float exe_exp2f(float a);
extern "C" __declspec(dllexport) float exe_acosf(float a);
extern "C" __declspec(dllexport) bool  exe_isfinite(float a);
#endif


#define PLATFORM_THREADCALL(name) void name(ThreadCallType function, int ThreadIndex)
typedef PLATFORM_THREADCALL(platform_threadcall);

typedef float platform_sqrtf(float n);
typedef float platform_floorf(float n);
typedef float platform_ceilf(float n);
typedef float platform_sinf(float n);
typedef float platform_cosf(float n);
typedef float platform_atan2(float a, float b);
typedef float platform_log10(float n);
typedef float platform_fabsf(float n);
typedef float platform_pow(float a, float b);
typedef float platform_fminf(float a, float b);
typedef float platform_fmaxf(float a, float b);
typedef float platform_roundf(float a);
typedef float platform_exp2f(float a);
typedef float platform_acosf(float a);
typedef bool  platform_isfinite(float a);

float external_sqrtf(float n);
float external_floorf(float n);
float external_ceilf(float n);
float external_sinf(float n);
float external_cosf(float n);
float external_atan2(float a, float b);
float external_log10(float n);
float external_fabsf(float n);
float external_pow(float a, float b);
float external_fminf(float a, float b);
float external_fmaxf(float a, float b);
float external_roundf(float a);
float external_exp2f(float a);
float external_acosf(float a);
bool  external_isfinite(float a);


struct game_memory
{
    bool32 Initialized;
    uint64 PermanentStorageSize;
    void*  PermanentStorage; // Note: REQUIRED to be initialized to all 0s.
    uint64 TransientStorageSize;
    void*  TransientStorage; // Note: REQUIRED to be initialized to all 0s.

    //platform_read_file* PlatformReadFile;
    //platform_free_file_memory* PlatformFreeFileMemory;
    platform_write_file* PlatformWriteFile; // Only avilable on win32 atm.
    platform_read_image_into_memory* PlatformReadImageIntoMemory;
    platform_read_sound_into_memory* PlatformReadSoundIntoMemory;
    platform_print* PlatformPrint;
    platform_time* PlatformTime;

    platform_sqrtf* sqrtf;
    platform_floorf* floorf;
    platform_ceilf* ceilf;
    platform_sinf* sinf;
    platform_cosf* cosf;
    platform_atan2* atan2;
    platform_log10* log10;
    platform_fabsf* fabsf;
    platform_pow* pow;
    platform_fminf* fminf;
    platform_fmaxf* fmaxf;
    platform_roundf* roundf;
    platform_exp2f* exp2f;
    platform_acosf* acosf;
    platform_isfinite* isfinite;


    // call a function on another thread.
    platform_threadcall* PlatformThreadCall;
    volatile int ThreadCount;
    //volatile bool ThreadsGo;
    volatile bool ThreadsExecuting[100]; // bool is true if the thread is currently executing.
};

// Global game memory struct, maybe clean up? used for injecting math test stuff atm.
static game_memory* GlobalGameMemory;

struct game_clocks
{
    float DeltaTime;
};

void InputUpdateButton(game_button_state* Button, bool Held)
{
    Button->Held = Held;
    Button->Down = false;
    Button->Up = false;
    if (Button->Held != Button->LastHeld)
    {
        Button->LastHeld = Button->Held;
        if (Button->Held)
        {
            Button->Up = true;
        }
        else
        {
            Button->Down = true;
        }
    }
}

void InputUpdateInt(game_int_state* Button, int32 NewValue)
{
    Button->Value = NewValue;
    Button->ValueDelta = Button->Value - Button->LastValue;
    Button->LastValue = Button->Value;
}
void InputUpdateFloat(game_float_state* Button, float NewValue)
{
    Button->Value = NewValue;
    Button->ValueDelta = Button->Value - Button->LastValue;
    Button->LastValue = Button->Value;
}

struct game_input
{
    UNION_INT(MouseX)
    UNION_INT(MouseY)
    UNION_INT(MouseZ)

    UNION_BUTTON(MouseLeft)
    UNION_BUTTON(MouseRight)
    UNION_BUTTON(MouseMiddle)

    union
    {
        game_button_state Letters[26];
        struct
        {
            UNION_BUTTON(A)
            UNION_BUTTON(B)
            UNION_BUTTON(C)
            UNION_BUTTON(D)
            UNION_BUTTON(E)
            UNION_BUTTON(F)
            UNION_BUTTON(G)
            UNION_BUTTON(H)
            UNION_BUTTON(I)
            UNION_BUTTON(J)
            UNION_BUTTON(K)
            UNION_BUTTON(L)
            UNION_BUTTON(M)
            UNION_BUTTON(N)
            UNION_BUTTON(O)
            UNION_BUTTON(P)
            UNION_BUTTON(Q)
            UNION_BUTTON(R)
            UNION_BUTTON(S)
            UNION_BUTTON(T)
            UNION_BUTTON(U)
            UNION_BUTTON(V)
            UNION_BUTTON(W)
            UNION_BUTTON(X)
            UNION_BUTTON(Y)
            UNION_BUTTON(Z)
        };
    };

    union
    {
        game_button_state Numbers[10];
        struct
        {
            UNION_BUTTON(D0)
            UNION_BUTTON(D1)
            UNION_BUTTON(D2)
            UNION_BUTTON(D3)
            UNION_BUTTON(D4)
            UNION_BUTTON(D5)
            UNION_BUTTON(D6)
            UNION_BUTTON(D7)
            UNION_BUTTON(D8)
            UNION_BUTTON(D9)
        };
    };

    UNION_BUTTON(Space)
    UNION_BUTTON(Backspace)
    UNION_BUTTON(Enter)
    UNION_BUTTON(Escape)
    UNION_BUTTON(Delete)

    UNION_BUTTON(Left)
    UNION_BUTTON(Right)
    UNION_BUTTON(Up)
    UNION_BUTTON(Down)

    UNION_BUTTON(Shift)
    UNION_BUTTON(Ctrl)
    UNION_BUTTON(Alt)

    float DeltaTime;

    game_controller_input Controllers[4];
};


#define GAME_UPDATE_AND_RENDER(name) void name(thread_context* Thread, game_memory* Memory, game_input* Input, game_offscreen_buffer* VideoBuffer, game_sound_output_buffer* SoundBuffer) // external function template
typedef GAME_UPDATE_AND_RENDER(game_update_and_render); // a way to define a function with that template
GAME_UPDATE_AND_RENDER(GameUpdateAndRenderStub) { } // define stub function




// Call this to set all input to nothing. useful for when the window is de-seleted.
void ResetInput(game_input* Input)
{
    //InputUpdateInt(&Input.StateMouseX, 0);
    //InputUpdateInt(&Input.StateMouseY, 0);

    InputUpdateInt(&Input->StateMouseZ, false);

    InputUpdateButton(&Input->StateMouseLeft, false);
    InputUpdateButton(&Input->StateMouseRight, false);
    InputUpdateButton(&Input->StateMouseMiddle, false);

    InputUpdateButton(&Input->StateSpace, false);
    InputUpdateButton(&Input->StateBackspace, false);
    InputUpdateButton(&Input->StateEnter, false);
    InputUpdateButton(&Input->StateEscape, false);
    InputUpdateButton(&Input->StateDelete, false);

    InputUpdateButton(&Input->StateLeft, false);
    InputUpdateButton(&Input->StateRight, false);
    InputUpdateButton(&Input->StateUp, false);
    InputUpdateButton(&Input->StateDown, false);

    InputUpdateButton(&Input->StateShift, false);
    InputUpdateButton(&Input->StateCtrl, false);
    InputUpdateButton(&Input->StateAlt, false);

    for (int i = 0; i < 26; i++)
    {
        InputUpdateButton(&Input->Letters[i], false);
    }

    for (int i = 0; i < 10; i++)
    {
        InputUpdateButton(&Input->Numbers[i], false);
    }


    for (int i = 0; i < 4; i++)
    {
        InputUpdateFloat(&Input->Controllers[i].StateLeftStickX, 0.0f);
        InputUpdateFloat(&Input->Controllers[i].StateLeftStickY, 0.0f);
        InputUpdateFloat(&Input->Controllers[i].StateRightStickX, 0.0f);
        InputUpdateFloat(&Input->Controllers[i].StateRightStickY, 0.0f);

        InputUpdateButton(&Input->Controllers[i].StateA, false);
        InputUpdateButton(&Input->Controllers[i].StateB, false);
        InputUpdateButton(&Input->Controllers[i].StateX, false);
        InputUpdateButton(&Input->Controllers[i].StateY, false);

        InputUpdateButton(&Input->Controllers[i].StateUp, false);
        InputUpdateButton(&Input->Controllers[i].StateDown, false);
        InputUpdateButton(&Input->Controllers[i].StateLeft, false);
        InputUpdateButton(&Input->Controllers[i].StateRight, false);

        InputUpdateButton(&Input->Controllers[i].StateLeftBumper, false);
        InputUpdateButton(&Input->Controllers[i].StateRightBumper, false);
        InputUpdateButton(&Input->Controllers[i].StateLeftTrigger, false);
        InputUpdateButton(&Input->Controllers[i].StateRightTrigger, false);

        InputUpdateButton(&Input->Controllers[i].StateStart, false);
        InputUpdateButton(&Input->Controllers[i].StateBack, false);
    }
}
