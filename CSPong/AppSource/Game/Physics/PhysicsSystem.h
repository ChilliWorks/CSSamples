//
//  PhysicsSystem.h
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

#ifndef _APPSOURCE_GAME_COLLISIONS_PHYSICSSYSTEM_H_
#define _APPSOURCE_GAME_COLLISIONS_PHYSICSSYSTEM_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/System.h>

namespace CSPong
{
    //------------------------------------------------------------
    /// State system that handles basic physics in a scene. This
    /// includes applying the physics update to dynamic bodies
    /// and performing collision checks between static and
    /// dynamic bodies.
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class PhysicsSystem final : public CSCore::StateSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(PhysicsSystem);
        
        //---------------------------------------------------
        /// Creates a new instance of the system.
        ///
        /// @author S Downie
        ///
        /// @return The new instance.
        //---------------------------------------------------
        static PhysicsSystemUPtr Create();
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
    private:
        friend class DynamicBodyComponent;
        friend class StaticBodyComponent;
        friend class TriggerComponent;
        //----------------------------------------------------------
        /// Private constructor to enforce use of factory method
        ///
        /// @author S Downie
        //----------------------------------------------------------
        PhysicsSystem() = default;
        //----------------------------------------------------------
        /// Registers a Dynamic Body Component with the system so that
        /// it receives physics updates and collision checks are
        /// performed on it.
        ///
        /// @author Ian Copland
        ///
        /// @param The Dynamic Body Component
        //----------------------------------------------------------
        void RegisterDynamicBody(DynamicBodyComponent* in_dynamicBody);
        //----------------------------------------------------------
        /// Removes a Dynamic Body Component from the system so that
        /// it no longer receives physics updates and collision
        /// checks are no longer performed on it.
        ///
        /// @author Ian Copland
        ///
        /// @param The Dynamic Body Component
        //----------------------------------------------------------
        void DeregisterDynamicBody(DynamicBodyComponent* in_dynamicBody);
        //----------------------------------------------------------
        /// Registers a Static Body Component with the system so that
        /// it is included in collision checks.
        ///
        /// @author Ian Copland
        ///
        /// @param The Static Body Component
        //----------------------------------------------------------
        void RegisterStaticBody(StaticBodyComponent* in_staticBody);
        //----------------------------------------------------------
        /// Removes a Static Body Component from the system so that
        /// it no longer included in collision checks.
        ///
        /// @author Ian Copland
        ///
        /// @param The Dynamic Body Component
        //----------------------------------------------------------
        void DeregisterStaticBody(StaticBodyComponent* in_staticBody);
        //----------------------------------------------------------
        /// Registers a Trigger Component with the system so that
        /// it is included in collision checks.
        ///
        /// @author Ian Copland
        ///
        /// @param The Trigger Component
        //----------------------------------------------------------
        void RegisterTrigger(TriggerComponent* in_trigger);
        //----------------------------------------------------------
        /// Removes a Trigger Component from the system so that
        /// it no longer included in collision checks.
        ///
        /// @author Ian Copland
        ///
        /// @param The Trigger Component
        //----------------------------------------------------------
        void DeregisterTrigger(TriggerComponent* in_trigger);
        //----------------------------------------------------------
        /// Called every fixed update frame. This will call physics
        /// update on all dynamic bodies and perform collision
        /// checks.
        ///
        /// @author Ian Copland
        ///
        /// @param The fixed time since last update.
        //----------------------------------------------------------
        void OnFixedUpdate(f32 in_deltaTime) override;
        //----------------------------------------------------------
        /// Performs collision tests on all dynamic bodies against
        /// other dynamic bodies and static bodies. If any are in
        /// collision the dynamic body will be notified to respond.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------
        void ResolveCollisions();
        
        std::vector<DynamicBodyComponent*> m_dynamicBodies;
        std::vector<StaticBodyComponent*> m_staticBodies;
        std::vector<TriggerComponent*> m_triggers;
    };
}

#endif
