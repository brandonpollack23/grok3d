#version 330 core
layout (location = 0) in vec3 aPos; // The position variable has attribute position 0.
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1.

out vec3 ourColor; // Output color to fragment shader.

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor; // Just forwarding the value.
}
