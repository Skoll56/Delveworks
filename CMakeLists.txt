cmake_minimum_required(VERSION 2.6)
project(DELVEWORKS)

include_directories(
src
${PROJECT_SOURCE_DIR}/contrib
)

if(MSVC)
	include_directories(
		${PROJECT_SOURCE_DIR}/contrib/msvc-windows-x86/include		
	)

	link_directories(
		${PROJECT_SOURCE_DIR}/contrib/msvc-windows-x86/lib
	)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s USE_SDL=2 -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 --preload-file ../resources -o out.html --shell-file shell.html") 
	set(CMAKE_EXECUTABLE_SUFFIX ".html") 
endif()

add_definitions(-DSDL_MAIN_HANDLED)


add_library(Engine
src/Engine/Core.cpp
src/Engine/Entity.cpp
src/Engine/Camera.cpp
src/Engine/Collision.cpp
src/Engine/Input.cpp
src/Engine/Shader.cpp
src/Engine/stb_image.cpp
src/Engine/Texture.cpp
src/Engine/VertexArray.cpp
src/Engine/VertexBuffer.cpp
src/Engine/Component.cpp
src/Engine/Resource.cpp
src/Engine/PhysicsNew.cpp
src/Engine/MeshRenderer.cpp
src/Engine/Collider.cpp
src/Engine/Light.cpp
src/Engine/RenderTexture.cpp
src/Engine/Exception.cpp
src/Engine/Console.cpp
src/Engine/Surface.cpp

src/Engine/Core.h
src/Engine/Engine.h
src/Engine/Entity.h
src/Engine/Component.h
src/Engine/Light.h
src/Engine/Camera.h
src/Engine/Collision.h
src/Engine/Input.h
src/Engine/Shader.h
src/Engine/stb_image.h
src/Engine/stb_vorbis.h
src/Engine/Texture.h
src/Engine/VertexArray.h
src/Engine/VertexBuffer.h
src/Engine/Resource.h
src/Engine/PhysicsNew.h
src/Engine/MeshRenderer.h
src/Engine/Collider.h
src/Engine/RenderTexture.h
src/Engine/Console.h
src/Engine/WavFile.h
src/Engine/Surface.h
)


if(MSVC OR MINGW)
target_link_libraries(Engine glew32 opengl32 SDL2 openal32)
else()
target_link_libraries(Engine GLEW GL)
endif()



add_executable(demo WIN32
src/demo/main.cpp
)

target_link_libraries(demo Engine)