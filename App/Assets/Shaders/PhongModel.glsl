#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;

uniform mat4 u_VP;
uniform mat4 u_Model;

out vec3 v_Normal;
out vec3 v_FragWorldPosition;

void main()
{
	// To ensure that non-uniform scales don't mess with the normal vectors, we multiply
	// the normal vector (a_Normal) with the normal matrix
	// The normal matrix is defined as the transpose of the inverse of the upper 3x3 section of the model matrix
	// TODO: Calculate this in CPU and send as an uniform, the inverse operation is expensive to do for every vertex.
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	v_FragWorldPosition = vec3(u_Model * vec4(a_Position, 1.0));

	gl_Position = u_VP * u_Model * vec4(a_Position, 1.0);
};

#type fragment
#version 330 core

in vec3 v_Normal;
in vec3 v_FragWorldPosition;

out vec4 o_Color;

uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

void main()
{
	//  --- Diffuse light calculation ---
	vec3 normalVector = normalize(v_Normal);
	vec3 lightDirection = normalize(u_LightPosition - v_FragWorldPosition);

	// The greater the angle (up to 90) the greater the factor (more light)
	float diffuseFactor = max(dot(lightDirection, normalVector), 0.0);

	// We apply the factor but leave the alpha out of it
	vec3 diffuseColor = diffuseFactor * vec3(u_LightColor);

	// --- Specular light calculation ---
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(u_CameraPosition - v_FragWorldPosition);
	// relflect() expects a vector pointing from the light source to the fragment, so we invent lightDirection
	vec3 reflectionDirection = reflect(-lightDirection, normalVector);
	// Now we calculate the specular component
	float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
	vec3 specularColor = specularStrength * specularFactor * vec3(u_LightColor);

	// --- Ambient light calculation ---
	float ambientStrenght = 0.1;
	vec3 ambientColor = ambientStrenght * vec3(u_LightColor);

	o_Color = vec4(specularColor + ambientColor + diffuseColor, u_LightColor.a) * u_ObjectColor;
};