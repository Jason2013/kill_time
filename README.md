# Introduction to Kill Time

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/14567ea5c7e44c34a64d2aa3bf6f7611)](https://www.codacy.com/app/Jason2013/kill_time?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Jason2013/kill_time&amp;utm_campaign=Badge_Grade)

**Kill Time** is a small 3D FPS game.

**Control Keys:**

+ Up Key / W: Move forward
+ Down Key / S: Move backwoard
+ Left Key / A: Move left
+ Right Key / D: Move right
+ Left Mouse Button: Fire
+ Right Mouse Button: Switch sniperscope
+ F2 Key: Switch weapon between rocket and rifle
+ Escape Key: Exit

**Language/Tools:** C/C++, OpenGL(Graphics), DirectX(Sound, Input), MS Visual C++ 2015/2017, cmake, git

**Download url:** https://github.com/Jason2013/kill_time/releases/latest

**Source Code:** https://github.com/Jason2013/kill_time

**Features:**
+	3D model loading and rendering
+	Particle system for explosion
+	Dynamic collision detection
+	Random map generation
+	Transparency effect
+	A complete game engine

**Screenshots:**

![Screen Shot 1](https://github.com/Jason2013/kill_time/blob/master/docs/attack0.png)

![Screen Shot 2](https://github.com/Jason2013/kill_time/blob/master/docs/attack1.png)

![Screen Shot 3](https://github.com/Jason2013/kill_time/blob/master/docs/attack2.png)

**Screencast:**

[Watch on Youtube](https://youtu.be/KJ1-DIhSaL0)

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
cmake -G"Visual Studio 14 2015" ..
```

4. **Open the generated solution file by `Visual Studio 2015` and build**. That's all, enjoy!
