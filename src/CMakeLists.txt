if(WIN32)
	set(DROME_LIBRARY_DIR bin)
else(WIN32)
	set(DROME_LIBRARY_DIR lib)
endif(WIN32)

subdirs(
	DromeCore
#	DromeGfx
	DromeGL
#	DromeGui
	DromeMath
)
