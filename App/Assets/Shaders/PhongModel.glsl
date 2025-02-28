#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;
layout(location=2) in vec2 a_TexCoord;

uniform mat4 u_VP;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat3 u_Normal;
uniform vec3 u_LightPosition;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragViewPosition;
out vec3 v_LightPosition;

void main()
{
	// To ensure that non-uniform scales don't mess with the normal vectors, we multiply
	// the normal vector (a_Normal) with the normal matrix
	// The normal matrix is defined as the transpose of the inverse of the upper 3x3 section of the model matrix
	// TODO: Calculate this in CPU and send as an uniform, the inverse operation is expensive to do for every vertex.
	v_Normal = u_Normal * a_Normal;
	v_FragViewPosition = vec3(u_View * u_Model * vec4(a_Position, 1.0));
	v_LightPosition = vec3(u_View * vec4(u_LightPosition, 1.0));
	v_TexCoord = a_TexCoord;

	gl_Position = u_VP * u_Model * vec4(a_Position, 1.0);
};

#type fragment
#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
	vec3 position; // This is a point light with attenuation

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constAttenuation;
	float linearAttenuation;
	float quadAttenuation;
};

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragViewPosition;

uniform Material u_Material;
uniform Light u_Light;  

out vec4 o_Color;

void main()
{
	// --- Attenuation factor ---
	float distance = length(u_Light.position - v_FragViewPosition);
	float distAttenuation = 1 / (u_Light.constAttenuation + u_Light.linearAttenuation * distance + u_Light.quadAttenuation * (distance * distance));

	//  --- Diffuse light factor calculation ---
	vec3 normalVector = normalize(v_Normal);
	// Lighting calculations inside the shader expect a vector pointing from the fragment to the source.
	vec3 lightDirection = normalize(u_Light.position - v_FragViewPosition);
	// The greater the angle (up to 90) the greater the factor (more light)
	float diffuseFactor = max(dot(normalVector, lightDirection), 0.0);

	// --- Specular light factor calculation ---
	vec3 viewDirection = normalize(-v_FragViewPosition);
	// relflect() expects a vector pointing from the light source to the fragment, so we invert lightDirection
	vec3 reflectionDirection = reflect(-lightDirection, normalVector);
	// Now we calculate the specular component
	float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0), u_Material.shininess);

	// --- Calculate color for each component ---
	vec3 ambientColor = u_Light.ambient * distAttenuation * vec3(texture(u_Material.diffuse, v_TexCoord));
	vec3 diffuseColor = u_Light.diffuse * distAttenuation * diffuseFactor * vec3(texture(u_Material.diffuse, v_TexCoord));
	vec3 specularColor = u_Light.specular * distAttenuation * specularFactor * vec3(texture(u_Material.specular, v_TexCoord));

	o_Color = vec4(specularColor + ambientColor + diffuseColor, 1.0);
};