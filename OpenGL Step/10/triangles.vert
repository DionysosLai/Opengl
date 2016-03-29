#version 330 core
layout (location = 0) in vec3 vPosition;
out vec4 fColor;
void main()
{
	gl_Position = vec4(vPosition, 1.0);
	/// clamp 夹逼函数，因为有些位置是-1~0, 而颜色取值是0~1.0,任何小于0.0的颜色值，都会被渲染为黑色
	fColor = vec4(clamp(vPosition, 0.0, 1.0), 1.0);		/// 根据位置，设置元素颜色
}