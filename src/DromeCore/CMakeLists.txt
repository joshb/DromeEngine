set(
	SRCS
	Button.cpp
	ButtonState.cpp
	Endian.cpp
	EventHandler.cpp
	Exception.cpp
	File.cpp
	IOContext.cpp
	String.cpp
	Util.cpp
	Xml.cpp
)

# use Cocoa on Mac OS X, SDL elsewhere
if(APPLE)
	# link to Cocoa
	find_library(COCOA_LIBRARY Cocoa)
	set(LIBS ${LIBS} ${COCOA_LIBRARY})

	# include path to Cocoa headers
	find_path(COCOA_INCLUDE_DIR Cocoa/Cocoa.h)
	include_directories(${COCOA_INCLUDE_DIR})

	# build with Cocoa support
	set(SRCS ${SRCS} File_Apple.mm IOContext_Cocoa.mm)
	add_definitions(-DCOCOA_FOUND)
else(APPLE)
	find_package(SDL)
	if(SDL_FOUND)
		# link to SDL and include path to SDL headers
		set(LIBS ${LIBS} ${SDL_LIBRARY})
		include_directories(${SDL_INCLUDE_DIR})

		# build with SDL support
		set(SRCS ${SRCS} IOContext_SDL.cpp)
		add_definitions(-DSDL_FOUND)
	endif(SDL_FOUND)
endif(APPLE)

add_library(DromeCore STATIC ${SRCS})
target_link_libraries(
	DromeCore
	DromeMath
	${LIBS}
)

install(
	TARGETS DromeCore
	RUNTIME DESTINATION ${DROME_LIBRARY_DIR}
	LIBRARY DESTINATION ${DROME_LIBRARY_DIR}
	ARCHIVE DESTINATION ${DROME_LIBRARY_DIR}
)
