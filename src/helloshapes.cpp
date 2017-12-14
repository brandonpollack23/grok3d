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
        std::copy(triangleFloats, &triangleFloats[9], vertexes);

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

auto HelloSquareTest(char* args[]) -> void
{
    auto engineInitialization = [args](GRK_EntityComponentManager& ecm) -> GRK_Result
    {
        auto triangleEntity = ecm.CreateEntity();

        // create vertex array
        float squareFloats[] = {
            0.5f,  0.5f,  0.0f, //top right
            0.5f,  -0.5f, 0.0f, //bottom right
            -0.5f, -0.5f, 0.0f, //bottom left
            -0.5f, 0.5f,  0.0f  //top left
        };
        auto vertexes = new float[12];
        std::copy(squareFloats, &squareFloats[12], vertexes);

        // create index order array
        unsigned int squareIndexes[] = {
            0, 1, 3, //top right triangle
            1, 2, 3  //bottom left triangle
        };
        auto indices = new unsigned int[6];
        std::copy(squareIndexes, &squareIndexes[6], indices);

        // load shaders
        auto vertexShader =
            ShaderManager::GRK_LoadShaderFile(args[2], ShaderManager::ShaderType::VertexShader);
        auto fragShader =
            ShaderManager::GRK_LoadShaderFile(args[3], ShaderManager::ShaderType::FragmentShader);
        auto shaderProgram = ShaderManager::GRK_CreateShaderProgram({vertexShader, fragShader});
        
        auto rc = GRK_RenderComponent(
                vertexes,
                4,
                sizeof(float),
                GRK_PrimitiveType::Unsigned_Int,
                indices,
                6,
                GRK_OpenGLPrimitive::GL_Triangles,
                shaderProgram);

        return triangleEntity.AddComponent(std::move(rc));
    };

    GRK_Engine engine(engineInitialization);
    engine.Run();
}
