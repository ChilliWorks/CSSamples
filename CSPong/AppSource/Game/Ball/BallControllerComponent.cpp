//
//  BallControllerComponent.cpp
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

#include <Game/Ball/BallControllerComponent.h>

#include <Game/Physics/DynamicBodyComponent.h>

#include <ChilliSource/Core/Delegate.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>

namespace CSPong
{
    namespace
    {
        const f32 k_initialImpulse = 3000.0f;
        const f32 k_collisionImpulse = 300.0f;
    }
    
    CS_DEFINE_NAMEDTYPE(BallControllerComponent);
    //----------------------------------------------------------
    //----------------------------------------------------------
    BallControllerComponent::BallControllerComponent(DynamicBodyComponent* in_dynamicBodyComponent)
        : m_dynamicBodyComponent(in_dynamicBodyComponent), m_active(false)
    {
        CS_ASSERT(m_dynamicBodyComponent != nullptr, "Must provide dynamic body for ball controller.");
        
        m_collisionConnection = m_dynamicBodyComponent->GetCollisionEvent().OpenConnection(CSCore::MakeDelegate(this, &BallControllerComponent::OnCollision));
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool BallControllerComponent::IsA(CSCore::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == BallControllerComponent::InterfaceID;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void BallControllerComponent::Activate()
    {
        CS_ASSERT(GetEntity() != nullptr, "Must be attached to an entity to activate.");
        
        if (m_active == false)
        {
            m_active = true;
            
            //Pick a random direction to start the ball
            f32 angleLimitMin = (CSCore::MathUtils::k_pi) * 0.25f;
            f32 angleLimitMax = (CSCore::MathUtils::k_pi) * 0.75f;
            m_currentDirection.x = std::sin(CSCore::MathUtils::RandomInRange(angleLimitMin, angleLimitMax));
            m_currentDirection.y = std::cos(CSCore::MathUtils::RandomInRange(angleLimitMin, angleLimitMax));
            
            if(CSCore::MathUtils::FRand() > 0.5f)
            {
                m_currentDirection.x = -m_currentDirection.x;
            }
            
            if(CSCore::MathUtils::FRand() > 0.5f)
            {
                m_currentDirection.y = -m_currentDirection.y;
            }
            
            m_dynamicBodyComponent->ApplyImpulse(m_currentDirection * k_initialImpulse);
        }
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void BallControllerComponent::Deactivate()
    {
        CS_ASSERT(GetEntity() != nullptr, "Must be attached to an entity to deactivate.");
        
        if (m_active == true)
        {
            m_active = false;
            
            GetEntity()->GetTransform().SetPosition(CSCore::Vector3::k_zero);
            m_dynamicBodyComponent->SetVelocity(CSCore::Vector2());
        }
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void BallControllerComponent::OnCollision(const CSCore::Vector2& in_direction, CSCore::Entity* in_collidedWith)
    {
        m_currentDirection -= 2.0f * in_direction * CSCore::Vector2::DotProduct(m_currentDirection, in_direction);
        m_dynamicBodyComponent->ApplyImpulse(m_currentDirection * k_collisionImpulse);
    }
}
