# CMake Version
cmake_minimum_required(VERSION 3.15...4.2.3)

# App version
set(CPPGAMEFRAMEWORK_VERSION_MAJOR 0)
set(CPPGAMEFRAMEWORK_VERSION_MINOR 0)
set(CPPGAMEFRAMEWORK_VERSION_PATCH 1)
set(CPPGAMEFRAMEWORK_VERSION 
"${CPPGAMEFRAMEWORK_VERSION_MAJOR}.${CPPGAMEFRAMEWORK_VERSION_MINOR}.${CPPGAMEFRAMEWORK_VERSION_PATCH}")

# Create project + code language
project (cppgameframework VERSION ${CPPGAMEFRAMEWORK_VERSION} LANGUAGES C CXX)

# Options variables ( samples, static libraries)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(CPPGAMEFRAMEWORK_BUILD_SAMPLES "Build samples" ON)

# Properties ( use folders for solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
 
# Multithreading compilation for Visual Studio
if(MSVC)
    add_compile_options(/MP)
endif(MSVC)

# Add libraries
#add_subdirectory(externals EXCLUDE_FROM_ALL)
#add_subdirectory(math)
#add_subdirectory(graphics)

# Build samples if option active
if(CPPGAMEFRAMEWORK_BUILD_SAMPLES)
    add_subdirectory(samples)
endif(CPPGAMEFRAMEWORK_BUILD_SAMPLES)