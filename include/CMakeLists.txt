file(GLOB DromeCore "${CMAKE_CURRENT_SOURCE_DIR}/DromeCore/*")
#file(GLOB DromeGfx "${CMAKE_CURRENT_SOURCE_DIR}/DromeGfx/*")
file(GLOB DromeGL "${CMAKE_CURRENT_SOURCE_DIR}/DromeGL/*")
#file(GLOB DromeGui "${CMAKE_CURRENT_SOURCE_DIR}/DromeGui/*")
file(GLOB DromeMath "${CMAKE_CURRENT_SOURCE_DIR}/DromeGui/*")

install(
	FILES ${DromeCore}
	DESTINATION include/DromeCore
)

#install(
#	FILES ${DromeGfx}
#	DESTINATION include/DromeGfx
#)

install(
	FILES ${DromeGL}
	DESTINATION include/DromeGL
)

#install(
#	FILES ${DromeGui}
#	DESTINATION include/DromeGui
#)

install(
	FILES ${DromeMath}
	DESTINATION include/DromeMath
)
