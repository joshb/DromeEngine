set(
	SRCS
	BoundingBox.cpp
	BoundingSphere.cpp
	Matrix4.cpp
	PhysicsObject.cpp
	Quaternion.cpp
	Rect2i.cpp
	Vector2i.cpp
	Vector3.cpp
	Vector4.cpp
)

add_library(DromeMath STATIC ${SRCS})

install(
	TARGETS DromeMath
	RUNTIME DESTINATION ${DROME_LIBRARY_DIR}
	LIBRARY DESTINATION ${DROME_LIBRARY_DIR}
	ARCHIVE DESTINATION ${DROME_LIBRARY_DIR}
)
