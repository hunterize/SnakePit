#include "SpriteBatch.h"

namespace SnakEngine
{



	SpriteBatch::SpriteBatch() : m_uiVbo(0), m_uiVao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::Init()
	{
		CreateVertexArray();
	}

	void SpriteBatch::Begin(GlyphSortType sortType)
	{
		m_eSortType = sortType;
		m_RenderBatches.clear();

		//delete glyphs object to avoid memory leak
		//for (int i = 0; i < m_Glyphs.size(); i++)
		//{
		//	delete m_Glyphs[i];
		//}
		m_Glyphs.clear();
	}
	void SpriteBatch::End()
	{
		//store glyph pointers
		m_GlyphPointers.resize(m_Glyphs.size());
		for (int i = 0; i < m_Glyphs.size(); i++)
		{
			m_GlyphPointers[i] = &m_Glyphs[i];
		}

		SortGlyphs();

		CreateRenderBatches();
	}

	void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
	{

		m_Glyphs.emplace_back(destRect, uvRect, texture, depth, color);

	}
	void SpriteBatch::RenderBatch()
	{
		glBindVertexArray(m_uiVao);

		for (int i = 0; i < m_RenderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_RenderBatches[i].m_texture);

			glDrawArrays(GL_TRIANGLES, m_RenderBatches[i].m_offset, m_RenderBatches[i].m_numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::CreateVertexArray()
	{
		if (m_uiVao == 0)
		{
			glGenVertexArrays(1, &m_uiVao);
		}
		glBindVertexArray(m_uiVao);

		if (m_uiVbo == 0)
		{
			glGenBuffers(1, &m_uiVbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVbo);

		//tell opengl that we want to use the first
		//attribute array. we only need one array right now
		//since we are only using position, not real, we are using 0, 1 and 2
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);


		//this is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		//this is the color attribute pointer, color elements are represented by one byte data 0-255, it needs to be normalized
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		//this is the uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteBatch::CreateRenderBatches()
	{
		//this whole vertices in the vbo
		std::vector<Vertex> vertices;
		vertices.resize(m_GlyphPointers.size() * 6); //each glyph has 6 vertices (two triangles)

		if (m_GlyphPointers.empty())
			return;

		int cv = 0; //current vertex
		int offset = 0;
		m_RenderBatches.emplace_back(offset, 6, m_GlyphPointers[0]->m_uiTexture);
		vertices[cv++] = m_GlyphPointers[0]->m_cTopLeft;
		vertices[cv++] = m_GlyphPointers[0]->m_cBottomLeft;
		vertices[cv++] = m_GlyphPointers[0]->m_cBottomRight;
		vertices[cv++] = m_GlyphPointers[0]->m_cBottomRight;
		vertices[cv++] = m_GlyphPointers[0]->m_cTopRight;
		vertices[cv++] = m_GlyphPointers[0]->m_cTopLeft;
		offset += 6;


		//set renderbatch by current glyph
		for (int cg = 1; cg < m_GlyphPointers.size(); cg++)
		{
			if (m_GlyphPointers[cg]->m_uiTexture != m_GlyphPointers[cg - 1]->m_uiTexture)
			{
				m_RenderBatches.emplace_back(offset, 6, m_GlyphPointers[cg]->m_uiTexture);
			}
			else
			{
				m_RenderBatches.back().m_numVertices += 6;
			}
			vertices[cv++] = m_GlyphPointers[cg]->m_cTopLeft;
			vertices[cv++] = m_GlyphPointers[cg]->m_cBottomLeft;
			vertices[cv++] = m_GlyphPointers[cg]->m_cBottomRight;
			vertices[cv++] = m_GlyphPointers[cg]->m_cBottomRight;
			vertices[cv++] = m_GlyphPointers[cg]->m_cTopRight;
			vertices[cv++] = m_GlyphPointers[cg]->m_cTopLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_uiVbo);

		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}


	void SpriteBatch::SortGlyphs()
	{
		switch (m_eSortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_GlyphPointers.begin(), m_GlyphPointers.end(), CompareBackToFront );
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_GlyphPointers.begin(), m_GlyphPointers.end(), CompareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(m_GlyphPointers.begin(), m_GlyphPointers.end(), CompareTexture);
			break;
		}
	}

	bool SpriteBatch::CompareFrontToBack(Glyph*a, Glyph* b)
	{
		return (a->m_fDepth < b->m_fDepth);
	}
	bool SpriteBatch::CompareBackToFront(Glyph*a, Glyph* b)
	{
		return (a->m_fDepth > b->m_fDepth);
	}
	bool SpriteBatch::CompareTexture(Glyph*a, Glyph* b)
	{
		return (a->m_uiTexture < b->m_uiTexture);
	}
}
