#type vertex
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_VP * u_Model * vec4(a_Position, 1.0);
};

#type fragment
#version 330 core

out vec4 o_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	o_Color = texture(u_Texture, v_TexCoord);
};