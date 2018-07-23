cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(grok3dtests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

include_directories(grok3d/glm)
include_directories(grok3d/glm/glm/detail)
include_directories(grok3d/glm/glm/ext)
include_directories(grok3d/glm/glm/gtc)
include_directories(grok3d/glm/glm/gtx)
include_directories(grok3d/glm/glm/simd)
include_directories(grok3d/grok3d/inc)
include_directories(grok3d/grok3d/inc/Component)
include_directories(grok3d/grok3d/inc/Entity)
include_directories(grok3d/grok3d/inc/System)
include_directories(grok3d/grok3d/inc/utilities)
include_directories(grok3d/include)
include_directories(grok3d/include/glad)
include_directories(grok3d/include/KHR)
include_directories(grok3d/notstd)
include_directories(grok3d/notstd/notstd)
include_directories(inc)
include_directories(GLFW)

link_libraries(glfw3)

add_executable(grok3dtests
        .idea/codeStyles/Project.xml
        .idea/grok3dtests.iml
        .idea/misc.xml
        .idea/modules.xml
        .idea/vcs.xml
        .idea/workspace.xml
        grok3d/grok3d/inc/Component/ComponentHandle.h
        grok3d/grok3d/inc/Component/GameLogicComponent.h
        grok3d/grok3d/inc/Component/RenderComponent.h
        grok3d/grok3d/inc/Component/TransformComponent.h
        grok3d/grok3d/inc/Entity/EntityHandle.h
        grok3d/grok3d/inc/System/GameLogicSystem.h
        grok3d/grok3d/inc/System/RenderSystem.h
        grok3d/grok3d/inc/System/System.h
        grok3d/grok3d/inc/System/SystemManager.h
        grok3d/grok3d/inc/utilities/ShaderManager.h
        grok3d/grok3d/inc/Engine.h
        grok3d/grok3d/inc/EntityComponentManager.h
        grok3d/grok3d/inc/grok3d.h
        grok3d/grok3d/inc/grok3d_types.h
        grok3d/grok3d/src/Component/GameLogicComponent.cpp
        grok3d/grok3d/src/Component/RenderComponent.cpp
        grok3d/grok3d/src/Component/TransformComponent.cpp
        grok3d/grok3d/src/System/GameLogicSystem.cpp
        grok3d/grok3d/src/System/RenderSystem.cpp
        grok3d/grok3d/src/System/System.cpp
        grok3d/grok3d/src/System/SystemManager.cpp
        grok3d/grok3d/src/utilities/ShaderManager.cpp
        grok3d/grok3d/src/Engine.cpp
        grok3d/grok3d/src/glad.cpp
        grok3d/grok3d/grok3d.licenseheader
        grok3d/grok3d/grok3d.vcxproj
        grok3d/grok3d/grok3d.vcxproj.filters
        grok3d/grok3d/houndconfig.json
        grok3d/grok3d/Makefile
        grok3d/include/glad/glad.h
        grok3d/include/KHR/khrplatform.h
        grok3d/notstd/notstd/bidir_map.h
        grok3d/notstd/notstd/notstdtypetraits.h
        grok3d/notstd/notstd/tupleextensions.h
        grok3d/.gitattributes
        grok3d/.gitignore
        grok3d/.gitmodules
        grok3d/Doxyfile
        grok3d/grok3d.sln
        grok3d/LICENSE
        grok3d/README.md
        inc/helloshapes.h
        inc/movebackandforthtest.h
        shaders/fragment/orange.frag
        shaders/vertex/equal.vert
        src/helloshapes.cpp
        src/main.cpp
        src/movebackandforthtest.cpp
        .gitignore
        .gitmodules
        LICENSE
        Makefile)

file(COPY shaders DESTINATION ./)
