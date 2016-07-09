
#include "ParticleEngine2D.h"

namespace SnakEngine
{
	ParticleEngine2D::ParticleEngine2D()
	{
	}


	ParticleEngine2D::~ParticleEngine2D()
	{
		for (auto& it : m_cParticleBatches)
		{
			delete it;
		}
	}

	void ParticleEngine2D::AddParticleBatch(ParticleBatch2D* pb)
	{
		m_cParticleBatches.push_back(pb);
	}


	void ParticleEngine2D::Update(float deltaTime)
	{
		for (auto& it : m_cParticleBatches)
		{
			it->Update(deltaTime);
		}
	}


	void ParticleEngine2D::Draw(SpriteBatch* sb)
	{
		for (auto& it : m_cParticleBatches)
		{
			sb->Begin();

			it->Draw(sb);

			sb->End();
			sb->RenderBatch();

		}
	}
}