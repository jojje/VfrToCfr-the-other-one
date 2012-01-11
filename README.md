VfrToCfr
========

DESCRIPTION
-----------
AVISynth plugin for converting variable frame rate clips to constant frame rates.
To use the plugin you also need to have the FFmpegSource2 plugin.

EXAMPLE SCRIPT
--------------
    LoadPlugin("..\build\Release\VfrToCfr.dll")
    
    # Dump the time codes from the VFR clip
    ffvideosource("test.mp4",timecodes="timecodes.txt")
    
    # Use the time codes file to figure out when the frames should be
    # displayed in the resulting NTSC CFR clip.
    vfrtocfr(timecodes="timecodes.txt", fpsnum=30000, fpsden=1001)

COMPILING
---------
### Prerequisites ###
* Visual Studio or Microsoft C++ compiler for compiling the plugin
* Asciidoc for generating the help file
* Ruby for running the Rakefile ("make" script)

Compile the plugin from the build directory by typing nmake, or open up 
the project in visual studio and compile the project from there.
