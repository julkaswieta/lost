cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Lost)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
## SFML library
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# Box2D library
add_subdirectory("lib/b2d/Box2D")
#include_directories("lib/b2d/Box2D/")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

### Copy resources
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} 
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

#### Copy DLLs to the output folder ####
function (copy_sfml_dll MyTarget )
add_custom_command(TARGET ${MyTarget} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_directory  # which executes "cmake - E copy_if_different..."
        "${CMAKE_BINARY_DIR}/lib/sfml/bin/$<CONFIGURATION>"      # <--this is in-file
        $<TARGET_FILE_DIR:${MyTarget}>)                 # <--this is out-file path
endfunction()

#### Lost ####
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_executable(LOST ${SOURCES})
target_include_directories(LOST PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(LOST Box2D sfml-graphics)
copy_sfml_dll(LOST)
set_target_properties(LOST 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration))
add_dependencies(LOST copy_resources)
