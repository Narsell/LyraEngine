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

uniform vec3 u_Color;
out vec4 o_Color;

void main()
{
	o_Color = vec4(u_Color, 1.0);
};