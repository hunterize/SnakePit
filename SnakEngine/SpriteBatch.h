#pragma once

#include <GLEW\glew.h>
#include "Vertex.h"
#include <glm\glm.hpp>
#include <vector>
#include <cstdlib>
#include <algorithm>




namespace SnakEngine
{
	struct Glyph
	{
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
		std::vector<Glyph*> m_Glyphs;
		std::vector<CRenderBatch> m_RenderBatches;

		void CreateRenderBatches();
		void CreateVertexArray();
		void SortGlyphs();

		static bool CompareFrontToBack(Glyph*a, Glyph* b);
		static bool CompareBackToFront(Glyph*a, Glyph* b);
		static bool CompareTexture(Glyph*a, Glyph* b);


	};
}

