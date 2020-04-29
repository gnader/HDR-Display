project(hdr_image)

cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# COMPITLER OPTIONS ########################################################
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
############################################################################

# CIMAGE OPTIONS ###########################################################
add_definitions(-Dcimg_display=0)

if(WIN32)
    add_definitions(-Dcimg_OS=2)

#    set(_GNUWIN32PATH "D:/Program Files/GnuWin32")
#    set(CMAKE_PREFIX_PATH ${_GNUWIN32PATH} $(CMAKE_PREFIX_PATH))

#    add_definitions(-Dcimg_use_jpeg)
else()
    add_definitions(-Dcimg_OS=0)
endif()
############################################################################

# PACKAGES #################################################################
find_package(JPEG REQUIRED)

# INCLUDES #################################################################
include_directories(extern/)
include_directories(src/)
#if(JPEG_FOUND)
#    include_directories(${JPEG_INCLUDE_DIR})
#endif()
############################################################################

# SOURCES ##################################################################
set(SOURCES_FILES
    src/image.cpp
    src/image_io.cpp
    src/input_parser.cpp)

set(HEADER_FILES
    src/image.h
    src/image_io.h
    src/input_parser.h
    src/psf.h
    src/display_response.h
    src/hdr_display.h)

add_library(lhdr ${SOURCES_FILES})
############################################################################

# LIBS #####################################################################
set(LIBS lhdr)

#if(JPEG_FOUND)
#    set(LIBS ${LIBS} ${JPEG_LIBRARIES})
#endif()

if(NOT WIN32)
    set(LIBS ${LIBS} pthreads)
endif()

############################################################################

# EXECUTABLE ###############################################################
add_executable(hdr src/main.cpp)
target_link_libraries(hdr ${LIBS})
############################################################################
