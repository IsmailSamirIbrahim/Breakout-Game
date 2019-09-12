cmake_minimum_required(VERSION 3.9)

# list the header files
set(HEADER_FILES
	include/program/program.h
)

# list the source files
set(SOURCE_FILES
	src/program/program.cpp
)


# add library target
add_library(program
	${HEADER_FILES}
	${SOURCE_FILES}
)

# make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

target_link_libraries(program
	PRIVATE
		glew
		glm
)

# Windows specfic flags to enable W variations of winapi
if(WIN32)
	target_compile_definitions(program
		PRIVATE
			OS_WINDOWS=1
	)
endif()


# enable C++17
# disable any compiler specifc extensions
# add d suffix in debug mode
target_compile_features(program PUBLIC cxx_std_17)
set_target_properties(program PROPERTIES
	CXX_EXTENSIONS OFF
	DEBUG_POSTFIX d
)

# define debug macro
target_compile_definitions(program PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

# generate exports header file
include(GenerateExportHeader)
generate_export_header(program
	EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/program/Exports.h
)

# list include directories
target_include_directories(program
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)