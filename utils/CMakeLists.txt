set(SRCS Block.cpp MyScene1.cpp main.cpp)
add_executable(dromenormal dromenormal.cpp)

target_link_libraries(
	dromenormal
	DromeCore
	DromeGfx
	DromeMath
)

install(
	TARGETS dromenormal
	RUNTIME DESTINATION bin
)
