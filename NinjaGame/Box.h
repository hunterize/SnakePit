#pragma once

#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <SnakEngine\Vertex.h>
#include <SnakEngine\GLTexture.h>
#include <SnakEngine\SpriteBatch.h>

class CBox
{
public:
	CBox();
	~CBox();

	void Init(b2World* pWorld, 
			  const glm::vec2& position, 
			  const glm::vec2& dimension, 
		      SnakEngine::GLTexture texture, 
			  SnakEngine::Color color,
		      bool isFixedRotation,
			  float density = 3.0f,
			  glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
			  );

	void Draw(SnakEngine::SpriteBatch& spriteBatch);

	b2Body* GetBody() { return m_pBody; }
	b2Fixture* GetFixture() { return m_pFixture; }
	const glm::vec2& GetDimension() const { return m_cDimension; }
	const SnakEngine::Color& GetColor() const { return m_cColor;}


private:
	b2Body* m_pBody = nullptr;
	b2Fixture* m_pFixture = nullptr;
	glm::vec2 m_cDimension;
	glm::vec4 m_cUVRect;
	
	SnakEngine::Color m_cColor;
	SnakEngine::GLTexture m_cTexture;
};

