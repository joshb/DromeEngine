set(SRCS Block.cpp MyEventHandler.cpp MyScene1.cpp)
if(APPLE)
	set(SRCS ${SRCS} main.mm)
else(APPLE)
	set(SRCS ${SRCS} main.cpp)
endif(APPLE)
add_executable(DromeDemo ${SRCS})

target_link_libraries(
	DromeDemo
	DromeCore
	DromeGfx
	DromeGui
	DromeMath
)

if(APPLE)
	add_custom_command(
		TARGET DromeDemo
		POST_BUILD

		# remove old DromeDemo.app
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/demo/DromeDemo.app

		# create DromeDemo.app
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/demo/DromeDemo.app ${CMAKE_BINARY_DIR}/demo/DromeDemo.app
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/demo/Data ${CMAKE_BINARY_DIR}/demo/DromeDemo.app/Contents/Resources/Data
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/demo/DromeDemo ${CMAKE_BINARY_DIR}/demo/DromeDemo.app/Contents/MacOS/DromeDemo
	)
else(APPLE)
	add_custom_command(
		TARGET DromeDemo
		POST_BUILD

		# remove old data directory
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/demo/Data

		# copy data directory
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/demo/Data ${CMAKE_BINARY_DIR}/demo/Data
	)
endif(APPLE)

install(
	TARGETS DromeDemo
	RUNTIME DESTINATION bin
)
