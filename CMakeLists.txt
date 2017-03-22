cmake_minimum_required (VERSION 2.8)
PROJECT (GP3Game)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
LIST(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -ftest-coverage -fprofile-arcs")

# Turning on Full Optimization to help with model loading
if(MSVC)
    # spexicy all optimizations
    string(REPLACE "/Od" "/Ox" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    # turn of conflicting Run Time Checks
    string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    # enable inlining of everything
    string(REPLACE "/Ob0" "/Ob2" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    # Release already has everything except for /Ox
    string(REPLACE "/O2" "/Ox" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
elseif(CMAKE_COMPILER_IS_GNUCC)
    # not sure, have to get my hands on a linux machine
    # need to compile with -Ofast
endif()

#find packages
message("3rd party dependencies: $ENV{CMAKE_PREFIX_PATH}")
SET(CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH}")

#make sure packages are installed before continue
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_Image REQUIRED)
FIND_PACKAGE(SDL2_TTF REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(GLM REQUIRED)
FIND_PACKAGE(FBX REQUIRED)
FIND_PACKAGE(Bullet REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)

#GP3Engine library
FILE(GLOB GP3Engine_SRC GP3Engine/src/*.cpp GP3Engine/src/Components/*.cpp GP3Engine/include/*.h GP3Engine/include/Components/*.h)
add_library(GP3Engine ${GP3Engine_SRC})
SET_TARGET_PROPERTIES(GP3Engine PROPERTIES LINKER_LANGUAGE CXX)

#source files
FILE(GLOB SRC src/*.cpp include/*.h)
ADD_EXECUTABLE(GP3Game ${SRC})

#include header files
INCLUDE_DIRECTORIES(GP3Game include/ GP3Engine/include/ ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${FBX_INCLUDE_DIR} ${BULLET_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR} ${ALUT_INCLUDE_DIR})

#link libraries
TARGET_LINK_LIBRARIES(GP3Game GP3Engine ${SDL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${FBX_LIBRARY} ${BULLET_LIBRARIES} ${OPENAL_LIBRARY} ${ALUT_LIBRARY})

#copy assets folder
if (MSVC)
add_custom_command(TARGET GP3Game  POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/dlls $<TARGET_FILE_DIR:GP3Game>)
endif (MSVC)

add_custom_command(TARGET GP3Game  PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                 ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:GP3Game>/assets)
