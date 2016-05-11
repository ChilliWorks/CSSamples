//
//  DynamicBodyComponent.h
//  CSPong
//  Created by Scott Downie on 02/07/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _APPSOURCE_GAME_COLLISIONS_DYNAMICBODYCOMPONENT_H_
#define _APPSOURCE_GAME_COLLISIONS_DYNAMICBODYCOMPONENT_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Event.h>
#include <ChilliSource/Core/Math.h>

#include <functional>

namespace CSPong
{
    //------------------------------------------------------------
    /// A class that represents a dynamic collision body. A dynamic
    /// collision body will react to collisions
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class DynamicBodyComponent final : public CS::Component
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(DynamicBodyComponent);
        //----------------------------------------------------------
        /// A delegate that will be called whenever the dynamic
        /// body collides with another dynamic body or a static
        /// body.
        ///
        /// @author Ian Copland
        ///
        /// @param The direction of the collision.
        /// @param The entity collided with.
        //----------------------------------------------------------
        using CollisionDelegate = std::function<void(const CS::Vector2&, CS::Entity*)>;
        //----------------------------------------------------------
        /// A delegate that will be called whenever the dynamic
        /// body collides with a trigger.
        ///
        /// @author Ian Copland
        ///
        /// @param The entity collided with.
        //----------------------------------------------------------
        using TriggerDelegate = std::function<void(CS::Entity*)>;
        //----------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The physics system.
        /// @param The size of the body.
        /// @param The drag factor of the body, This should be
        /// in the range 0.0 - 1.0 with a larger value indicating
        /// greater drag.
        /// @param The coefficient of restitution of the body.
        //----------------------------------------------------------
        DynamicBodyComponent(PhysicsSystem* in_physicsSystem, const CS::Vector2& in_size, f32 in_mass, f32 in_dragFactor, f32 in_coefficientOfRestitution);
        //----------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CS::InterfaceIDType in_interfaceId) const override;
        //------------------------------------------------------------
        /// Applies an impulse to the dynamic body. The impulse is
        /// accumualated over the course of a phyics frame and applied
        /// when the physics update occurs.
        ///
        /// @author Ian Copland
        ///
        /// @param Local position of impact
        /// @param Direction of impact (i.e. surface normal of collision body)
        /// @param The overlap vector if the tow objects intersect
        //------------------------------------------------------------
        void ApplyImpulse(const CS::Vector2& in_impulse);
        //------------------------------------------------------------
        /// Sets the current velocity of the physics body. In most
        /// cases it is better to use ApplyImpluse() instead of directly
        /// setting the velocity.
        ///
        /// @author Ian Copland
        ///
        /// @param The new velocity.
        //------------------------------------------------------------
        void SetVelocity(const CS::Vector2& in_velocity);
        //------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The size of the physics body.
        //------------------------------------------------------------
        const CS::Vector2& GetSize() const;
        //------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The mass of the physics body.
        //------------------------------------------------------------
        f32 GetMass() const;
        //------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The current velocity of the physics body.
        //------------------------------------------------------------
        const CS::Vector2& GetVelocity() const;
        //------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param An event that can be listened to in order to receive
        /// callbacks when a collision occurs.
        //------------------------------------------------------------
        CS::IConnectableEvent<CollisionDelegate>& GetCollisionEvent();
        //------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param An event that can be listened to in order to receive
        /// callbacks when a trigger occurs.
        //------------------------------------------------------------
        CS::IConnectableEvent<TriggerDelegate>& GetTriggerEvent();
        
    private:
        friend class PhysicsSystem;
        //------------------------------------------------------------
        /// Called when the component is added to the scene. This can be
        /// either when its owner entity is added to the scene or when
        /// attached an an entity that is already in the scene. This will
        /// register the dynamic body with the physics system so that it
        /// can receive physics updates.
        ///
        /// @author Ian Copland
        ///
        /// @param The fixed time between physics frames.
        //------------------------------------------------------------
        void OnAddedToScene() override;
        //------------------------------------------------------------
        /// Called at a static timestep by the physics system,
        /// immediately before collisions are resolved. This will apply
        /// the currently accumulated impulse to the dynamic body.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------
        void OnPhysicsUpdate(f32 in_deltaTime);
        //------------------------------------------------------------
        /// Called when the dynamic body collides with another dynamic
        /// body. This will resolve the collision by moving the body
        /// such that it is no longer in collision and will update the
        /// objects velocity accordingly taking into account the
        /// difference in mass between the objects. A collision event
        /// will also be fired.
        ///
        /// @author Ian Copland
        ///
        /// @param The direction of the collision relative to this
        /// collision body.
        /// @param The translation vector that will resolve intersection
        /// between the bodies.
        /// @param The dynamic body this is in collision with.
        //------------------------------------------------------------
        void OnDynamicCollision(const CS::Vector2& in_collisionDirection, const CS::Vector2& in_intersectionResolution, DynamicBodyComponent* in_collidedWith);
        //------------------------------------------------------------
        /// Called when the dynamic body collides with a static body.
        /// This will resolve the collision by moving the body such
        /// that it is no longer in collision and will update the
        /// objects velocity accordingly. A collision event will also
        /// be fired.
        ///
        /// @author Ian Copland
        ///
        /// @param The direction of the collision relative to this
        /// collision body.
        /// @param The translation vector that will resolve intersection
        /// between the bodies.
        /// @param The static body this is in collision with.
        //------------------------------------------------------------
        void OnStaticCollision(const CS::Vector2& in_collisionDirection, const CS::Vector2& in_intersectionResolution, StaticBodyComponent* in_collidedWith);
        //------------------------------------------------------------
        /// Called when the dynamic body collides with a trigger. This
        /// will fire the trigger event.
        ///
        /// @author Ian Copland
        ///
        /// @param The trigger collided with.
        //------------------------------------------------------------
        void OnTriggerCollision(TriggerComponent* in_collidedWith);
        //------------------------------------------------------------
        /// Called when the component is removed from the scene. This
        /// can be either when its owner entity is removed from the
        /// scenbe or when the component is removed from an entity that
        /// is currently in the scene. This will deregister itself
        /// from the physics system and therefore stops receiving
        /// physics updates.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------
        void OnRemovedFromScene() override;
        
        PhysicsSystem* m_physicsSystem;
        const CS::Vector2 m_size;
        const f32 m_mass;
        const f32 m_dragFactor;
        const f32 m_coefficientOfRestitution;
        CS::Vector2 m_impulse;
        CS::Vector2 m_velocity;
        CS::Event<CollisionDelegate> m_collisionEvent;
        CS::Event<TriggerDelegate> m_triggerEvent;
    };
}

#endif
