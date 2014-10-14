//
//  AIControllerComponent.cpp
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

#include <Game/Paddle/AIControllerComponent.h>

#include <Game/Physics/DynamicBodyComponent.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>

namespace CSPong
{
    namespace
    {
        const f32 k_maxForce = 500000.0f;
    }
    
    CS_DEFINE_NAMEDTYPE(AIControllerComponent);
    
    //----------------------------------------------------------
    //----------------------------------------------------------
    AIControllerComponent::AIControllerComponent(const DynamicBodyComponentSPtr& in_body, const CSCore::EntitySPtr& in_targetEntity)
    : m_body(in_body), m_target(in_targetEntity)
    {
        m_targetBody = m_target->GetComponent<DynamicBodyComponent>();
        CS_ASSERT(m_targetBody != nullptr, "Cannot have null target body");
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool AIControllerComponent::IsA(CSCore::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == AIControllerComponent::InterfaceID;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void AIControllerComponent::Reset()
    {
        CS_ASSERT(GetEntity() != nullptr, "Must have an entity to reset");
        CSCore::Vector3 currentPos(GetEntity()->GetTransform().GetLocalPosition());
        GetEntity()->GetTransform().SetPosition(currentPos.x, 0.0f, currentPos.z);
    }
    //----------------------------------------------------
    //----------------------------------------------------
    bool AIControllerComponent::IsTargetMovingTowardsUs(const CSCore::Vector3& in_currentPos) const
    {
        CSCore::Vector3 ourDirection = CSCore::Vector3::Normalise(CSCore::Vector3::k_zero - in_currentPos);
        CSCore::Vector2 targetDirection = CSCore::Vector2::Normalise(m_targetBody->GetVelocity());
        
        return CSCore::Vector2::DotProduct(ourDirection.XY(), targetDirection) < 0.0f;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void AIControllerComponent::OnFixedUpdate(f32 in_timeSinceLastUpdate)
    {
        //If ball is not coming towards us then just return to the centre
        CSCore::Vector3 currentPos(GetEntity()->GetTransform().GetWorldPosition());
        CSCore::Vector3 targetPos;
        if(IsTargetMovingTowardsUs(currentPos) == true)
        {
            targetPos = m_target->GetTransform().GetWorldPosition();
        }
        else
        {
            targetPos = currentPos;
            targetPos.y = 0.0f;
        }
        
        CSCore::Vector3 distance(targetPos - currentPos);
        
        m_body->ApplyImpulse(CSCore::Vector2(0.0f, distance.y) * k_maxForce * in_timeSinceLastUpdate);
    }
}