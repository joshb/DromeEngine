set(
	SRCS
	Camera.cpp
	CubeMesh.cpp
	CylinderMesh.cpp
	Driver.cpp
	DriverGL.cpp
	Framebuffer.cpp
	Image.cpp
	Md2Mesh.cpp
	Mesh.cpp
	ParticleEmitter.cpp
	PcxImage.cpp
	PngImage.cpp
	ShaderProgram.cpp
	SphereMesh.cpp
	Texture.cpp
	Types.cpp
	VertexBuffer.cpp
)

find_package(OpenGL)
if(APPLE)
	set(OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR}/Headers)
	add_definitions(-DAPPLE)
endif(APPLE)
find_package(PNG)

add_library(DromeGfx STATIC ${SRCS})
target_link_libraries(
	DromeGfx
	DromeCore
	DromeMath
	${OPENGL_gl_LIBRARY}
	${PNG_LIBRARIES}
)
include_directories(
	${OPENGL_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
)

install(
	TARGETS DromeGfx
	RUNTIME DESTINATION ${DROME_LIBRARY_DIR}
	LIBRARY DESTINATION ${DROME_LIBRARY_DIR}
	ARCHIVE DESTINATION ${DROME_LIBRARY_DIR}
)
