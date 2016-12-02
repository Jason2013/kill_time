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
    afxres.h
    audiosystem.cpp
    audiosystem.h
    camera.cpp
    camera.h
    enemy.h
    engine.cpp
    engine.h
    entity.cpp
    entity.h
    explosion.cpp
    explosion.h
    font.cpp
    font.h
    GLTools.h
    gui.cpp
    gui.h
    HiResTimer.h
    InputSystem.cpp
    InputSystem.h
    md2.cpp
    md2.h
    object.h
    oglwindow.cpp
    oglwindow.h
    ogro.cpp
    ogro.h
    Particles.cpp
    Particles.h
    player.cpp
    player.h
    resource.h
    resources.rc
    rocket.cpp
    rocket.h
    simpengine.cpp
    simpengine.h
    sod.cpp
    sod.h
    startupOptions.cpp
    startupOptions.h
    terrain.cpp
    terrain.h
    texture.cpp
    texture.h
    tree.cpp
    tree.h
    types.h
    utils.cpp
    utils.h
    vector.h
    vtrace.cpp
    vtrace.h
    winmain.cpp
    world.cpp
    world.h
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
