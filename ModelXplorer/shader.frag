#version 410 core
#extension GL_ARB_separate_shader_objects : enable

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform vec3 ambientColor;   // Color de la luz ambiental
uniform float ambientIntensity; // Intensidad de la luz ambiental

// Textura de sombras
uniform sampler2D shadowMap; // Textura de mapa de sombras
uniform mat4 lightSpaceMatrix; // Matriz de espacio de luz

void main() {
    vec4 texColor = texture(texture_diffuse, TexCoord);

    // Coordenadas de proyección de fragmento en espacio de luz
    vec4 projCoords = lightSpaceMatrix * vec4(gl_FragCoord.xyz, 1.0);

    // Convertir a coordenadas de textura
    vec3 projCoords3D = projCoords.xyz / projCoords.w;
    projCoords3D = projCoords3D * 0.5 + 0.5;

    // Calculamos la distancia de la luz a la superficie del fragmento
    float closestDepth = texture(shadowMap, projCoords3D.xy).r;
    float currentDepth = projCoords3D.z;

    // Calcular la sombra usando una factor de bias
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // Aplicar sombra al color final
    vec3 shadowColor = vec3(0.3); // Color de sombra (ajustar según necesidad)
    vec3 finalColor = texColor.rgb * vec3(ambientColor) * ambientIntensity * (1.0 - shadow) + shadowColor * shadow;

    FragColor = vec4(finalColor, texColor.a);
}
