import vapoursynth as vs
import os

def relative(path):
  """ Make the provided path relative to this script file """
  return os.path.join(os.path.abspath(os.path.dirname(__file__)), path)

plugin    = relative("../build/Debug/VfrToCfr.dll")
source    = relative("test.mp4")
tc_file   = relative("timecodes.txt")

core = vs.get_core()
core.avs.LoadPlugin(plugin)

clip = core.ffms2.Source(source=source, timecodes=tc_file)
clip = core.avs.VfrToCfr(clip, TIMECODES=tc_file, FPSNUM=30000, FPSDEN=1001, DEBUG=True)

clip.set_output()
