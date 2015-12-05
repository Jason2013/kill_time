cmake_minimum_required(VERSION 2.8)
set(PROJ_NAME KILL_TIME)
project(${PROJ_NAME})

# sakra/cotire
# https://github.com/sakra/cotire
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cotire)

# Microsoft Windows SDK for Windows 7 and .NET Framework 3.5 SP1
# https://www.microsoft.com/en-us/download/details.aspx?id=3138
set(WIN_SDK7_DIR "C:/Program Files/Microsoft SDKs/Windows/v7.0A")

# DirectX 8.1 SDK for VC++
# http://download.csdn.net/download/mr_luowei2009/4262718
set(DX8_DIR C:/chenchang/csdn/killtime/DX8_VcSdk)

include_directories(
${WIN_SDK7_DIR}/Include
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
audiosystem.h
camera.h
enemy.h
engine.h
entity.h
explosion.h
font.h
GLTools.h
gui.h
HiResTimer.h
InputSystem.h
md2.h
object.h
oglwindow.h
ogro.h
Particles.h
player.h
resource.h
rocket.h
simpengine.h
sod.h
startupOptions.h
terrain.h
texture.h
tree.h
tree.org.h
types.h
utils.h
vector.h
vtrace.h
world.h
audiosystem.cpp
camera.cpp
engine.cpp
entity.cpp
explosion.cpp
font.cpp
gui.cpp
InputSystem.cpp
md2.cpp
oglwindow.cpp
ogro.cpp
Particles.cpp
player.cpp
rocket.cpp
simpengine.cpp
sod.cpp
startupOptions.cpp
terrain.cpp
texture.cpp
tree.cpp
utils.cpp
vtrace.cpp
winmain.cpp
world.cpp
resources.rc
)

add_executable(${PROJ_NAME} ${FILES})

set_target_properties(${PROJ_NAME}
                      PROPERTIES
		      LINK_FLAGS "/SUBSYSTEM:WINDOWS /SAFESEH:NO")

set_target_properties(${PROJ_NAME} PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

cotire(${PROJ_NAME})

add_custom_command(
   TARGET ${PROJ_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PROJ_NAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Debugging -> Working Directory -> $(ProjectDir)..
