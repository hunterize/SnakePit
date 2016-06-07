#pragma once
#include "GLTexture.h"
#include <string>
#include <vector>
#include "IOManager.h"
#include "Errors.h"
#include "picoPNG.h"

namespace SnakEngine
{
	class ImageLoader
	{
	public:
		GLTexture static LoadPNG(const std::string& filePath);
	};

}