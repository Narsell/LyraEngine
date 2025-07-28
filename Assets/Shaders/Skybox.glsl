#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec3 v_TexCoords; 

void main()
{
    v_TexCoords = a_Position;
	gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
};

#type fragment
#version 330 core
in vec3 v_TexCoords; 

uniform samplerCube u_Skybox;
out vec4 o_Color;

void main()
{
	o_Color = texture(u_Skybox, v_TexCoords);
};
