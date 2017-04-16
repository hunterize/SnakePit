#pragma once
#include <glm\glm.hpp>
#include "GLTexture.h"

namespace SnakEngine {

	class CTileSheet
	{
	public:
		void Init(const GLTexture& texture, const glm::ivec2& dim);
		glm::vec4 GetUVs(int index);
		const glm::ivec2& GetDimension() { return m_cDimension; }
		const GLTexture& GetTexture() { return m_cTexture; }
	private:
		GLTexture m_cTexture;
		glm::ivec2 m_cDimension;
	};
}


