#version 330 core
layout (location = 0) in vec3 vPosition;

void main()
{
	gl_Position = vec4(0.5*vPosition.x, 0.5*vPosition.y, vPosition.z, 1.0);
}