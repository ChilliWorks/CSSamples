//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#include <Collision/CollisionSystem.h>

#include <ChilliSource/Core/Math.h>

#include <Environment/LevelGeneratorSystem.h>
#include <Player/PlayerController.h>

namespace CSRunner
{
    CS_DEFINE_NAMEDTYPE(CollisionSystem);
    
    //------------------------------------------------------------
    CollisionSystemUPtr CollisionSystem::Create(const LevelGeneratorSystem* levelGenerator) noexcept
    {
        return CollisionSystemUPtr(new CollisionSystem(levelGenerator));
    }
    
    //------------------------------------------------------------
    CollisionSystem::CollisionSystem(const LevelGeneratorSystem* levelGenerator) noexcept
    : m_levelGenerator(levelGenerator)
    {
        
    }
    
    //------------------------------------------------------------
    bool CollisionSystem::IsA(CS::InterfaceIDType interfaceId) const noexcept
    {
        return interfaceId == CollisionSystem::InterfaceID;
    }
    
    //------------------------------------------------------------
    void CollisionSystem::OnUpdate(f32 timeSinceLastUpdate) noexcept
    {
        CS::Rectangle playerBox = m_playerController->GetWorldCollisionBox();
        
        //Player only collides with one obstacle at a time to prevent them being doubly punished
        u32 index = 0;
        std::vector<CS::Rectangle> obstacleBoxes = m_levelGenerator->GetObstacleCollisionBoxes();
        for(const auto& obBox : obstacleBoxes)
        {
            if(CS::ShapeIntersection::Intersects(playerBox, obBox) == true)
            {
                m_onPlayerObstacleCollision.NotifyConnections(index);
                break;
            }
            
            ++index;
        }
    }
}

