#include "SpriteBatch.h"

namespace SnakEngine
{
	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
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

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float angle)
	{
		m_uiTexture = texture;
		m_fDepth = depth;

		//the half rectangle of the Glyph Rect
		glm::vec2 rec(destRect.z / 2.0f, destRect.w / 2.0f);

		//move the centre of the glyph to the origin (0,0)
		glm::vec2 topleft(-rec.x, rec.y);
		glm::vec2 bottomleft(-rec.x, -rec.y);
		glm::vec2 bottomright(rec.x, -rec.y);
		glm::vec2 topright(rec.x, rec.y);

		//rotate the glyph and move it back (bottom left vertex to origin) 
		topleft = RotatePoint(topleft, angle) + rec;
		bottomleft = RotatePoint(bottomleft, angle) + rec;
		bottomright = RotatePoint(bottomright, angle) + rec;
		topright = RotatePoint(topright, angle) + rec;

		m_cTopLeft.color = color;
		m_cTopLeft.SetPosition(destRect.x + topleft.x, destRect.y + topleft.y);
		m_cTopLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

		m_cBottomLeft.color = color;
		m_cBottomLeft.SetPosition(destRect.x + bottomleft.x, destRect.y + bottomleft.y);
		m_cBottomLeft.SetUV(uvRect.x, uvRect.y);

		m_cBottomRight.color = color;
		m_cBottomRight.SetPosition(destRect.x + bottomright.x, destRect.y + bottomright.y);
		m_cBottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

		m_cTopRight.color = color;
		m_cTopRight.SetPosition(destRect.x + topright.x, destRect.y + topright.y);
		m_cTopRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	glm::vec2 Glyph::RotatePoint(const glm::vec2& pos, float angle)
	{
		glm::vec2 vertex;

		//rotate a point around the origin(0,0)
		vertex.x = pos.x * cos(angle) - pos.y * sin(angle);
		vertex.y = pos.x * sin(angle) + pos.y * cos(angle);

		return vertex;
	}

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

	void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float angle)
	{

		m_Glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);

	}

	void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, glm::vec2 dir)
	{

		const glm::vec2 initial(1.0f, 0.0f);
		
		//vector dot production to alculate the angle from the initial direction (positive x) to the dir 
		float angle = acos(glm::dot(initial, dir));
		
		if (dir.y < 0.0f)
		{
			angle = -angle;
		}

		m_Glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);

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
