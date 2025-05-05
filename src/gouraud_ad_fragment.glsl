#version 460 core

in vec3 lightingColor;   // Cor interpolada do vertex shader
uniform vec3 rgb;        // Cor base do objeto (definida via uniform)

out vec4 FragColor;      // Saída final

void main() {
    // Aplica a cor base (rgb) à iluminação calculada no vértice
    FragColor = vec4(lightingColor * rgb, 1.0);
}