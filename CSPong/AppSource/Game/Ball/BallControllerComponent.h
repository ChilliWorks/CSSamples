//
//  BallControllerComponent.h
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

#ifndef _APPSOURCE_GAME_BALL_BALLCONTROLLERCOMPONENT_H_
#define _APPSOURCE_GAME_BALL_BALLCONTROLLERCOMPONENT_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>

namespace CSPong
{
    //------------------------------------------------------------
    /// Class that moves the ball around the arena and responds
    /// to collisions
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class BallControllerComponent final : public CS::Component
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(BallControllerComponent);
        //----------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The dynamic body component
        //----------------------------------------------------------
        BallControllerComponent(DynamicBodyComponent* in_dynamicBodyComponent);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CS::InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------
        /// Activate the ball and starts it moving.
        ///
        /// @author S Downie
        //----------------------------------------------------
        void Activate();
        //----------------------------------------------------
        /// Deactivate the ball and resets it position back
        /// to the centre.
        ///
        /// @author S Downie
        //----------------------------------------------------
        void Deactivate();

    private:
        //----------------------------------------------------
        /// Called whenever the owning entity collides with
        /// a static or dynamic body. This is used to update
        /// the balls current direction.
        ///
        /// @author Ian Copland
        ///
        /// @param The collision direction.
        /// @param The entity collided with.
        //----------------------------------------------------
        void OnCollision(const CS::Vector2& in_direction, CS::Entity* in_collidedWith);
        
    private:
        
        DynamicBodyComponent* m_dynamicBodyComponent;
        CS::Vector2 m_currentDirection;
        CS::EventConnectionUPtr m_collisionConnection;
        
        bool m_active = true;
    };
}

#endif
