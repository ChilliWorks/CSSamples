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

namespace CSPong
{
	CS_DEFINE_NAMEDTYPE(ParticleEffectComponentFactory);

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
		for (std::vector<int>::size_type i = 0; i != m_collisionConnections.size(); i++)
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
			case ParticleType::k_magmaBurst:
				particleEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particles/MagmaBurst.csparticle");
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
	ParticleEffectComponentFactory::ParticleType ParticleEffectComponentFactory::GetBallParticleType() const
	{
		return m_ballParticleType;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	ParticleEffectComponentFactory::ParticleType ParticleEffectComponentFactory::GetOpponentPaddleParticleType() const
	{
		return m_opponentPaddleParticleType;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	ParticleEffectComponentFactory::ParticleType ParticleEffectComponentFactory::GetPlayerPaddleParticleType() const
	{
		return m_playerPaddleParticleType;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void ParticleEffectComponentFactory::SetBallParticleType(const ParticleType in_particleType)
	{
		m_ballParticleType = in_particleType;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void ParticleEffectComponentFactory::SetOpponentPaddleParticleType(const ParticleType in_particleType)
	{
		m_opponentPaddleParticleType = in_particleType;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void ParticleEffectComponentFactory::SetPlayerPaddleParticleType(const ParticleType in_particleType)
	{
		m_playerPaddleParticleType = in_particleType;
	}
}