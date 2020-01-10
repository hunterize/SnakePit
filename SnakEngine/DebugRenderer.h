#pragma once

#include "GLSLProgram.h"
#include "Vertex.h"
#include <glm\glm.hpp>
#include <vector>

namespace SnakEngine {

	struct DebugVertex
	{
		glm::vec2 position;
		Color color;
	};

	class CDebugRenderer
	{
	public:
		CDebugRenderer();
		~CDebugRenderer();

		void Init();
		void Dispose();
		void DrawBox(const glm::vec4& destRect, const Color& color, float angle);
		void DrawCircle(const glm::vec2& center, const Color& color, float radius);
		void End();
		void Render(const glm::mat4& projectMatrix, float lineWidth);

	private:
		std::vector<DebugVertex> m_debugVertexes;
		std::vector<GLuint> m_indices;
		GLuint m_vbo = 0;
		GLuint m_vao = 0;
		GLuint m_ibo = 0;
		int m_iElements = 0;
		GLSLProgram m_cShader;

		glm::vec2 RotatePoint(const glm::vec2& pos, float angle);
	};
}

