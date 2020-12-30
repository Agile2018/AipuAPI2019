#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

//uniform mat4 u_MVP;

void main()
{
	
	gl_Position = vec4(position, 1.0f); //u_MVP * 
	ourColor = color;
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
};

#shader fragment
#version 330 core

in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture;
void main()
{	
	color = texture(ourTexture, TexCoord);
};