#include "TileSheet.h"

namespace SnakEngine {

	void CTileSheet::Init(const GLTexture& texture, const glm::ivec2& dim)
	{
		m_cTexture = texture;

		if (dim.x == 0 || dim.y == 0)
		{
			m_cDimension.x = 1;
			m_cDimension.y = 1;
		}
		else
		{
			m_cDimension = dim;
		}

	}

	//uv coordinate in opengl start at (0,0) for the lower left corner of a texture
	//image to (1,1) for the upper right corner of a texture image
	glm::vec4 CTileSheet::GetUVs(int index)
	{
		glm::vec4 uvs;

		if (index >= 0 && index < m_cDimension.x * m_cDimension.y)
		{
			int xTile = index % m_cDimension.x;
			int yTile = index / m_cDimension.x;

			uvs.x = xTile / (float)m_cDimension.x;
			uvs.y = yTile / (float)m_cDimension.y;
			uvs.z = 1.0f / m_cDimension.x;
			uvs.w = 1.0f / m_cDimension.y;
		}
		else
		{
			uvs.x = 0.0f;
			uvs.y = 0.0f;
			uvs.z = 1.0f;
			uvs.w = 1.0f;
		}

		return uvs;
	}

}