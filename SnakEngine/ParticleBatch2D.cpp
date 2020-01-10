
#include "ParticleBatch2D.h"

namespace SnakEngine
{

	ParticleBatch2D::ParticleBatch2D()
	{
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] m_pParticles;
	}


	void ParticleBatch2D::Init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc/* = DefaultParticleUpdate*/)
	{
		m_iMaxParticles = maxParticles;
		m_fDecayRate = decayRate;
		m_texture = texture;
		m_pUpdateFunction = updateFunc;

		//keep successive memory 
		m_pParticles = new Particle2D[maxParticles];
	}


	//update particle by update function
	void ParticleBatch2D::Update(float deltaTime)
	{
		for (int i = 0; i < m_iMaxParticles; i++)
		{
			if (m_pParticles[i].life > 0.0f)
			{
				m_pUpdateFunction(m_pParticles[i], deltaTime);
				m_pParticles[i].life -= m_fDecayRate * deltaTime;
			}
		}
	}


	//draw the particle with spritebatch
	void ParticleBatch2D::Draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_iMaxParticles; i++)
		{
			Particle2D& p = m_pParticles[i];

			if (p.life > 0.0f)
			{
				glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
				spriteBatch->Draw(destRect, uvRect, m_texture.ID, 0.0f, p.color);

			}
		}
	}

	//add particle to the free slot of the memory
	void ParticleBatch2D::AddParticle(const glm::vec2& position, const glm::vec2& velocity, const Color& color, float width)
	{
		
		Particle2D* p = m_pParticles + GetFreeSlot();
		
		p->life = 1.0f;
		p->position = position;
		p->velocity = velocity;
		p->color = color;
		p->width = width;
		

	}

	//find free slot for the particle
	int ParticleBatch2D::GetFreeSlot()
	{
		for (int i = m_iFreeSlot; i < m_iMaxParticles; i++)
		{
			if (m_pParticles[i].life <= 0.0f)
			{
				m_iFreeSlot = i;
				return i;
			}
		}

		for (int i = 0; i < m_iFreeSlot; i++)
		{
			if (m_pParticles[i].life <= 0.0f)
			{
				m_iFreeSlot = i;
				return i;
			}
		}

		//from the first slot of the memory
		return 0;
	}
}