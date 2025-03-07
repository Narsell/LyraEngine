#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;
layout(location=2) in vec2 a_TexCoord;

uniform mat4 u_VP;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat3 u_Normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragViewPosition;

void main()
{
	// To ensure that non-uniform scales don't mess with the normal vectors, we multiply
	// the normal vector (a_Normal) with the normal matrix
	// The normal matrix is defined as the transpose of the inverse of the upper 3x3 section of the model matrix
	v_Normal = u_Normal * a_Normal;
	v_FragViewPosition = vec3(u_View * u_Model * vec4(a_Position, 1.0));
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

struct DirLight 
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define N_POINT_LIGHTS 4  

struct PointLight
{
	vec3 position;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constAttenuation;
	float linearAttenuation;
	float quadAttenuation;
};

// This is a spot light at the camera's position, like a flashlight.
// Because we're working in view space instead of world space, the light's direction (equal to the camera's direction) is always (0, 0, -1)
struct SpotLight {	
	vec3 position;
	vec3 direction;

	float innerCutoffCosine; // Inner and outer cutoff 'angles' for this spot light
	float outerCutoffCosine;

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

uniform DirLight u_DirLight;
uniform SpotLight u_SpotLight;
uniform PointLight u_PointLights[N_POINT_LIGHTS];

out vec4 o_Color;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);

	// Diffuse
	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	// Specular
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);

	vec3 diffuse  = light.diffuse  * diffuseFactor * vec3(texture(u_Material.diffuse, v_TexCoord));
	vec3 ambient  = light.ambient  * vec3(texture(u_Material.diffuse, v_TexCoord));
	vec3 specular = light.specular * specularFactor * vec3(texture(u_Material.specular, v_TexCoord));

	return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragmentPos, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragmentPos);

	// Diffuse
	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	// Specular
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);
	// Attenuation
	float distance = length(light.position - fragmentPos);
	float distAttenuation = 1.0 / (light.constAttenuation + light.linearAttenuation * distance + light.quadAttenuation * (distance * distance));

	vec3 ambient  = distAttenuation * light.ambient  * vec3(texture(u_Material.diffuse, v_TexCoord));
	vec3 diffuse  = distAttenuation * light.diffuse  * diffuseFactor * vec3(texture(u_Material.diffuse, v_TexCoord));
	vec3 specular = distAttenuation * light.specular * specularFactor * vec3(texture(u_Material.specular, v_TexCoord));

	return ambient + diffuse + specular;

}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPos)
{
	// Lighting calculations inside the shader expect a vector pointing from the fragment to the source.
	vec3 lightToFragDirection = normalize(light.position - fragmentPos);
	
	// --- Attenuation factor ---
	float dist = length(fragmentPos);
	float distAttenuation = 1.0 / (light.constAttenuation + light.linearAttenuation * dist + light.quadAttenuation * (dist * dist));

	// --- Spotlight cutoff range calculation with soft edges - Cosine of angle from camera (light) to fragment
	float cosTheta = dot(lightToFragDirection, normalize(-light.direction));
	float cosEpsilon = light.innerCutoffCosine - light.outerCutoffCosine;
	float spotLightIntensity = clamp((cosTheta - light.outerCutoffCosine) / cosEpsilon, 0.0, 1.0);

	//  --- Diffuse light factor calculation ---
	// The greater the angle (up to 90) the greater the factor (more light)
	float diffuseFactor = max(dot(normal, lightToFragDirection), 0.0);

	// --- Specular light factor calculation ---
	vec3 viewDirection = normalize(-fragmentPos);
	// reflect() expects a vector pointing from the light source to the fragment, so we invert lightToFragDirection
	vec3 reflectionDirection = reflect(-lightToFragDirection, normal);
	// Now we calculate the specular component
	float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0), u_Material.shininess);

	// --- Calculate color for each component ---
	vec3 ambient  = distAttenuation * light.ambient  * vec3(texture(u_Material.diffuse, v_TexCoord));
	vec3 diffuse  = distAttenuation * light.diffuse  * spotLightIntensity * diffuseFactor * vec3(texture(u_Material.diffuse, v_TexCoord));
	vec3 specular = distAttenuation * light.specular * spotLightIntensity * specularFactor * vec3(texture(u_Material.specular, v_TexCoord));

	return specular + ambient + diffuse;
}

void main()
{

	vec3 normalVector = normalize(v_Normal);
	vec3 viewDirection = normalize(-v_FragViewPosition);

	vec3 result = CalculateDirLight(u_DirLight, normalVector, viewDirection);

	for (int i = 0; i < N_POINT_LIGHTS; i++)
	{
		result += CalculatePointLight(u_PointLights[i], normalVector, v_FragViewPosition, viewDirection);
	}

	result += CalculateSpotLight(u_SpotLight, normalVector, v_FragViewPosition);

	o_Color = vec4(result, 1.0);

};