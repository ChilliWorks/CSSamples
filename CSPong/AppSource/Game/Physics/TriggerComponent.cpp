//
//  TriggerComponent.cpp
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

#include <Game/Physics/TriggerComponent.h>

#include <Game/Physics/PhysicsSystem.h>

namespace CSPong
{
    CS_DEFINE_NAMEDTYPE(TriggerComponent);
    //-----------------------------------------------------
    //-----------------------------------------------------
    TriggerComponent::TriggerComponent(PhysicsSystem* in_physicsSystem, const CSCore::Vector2& in_size)
    : m_physicsSystem(in_physicsSystem), m_size(in_size)
    {
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool TriggerComponent::IsA(CSCore::InterfaceIDType in_interfaceId) const
    {
        return (TriggerComponent::InterfaceID == in_interfaceId);
    }
    //-----------------------------------------------------
    //-----------------------------------------------------
    const CSCore::Vector2& TriggerComponent::GetSize() const
    {
        return m_size;
    }
    //-----------------------------------------------------
    //-----------------------------------------------------
    void TriggerComponent::OnAddedToScene()
    {
        m_physicsSystem->RegisterTrigger(this);
    }
    //-----------------------------------------------------
    //------------------------------------------------------
    void TriggerComponent::OnRemovedFromScene()
    {
        m_physicsSystem->DeregisterTrigger(this);
    }
}