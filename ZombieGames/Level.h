#pragma once
#include <vector>
#include <string>
#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\Errors.h>
#include <SnakEngine\ResourceManager.h>
#include <fstream>
#include <iostream>

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;

class Level
{
public:
	Level(const std::string& fileName);
	~Level();
	void draw();
	int GetHumanNumber() { return m_iNumHumans; }
	glm::vec2 GetPlayerPos() { return m_cPlayerPos; }
	const std::vector<std::string>& GetLevelData() const { return m_cLevelData; }
	const std::vector<glm::vec2>& GetZombiePosition() const { return m_cZombiePos; }
	int GetColumn() {return m_cLevelData[0].size(); }
	int GetRow() { return m_cLevelData.size(); }

private:
	SnakEngine::SpriteBatch m_cSpriteBatch;
	std::vector<std::string> m_cLevelData;
	int m_iNumHumans;
	glm::vec2 m_cPlayerPos;
	std::vector<glm::vec2> m_cZombiePos;

	const int m_iTileWidth;
	const int m_iTileHeight;
};

