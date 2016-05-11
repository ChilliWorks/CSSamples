//
//  CameraTiltComponent.cpp
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

#include <Game/Camera/CameraTiltComponent.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Input/Accelerometer.h>

namespace CSPong
{
    CS_DEFINE_NAMEDTYPE(CameraTiltComponent);
    
    //----------------------------------------------------------
    //----------------------------------------------------------
    CameraTiltComponent::CameraTiltComponent(const CS::Vector3& in_viewDirection)
    : m_restingViewDirection(in_viewDirection)
    {
        
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool CameraTiltComponent::IsA(CS::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == CameraTiltComponent::InterfaceID;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void CameraTiltComponent::OnAddedToScene()
    {
        m_accelerometer = CS::Application::Get()->GetSystem<CS::Accelerometer>();
        
        if(m_accelerometer != nullptr)
        {
            m_accelerometer->StartUpdating();
        }
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void CameraTiltComponent::OnRemovedFromScene()
    {
        if(m_accelerometer != nullptr)
        {
            m_accelerometer->StopUpdating();
        }
        
        m_accelerometer = nullptr;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void CameraTiltComponent::OnFixedUpdate(f32 in_timeSinceLastUpdate)
    {
        if (m_accelerometer != nullptr)
        {
            const f32 k_angleFromFlat = -CS::MathUtils::k_pi * 0.25f;
            const f32 k_accelerationFactor = 40.0f;
            const f32 k_deadzoneLimit = 10.0f;
            const f32 k_drag = 0.85f;
            const f32 k_scale = 1.0f;
            
            //calculate the offset from the perfect down vector from the accelerometer
            CS::Matrix4 matZDownSpace;
            matZDownSpace.RotateX(-k_angleFromFlat);
            CS::Vector3 perfectDown = CS::Vector3::k_unitNegativeZ;
            CS::Vector3 reading = m_accelerometer->GetAcceleration();
            CS::Vector3 actualDown = reading * matZDownSpace;
            CS::Vector3 offset = perfectDown - actualDown;
            CS::Vector3 offsetXY = CS::Vector3(offset.x, offset.y, 0.0f);
            CS::Vector3 offsetXYDirection = CS::Vector3::Normalise(offsetXY);
            
            //calculate the angle and magnitude of the acceleration
            f32 angle = -CS::MathUtils::k_pi / 2.0f - atan2(offsetXYDirection.y, offsetXYDirection.x);
            f32 accelerationMagnitude = offsetXY.Length() * k_accelerationFactor * in_timeSinceLastUpdate;
            
            //calulate the acceleration vector taking into account a small deadzone
            f32 deadZoneMagnitude = k_deadzoneLimit * in_timeSinceLastUpdate;
            CS::Vector3 acceleration;
            if (accelerationMagnitude > deadZoneMagnitude)
            {
                acceleration.x = std::sin(angle) * (accelerationMagnitude - deadZoneMagnitude) * k_scale;
                acceleration.z = 0.0f;
                acceleration.y = std::cos(angle) * (accelerationMagnitude - deadZoneMagnitude) * k_scale;
            }
            else
            {
                acceleration = CS::Vector3::k_zero;
            }
            
            //calculate the velocity and offset and apply drag.
            m_tiltVelocity += acceleration;
            m_tiltVelocity *= k_drag;
            m_unitTiltOffset += m_tiltVelocity * in_timeSinceLastUpdate;
            m_unitTiltOffset *= k_drag;
            
            CS::Vector3 position(GetEntity()->GetTransform().GetLocalPosition());
            CS::Vector3 target(position + m_restingViewDirection);
            target += m_unitTiltOffset * m_restingViewDirection.Length();
            
            GetEntity()->GetTransform().SetLookAt(position, target, CS::Vector3::k_unitPositiveY);
        }
    }
}
