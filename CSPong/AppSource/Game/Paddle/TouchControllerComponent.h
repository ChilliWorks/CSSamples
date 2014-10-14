//
//  TouchControllerComponent.h
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

#ifndef _APPSOURCE_GAME_PADDLE_TOUCHCONTROLLERCOMPONENT_H_
#define _APPSOURCE_GAME_PADDLE_TOUCHCONTROLLERCOMPONENT_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Input/Pointer.h>

namespace CSPong
{
    //------------------------------------------------------------
    /// Class that translates screen touches into entity
    /// movement along the ground axis
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class TouchControllerComponent final : public CSCore::Component
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(TouchControllerComponent);
        
        //----------------------------------------------------------
        /// Constructor
        ///
        /// @author S Downie
        ///
        /// @param Physics body used to move the object
        /// @param Camera used to project input into world space
        //----------------------------------------------------------
        TouchControllerComponent(const DynamicBodyComponentSPtr& in_body, const CSRendering::CameraComponentSPtr& in_cameraComponent);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
        
    private:
        
        //----------------------------------------------------
        /// Triggered when the component is attached to
        /// an entity and that entity is on a scene.
        /// The component will start listening for
        /// user input.
        ///
        /// @author S Downie
        //----------------------------------------------------
        void OnAddedToScene() override;
        //----------------------------------------------------
        /// Triggered when the component is removed from
        /// an entity or when the entity is removed from the
        /// scene. The component will stop listening for
        /// user input.
        ///
        /// @author S Downie
        //----------------------------------------------------
        void OnRemovedFromScene() override;
        //-----------------------------------------------------------
        /// Called when the window receives cursor/touch input.
        /// Moves the entity of this component along the ground axis
        /// towards the pointer location
        ///
        /// @author S Downie
        ///
        /// @param The pointer
        /// @param The timestamp.
        /// @param Input type
        //-----------------------------------------------------------
        void OnPointerDown(const CSInput::Pointer& in_pointer, f64 in_timestamp, CSInput::Pointer::InputType in_inputType);
        //-----------------------------------------------------------
        /// Called when the window receives cursor/touch input.
        /// Moves the entity of this component along the ground axis
        /// towards the pointer location
        ///
        /// @author S Downie
        ///
        /// @param The pointer
        /// @param The timestamp.
        //-----------------------------------------------------------
        void OnPointerMoved(const CSInput::Pointer& in_pointer, f64 in_timestamp);
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @param Time since last update in seconds
        //----------------------------------------------------
        void OnFixedUpdate(f32 in_timeSinceLastUpdate) override;
        //----------------------------------------------------
        /// Update the target position based on the given
        /// input position
        ///
        /// @author S Downie
        ///
        /// @param Screen space position
        //----------------------------------------------------
        void UpdateTargetPosition(const CSCore::Vector2& in_screenPos);
        
    private:
        
        CSCore::EventConnectionUPtr m_pointerDownConnection;
        CSCore::EventConnectionUPtr m_pointerMovedConnection;
        DynamicBodyComponentSPtr m_body;
        CSRendering::CameraComponentSPtr m_cameraComponent;
        
        CSCore::Vector3 m_targetPos;
        
        f32 m_currentSpeed = 0.0f;
    };
}

#endif
