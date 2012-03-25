add_executable(dromenormal dromenormal.cpp)
add_executable(drometexheader drometexheader.cpp)

target_link_libraries(
	dromenormal
	DromeCore
	DromeGfx
	DromeMath
)

target_link_libraries(
	drometexheader
	DromeCore
	DromeGfx
	DromeMath
)

install(
	TARGETS dromenormal drometexheader
	RUNTIME DESTINATION bin
)
