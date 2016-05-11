//
//  DynamicBodyComponent.cpp
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

#include <Game/Physics/DynamicBodyComponent.h>

#include <Game/Physics/PhysicsSystem.h>
#include <Game/Physics/StaticBodyComponent.h>
#include <Game/Physics/TriggerComponent.h>

#include <ChilliSource/Core/Entity.h>

namespace CSPong
{
    CS_DEFINE_NAMEDTYPE(DynamicBodyComponent);
    //----------------------------------------------------------
    //----------------------------------------------------------
    DynamicBodyComponent::DynamicBodyComponent(PhysicsSystem* in_physicsSystem, const CS::Vector2& in_size, f32 in_mass, f32 in_dragFactor, f32 in_coefficientOfRestitution)
        : m_physicsSystem(in_physicsSystem), m_size(in_size), m_mass(in_mass), m_dragFactor(in_dragFactor), m_coefficientOfRestitution(in_coefficientOfRestitution)
    {
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool DynamicBodyComponent::IsA(CS::InterfaceIDType in_interfaceId) const
    {
        return (DynamicBodyComponent::InterfaceID == in_interfaceId);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void DynamicBodyComponent::ApplyImpulse(const CS::Vector2& in_impulse)
    {
        m_impulse += in_impulse;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void DynamicBodyComponent::SetVelocity(const CS::Vector2& in_velocity)
    {
        m_velocity = in_velocity;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    const CS::Vector2& DynamicBodyComponent::GetSize() const
    {
        return m_size;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    f32 DynamicBodyComponent::GetMass() const
    {
        return m_mass;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    const CS::Vector2& DynamicBodyComponent::GetVelocity() const
    {
        return m_velocity;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::IConnectableEvent<DynamicBodyComponent::CollisionDelegate>& DynamicBodyComponent::GetCollisionEvent()
    {
        return m_collisionEvent;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::IConnectableEvent<DynamicBodyComponent::TriggerDelegate>& DynamicBodyComponent::GetTriggerEvent()
    {
        return m_triggerEvent;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void DynamicBodyComponent::OnAddedToScene()
    {
        m_physicsSystem->RegisterDynamicBody(this);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void DynamicBodyComponent::OnPhysicsUpdate(f32 in_deltaTime)
    {
        CS::Vector2 accceleration = m_impulse / m_mass;
        m_impulse = CS::Vector2::k_zero;
        m_velocity += accceleration * in_deltaTime;
        m_velocity *= (1.0f - m_dragFactor);
        CS::Vector2 displacement = m_velocity * in_deltaTime;
        
        CS::Vector3 currentPosition = GetEntity()->GetTransform().GetWorldPosition();
        CS::Vector3 newPosition(currentPosition.x + displacement.x, currentPosition.y + displacement.y, currentPosition.z);
        
        GetEntity()->GetTransform().SetPosition(newPosition);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void DynamicBodyComponent::OnDynamicCollision(const CS::Vector2& in_collisionDirection, const CS::Vector2& in_intersectionResolution, DynamicBodyComponent* in_collidedWith)
    {
        f32 collisionShare = in_collidedWith->GetMass() / (GetMass() + in_collidedWith->GetMass());
        
        CS::Vector2 displacement = in_intersectionResolution * collisionShare;
        GetEntity()->GetTransform().MoveBy(displacement.x, displacement.y, 0.0f);
        
        CS::Vector2 velocityChange = collisionShare * ((1.0f + m_coefficientOfRestitution) * in_collisionDirection * CS::Vector2::DotProduct(m_velocity, in_collisionDirection));
        m_velocity -= velocityChange;
        
        m_collisionEvent.NotifyConnections(in_collisionDirection, in_collidedWith->GetEntity());
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void DynamicBodyComponent::OnStaticCollision(const CS::Vector2& in_collisionDirection, const CS::Vector2& in_intersectionResolution, StaticBodyComponent* in_collidedWith)
    {
        GetEntity()->GetTransform().MoveBy(in_intersectionResolution.x, in_intersectionResolution.y, 0.0f);
        
        CS::Vector2 velocityChange = (1.0f + m_coefficientOfRestitution) * in_collisionDirection * CS::Vector2::DotProduct(m_velocity, in_collisionDirection);
        m_velocity -= velocityChange;
        
        m_collisionEvent.NotifyConnections(in_collisionDirection, in_collidedWith->GetEntity());
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void DynamicBodyComponent::OnTriggerCollision(TriggerComponent* in_collidedWith)
    {
        m_triggerEvent.NotifyConnections(in_collidedWith->GetEntity());
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void DynamicBodyComponent::OnRemovedFromScene()
    {
        m_physicsSystem->DeregisterDynamicBody(this);
    }
}