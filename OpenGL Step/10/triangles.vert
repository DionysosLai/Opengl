#version 330 core
layout (location = 0) in vec3 vPosition;
out vec4 fColor;
void main()
{
	gl_Position = vec4(vPosition, 1.0);
	/// clamp �бƺ�������Ϊ��Щλ����-1~0, ����ɫȡֵ��0~1.0,�κ�С��0.0����ɫֵ�����ᱻ��ȾΪ��ɫ
	fColor = vec4(clamp(vPosition, 0.0, 1.0), 1.0);		/// ����λ�ã�����Ԫ����ɫ
}