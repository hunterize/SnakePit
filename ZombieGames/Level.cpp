#include "Level.h"



Level::Level(const std::string& fileName):m_iNumHumans(0), m_iTileWidth(TILE_WIDTH), m_iTileHeight(TILE_HEIGHT)
{
	std::ifstream file;
	file.open(fileName);
	if (file.fail())
	{
		SnakEngine::fatalError("Failed to open " + fileName);
	}

	//get human numbers, the string before the number is ignored
	std::string tmp;
	file >> tmp >> m_iNumHumans;
	//read the rest ('\n') of the fist line
	std::getline(file, tmp);

	while (std::getline(file, tmp))
	{
		m_cLevelData.push_back(tmp);
	}

	file.close();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	SnakEngine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	m_cSpriteBatch.Init();
	m_cSpriteBatch.Begin();

	for (int row = 0; row <m_cLevelData.size(); row++)
		for (int column = 0; column < m_cLevelData[row].size(); column++)
		{
			char data = m_cLevelData[row][column];

			//level data is read from the file line by line from the top to bottom, but world coordinate system is origin at bottom left
			glm::vec4 destRect(column * m_iTileWidth, (m_cLevelData.size() - 1 - row) * m_iTileHeight, m_iTileWidth, m_iTileHeight);

			switch (data)
			{
			case 'B':
			case 'R':
				m_cSpriteBatch.Draw(destRect, uvRect, SnakEngine::ResourceManager::GetTexture("Textures/red_bricks.png").ID, 0.0f, whiteColor);
				break;
			case 'G':
				m_cSpriteBatch.Draw(destRect, uvRect, SnakEngine::ResourceManager::GetTexture("Textures/glass.png").ID, 0.0f, whiteColor);
				break;
			case 'L':
				m_cSpriteBatch.Draw(destRect, uvRect, SnakEngine::ResourceManager::GetTexture("Textures/light_bricks.png").ID, 0.0f, whiteColor);
				break;
			case '@':
				m_cLevelData[row][column] = '.';
				m_cPlayerPos.x = column * m_iTileWidth;
				m_cPlayerPos.y = (m_cLevelData.size() - 1 - row) * m_iTileHeight;
				break;
			case 'Z':
				m_cLevelData[row][column] = '.';
				m_cZombiePos.emplace_back(glm::vec2(column * m_iTileWidth, (m_cLevelData.size() - 1 - row) * m_iTileHeight));
				break;
			case '.':
				break;

			default:
				std::printf("Unexpected symbol %c at (%d,%d)", data, row, column);
				break;
			}

		}

	m_cSpriteBatch.End();




}


Level::~Level()
{
}

void Level::draw()
{
	m_cSpriteBatch.RenderBatch();
}
