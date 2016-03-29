#version 330 core

out vec4 vFragColor;
in vec4 fColor;
void main()
{
	vFragColor = fColor;	///< 使用插值颜色当做片段颜色
}