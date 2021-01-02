#include "VfrToCfr.h"

#define VERSION "1.2"

AVSValue __cdecl Create_VfrToCfr(AVSValue args, void* user_data, IScriptEnvironment* env);

extern "C" __declspec(dllexport) const char* __stdcall AvisynthPluginInit2(IScriptEnvironment* env) {
    env->AddFunction("VfrToCfr", "c[TIMECODES]s[FPSNUM]i[FPSDEN]i[DEBUG]b", Create_VfrToCfr, 0);
    return "'VfrToCfr' plugin v" VERSION ", author: tinjon[at]gmail.com";
}
