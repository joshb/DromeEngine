set(
	SRCS
	Cube.cpp
	GLWindow_GLFW.cpp
	ShaderProgram.cpp
	Sphere.cpp
	Square.cpp
    StaticMesh.cpp
	Texture.cpp
	VertexBuffer.cpp
)

find_package(OpenGL)
if(APPLE)
	set(OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR}/Headers)
	add_definitions(-DAPPLE)
endif(APPLE)

find_library(GLEW_LIBRARY NAMES GLEW libGLEW)

add_library(DromeGL STATIC ${SRCS})
target_link_libraries(
	DromeGL
	DromeCore
	DromeMath
	${OPENGL_gl_LIBRARY}
	${GLEW_LIBRARY}
)
include_directories(
	${OPENGL_INCLUDE_DIR}
)

install(
	TARGETS DromeGL
	RUNTIME DESTINATION ${DROME_LIBRARY_DIR}
	LIBRARY DESTINATION ${DROME_LIBRARY_DIR}
	ARCHIVE DESTINATION ${DROME_LIBRARY_DIR}
)
