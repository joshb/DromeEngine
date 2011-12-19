set(SRCS Block.cpp MyEventHandler.cpp MyScene1.cpp main.cpp)
add_executable(DromeDemo ${SRCS})

target_link_libraries(
	DromeDemo
	DromeCore
	DromeGfx
	DromeGui
	DromeMath
)

add_custom_command(
	TARGET DromeDemo
	POST_BUILD

	# remove old data directory
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/demo/Data

	# copy data directory
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/demo/Data ${CMAKE_BINARY_DIR}/demo/Data
)

install(
	TARGETS DromeDemo
	RUNTIME DESTINATION bin
)
