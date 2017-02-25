function load64bit() {
  LoadPlugin(".\64bit\ffms2-x64.dll")
  Import(".\64bit\FFMS2.avsi")
  LoadPlugin("..\build\x64\Debug\VfrToCfr.dll")
}
function load32bit() {
  LoadPlugin("..\build\Debug\VfrToCfr.dll")
}

# load64bit()
load32bit()
SetFilterMTMode("vfrtocfr", 1)

ffvideosource("test.mp4",timecodes="timecodes.txt")
vfrtocfr(timecodes="timecodes.txt", fpsnum=30000,fpsden=1001,debug=true)

# prefetch(4)  # uncomment to test multi-threading
