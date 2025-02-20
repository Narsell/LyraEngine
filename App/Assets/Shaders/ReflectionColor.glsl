#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_VP * u_Model * vec4(a_Position, 1.0);
};

#type fragment
#version 330 core

out vec4 o_Color;

uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;

void main()
{
	o_Color = u_LightColor * u_ObjectColor;
};