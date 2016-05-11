//
//  AIControllerComponent.h
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

#ifndef _APPSOURCE_GAME_PADDLE_AICONTROLLERCOMPONENT_H_
#define _APPSOURCE_GAME_PADDLE_AICONTROLLERCOMPONENT_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Entity.h>

namespace CSPong
{
    //------------------------------------------------------------
    /// Class that controls the AI logic for attempting to
    /// hit the ball
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class AIControllerComponent final : public CS::Component
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(AIControllerComponent);
        
        //----------------------------------------------------------
        /// Constructor
        ///
        /// @author S Downie
        ///
        /// @param Physics body used to move the object
        /// @param Entity to chase (ball)
        //----------------------------------------------------------
        AIControllerComponent(const DynamicBodyComponentSPtr& in_body, const CS::EntitySPtr& in_targetEntity);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CS::InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------
        /// Reset the paddle back to the centre of its rail.
        /// Usually after a goal
        ///
        /// @author S Downie
        //----------------------------------------------------
        void Reset();
        
    private:
        
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @param Current position of the entity
        ///
        /// @return True if the paddle is moving towards us
        //----------------------------------------------------
        bool IsTargetMovingTowardsUs(const CS::Vector3& in_currentPos) const;
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @param Time since last update in seconds
        //----------------------------------------------------
        void OnFixedUpdate(f32 in_timeSinceLastUpdate) override;
        
    private:
        
        CS::EntitySPtr m_target;
        DynamicBodyComponentSPtr m_targetBody;
        DynamicBodyComponentSPtr m_body;
        f32 m_currentSpeed = 0.0f;
    };
}

#endif
