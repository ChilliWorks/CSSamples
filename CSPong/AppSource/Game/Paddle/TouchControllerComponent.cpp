//
//  TouchControllerComponent.cpp
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

#include <Game/Paddle/TouchControllerComponent.h>

#include <Game/Physics/DynamicBodyComponent.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Delegate.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Rendering/Camera.h>

namespace CSPong
{
    namespace
    {
        const f32 k_maxForce = 500000.0f;
    }
    
    CS_DEFINE_NAMEDTYPE(TouchControllerComponent);
    
    //----------------------------------------------------------
    //----------------------------------------------------------
    TouchControllerComponent::TouchControllerComponent(const DynamicBodyComponentSPtr& in_body, const CS::CameraComponentSPtr& in_cameraComponent)
    : m_body(in_body), m_cameraComponent(in_cameraComponent)
    {
        
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool TouchControllerComponent::IsA(CS::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == TouchControllerComponent::InterfaceID;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void TouchControllerComponent::OnAddedToScene()
    {
        auto pointerSystem = CS::Application::Get()->GetSystem<CS::PointerSystem>();
        m_pointerDownConnection = pointerSystem->GetPointerDownEvent().OpenConnection(CS::MakeDelegate(this, &TouchControllerComponent::OnPointerDown));
        m_pointerMovedConnection = pointerSystem->GetPointerMovedEvent().OpenConnection(CS::MakeDelegate(this, &TouchControllerComponent::OnPointerMoved));
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void TouchControllerComponent::OnRemovedFromScene()
    {
        m_pointerDownConnection.reset();
        m_pointerMovedConnection.reset();
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void TouchControllerComponent::UpdateTargetPosition(const CS::Vector2& in_screenPos)
    {
        CS::Ray worldRay = m_cameraComponent->Unproject(in_screenPos);
        
        f32 distanceToGroundPlace = -worldRay.vOrigin.z / worldRay.vDirection.z;
        m_targetPos = worldRay.vOrigin + (worldRay.vDirection * distanceToGroundPlace);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void TouchControllerComponent::OnPointerDown(const CS::Pointer& in_pointer, f64 in_timestamp, CS::Pointer::InputType in_inputType)
    {
        UpdateTargetPosition(in_pointer.GetPosition());
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void TouchControllerComponent::OnPointerMoved(const CS::Pointer& in_pointer, f64 in_timestamp)
    {
        UpdateTargetPosition(in_pointer.GetPosition());
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void TouchControllerComponent::OnFixedUpdate(f32 in_timeSinceLastUpdate)
    {
        CS::Vector3 currentPos(GetEntity()->GetTransform().GetWorldPosition());
        CS::Vector3 direction(m_targetPos - currentPos);
        
        m_body->ApplyImpulse(CS::Vector2(0.0f, direction.y) * k_maxForce * in_timeSinceLastUpdate);
    }
}