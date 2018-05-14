# Introduction to Kill Time

**Kill Time** is a small 3D FPS game developed in my spare time.

**Control Keys:**

+ Up Key / W: Move forward
+ Down Key / S: Move backwoard
+ Left Key / A: Move left
+ Right Key / D: Move right
+ Left Mouse Button: Fire
+ Escape Key: Exit

**Language/Tools:** C/C++, OpenGL(Graphics), DirectX(Sound, Input), MS Visual C++ 2015, cmake, git

**Download url:** https://github.com/Jason2013/kill_time/files/1998613/killtime.zip

**Source Code:** https://github.com/Jason2013/kill_time

**Features:**
+	3D model loading and rendering
+	Particle system for explosion
+	Dynamic collision detection
+	Random map generation
+	Transparency effect
+	A completed game engine

**Screenshots:**

![Screen Shot 1](https://github.com/Jason2013/killtime-opt/raw/master/docs/shoot1.jpg)

![Screen Shot 2](https://github.com/Jason2013/killtime-opt/raw/master/docs/shoot2.jpg)

# Build from source code

1. **Clone the source code from github**:

```
git clone https://github.com/Jason2013/kill_time.git
```

2. **Make a directory for build**: 

```
cd <SRC>
mkdir build
```

3. **Generate `Visual Studio 2015` solution and project files by `CMake`**:

```
cd build
cmake .. -G"Visual Studio 14 2015"
```

4. **Open the generated solution file by `Visual Studio 2015` and build**. That's all, enjoy!



