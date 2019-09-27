cmake_minimum_required(VERSION 3.9)

# list the header files
set(HEADER_FILES
	include/renderer/renderer.h
	include/renderer/window.h
)

# list the source files
set(SOURCE_FILES
	src/renderer/renderer.cpp
	src/renderer/window.cpp
)

# add library target
add_library(renderer
	${HEADER_FILES}
	${SOURCE_FILES}
)

# make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

target_link_libraries(renderer
	PRIVATE
		glew
		glm
		glfw
		program
		game
		texture
)

# Windows specfic flags to enable W variations of winapi
if(WIN32)
	target_compile_definitions(renderer
		PRIVATE
			OS_WINDOWS=1
			_CRT_SECURE_NO_WARNINGS
	)
elseif(UNIX)
	target_compile_definitions(renderer
		PRIVATE
			OS_LINUX=1
	)
endif()

# enable C++17
# disable any compiler specifc extensions
# add d suffix in debug mode
target_compile_features(renderer PUBLIC cxx_std_17)
set_target_properties(renderer PROPERTIES
	CXX_EXTENSIONS OFF
	DEBUG_POSTFIX d
)

# define debug macro
target_compile_definitions(renderer PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

# generate exports header file
include(GenerateExportHeader)
generate_export_header(renderer
	EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/renderer/Exports.h
)

# list include directories
target_include_directories(renderer
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/../external/glm
)