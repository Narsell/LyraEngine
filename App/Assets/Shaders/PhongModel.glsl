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

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragViewPosition;
in vec3 v_LightPosition;

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec4 u_LightColor;
uniform float u_AmbientStrenght;
uniform float u_SpecularStrenght;
uniform float u_ShininessFactor;

void main()
{
	//  --- Diffuse light calculation ---
	vec3 normalVector = normalize(v_Normal);
	vec3 lightDirection = normalize(v_LightPosition - v_FragViewPosition);

	// The greater the angle (up to 90) the greater the factor (more light)
	float diffuseFactor = max(dot(lightDirection, normalVector), 0.0);

	// We apply the factor but leave the alpha out of it
	vec3 diffuseColor = diffuseFactor * vec3(u_LightColor);

	// --- Specular light calculation ---
	vec3 viewDirection = normalize(-v_FragViewPosition);
	// relflect() expects a vector pointing from the light source to the fragment, so we invent lightDirection
	vec3 reflectionDirection = reflect(-lightDirection, normalVector);
	// Now we calculate the specular component
	float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0), u_ShininessFactor);
	vec3 specularColor = u_SpecularStrenght * specularFactor * vec3(u_LightColor);

	// --- Ambient light calculation ---
	vec3 ambientColor = u_AmbientStrenght * vec3(u_LightColor);

	o_Color = vec4(specularColor + ambientColor + diffuseColor, u_LightColor.a) * texture(u_Texture, v_TexCoord);
};