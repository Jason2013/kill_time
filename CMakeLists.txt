cmake_minimum_required(VERSION 2.8)
set(PROJ_NAME KILL_TIME)
project(${PROJ_NAME})

include_directories(
"C:/Program Files/Microsoft Visual Studio 14.0/VC/include"
"C:/Program Files/Microsoft SDKs/Windows/v7.0A/Include"
"C:/chenchang/csdn/killtime/DX8_VcSdk/include"
)

link_libraries(
opengl32.lib
glu32.lib
winmm.lib
C:/chenchang/csdn/killtime/DX8_VcSdk/lib/dxguid.lib
C:/chenchang/csdn/killtime/DX8_VcSdk/lib/dinput8.lib
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
stdafx.h
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
stdafx.cpp
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

# Linker -> Advanced -> Image Has Safe Exception Handlers: No (/SAFESEH:NO)
# Linker -> System -> SubSystem: Windows (/SUBSYSTEM:WINDOWS)
# Debugging -> Working Directory -> $(ProjectDir)..
# Precompiled header files: stdafx.h stdafx.cpp
# 整理头文件、Lib文件
