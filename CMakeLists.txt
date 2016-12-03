cmake_minimum_required(VERSION 2.8)
set(PROJ_NAME KILL_TIME)
project(${PROJ_NAME})

# sakra/cotire
# https://github.com/sakra/cotire
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cotire)

# Microsoft Windows SDK (for basetsd.h, in which POINTER_64 is defined)
set(WIN_SDK_DIR
    "C:/Program Files (x86)/Windows Kits/8.1/Include/shared"
)

# DirectX 8.1 SDK for VC++
# http://download.csdn.net/download/mr_luowei2009/4262718
set(DX8_DIR C:/chenchang/dev/DX8_VcSdk)

include_directories(
    ${WIN_SDK_DIR}
    ${DX8_DIR}/include
)

link_libraries(
    opengl32.lib
    glu32.lib
    winmm.lib
    ${DX8_DIR}/lib/dxguid.lib
    ${DX8_DIR}/lib/dinput8.lib
)

set(FILES
    src/afxres.h
    src/audiosystem.cpp
    src/audiosystem.h
    src/camera.cpp
    src/camera.h
    src/enemy.h
    src/engine.cpp
    src/engine.h
    src/entity.cpp
    src/entity.h
    src/explosion.cpp
    src/explosion.h
    src/font.cpp
    src/font.h
    src/GLTools.h
    src/gui.cpp
    src/gui.h
    src/HiResTimer.h
    src/InputSystem.cpp
    src/InputSystem.h
    src/md2.cpp
    src/md2.h
    src/object.h
    src/oglwindow.cpp
    src/oglwindow.h
    src/ogro.cpp
    src/ogro.h
    src/Particles.cpp
    src/Particles.h
    src/player.cpp
    src/player.h
    src/resource.h
    src/resources.rc
    src/rocket.cpp
    src/rocket.h
    src/simpengine.cpp
    src/simpengine.h
    src/sod.cpp
    src/sod.h
    src/startupOptions.cpp
    src/startupOptions.h
    src/terrain.cpp
    src/terrain.h
    src/texture.cpp
    src/texture.h
    src/tree.cpp
    src/tree.h
    src/types.h
    src/utils.cpp
    src/utils.h
    src/vector.h
    src/vtrace.cpp
    src/vtrace.h
    src/winmain.cpp
    src/world.cpp
    src/world.h
)

add_executable(${PROJ_NAME} ${FILES})

set_target_properties(${PROJ_NAME} PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:WINDOWS /SAFESEH:NO"
    COTIRE_ADD_UNITY_BUILD FALSE
)

cotire(${PROJ_NAME})

add_custom_command(
   TARGET ${PROJ_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/media" "${CMAKE_BINARY_DIR}/media"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/models" "${CMAKE_BINARY_DIR}/models"
)
