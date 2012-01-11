#include <windows.h>
#include <avisynth.h>
#include "framecalc.h"

class VfrToCfr : public GenericVideoFilter {
    bool debug;                                // Whether or not to render internal stuff on the rendered video.
    FrameCalculator framecalc;                 // A helper to calculate which source frame corresponds to which display frame.

    enum State {                               // Add lifecycle phases and guards just in case someone against all reason decides
        notInitialized,                        // to run this script threaded. It's plenty fast without and the user will run 
        initializing,                          // out of IO bandwidth before they'll get this filter to make a dent on CPU...
        initialized
    };
    State volatile state;                      // The initialization state of the filter

public:
    VfrToCfr(PClip _child, const char* timecodes_filename, int fps_num, int fps_den, bool debug, IScriptEnvironment* env);
    ~VfrToCfr();
    PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
private:
    void raiseError(IScriptEnvironment* env, const char* msg);
    PVideoFrame subtitle(IScriptEnvironment* env, PVideoFrame src, char* msg, int x, int y);
    double GetFps(PClip clip);
    void lazyInitialize(IScriptEnvironment* env);  // so ffmpegsource has time to generate the timecodes file before we try using it.
};
