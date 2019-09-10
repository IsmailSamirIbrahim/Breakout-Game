cmake_minimum_required(VERSION 3.9)

project(2D-Game VERSION 1.0.0 LANGUAGES CXX C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

#external libraries
add_subdirectory(external/glew)

# internal libraries
add_subdirectory(playground)
add_subdirectory(game)

# Organize Targets into folders
set_target_properties(
	game
	PROPERTIES FOLDER Game
)

set_target_properties(
	glew
	PROPERTIES FOLDER External
)

set_target_properties(
	playground
	PROPERTIES FOLDER Testing
)