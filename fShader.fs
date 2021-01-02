#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 view_position;
uniform Material material;
uniform Light light;


void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light.position - FragPos);
    float difference = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * difference * vec3(texture(material.diffuse, TexCoords));

    // Specular
    vec3 view_dir = normalize(view_position - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // Emission
    vec3 emission = vec3(0.0);
    if(texture(material.specular, TexCoords).rgb == 0.0) // Check if there is no RGB value (black)
    {
        // Draw the emission if coords. are over black fragments
        // in the specular map
        emission = vec3(texture(material.emission, TexCoords));
    }

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0f);
}