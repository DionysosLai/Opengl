#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint framentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	/// 读取vectex shader code
	std::ifstream vertexShaderStream(vertex_file_path, std::ios::in);
	std::string vertexShaderCode;
	if (vertexShaderStream.is_open())
	{
		std::string line;
		while (getline(vertexShaderStream, line))
			vertexShaderCode += "\n" + line;
		vertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}


	/// 读取fragment shader code 
	std::ifstream fragmentShaderStream(fragment_file_path, std::ios::in);
	std::string fragmentShaderCode;
	if (fragmentShaderStream.is_open())
	{
		std::string line;
		while (getline(fragmentShaderStream, line))
			fragmentShaderCode += "\n" + line;
		fragmentShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", fragment_file_path);
		getchar();
		return 0;
	}

	/// specify vertex shorce and compile
	const GLchar *vertexCodePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexCodePointer, NULL);
	glCompileShader(vertexShaderID);

	GLint success;
	int infoLogLength;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	/// compile fragment shader
	const GLchar *fragmentCodePointer = fragmentShaderCode.c_str();
	glShaderSource(framentShaderID, 1, &fragmentCodePointer, NULL);
	glCompileShader(framentShaderID);


	glGetShaderiv(framentShaderID, GL_COMPILE_STATUS, &success);
	glGetShaderiv(framentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(framentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}


	/// link the program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, framentShaderID);
	glLinkProgram(programID);


	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0){
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, framentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(framentShaderID);

	return programID;
}

#endif