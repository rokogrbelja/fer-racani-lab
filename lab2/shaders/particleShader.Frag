#version 330 core

in vec2 texCoords;

out vec4 fColor;

uniform sampler2D texture;

uniform float alpha;

void main() 
{ 
    fColor = texture2D(texture, texCoords);
	fColor.a = alpha;
} 