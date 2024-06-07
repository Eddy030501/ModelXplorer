#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 color;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;

void main()
{
    // Retrieves the color from the texture using the texture coordinates
    FragColor = texture(tex0, texCoord) * vec4(1.0, 1.0, 1.0, 1.0); // Multiplica por 1.0 para mantener el mismo color
}

