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
	// TODO: Calculate this in CPU and send as an uniform, the inverse operation is expensive to do for every vertex.
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

struct Light {
	vec3 direction;  // This is a spot light at the camera's position, so like a flashlight

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
uniform Light u_Light;  

out vec4 o_Color;

void main()
{
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);

	// Lighting calculations inside the shader expect a vector pointing from the fragment to the source.
	vec3 lightToFragDirection = normalize(-v_FragViewPosition);
	
	// --- Attenuation factor ---
	float distance = length(v_FragViewPosition);
	float distAttenuation = 1.0; // Disabling this temporarily / (u_Light.constAttenuation + u_Light.linearAttenuation * distance + u_Light.quadAttenuation * (distance * distance));

	// Spotlight with soft edges - Cosine of angle from camera (light) to fragment
	float cosTheta = dot(lightToFragDirection, normalize(-u_Light.direction));
	float cosEpsilon = u_Light.innerCutoffCosine - u_Light.outerCutoffCosine;
	float spotLightIntensity = clamp((cosTheta - u_Light.outerCutoffCosine) / cosEpsilon, 0.0, 1.0);

	//  --- Diffuse light factor calculation ---
	vec3 normalVector = normalize(v_Normal);
	// The greater the angle (up to 90) the greater the factor (more light)
	float diffuseFactor = max(dot(normalVector, lightToFragDirection), 0.0);

	// --- Specular light factor calculation ---
	vec3 viewDirection = normalize(-v_FragViewPosition);
	// relflect() expects a vector pointing from the light source to the fragment, so we invert lightToFragDirection
	vec3 reflectionDirection = reflect(-lightToFragDirection, normalVector);
	// Now we calculate the specular component
	float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0), u_Material.shininess);

	// --- Calculate color for each component ---
	diffuseColor = u_Light.diffuse * spotLightIntensity * distAttenuation * diffuseFactor * vec3(texture(u_Material.diffuse, v_TexCoord));
	specularColor = u_Light.specular * spotLightIntensity * distAttenuation * specularFactor * vec3(texture(u_Material.specular, v_TexCoord));
	vec3 ambientColor = u_Light.ambient * distAttenuation * vec3(texture(u_Material.diffuse, v_TexCoord));

	o_Color = vec4(specularColor + ambientColor + diffuseColor, 1.0);
};