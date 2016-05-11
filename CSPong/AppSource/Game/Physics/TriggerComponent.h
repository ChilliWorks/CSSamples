//
//  TriggerComponent.h
//  CSPong
//  Created by Ian Copland on 03/07/2014.
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

#ifndef _CSPONG_GAME_PHYSICS_TRIGGERCOMPONENT_H_
#define _CSPONG_GAME_PHYSICS_TRIGGERCOMPONENT_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>

namespace CSPong
{
    //------------------------------------------------------------
    /// A physics trigger component. Dynamic bodies will receive
    /// the "trigger" notification when in collision with this
    /// instead of resolving the collision.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------
    class TriggerComponent : public CS::Component
    {
    public:
        CS_DECLARE_NAMEDTYPE(TriggerComponent);
        //-----------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The physics system.
        /// @param The size of the physics body.
        //-----------------------------------------------------
        TriggerComponent(PhysicsSystem* in_physicsSystem, const CS::Vector2& in_size);
        //----------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CS::InterfaceIDType in_interfaceId) const override;
        //-----------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The size of the physics body.
        //-----------------------------------------------------
        const CS::Vector2& GetSize() const;
    private:
        //-----------------------------------------------------
        /// Called when the component is added to the scene.
        /// This can be either when its owner entity is added to
        /// the scene or when attached an an entity that is already
        /// in the scene. This will register the trigger with
        /// the physics system so that it is included in collision
        /// checks.
        ///
        /// @author Ian Copland
        //-----------------------------------------------------
        void OnAddedToScene() override;
        //-----------------------------------------------------
        /// Called when the component is removed from the scene.
        /// This can be either when its owner entity is removed
        /// from the scene or when the component is removed from
        /// an entity that is currently in the scene. This will
        /// deregister itself from the physics system and
        /// therefore will no longer be included in collision
        /// checks.
        ///
        /// @author Ian Copland
        //------------------------------------------------------
        void OnRemovedFromScene() override;
        
        PhysicsSystem* m_physicsSystem;
        const CS::Vector2 m_size;
    };
}

#endif
