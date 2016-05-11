//
//  CameraTiltComponent.h
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

#ifndef _APPSOURCE_GAME_CAMERA_CAMERATILTCOMPONENT_H_
#define _APPSOURCE_GAME_CAMERA_CAMERATILTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>

namespace CSPong
{
    //------------------------------------------------------------
    /// Tilts the camera based on the current accelerometer values
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class CameraTiltComponent final : public CS::Component
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(CameraTiltComponent);
        //----------------------------------------------------------
        /// Constructor
        ///
        /// @author S Downie
        ///
        /// @param Camera view direction
        //----------------------------------------------------------
        CameraTiltComponent(const CS::Vector3& in_viewDirection);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CS::InterfaceIDType in_interfaceId) const override;
        
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
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @param Time since last update in seconds
        //----------------------------------------------------
        void OnFixedUpdate(f32 in_timeSinceLastUpdate) override;
        
    private:
        
        CS::Vector3 m_tiltVelocity;
        CS::Vector3 m_unitTiltOffset;
        CS::Vector3 m_restingViewDirection;
        CS::Accelerometer* m_accelerometer = nullptr;
    };
}

#endif
