INCLUDES = C:\Program Files (x86)\AviSynth 2.5\FilterSDK\include
NAME = VfrToCfr
DLL = $(NAME).dll
BUILD = Release

$(BUILD)/$(DLL):
	mkdir -p $(BUILD)
	cd $(BUILD)
	cl /c /Zi /nologo /W3 /WX- /O2 /Oi /Oy- /GL /D WIN32 /D NDEBUG /D _WINDOWS /D _USRDLL /D PLUGINTEMPLATE_EXPORTS /D _WINDLL /D _UNICODE /D UNICODE /Gm- /EHsc /MT /GS /Gy /fp:precise /Zc:wchar_t /Zc:forScope /Gd /TP /analyze- /errorReport:queue /I"$(INCLUDES)" ..\..\src\*.cpp
	rc /D _UNICODE /D UNICODE /l"0x0409" /nologo /fo"$(NAME).res" ..\$(NAME).rc
	link /OUT:"$(DLL)" /INCREMENTAL:NO /NOLOGO kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /MANIFEST:NO /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /DLL "$(NAME).res" *.obj

clean:
	rm -rf $(BUILD)
