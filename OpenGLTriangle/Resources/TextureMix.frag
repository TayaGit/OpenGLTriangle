#version 460 core

// Inputs from Vextex Shader
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniforms Inputs
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// Outputs
out vec4 FinalColor;

void main()
{
    FinalColor = mix(texture(Texture0, FragTexCoords), texture(Texture1, FragTexCoords), 0.5f);
}
