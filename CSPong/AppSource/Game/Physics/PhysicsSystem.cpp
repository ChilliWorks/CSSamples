//
//  PhysicsSystem.cpp
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

#include <Game/Physics/PhysicsSystem.h>

#include <Game/Physics/DynamicBodyComponent.h>
#include <Game/Physics/StaticBodyComponent.h>
#include <Game/Physics/TriggerComponent.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>

namespace CSPong
{
    namespace
    {
        //---------------------------------------------------------
        /// A container for information returned from a collision
        /// test such as the intersection or the collision direction.
        ///
        /// @author Ian Copland
        //---------------------------------------------------------
        struct CollisionResolution
        {
            CSCore::Vector2 m_intersectionResolution;
            CSCore::Vector2 m_collisionDirection;
        };
        //---------------------------------------------------------
        /// Tests for collision between two dynamic bodies. If
        /// a collision did occur, information on the collision
        /// will be returned in the CollisionResolution struct.
        ///
        /// @author Ian Copland
        ///
        /// @param The dynamic body A.
        /// @param The dynamic body B.
        /// @param [Out] The collision resolution.
        ///
        /// @return Whether or not the bodies were in collision.
        //---------------------------------------------------------
        bool TestCollision(const DynamicBodyComponent* in_bodyA, const DynamicBodyComponent* in_bodyB, CollisionResolution& out_resolution)
        {
            const CSCore::Entity* entityA = in_bodyA->GetEntity();
            const CSCore::Entity* entityB = in_bodyB->GetEntity();
            
            const CSCore::Rectangle rectA(entityA->GetTransform().GetWorldPosition().XY(), in_bodyA->GetSize());
            const CSCore::Rectangle rectB(entityB->GetTransform().GetWorldPosition().XY(), in_bodyB->GetSize());
            
            if (rectA.Left() < rectB.Right() && rectA.Right() > rectB.Left() && rectA.Bottom() < rectB.Top() && rectA.Top() > rectB.Bottom())
            {
                f32 minIntersection = 9999999.0f;
                out_resolution.m_collisionDirection = CSCore::Vector2(1.0f, 0.0f);
                
                f32 intersection = rectB.Right() - rectA.Left();
                if (intersection < minIntersection)
                {
                    minIntersection = intersection;
                    out_resolution.m_collisionDirection = CSCore::Vector2(1.0f, 0.0f);
                }
                
                intersection = rectA.Right() - rectB.Left();
                if (intersection < minIntersection)
                {
                    minIntersection = intersection;
                    out_resolution.m_collisionDirection = CSCore::Vector2(-1.0f, 0.0f);
                }
                
                intersection = rectB.Top() - rectA.Bottom();
                if (intersection < minIntersection)
                {
                    minIntersection = intersection;
                    out_resolution.m_collisionDirection = CSCore::Vector2(0.0f, 1.0f);
                }
                
                intersection = rectA.Top() - rectB.Bottom();
                if (intersection < minIntersection)
                {
                    minIntersection = intersection;
                    out_resolution.m_collisionDirection = CSCore::Vector2(0.0f, -1.0f);
                }
                
                out_resolution.m_intersectionResolution = out_resolution.m_collisionDirection * minIntersection;
                
                return true;
            }
            
            return false;
        }
        //---------------------------------------------------------
        /// Tests for collision between a dynamic body and a static
        /// body. If a collision did occur, information on the
        /// collision will be returned in the CollisionResolution
        /// struct.
        ///
        /// @author Ian Copland
        ///
        /// @param The dynamic body.
        /// @param The static body.
        /// @param [Out] The collision resolution.
        ///
        /// @return Whether or not the bodies were in collision.
        //---------------------------------------------------------
        bool TestCollision(const DynamicBodyComponent* in_dynamic, const StaticBodyComponent* in_static, CollisionResolution& out_resolution)
        {
            const CSCore::Entity* dynamicEnt = in_dynamic->GetEntity();
            const CSCore::Entity* staticEnt = in_static->GetEntity();
            
            const CSCore::Rectangle dynamicRect(dynamicEnt->GetTransform().GetWorldPosition().XY(), in_dynamic->GetSize());
            const CSCore::Rectangle staticRect(staticEnt->GetTransform().GetWorldPosition().XY(), in_static->GetSize());
            
            if (dynamicRect.Left() < staticRect.Right() && dynamicRect.Right() > staticRect.Left() && dynamicRect.Bottom() < staticRect.Top() && dynamicRect.Top() > staticRect.Bottom())
            {
                f32 minIntersection = 9999999.0f;
                out_resolution.m_collisionDirection = CSCore::Vector2(1.0f, 0.0f);
                
                f32 intersection = staticRect.Right() - dynamicRect.Left();
                if (intersection < minIntersection)
                {
                    minIntersection = intersection;
                    out_resolution.m_collisionDirection = CSCore::Vector2(1.0f, 0.0f);
                }
                
                intersection = dynamicRect.Right() - staticRect.Left();
                if (intersection < minIntersection)
                {
                    minIntersection = intersection;
                    out_resolution.m_collisionDirection = CSCore::Vector2(-1.0f, 0.0f);
                }
                
                intersection = staticRect.Top() - dynamicRect.Bottom();
                if (intersection < minIntersection)
                {
                    minIntersection = intersection;
                    out_resolution.m_collisionDirection = CSCore::Vector2(0.0f, 1.0f);
                }
                
                intersection = dynamicRect.Top() - staticRect.Bottom();
                if (intersection < minIntersection)
                {
                    minIntersection = intersection;
                    out_resolution.m_collisionDirection = CSCore::Vector2(0.0f, -1.0f);
                }
                
                out_resolution.m_intersectionResolution = out_resolution.m_collisionDirection * minIntersection;
                
                return true;
            }
            
            return false;
        }
        //---------------------------------------------------------
        /// Tests for collision between a dynamic body and a trigger.
        ///
        /// @author Ian Copland
        ///
        /// @param The dynamic body.
        /// @param The trigger.
        ///
        /// @return Whether or not the bodies were in collision.
        //---------------------------------------------------------
        bool TestCollision(const DynamicBodyComponent* in_dynamic, const TriggerComponent* in_static)
        {
            const CSCore::Entity* dynamicEnt = in_dynamic->GetEntity();
            const CSCore::Entity* staticEnt = in_static->GetEntity();
            
            CSCore::Rectangle dynamicRect(dynamicEnt->GetTransform().GetWorldPosition().XY(), in_dynamic->GetSize());
            CSCore::Rectangle staticRect(staticEnt->GetTransform().GetWorldPosition().XY(), in_static->GetSize());
            if (dynamicRect.Left() < staticRect.Right() && dynamicRect.Right() > staticRect.Left() && dynamicRect.Bottom() < staticRect.Top() && dynamicRect.Top() > staticRect.Bottom())
            {
                return true;
            }
            
            return false;
        }
    }
    
