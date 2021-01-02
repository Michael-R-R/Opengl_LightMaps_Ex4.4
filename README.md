# Opengl_LightMaps_Ex4.4
Example of light maps - Diffuse/Specular/Emission - Exercise 4.4 from learnopengl's pdf book

Images used from https://learnopengl.com/Lighting/Lighting-maps

EXERCISE DIRECTIONS:
Also add something they call an emission map which is a texture that stores emission values per fragment. Emission values are colors an object may emit as if it contains a light source itself; this way an object can glow regardless of the light conditions. Emission maps are often what you see when objects in a game glow (like the eyes of a robot, or light strips on a container). Add the following texture (by creativesam) as an emission map onto the container as if the letters emit light:

NOTABLE SOURCE CODE CHANGES:
- Change to my Shader class to allow the loading of both .jpg and .png without throwing a "access violation" error due to different channel amounts with the different images.
- Added vector data structure in the Shader class to store all the texture IDs to more easily bind the textures with less code.

![OpenGL_Emission](https://user-images.githubusercontent.com/54217603/103465821-3b50d980-4d0d-11eb-9f08-59f008b2199e.gif)

