set(SRCS main.cpp)
add_executable(DromeDemo ${SRCS})

target_link_libraries(
	DromeDemo
	DromeCore
	DromeGL
	DromeMath
	glfw
)

install(
	TARGETS DromeDemo
	RUNTIME DESTINATION bin
)
