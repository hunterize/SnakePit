#include "DebugRenderer.h"

namespace {
	const char* VERTEXSHADER = R"(
		#version 130
		//The vertex shader operates on each vertex

		//input data from the VBO. Each vertex is 2 floats
		in vec2 vertexPosition;
		in vec4 vertexColor;

		out vec2 fragmentPosition;
		out vec4 fragmentColor;

		uniform mat4 P;

		void main() 
		{
			//Set the x,y position on the screen
			gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
			//the z position is zero since we are in 2D
			gl_Position.z = 0.0;
    
			//Indicate that the coordinates are normalized
			gl_Position.w = 1.0;
    
			fragmentPosition = vertexPosition;
    
			fragmentColor = vertexColor;
		}
	)";

	const char* FRAGMENTSHADER = R"(
		#version 130
		//The fragment shader operates on each pixel in a given polygon

		in vec2 fragmentPosition;
		in vec4 fragmentColor;

		//This is the 3 component float vector that gets outputted to the screen
		//for each pixel.
		out vec4 color;

		void main() 
		{
			color = fragmentColor;
		}
	)";
}



namespace SnakEngine {

	const float PI = 3.1415926535f;

	CDebugRenderer::CDebugRenderer()
	{
	}


	CDebugRenderer::~CDebugRenderer()
	{
		Dispose();
	}

	void CDebugRenderer::Init()
	{
		m_cShader.CompileShadersRaw(VERTEXSHADER, FRAGMENTSHADER);
		m_cShader.AddAttribute("vertexPosition");
		m_cShader.AddAttribute("vertexColor");
		m_cShader.LinkShaders();

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

		glBindVertexArray(0);

	}
	void CDebugRenderer::Dispose()
	{
		if (m_vao)
		{
			glDeleteVertexArrays(1, &m_vao);
		}
		if (m_vbo)
		{
			glDeleteBuffers(1, &m_vbo);
		}
		if (m_ibo)
		{
			glDeleteBuffers(1, &m_ibo);
		}

		m_cShader.Dispose();

	}

	glm::vec2 CDebugRenderer::RotatePoint(const glm::vec2& pos, float angle)
	{
		glm::vec2 tmpPos;
		tmpPos.x = pos.x * cos(angle) - pos.y * sin(angle);
		tmpPos.y = pos.x * sin(angle) + pos.y * cos(angle);
		return tmpPos;
	}

	void CDebugRenderer::DrawBox(const glm::vec4& destRect, const Color& color, float angle)
	{
		//add 4 more vertexes for the box 
		int size = m_debugVertexes.size();
		m_debugVertexes.resize(size + 4);

		glm::vec2 halfDimension(destRect.z / 2.0f, destRect.w / 2.0f);

		//get points centered at origin
		glm::vec2 topleft(- halfDimension.x, halfDimension.y);
		glm::vec2 bottomleft(- halfDimension.x, - halfDimension.y);
		glm::vec2 bottomright(halfDimension.x, - halfDimension.y);
		glm::vec2 topright(halfDimension.x, halfDimension.y);

		glm::vec2 offset(destRect.x, destRect.y);

		m_debugVertexes[size].position = RotatePoint(topleft, angle) + halfDimension + offset;
		m_debugVertexes[size + 1].position = RotatePoint(bottomleft, angle) + halfDimension + offset;
		m_debugVertexes[size + 2].position = RotatePoint(bottomright, angle) + halfDimension + offset;
		m_debugVertexes[size + 3].position = RotatePoint(topright, angle) + halfDimension + offset;

		for (int i = size; i < size + 4; i++)
		{
			m_debugVertexes[i].color = color;
		}

		//add 8 more indices for the box vertexes
		m_indices.reserve(m_debugVertexes.size() + 8);

		m_indices.push_back(size);
		m_indices.push_back(size + 1);

		m_indices.push_back(size + 1);
		m_indices.push_back(size + 2);

		m_indices.push_back(size + 2);
		m_indices.push_back(size + 3);

		m_indices.push_back(size + 3);
		m_indices.push_back(size);

	}
	void CDebugRenderer::DrawCircle(const glm::vec4& destRect, const Color& color, float radius)
	{
		static const int CIRCLEVERTEXESNUM = 100;
		int size = m_debugVertexes.size();

		m_debugVertexes.resize(size + CIRCLEVERTEXESNUM);
		//add 100 more vertexes
		for (int i = size; i < size + CIRCLEVERTEXESNUM; i++)
		{
			float angle = ((float)i / CIRCLEVERTEXESNUM) * PI * 2.0f;
			m_debugVertexes[i].position.x = cos(angle) * radius + destRect.x;
			m_debugVertexes[i].position.y = sin(angle) * radius + destRect.y;
			m_debugVertexes[i].color = color;
		}

		m_indices.reserve(m_indices.size() + CIRCLEVERTEXESNUM * 2);
		//add 200 more indices
		for (int i = size; i < size + CIRCLEVERTEXESNUM - 1; i++)
		{
			m_indices.push_back(i);
			m_indices.push_back(i + 1);
		}

		m_indices.push_back(size + CIRCLEVERTEXESNUM - 1);
		m_indices.push_back(size);


	}

	void CDebugRenderer::End()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, m_debugVertexes.size() * sizeof(CDebugRenderer), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_debugVertexes.size() * sizeof(CDebugRenderer), m_debugVertexes.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0, m_indices.size() * sizeof(GLuint), m_indices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_iElements = m_indices.size();
		m_indices.clear();
		m_debugVertexes.clear();

		
	}

	void CDebugRenderer::Render(const glm::mat4& projectMatrix, float lineWidth)
	{
		m_cShader.use();

		GLint pUniform = m_cShader.GetUniformLocation("P");
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectMatrix[0][0]);

		glLineWidth(lineWidth);
		glBindVertexArray(m_vao);
		glDrawElements(GL_LINES, m_iElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		m_cShader.unuse();
	}
}