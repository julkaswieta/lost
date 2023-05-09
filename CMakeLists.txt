cmake_minimum_required(VERSION 3.9)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
elseif(APPLE)
  message("hello apple")
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9 CACHE STRING "")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Lost)

#### Setup Directories ####
# Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

### Copy resources ###
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} 
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

#### Add External Dependencies ####
# SFML - graphics library
add_subdirectory("lib/sfml" EXCLUDE_FROM_ALL)
#include_directories("lib/sfml/include")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# B2D - Box2D phyics library
add_subdirectory("lib/b2d/Box2D" EXCLUDE_FROM_ALL)
#include_directories("lib/b2d/Box2D/")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

file(GLOB_RECURSE RESOURCE_FILES "res/*.*")

# SFML_GamepadSupport
file(GLOB_RECURSE SOURCE_FILES lib/SFML_GamepadSupport/*.cpp lib/SFML_GamepadSupport/*.h)
add_library(lib_gamepad STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_gamepad INTERFACE "${CMAKE_SOURCE_DIR}/lib/SFML_GamepadSupport" PUBLIC SYSTEM ${B2D_INCS})
target_link_libraries(lib_gamepad sfml-graphics Box2D)

## Engine lib
file(GLOB_RECURSE SOURCE_FILES lib/engine/*.cpp lib/engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/lib/engine/" PUBLIC SYSTEM ${B2D_INCS})
target_link_libraries(lib_engine lib_tile_level_loader lib_maths lib_ecm lib_gamepad Box2D sfml-graphics sfml-audio)

## Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib/lib_maths" INTERFACE ${SFML_INCS})

## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib/lib_tile_level_loader/*.cpp lib/lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib/lib_tile_level_loader/" )
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

## ECM lib
file(GLOB_RECURSE SOURCE_FILES lib/lib_ecm/*.cpp lib/lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)

#### Copy DLL Files ####
function (copy_sfml_dll MyTarget )
add_custom_command(TARGET ${MyTarget} POST_BUILD			# Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_directory				# Which executes "cmake - E copy_if_different..."
        "${CMAKE_BINARY_DIR}/lib/sfml/bin/$<CONFIGURATION>"	# <--this is in-file
        $<TARGET_FILE_DIR:${MyTarget}>)                 	# <--this is out-file path
add_custom_command(TARGET ${MyTarget} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory  			
        "${CMAKE_SOURCE_DIR}/lib/sfml/extlibs/bin/x64"      
        $<TARGET_FILE_DIR:${MyTarget}>)
endfunction()

## Lost
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
file(GLOB_RECURSE CMPNTS src/components/*.cpp src/components/*.h)
file(GLOB_RECURSE SCENES src/scenes/*.cpp src/scenes/*.h)
add_executable(LOST ${SOURCES} ${RESOURCE_FILES})
source_group("components" FILES ${CMPNTS})
source_group("resources" FILES ${RESOURCE_FILES})
#target_include_directories(LOST SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(LOST lib_engine)
set(EXECUTABLES ${EXECUTABLES} LOST)
copy_sfml_dll(LOST)
set_target_properties(LOST 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration))
add_dependencies(LOST copy_resources)