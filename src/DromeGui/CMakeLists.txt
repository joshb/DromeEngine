set(
	SRCS
	Font.cpp
	Label.cpp
	Picture.cpp
	PushButton.cpp
	Widget.cpp
)

if(APPLE)
	add_definitions(-DAPPLE)
	set(SRCS ${SRCS} CoreGraphicsFont.cpp)
else(APPLE)
	find_package(Freetype)
	set(LIBRARIES ${LIBRARIES} ${FREETYPE_LIBRARIES})
	include_directories(${FREETYPE_INCLUDE_DIRS})
	set(SRCS ${SRCS} TrueTypeFont.cpp)
endif(APPLE)

add_library(DromeGui STATIC ${SRCS})
target_link_libraries(
	DromeGui
	DromeCore
	DromeGfx
	DromeMath
	${LIBRARIES}
)

install(
	TARGETS DromeGui
	RUNTIME DESTINATION ${DROME_LIBRARY_DIR}
	LIBRARY DESTINATION ${DROME_LIBRARY_DIR}
	ARCHIVE DESTINATION ${DROME_LIBRARY_DIR}
)