    CS_DEFINE_NAMEDTYPE(PhysicsSystem);
    //---------------------------------------------------
    //---------------------------------------------------
    PhysicsSystemUPtr PhysicsSystem::Create()
    {
        return PhysicsSystemUPtr(new PhysicsSystem());
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool PhysicsSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == PhysicsSystem::InterfaceID;
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void PhysicsSystem::RegisterDynamicBody(DynamicBodyComponent* in_dynamicBody)
    {
        m_dynamicBodies.push_back(in_dynamicBody);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void PhysicsSystem::DeregisterDynamicBody(DynamicBodyComponent* in_dynamicBody)
    {
        for (auto it = m_dynamicBodies.begin(); it != m_dynamicBodies.end();)
        {
            if ((*it) == in_dynamicBody)
            {
                it = m_dynamicBodies.erase(it);
                break;
            }
            else
            {
                ++it;
            }
        }
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void PhysicsSystem::RegisterStaticBody(StaticBodyComponent* in_staticBody)
    {
        m_staticBodies.push_back(in_staticBody);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void PhysicsSystem::DeregisterStaticBody(StaticBodyComponent* in_staticBody)
    {
        for (auto it = m_staticBodies.begin(); it != m_staticBodies.end();)
        {
            if ((*it) == in_staticBody)
            {
                it = m_staticBodies.erase(it);
                break;
            }
            else
            {
                ++it;
            }
        }
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void PhysicsSystem::RegisterTrigger(TriggerComponent* in_trigger)
    {
        m_triggers.push_back(in_trigger);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void PhysicsSystem::DeregisterTrigger(TriggerComponent* in_trigger)
    {
        for (auto it = m_triggers.begin(); it != m_triggers.end();)
        {
            if ((*it) == in_trigger)
            {
                it = m_triggers.erase(it);
                break;
            }
            else
            {
                ++it;
            }
        }
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void PhysicsSystem::OnFixedUpdate(f32 in_deltaTime)
    {
        for (auto& dynamicBody : m_dynamicBodies)
        {
            dynamicBody->OnPhysicsUpdate(in_deltaTime);
        }
        
        ResolveCollisions();
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void PhysicsSystem::ResolveCollisions()
    {
        CollisionResolution resolution;
        
        for (auto& dynamicBodyA : m_dynamicBodies)
        {
            for (auto& dynamicBodyB : m_dynamicBodies)
            {
                if (dynamicBodyA != dynamicBodyB)
                {
                    if (TestCollision(dynamicBodyA, dynamicBodyB, resolution) == true)
                    {
                        dynamicBodyA->OnDynamicCollision(resolution.m_collisionDirection, resolution.m_intersectionResolution, dynamicBodyB);
                        
                        resolution.m_collisionDirection *= -1.0f;
                        resolution.m_intersectionResolution *= -1.0f;
                        dynamicBodyB->OnDynamicCollision(resolution.m_collisionDirection, resolution.m_intersectionResolution, dynamicBodyA);
                    }
                }
            }
        }
        
        for (auto& dynamicBody : m_dynamicBodies)
        {
            for (const auto& staticBody : m_staticBodies)
            {
                if (TestCollision(dynamicBody, staticBody, resolution) == true)
                {
                    dynamicBody->OnStaticCollision(resolution.m_collisionDirection, resolution.m_intersectionResolution, staticBody);
                }
            }
        }
        
        for (auto& dynamicBody : m_dynamicBodies)
        {
            for (const auto& trigger : m_triggers)
            {
                if (TestCollision(dynamicBody, trigger) == true)
                {
                    dynamicBody->OnTriggerCollision(trigger);
                }
            }
        }
    }
}