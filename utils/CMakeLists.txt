add_executable(dromenormal dromenormal.cpp)
add_executable(drometexheader drometexheader.cpp)

target_link_libraries(
	dromenormal
	DromeCore
	DromeMath
)

target_link_libraries(
	drometexheader
	DromeCore
	DromeMath
)

install(
	TARGETS dromenormal drometexheader
	RUNTIME DESTINATION bin
)
