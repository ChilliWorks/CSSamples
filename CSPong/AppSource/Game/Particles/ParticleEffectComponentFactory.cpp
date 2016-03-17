//
//  ParticleEffectComponentFactory.cpp
//  CSPong
//  Created by Angela Gross on 12/01/2016.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <Game/Particles/ParticleEffectComponentFactory.h>

#include <ChilliSource/Rendering/Particle.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Base.h>

#include <sstream>

namespace CSPong
{
	CS_DEFINE_NAMEDTYPE(ParticleEffectComponentFactory);

	const ParticleEffectComponentFactory::ParticleType ParticleEffectComponentFactory::k_gameParticles[] =
	{
		ParticleEffectComponentFactory::ParticleType::k_blueMagmaBurst, ParticleEffectComponentFactory::ParticleType::k_blueIceCreamBurst,
		ParticleEffectComponentFactory::ParticleType::k_yellowMagmaBurst, ParticleEffectComponentFactory::ParticleType::k_pinkIceCreamBurst,
		ParticleEffectComponentFactory::ParticleType::k_smokeStream, ParticleEffectComponentFactory::ParticleType::k_beamStream
	};

	//---------------------------------------------------
	//---------------------------------------------------
	ParticleEffectComponentFactoryUPtr ParticleEffectComponentFactory::Create()
	{
		return ParticleEffectComponentFactoryUPtr(new ParticleEffectComponentFactory());
	}
	//---------------------------------------------------
	//---------------------------------------------------
	void ParticleEffectComponentFactory::OnDestroy()
	{
		ReleaseCollisionConnections();
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	ParticleEffectComponentFactory::ParticleEffectComponentFactory()
	{
		/*int particleSize = sizeof(CSRendering::Particle);

		std::ostringstream oss;
		oss << "TESTINGGG: Size of the particle struct is: " << particleSize;

		CS_LOG_VERBOSE(oss.str());*/
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	bool ParticleEffectComponentFactory::IsA(CSCore::InterfaceIDType in_interfaceId) const
	{
		return in_interfaceId == ParticleEffectComponentFactory::InterfaceID;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	void ParticleEffectComponentFactory::ReleaseCollisionConnections()
	{
		for (std::vector<CSCore::EventConnectionSPtr>::size_type i = 0; i != m_collisionConnections.size(); i++)
		{
			m_collisionConnections[i].reset();
		}

		m_collisionConnections.clear();
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	CSRendering::ParticleEffectComponentSPtr ParticleEffectComponentFactory::CreateOnCollisionParticleEffectComponent(const ParticleType in_particleType, CSCore::IConnectableEvent<DynamicBodyComponent::CollisionDelegate>& in_collisionEvent)
	{
		CSRendering::ParticleEffectComponentSPtr particleEffectComponent = CreateParticleEffectComponent(in_particleType, false);

		//The m_collisionConnections vector keeps the event connection in scope. In order to avoid having two copies of a shared pointer
		//and having the program hang on exit, the event connection is pushed directly onto the m_collisionConnections vector.
		m_collisionConnections.push_back
		(
			in_collisionEvent.OpenConnection([=](const CSCore::Vector2& in_collisionDirection, CSCore::Entity* in_collidedEntity)
			{
				if (particleEffectComponent != nullptr && !particleEffectComponent->IsPlaying())
				{
					particleEffectComponent->Play();
				}
			})
		);

		return particleEffectComponent;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	CSRendering::ParticleEffectComponentUPtr ParticleEffectComponentFactory::CreateParticleEffectComponent(const ParticleType in_particleType, const bool in_looping) const
	{
		auto resourcePool = CSCore::Application::Get()->GetResourcePool();
		auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();

		CSRendering::ParticleEffectCSPtr particleEffect = nullptr;
		switch (in_particleType)
		{
			case ParticleType::k_blueMagmaBurst:
				particleEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particles/BlueMagmaBurst.csparticle");
				break;
			case ParticleType::k_yellowMagmaBurst:
				particleEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particles/YellowMagmaBurst.csparticle");
				break;
			case ParticleType::k_blueIceCreamBurst:
				particleEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particles/BlueIceCreamBurst.csparticle");
				break;
			case ParticleType::k_pinkIceCreamBurst:
				particleEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particles/PinkIceCreamBurst.csparticle");
				break;
			case ParticleType::k_beamStream:
				particleEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particles/BeamStream.csparticle");
				break;
			case ParticleType::k_smokeStream:
				particleEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particles/SmokeStream.csparticle");
				break;
			case ParticleType::k_sparksStream:
				particleEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particles/SparksStream.csparticle");
				break;
			default:
				particleEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particles/SmokeStream.csparticle");
				break;
		}

		CSRendering::ParticleEffectComponentUPtr particleComponent = renderFactory->CreateParticleEffectComponent(particleEffect);

		if (in_looping)
		{
			particleComponent->SetPlaybackType(CSRendering::ParticleEffectComponent::PlaybackType::k_looping);
		}
		else
		{
			particleComponent->SetPlaybackType(CSRendering::ParticleEffectComponent::PlaybackType::k_once);
		}

		return particleComponent;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void ParticleEffectComponentFactory::AddPlayerParticlesOnCollision(CSCore::EntitySPtr in_playerEntity, CSCore::IConnectableEvent<DynamicBodyComponent::CollisionDelegate>& in_collisionEvent)
	{
		//Add magma particles if it's chosen
		if (m_particlesChosen[k_playerParticlesIx])
		{
			auto particleComponent = CreateOnCollisionParticleEffectComponent(k_gameParticles[k_playerParticlesIx], in_collisionEvent);
			in_playerEntity->AddComponent(particleComponent);
		}

		//Add ice cream particles if it's chosen
		if (m_particlesChosen[k_playerParticlesIx + 1])
		{
			auto particleComponent = CreateOnCollisionParticleEffectComponent(k_gameParticles[k_playerParticlesIx + 1], in_collisionEvent);
			in_playerEntity->AddComponent(particleComponent);
		}
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void ParticleEffectComponentFactory::AddOpponentParticlesOnCollision(CSCore::EntitySPtr in_opponentEntity, CSCore::IConnectableEvent<DynamicBodyComponent::CollisionDelegate>& in_collisionEvent)
	{
		//Add magma particles if it's chosen
		if (m_particlesChosen[k_opponentParticlesIx])
		{
			auto particleComponent = CreateOnCollisionParticleEffectComponent(k_gameParticles[k_opponentParticlesIx], in_collisionEvent);
			in_opponentEntity->AddComponent(particleComponent);
		}

		//Add ice cream particles if it's chosen
		if (m_particlesChosen[k_opponentParticlesIx + 1])
		{
			auto particleComponent = CreateOnCollisionParticleEffectComponent(k_gameParticles[k_opponentParticlesIx + 1], in_collisionEvent);
			in_opponentEntity->AddComponent(particleComponent);
		}
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void ParticleEffectComponentFactory::AddBallParticles(CSCore::EntitySPtr in_ballEntity)
	{
		//Add smoke particles if it's chosen
		if (m_particlesChosen[k_ballParticlesIx])
		{
			CSRendering::ParticleEffectComponentSPtr particleComponent = CreateParticleEffectComponent(k_gameParticles[k_ballParticlesIx], true);
			in_ballEntity->AddComponent(particleComponent);
		}

		//Add beam particles if it's chosen
		if (m_particlesChosen[k_ballParticlesIx + 1])
		{
			CSRendering::ParticleEffectComponentSPtr particleComponent = CreateParticleEffectComponent(k_gameParticles[k_ballParticlesIx + 1], true);
			in_ballEntity->AddComponent(particleComponent);
		}
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void ParticleEffectComponentFactory::SetPlayerParticles(const bool in_magmaUsed, const bool in_iceCreamUsed)
	{
		m_particlesChosen[k_playerParticlesIx] = in_magmaUsed;
		m_particlesChosen[k_playerParticlesIx + 1] = in_iceCreamUsed;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void ParticleEffectComponentFactory::SetOpponentParticles(const bool in_magmaUsed, const bool in_iceCreamUsed)
	{
		m_particlesChosen[k_opponentParticlesIx] = in_magmaUsed;
		m_particlesChosen[k_opponentParticlesIx + 1] = in_iceCreamUsed;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void ParticleEffectComponentFactory::SetBallParticles(const bool in_smokeUsed, const bool in_beamUsed)
	{
		m_particlesChosen[k_ballParticlesIx] = in_smokeUsed;
		m_particlesChosen[k_ballParticlesIx + 1] = in_beamUsed;
	}
	
}