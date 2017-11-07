# BaoBUI
BaoBUI is an interfaced meant to render in a 3D environment the movements executed by the BaoBOS robotic arm.

[![BAOBOS VIDEO](https://imgur.com/afMjANT.png)](https://vimeo.com/236781582)

## GLFW
BaoBUI uses GLFW, an open-source platform for OpenGL.

## GLSL
GLSL is used as a shading langage.  
Due to the current lack of lighting, shaders are currently mainly used to store texture data.  

Most textures are taken from the [Portal Game Series](http://store.steampowered.com/app/620/Portal_2/), with exception for the [Companion Cube](https://orig00.deviantart.net/9efb/f/2011/172/2/2/companion_cube_face_by_bulbablaaah-d3jl8wz.png).

## SFML
BaoBUI uses responses and ambient audio from the [Portal Game Series](http://www.portal2sounds.com/).  
All sound is buffered and played via SFML.

## Controls
<kbd>UP</kbd>: Increase `Y-Coordinate`  
<kbd>DOWN</kbd>: Decrease `Y-Coordinate`  
<kbd>RIGHT</kbd>: Increase `X-Coordinate`  
<kbd>LEFT</kbd>: Decrease `X-Coordinate`  
<kbd>W</kbd>: Decrease `Z-Coordinate`   
<kbd>D</kbd>: Increase `Z-Coordinate`  
<kbd>T</kbd>: Positive `Yaw`  
<kbd>R</kbd>: Negative `Yaw`  
<kbd>T</kbd>: Positive `Pitch`  
<kbd>R</kbd>: Negative `Picth`  
<kbd>U</kbd>: Unlock Camera  
<kbd>L</kbd>: Lock Camera  

Note: Lock camera forces all X,Y,Z movements to be within the following bounds:  
`X witin [-10, 10]`, `Y within [-10, 10]`, `Z within [5, 15]`.

## Dependencies
`OpenGL`: minimum version required `3.3`.  
`GLFW`: minimum version required `3.2.x`.  
`SFML`: minimum version required `2.4.x`.

## Images
![BaoBUI #1](https://imgur.com/ScEpir3.png)
![BaoBUI #2](https://imgur.com/NA4Kxav.png)
