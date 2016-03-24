//
//  ParticleEffectComponentFactory.h
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

#ifndef _APPSOURCE_GAME_PARTICLES_PARTICLEEFFECTCOMPONENTFACTORY_H_
#define _APPSOURCE_GAME_PARTICLES_PARTICLEEFFECTCOMPONENTFACTORY_H_

#include <ForwardDeclarations.h>
#include <Game/Physics/DynamicBodyComponent.h>

#include <ChilliSource/Core/System.h>
#include <ChilliSource/Core/Event.h>

namespace CSPong
{
	//------------------------------------------------------------
	/// Creates different particle effects- instant/looping or on
	/// collision. And, since it is an app system, these effects
	/// will be able to be chosen during other game states.
	///
	/// @author Angela Gross
	//------------------------------------------------------------
	class ParticleEffectComponentFactory final : public CSCore::AppSystem
	{

	public:

		CS_DECLARE_NAMEDTYPE(ParticleEffectComponentFactory);

		//------------------------------------------------------------
		/// An enum that describes the different types of particle 
		/// effects that have been created for the game.
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		enum class ParticleType
		{
			k_blueMagmaBurst,
			k_yellowMagmaBurst,
			k_blueIceCreamBurst,
			k_pinkIceCreamBurst,
			k_beamStream,
			k_smokeStream,
			k_sparksStream
		};

		//------------------------------------------------------------
		/// Creates a new instance of the system.
		///
		/// @author Angela Gross
		///
		/// @return The new instance.
		//------------------------------------------------------------
		static ParticleEffectComponentFactoryUPtr Create();
		//------------------------------------------------------------
		/// Essentially a destructor- will reset collision connections 
		/// and empty the member vector.
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		void OnDestroy() override;
		//------------------------------------------------------------
		/// @author Angela Gross
		///
		/// @param Comparison Type
		///
		/// @return Whether the class matches the comparison type
		//------------------------------------------------------------
		bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
		//------------------------------------------------------------
		/// Manually resets collision connection pointers and clears
		/// the collision connection vector.
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		void ReleaseCollisionConnections();
		//------------------------------------------------------------
		/// Creates a particle effect component based on the given
		/// particle type. If the given particle type is invalid, then 
		/// it creates a smoke stream.
		///
		/// @author Angela Gross
		///
		/// @param The type of particle effect to create.
		/// @param Whether or not the particle effect should loop.
		///
		/// @return The new particle effect component. 
		//------------------------------------------------------------
		CSRendering::ParticleEffectComponentUPtr CreateParticleEffectComponent(const ParticleType in_particleType, const bool in_looping) const;
		//------------------------------------------------------------
		/// Creates a particle effect component that will play when
		/// the given collision event is triggered based on the given
		/// particle type. If the given particle type is invalid, then 
		/// it creates a smoke stream. (Note that this means the 
		/// particle effect will not loop and will only play on 
		/// collision. Also, a SHARED pointer is returned because
		/// of the delgate using the particle effect)
		///
		/// @author Angela Gross
		///
		/// @param The type of particle effect to create.
		/// @param The collision delegate event used to trigger the
		/// collision you want to play the particle effect during.
		///
		/// @return The new particle effect component. 
		//------------------------------------------------------------
		CSRendering::ParticleEffectComponentSPtr CreateOnCollisionParticleEffectComponent(const ParticleType in_particleType, CSCore::IConnectableEvent<DynamicBodyComponent::CollisionDelegate>& in_collisionEvent);
		//------------------------------------------------------------
		/// Creates and adds particle components to the player entity
		/// according to the k_gameParticles and m_particlesChosen 
		/// arrays. It plays these particles on collision.
		///
		/// @author Angela Gross
		///
		/// @param The entity that represents the player
		/// @param The collision delegate event used to trigger the
		/// collision you want to play the particle effect during.
		//------------------------------------------------------------
		void AddPlayerParticlesOnCollision(CSCore::EntitySPtr in_playerEntity, CSCore::IConnectableEvent<DynamicBodyComponent::CollisionDelegate>& in_collisionEvent);
		//------------------------------------------------------------
		/// Creates and adds particle components to the opponent entity
		/// according to the k_gameParticles and m_particlesChosen 
		/// arrays. It plays these particles on collision.
		///
		/// @author Angela Gross
		///
		/// @param The entity that represents the opponent
		//------------------------------------------------------------
		void AddOpponentParticlesOnCollision(CSCore::EntitySPtr in_opponentEntity, CSCore::IConnectableEvent<DynamicBodyComponent::CollisionDelegate>& in_collisionEvent);
		//------------------------------------------------------------
		/// Creates and adds particle components to the ball entity
		/// according to the k_gameParticles and m_particlesChosen 
		/// arrays.
		///
		/// @author Angela Gross
		///
		/// @param The entity that represents the ball
		//------------------------------------------------------------
		void AddBallParticles(CSCore::EntitySPtr in_ballEntity);
		//------------------------------------------------------------
		/// Sets the player particles that were chosen by the user
		/// in m_particlesChosen.
		///
		/// @author Angela Gross
		///
		/// @param Whether or not the magma particles will be added
		/// @param Whether or not the ice cream particles will be 
		/// added
		//------------------------------------------------------------
		void SetPlayerParticles(const bool in_magmaUsed, const bool in_iceCreamUsed);
		//------------------------------------------------------------
		/// Sets the opponent particles that were chosen by the user
		/// in m_particlesChosen.
		///
		/// @author Angela Gross
		///
		/// @param Whether or not the magma particles will be added
		/// @param Whether or not the ice cream particles will be 
		/// added
		//------------------------------------------------------------
		void SetOpponentParticles(const bool in_magmaUsed, const bool in_iceCreamUsed);
		//------------------------------------------------------------
		/// Sets the ball particles that were chosen by the user
		/// in m_particlesChosen.
		///
		/// @author Angela Gross
		///
		/// @param Whether or not the smoke particles will be added
		/// @param Whether or not the beam particles will be added
		//------------------------------------------------------------
		void SetBallParticles(const bool in_smokeUsed, const bool in_beamUsed);

	private:
		//----------------------------------------------------------
		/// Private constructor to enforce use of factory method.
		///
		/// @author Angela Gross
		//----------------------------------------------------------
		ParticleEffectComponentFactory();

		const int k_playerParticlesIx = 0;
		const int k_opponentParticlesIx = 2;
		const int k_ballParticlesIx = 4;
		const static ParticleEffectComponentFactory::ParticleType k_gameParticles[];
		bool m_particlesChosen[6];

		std::vector<CSCore::EventConnectionSPtr> m_collisionConnections;
	};
}

#endif