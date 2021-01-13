There is a compiled "demo.exe" program (from release mode) in the Build2 folder. That's Build2/demo.exe.
The camera can be moved by moving the mouse and using WSAD.

The source code can be found in src/demo and src/Engine.


To find elements relevant to the Graphics unit open the solution and use Cntrl+F to search for "GRAPHICS UNIT".
The files of note should be:

Shader.h & Shader.cpp
Core.h & Core.cpp
RenderTexture.h & RenderTexture.cpp
Light.h & Light.cpp
main.cpp

And in Resources/Shaders:
UI.frag
dirShadow.frag & dirShadow.vert
pointShadow.frag & pointShader.vert
Light.frag 

(IMPORTANT NOTE: The actual lighting here has been adapted from previously submitted work, only the addition of shadows is being submitted, not any code involved in casting light generally. I have marked in the code what parts are being submitted and what are not).


Acknowledgements:
Howard Van Waard and Karl Ward (General assistance)
Myself (This engine is built on code that I have submitted previously for other units or intend to submit in future. Only the new features are being submitted for marking)

References:
https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping |Most of the shadowmapping, cubemapping and optimising shader code has been adapted from this website.

https://sketchfab.com/models/f2b2894395864d49b12ca0065f30ade2?ref=related#download |The random statue of C'thulhu was obtained
from here.

https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing |One of the images in the video was taken from this website.

http://www.sonic.net/~surdules/articles/cg_shadowvolumes/index.html |One of the images in the video was taken from this website.