#include "ResourceManager.h"

namespace SnakEngine
{
	TextureCache ResourceManager::m_cTextureCache;

	GLTexture ResourceManager::GetTexture(std::string texturePath)
	{
		return m_cTextureCache.GetTexture(texturePath);
	}
}