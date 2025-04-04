#version 460 core

// Inputs from Vextex Shader
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniforms Inputs
uniform sampler2D Texture0;

// Outputs
out vec4 FinalColor;

void main()
{
    FinalColor = texture(Texture0, FragTexCoords); // * vec4(FragColor, 1.0f);
}
