#version 460 core
out vec4 FragColor;
uniform bool enableStereo;

in vec3 VertexPos; // Переименовали переменную FragPos в VertexPos
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform samplerCube skybox;

uniform bool useCubeMap;
uniform bool useReflections;

uniform int eye; // 0 for left eye, 1 for right eye

void main()
{
    vec3 viewDir = normalize(viewPos - VertexPos); // Изменено на VertexPos
    vec3 lightDir = normalize(lightPos - VertexPos); // Изменено на VertexPos
    vec3 halfwayDir = normalize(viewDir + lightDir);

    float specularStrength = texture(texture_specular1, TexCoords).r;

    float ambientStrength = 0.1;
    float diffuseStrength = max(dot(Normal, lightDir), 0.0);
    float specularValue = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);

    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
    vec3 diffuse = diffuseStrength * vec3(1.0, 1.0, 1.0);
    vec3 specular = specularStrength * vec3(1.0, 1.0, 1.0);

    vec3 lighting = ambient + diffuse + specular;

    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 finalColor = lighting * color;

    if (useReflections && useCubeMap) {
        vec3 reflectionColor = texture(skybox, reflect(VertexPos - viewPos, Normal)).rgb; // Изменено на VertexPos
        finalColor = mix(finalColor, reflectionColor, 0.3);
    }

    if (enableStereo) {
        float xOffset = 0.03; // Adjust this value based on your VR setup
        if (eye == 0) {
            VertexPos.x -= xOffset; // Изменено на VertexPos
        } else {
            VertexPos.x += xOffset; // Изменено на VertexPos
        }
    }

    FragColor = vec4(finalColor, 1.0);
}
