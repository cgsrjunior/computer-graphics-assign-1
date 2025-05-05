#version 460 core

layout (location = 0) in vec3 aPos;    // Posição do vértice
layout (location = 1) in vec3 aNormal; // Normal do vértice

uniform mat4 model;      // Matriz de modelo
uniform mat4 view;       // Matriz de visualização
uniform mat4 projection; // Matriz de projeção
uniform vec3 lightPos;   // Posição da luz (espaço do mundo)
uniform vec3 lightColor; // Cor da luz (ex: branco = vec3(1.0))

out vec3 lightingColor;  // Cor calculada (passada para o fragment shader)

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));
    
    vec3 normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    
    // --- CÁLCULOS DE ILUMINAÇÃO (NO VÉRTICE) ---
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    lightingColor = ambient + diffuse;
}