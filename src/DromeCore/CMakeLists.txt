set(
	SRCS
	Button.cpp
	ButtonState.cpp
	Endian.cpp
	EventHandler.cpp
	Exception.cpp
	File.cpp
	Image.cpp
	PcxImage.cpp
	PngImage.cpp
	String.cpp
	Util.cpp
)

# use Cocoa on Mac OS X
if(APPLE)
	# link to Cocoa
	find_library(COCOA_LIBRARY Cocoa)
	set(LIBS ${LIBS} ${COCOA_LIBRARY})

	# include path to Cocoa headers
	find_path(COCOA_INCLUDE_DIR Cocoa/Cocoa.h)
	include_directories(${COCOA_INCLUDE_DIR})

	# build with Cocoa support
	set(SRCS ${SRCS} File_Apple.mm)
	add_definitions(-DCOCOA_FOUND)
endif(APPLE)
find_package(PNG)

add_library(DromeCore STATIC ${SRCS})
target_link_libraries(
	DromeCore
	DromeMath
	${LIBS}
	${PNG_LIBRARIES}
)
include_directories(
	${PNG_INCLUDE_DIR}
)

install(
	TARGETS DromeCore
	RUNTIME DESTINATION ${DROME_LIBRARY_DIR}
	LIBRARY DESTINATION ${DROME_LIBRARY_DIR}
	ARCHIVE DESTINATION ${DROME_LIBRARY_DIR}
)
