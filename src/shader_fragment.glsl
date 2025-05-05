#version 460 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(0.8, 0.8, 0.8); // Luz menos intensa
uniform vec3 rgb; // Já substitui objectColor, pois é a cor que você quer controlar

void main() {
    //Luz ambiente
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //Luz difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = 0.7 * diff * lightColor; // Reduzido para 70%
    
    //Luz Especular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.3 * spec * lightColor; // Specular mais suave

    //Combina iluminação com a cor RGB
    vec3 result = (ambient + diffuse + specular) * rgb; // Aqui rgb substitui objectColor

    FragColor = vec4(result, 1.0); // Ignora iluminação
}