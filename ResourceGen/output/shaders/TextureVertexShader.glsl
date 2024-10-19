#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexPos;
layout (location = 3) in float aTexIndex;
layout (location = 4) in float aMatrixIndex;

out vec4 pixelColor;
out vec2 textureCoord;
out float texIndex;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 posMatrix[200];

void main()
{
	
	if(aMatrixIndex == -1)
		gl_Position = proj*view*vec4(aPos, 1.0);
	else
		gl_Position = proj*view*posMatrix[int(aMatrixIndex)]*vec4(aPos, 1.0);

	textureCoord = aTexPos;
	texIndex = aTexIndex;
	pixelColor = aColor;
}