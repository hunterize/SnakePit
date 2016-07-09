#pragma once

#include <vector>
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"


namespace SnakEngine
{

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		void AddParticleBatch(ParticleBatch2D* pb);
		void Update(float deltaTime);
		void Draw(SpriteBatch* sb);

	private:
		std::vector<ParticleBatch2D*> m_cParticleBatches;
	};
}

