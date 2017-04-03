#version 420

struct Material {
    sampler2D text;
	vec3 ambientCol;
	vec3 diffuseCol;
    vec3 specularCol;
    float shininess;
};  

struct Light {
    vec3 direction;
    vec3 position;
    vec3 ambientCol;
    vec3 diffuseCol;
	vec3 specularCol;
	float SpecularPow;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec3 cameraDirectionOut;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material mat;
uniform Light lig;

void main()
{
    //Ambient
    vec3 ambient = (0.1f * lig.ambientCol) * vec3(texture(mat.text, TexCoords));
  	
    //Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lig.position - FragPos);
	float diffuseTerm = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = lig.diffuseCol * diffuseTerm * vec3(texture(mat.text, TexCoords));  

	//Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	vec3 specular = lig.specularCol * (spec * mat.specularCol); 
        
	//final
	color = vec4(ambient + diffuse + specular, 1.0f);  
} 