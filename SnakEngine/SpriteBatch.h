#pragma once

#include <GLEW\glew.h>
#include "Vertex.h"
#include <glm\glm.hpp>
#include <vector>
#include <cstdlib>
#include <algorithm>




namespace SnakEngine
{
	class Glyph
	{
	public:
		Glyph() {};
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
		{
			m_uiTexture = texture;
			m_fDepth = depth;

			m_cTopLeft.color = color;
			m_cTopLeft.SetPosition(destRect.x, destRect.y + destRect.w);
			m_cTopLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

			m_cBottomLeft.color = color;
			m_cBottomLeft.SetPosition(destRect.x, destRect.y);
			m_cBottomLeft.SetUV(uvRect.x, uvRect.y);

			m_cBottomRight.color = color;
			m_cBottomRight.SetPosition(destRect.x + destRect.z, destRect.y);
			m_cBottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

			m_cTopRight.color = color;
			m_cTopRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
			m_cTopRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		}
		GLuint m_uiTexture;
		GLfloat m_fDepth;

		Vertex m_cTopLeft;
		Vertex m_cBottomLeft;
		Vertex m_cTopRight;
		Vertex m_cBottomRight;

	};

	enum class GlyphSortType
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};


	//offset is the pointer to the very first vertex of a group vertice which have the same texture
	class CRenderBatch
	{
	public:
		CRenderBatch(GLuint offset, GLuint numVertices, GLuint texture) : m_offset(offset), m_numVertices(numVertices), m_texture(texture)
		{}
		GLuint m_offset;
		GLuint m_numVertices;
		GLuint m_texture;

	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		void Init();
		void Begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void End();
		void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
		void RenderBatch();
	private:
		GLuint m_uiVbo;
		GLuint m_uiVao;

		GlyphSortType m_eSortType;

		std::vector<Glyph> m_Glyphs;
		std::vector<Glyph*> m_GlyphPointers;

		std::vector<CRenderBatch> m_RenderBatches;

		void CreateRenderBatches();
		void CreateVertexArray();
		void SortGlyphs();

		static bool CompareFrontToBack(Glyph*a, Glyph* b);
		static bool CompareBackToFront(Glyph*a, Glyph* b);
		static bool CompareTexture(Glyph*a, Glyph* b);


	};
}

