cmake_minimum_required(VERSION 3.8)

set(projectName ghostmodule-mongodb)
set(GHOST_MODULE_MONGODB_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE FILEPATH "Root directory of ghostmodule-mongodb")
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# define the project
project(${projectName} CXX)
enable_testing()
include(GoogleTest)

# fetch the dependencies with conan
include(${GHOST_MODULE_MONGODB_ROOT_DIR}/cmake/conan.cmake)
conan_check(REQUIRED)
conan_cmake_run(CONANFILE conanfile.txt
				BASIC_SETUP CMAKE_TARGETS
				BUILD missing)

set(PROTOC_OUTPUT ${GHOST_MODULE_MONGODB_ROOT_DIR}/generated/)
include(${GHOST_MODULE_MONGODB_ROOT_DIR}/cmake/generate_protobuf.cmake)

# Windows specific stuff
if (MSVC)
	message(STATUS "MSVC compiler detected - version: " ${MSVC_VERSION})
	
	if (MSVC_VERSION GREATER_EQUAL "1900")
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
		if (_cpp_latest_flag_supported)
			add_compile_options("/std:c++latest")
		endif()
	endif()
	
	message(STATUS "Setting D_WIN32_WINNT to 0x600")
	add_compile_options("-D_WIN32_WINNT=0x600")
	
	message(STATUS "Setting /EHsc flag")
	add_compile_options("/EHsc")
endif (MSVC)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

##########################################################################################################################################
######################################################## INCLUDE DIRECTORIES AND LINK ####################################################
##########################################################################################################################################

SET(GHOST_MODULE_MONGODB_INCLUDE_DIRECTORIES
	${GHOST_MODULE_MONGODB_ROOT_DIR}/include
	${PROTOC_OUTPUT}
	CACHE FILEPATH "Include directories of ghostmodule-mongodb"
)

# provide path to include directories
include_directories(
	${GHOST_MODULE_MONGODB_INCLUDE_DIRECTORIES}
)

##########################################################################################################################################
################################################################### TARGETS ##############################################################
##########################################################################################################################################

add_subdirectory(src)
