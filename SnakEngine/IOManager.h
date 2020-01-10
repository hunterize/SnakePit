#pragma once


#include <vector>

namespace SnakEngine
{
	class IOManager
	{
	public:
		static bool ReadFiletoBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);
		static bool ReadFiletoBuffer(const std::string& filePath, std::string& buffer);
	};

}