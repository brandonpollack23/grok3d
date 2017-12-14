#include "grok3d.h"

#include "../inc/movebackandforthtest.h"

#include <iostream>
#include <memory>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;
using namespace Grok3d::Utilities;

auto HelloTriangleTest(char* args[]) -> void
{
    auto engineInitialization = [args](GRK_EntityComponentManager& ecm) -> GRK_Result
    {
        auto triangleEntity = ecm.CreateEntity();

        float triangleFloats[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0
        };
        auto vertexes = new float[9];
        std::copy(triangleFloats, &triangleFloats[8], vertexes);

        std::copy(triangleFloats, triangleFloats + sizeof(triangleFloats)/sizeof(float), vertexes);

        auto vertexShader =
            ShaderManager::GRK_LoadShaderFile(args[2], ShaderManager::ShaderType::VertexShader);

        auto fragShader =
            ShaderManager::GRK_LoadShaderFile(args[3], ShaderManager::ShaderType::FragmentShader);

        auto shaderProgram = ShaderManager::GRK_CreateShaderProgram({vertexShader, fragShader});
        
        auto rc = GRK_RenderComponent(
                vertexes,
                3,
                sizeof(float),
                GRK_PrimitiveType::Unsigned_Int,
                nullptr,
                0,
                GRK_OpenGLPrimitive::GL_Triangles,
                shaderProgram);

        return triangleEntity.AddComponent(std::move(rc));
    };

    GRK_Engine engine(engineInitialization);
    engine.Run();
}
