#pragma once

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

#include <glm\glm.hpp>
#include <functional>


namespace SnakEngine
{

	//only public element members for easy access
	class Particle2D
	{
	public:
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		Color color;
		//particle life is initialized by 0.0f to indicate the free slot
		float life = 0.0f;
		float width = 0.0f;
	};


	//Defult particle update function, should be inline or static
	static void DefaultParticleUpdate(Particle2D& particle, float deltaTime)
	{
		particle.position += particle.velocity * deltaTime;
	}

	typedef void Func(Particle2D&, float);


	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void Init(int maxParticles, float decayRate, GLTexture texure, std::function<void(Particle2D&, float)> updateFunc = DefaultParticleUpdate);
		
		void Update(float deltaTime);

		void Draw(SpriteBatch* spriteBatch);

		void AddParticle(const glm::vec2& position, const glm::vec2& velocity, const Color& color, float width);

	private:
		std::function<void(Particle2D&, float)> m_pUpdateFunction;
		
		int m_iMaxParticles = 0;
		float m_fDecayRate = 0.1f;
		GLTexture m_texture;
		
		Particle2D* m_pParticles = nullptr;
		
		//start from 0
		int m_iFreeSlot = 0;
		int GetFreeSlot();
		
	};

}
