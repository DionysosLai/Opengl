#version 330 core

out vec4 vFragColor;
in vec4 fColor;
void main()
{
	vFragColor = fColor;	///< ʹ�ò�ֵ��ɫ����Ƭ����ɫ
}