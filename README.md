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

For the VapourSynth equivalent, take a look at `test/test.vpy`

COMPILING
---------
### Prerequisites ###
* Visual Studio 2013 or later for compiling the plugin
* Asciidoc for generating the help file

Compile the plugin from the Visual Studio GUI. A project and solution file
can be found in the _build_ directory.

LICENSE
-------
The project code and binaries are made available under LGPL v2 license.
