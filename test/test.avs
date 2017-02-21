# LoadPlugin(".\64bit\ffms2-x64.dll")
# Import(".\64bit\FFMS2.avsi")
# LoadPlugin("..\build\x64\Debug\VfrToCfr.dll")

LoadPlugin("..\build\Debug\VfrToCfr.dll")
ffvideosource("test.mp4",timecodes="timecodes.txt")

vfrtocfr(timecodes="timecodes.txt", fpsnum=30000,fpsden=1001,debug=true)
#ffinfo()
