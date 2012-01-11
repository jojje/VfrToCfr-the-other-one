LoadPlugin("..\build\Debug\VfrToCfr.dll")
ffvideosource("test.mp4",timecodes="timecodes.txt")

vfrtocfr(timecodes="timecodes.txt", fpsnum=30000,fpsden=1001,debug=true)
#ffinfo()
