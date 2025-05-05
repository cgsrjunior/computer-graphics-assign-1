#version 460 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;

out vec3 lightingColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));
    vec3 normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    // Ambient (fraco para destacar os outros componentes)
    vec3 ambient = 0.05 * lightColor;

    // Diffuse (intensidade alta)
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = 0.9 * diff * lightColor; // 90% de intensidade

    // Specular (brilho bem visível)
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0); // Expoente alto para brilho concentrado
    vec3 specular = 1.0 * spec * lightColor; // 100% de intensidade

    lightingColor = ambient + diffuse + specular;
}