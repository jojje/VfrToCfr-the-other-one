#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1

#include <stdlib.h>
#include <string.h>
#include "VfrToCfr.h"
#include "info.h"

using namespace std;

// RAII type of lock: https://goo.gl/kgy89M
struct Synchronize {
    explicit Synchronize(CRITICAL_SECTION& cs) : lock(cs) {
        EnterCriticalSection(&lock);
    }
    ~Synchronize() { LeaveCriticalSection(&lock); }
    CRITICAL_SECTION& lock;
};
CRITICAL_SECTION initCs;       // "mutex" to synchronize the lazy initialization

// ==========================================================================
// PRIVATE methods
// ==========================================================================

void VfrToCfr::raiseError(IScriptEnvironment* env, const char* msg) {
    char buff[1024];
    sprintf(buff, "[VfrToCfr] %s", msg);
    env->ThrowError(buff);
}

PVideoFrame VfrToCfr::subtitle(IScriptEnvironment* env, PVideoFrame src, char* msg, int x, int y) {
    env->MakeWritable(&src);
    DrawString(src,x,y,msg,child->GetVideoInfo().IsYUY2());
    return src;
}

double VfrToCfr::GetFps(PClip clip) {
    VideoInfo info = clip->GetVideoInfo();
    return (double) info.fps_numerator / (double) info.fps_denominator;
}

void VfrToCfr::lazyInitialize(IScriptEnvironment* env) {
    Synchronize lock(initCs);
    if (state != initialized) {    // First thread trying to init, go ahead.
        try {
            framecalc.initialize();
            if(framecalc.getTimeCodes().size() != child->GetVideoInfo().num_frames) {
                throw exception("Error, timecodes file and source clip do not contain the same number of frames!");;
            }
            state = initialized;
        } catch(exception e) {
            raiseError(env, e.what());
        }
	}
}

// ==========================================================================
// PUBLIC methods
// ==========================================================================

PVideoFrame __stdcall VfrToCfr::GetFrame(int n, IScriptEnvironment* env) {
    float srcFrame;
    InterpolationInfo ip;
    PVideoFrame f;

    lazyInitialize(env);

    framecalc.getInterpolationInfo(&ip,n);
    f = child->GetFrame(ip.frame1,env);

    if(debug) {
        char msg[256];
        if(ip.frame1 == ip.frame2) srcFrame = (float) ip.frame1;
        else srcFrame = ip.frame1 + (((float)ip.pct)/100);
        sprintf(msg, "Current frame:  %d",   n);             f = subtitle(env,f,msg, 0,1); 
        sprintf(msg, "Source frame:   %.2f", srcFrame);      f = subtitle(env,f,msg, 0,2);
        sprintf(msg, "Current FPS:    %.3f", GetFps(this));  f = subtitle(env,f,msg, 0,3);
        sprintf(msg, "Source FPS:     %.3f", GetFps(child)); f = subtitle(env,f,msg, 0,4);
    }
    return f;
}

// Constructor implementation
VfrToCfr::VfrToCfr(PClip _child, const char* timecodes_filename, int fps_num, int fps_den, bool _debug, IScriptEnvironment* env) :
    GenericVideoFilter(_child), 
    framecalc(FrameCalculator(timecodes_filename, fps_num, fps_den)),
    debug(_debug), state(notInitialized) {

    VideoInfo info = child->GetVideoInfo();

    if (fps_num <= 0) raiseError(env, "FPS numerator must be provided and be greater than 0");
    if (fps_den <= 0) raiseError(env, "FPS denominator must be provided and be greater than 0");

    vi.fps_numerator   = fps_num;
    vi.fps_denominator = fps_den;
    vi.num_frames      = (int)( info.num_frames * 
                           ((double)info.fps_denominator / (double)info.fps_numerator) * 
                           ((double)fps_num / (double)fps_den) );
}

VfrToCfr::~VfrToCfr() {}

AVSValue __cdecl Create_VfrToCfr(AVSValue args, void* user_data, IScriptEnvironment* env) {
    InitializeCriticalSection(&initCs);
    return new VfrToCfr(args[0].AsClip(),
         args[1].AsString(""),         // timecodes parameter
         args[2].AsInt(0),             // fps_num parameter
         args[3].AsInt(0),             // fps_den parameter
         args[4].AsBool(false),        // debug parameter
         env);
}
