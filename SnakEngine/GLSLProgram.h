#pragma once

#include <string>
#include <GLEW/glew.h>

namespace SnakEngine
{
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void CompileShadersRaw(const char* vertexRaw, const char* fragmentRaw);
		void LinkShaders();
		void AddAttribute(const std::string& attributeName);

		GLint GetUniformLocation(const std::string& uniformName);

		void use();
		void unuse();

		void Dispose();

	private:
		void CompileSingleShader(const char* source, const std::string& name, GLuint id);
		GLuint m_iProgramID;
		GLuint m_iVertexShaderID;
		GLuint m_iFragmentShaderID;
		int m_iNumAttributes;
	};

}