# OpenGL-Cpp-ObjC-Mac
My educational project: usage of OpenGL & C++ with Objective-C wrapper for Mac OS X.

## Functionality
- 3D scene using **OpenGL v4.1** and libraries: GLM, FreeImage, FreeType, Assimp.
- Drawing is based on `VBO`, `VAO` objects with several shaders of different kinds and various approaches: index drawing,
 by triangles, squares, etc.
- 3 types of **lights**: directional light, point light, spot light.
- Textures loading with **FreeImage v3.17**.
- Models loading with **Assimp v4.0.0**: `.3ds`, `.obj` files with multiple meshes and textures.
- Available scene objects: textured plain, textured box, sky box with 2 textures, loaded models, etc.
- 3 kinds of **fog**: lineal, exponential, bi-exponential. In addition, it uses **Simplex Noise** and **Fractional Brownian Motion**,
 so, the result looks fantastic!
- **Normal maps** are supported.
- Time of day and Sun are implemented: see sunrises and sunsets.
- Native Mac OS control: use **TrackPad** to move and rotate the camera.
- **Particle systems** are implemented with geometry shaders.
- **Terrain** object is created based on height map texture.
- Textured **Torus generation**: you can easily create a donut, a spring, a spiral, or even a spriral spring.
- Text output implemented with **FreeType v2.9**.
- **ScreenShots** saving based on copying bytes from screen buffer and saving with native Mac OS `NSBitmapImageRep`.


## The control
- **Arrows, W/A/S/D** – move though the scene.
- **Scale on TrackPad** – move though the scene.
- **Move on TrackPad** – rotate the camera.
- **Space** – stop the time.
- **Tab** – change fog type.
- **Esc** – make a screenshot.
- **Backspace** – turn normal maps on/off.
- **Enter** – turn the lights on/off.


---
## Time of day

Direction of directional light depends on the day time. Sun's position and color too.

Sunrise:

![sunrise](https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/sunrise.jpg)

Sunset:

![sunset](https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/sunset.jpg)


---
## Loaded models

![wolves](https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/m1.jpeg)

![house](https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/m2.jpeg)


---
## Normal maps

While usual textures describe the color of an object, normal maps describe its relief, roughness.
Here you can see box texture and its normal map:

<img src="https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/OpenGL-Cpp-Mac/SceneAssets/Textures/box.jpg" width="50%"/><img src="https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/OpenGL-Cpp-Mac/SceneAssets/Textures/box_n.jpg" width="50%"/>

Scene with normal maps:

![bon](https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/bon.jpg)

And without normal maps:

![bof](https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/bof.jpg)

[This](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/)
and [this](http://www.thetenthplanet.de/archives/1180) articles helped me to write the code for in-place
Tangents and Cotangents calculation.


---
## Terrain with height map

The implementation is here: [ItemMap.cpp](https://github.com/AivanF/OpenGL-Cpp-ObjC-Mac/blob/master/OpenGL-Cpp-Mac/Cpp/Items/ItemMap.cpp)
The texture:

![path](https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/OpenGL-Cpp-Mac/SceneAssets/Textures/path.jpg)

The results:

![ter](https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/s1.jpg)


---
## Dynamic fog

3 kinds of fog are implemented: lineal, exponential, bi-exponential. In addition, it uses
 [Simplex Noise](https://en.wikipedia.org/wiki/Simplex_noise)
 and [Fractional Brownian Motion](https://en.wikipedia.org/wiki/Fractional_Brownian_motion)
 algorithms with time, distance, and output X/Y as parameters.
 So, the result look great! Especially during the runtime and camera moving.

The implementation is here: [full.frag](https://github.com/AivanF/OpenGL-Cpp-ObjC-Mac/blob/master/OpenGL-Cpp-Mac/SceneAssets/Shaders/full.frag)

<img src="https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/f1.jpg" width="50%"/><img src="https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/f3.jpeg" width="50%"/>

![fog](https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/s2.jpg)


---
## Torus generation

My algorithm is very flexible and is capable of creation a model of any torus, srping, spiral, or a combination of them.
 It takes the following parameters:

- int **cntin** – verticis count in inner circle.
- int **cntout** – verticis count in outer circle.
- float **radin** – radius of inner circle.
- float **radout** – radius of outer circle.
- int **turnscnt** - count of turns.
- float **addheight** - additional height.
- float **addwidth** - additional width.
- float **texin** - number of textures around inner circle. Negative value means separate texture for each square.
- float **texout** - number of textures around outer circle. Negative value means separate texture for each square.

The implementation of the algorithm is available here: [ItemTorus.cpp](https://github.com/AivanF/OpenGL-Cpp-ObjC-Mac/blob/master/OpenGL-Cpp-Mac/Cpp/Items/ItemTorus.cpp)

Here you can see the obtained toruses with and w\o normal maps:

<img src="https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/t1on.jpg" width="50%"/><img src="https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/t1of.jpg" width="50%"/>

<img src="https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/t2on.jpg" width="50%"/><img src="https://raw.githubusercontent.com/AivanF/OpenGL-Cpp-ObjC-Mac/master/img/t2of.jpg" width="50%"/>

This algorithm was also used in my other project: [Own 3D Engine](https://github.com/AivanF/own-3d-engine) (no OpenGL, just C++ & Qt)

![toruses](http://aivanf.com/static/toruses.png)

---
# License

This code is provided 'as-is', without any express or implied warranty.
 You may not hold the author liable.
 
Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:
 
1. The origin of this code must not be misrepresented. You must not claim
 that you wrote the original software. When use the code, you must give appropriate credit,
 provide an active link to the original software, and indicate if changes were made.
 
2. Altered source versions must be plainly marked as such, and must not be misrepresented
 as being the original software.
 
3. This notice may not be removed or altered from any source distribution.
