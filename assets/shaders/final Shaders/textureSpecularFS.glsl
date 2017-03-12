#version 420

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};  

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec3 cameraDirectionOut;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    //Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  	
    //Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
	//vec3 lightDir = vec3(0.0f, 0.0f, 1.0f);
    float diff = max(dot(Normal, lightDir), 0.0);
	float diffuseTerm = dot(Normal, lightDir);
    vec3 diffuse = light.diffuse*diffuseTerm *vec3(texture(material.diffuse, TexCoords));  

	//Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular); 
        
	//color = vec4(ambient + diffuse + specular, 1.0f);  
	color = texture(material.diffuse, TexCoords);
	//color = vec4(TexCoords, 0, 1);
	//color = vec4(Normal, 1);
} 