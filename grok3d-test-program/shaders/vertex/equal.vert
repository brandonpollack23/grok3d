#version 330 core
layout (location = 0) in vec3 aPos; // The position variable has attribute position 0.

out vec4 vertexColor; // Specify a color output to the frag shader.

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = vec4((aPos.x + 1) / 2, (aPos.y + 1) / 2, (aPos.z + 1)/2, 1.0);
}
