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

#ifndef _APPSOURCE_COLLISION_COLLISIONSYSTEM_H_
#define _APPSOURCE_COLLISION_COLLISIONSYSTEM_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Event.h>
#include <ChilliSource/Core/System.h>

#include <functional>

namespace CSRunner
{
    /// Simple system that runs during the main game state checking for collisions between the generated objects and the player
    ///
    class CollisionSystem final : public CS::StateSystem
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(CollisionSystem);
        
        /// Factory method to create the system
        ///
        /// @param levelGenerator
        ///     Used to grab the current obstacles
        ///
        static CollisionSystemUPtr Create(const LevelGeneratorSystem* levelGenerator) noexcept;
        
        ///
        bool IsA(CS::InterfaceIDType interfaceId) const noexcept override;
        
        /// @param playerController
        ///     The player component used to check against obstacles
        ///
        void SetPlayer(const PlayerController* playerController) noexcept { m_playerController = playerController; }
        
        /// @return Event that is called when the player collides with an obstacle
        ///
        CS::IConnectableEvent<std::function<void(u32)>>& GetPlayerObstacleCollisionEvent() noexcept { return m_onPlayerObstacleCollision; };
        
    private:
        
        ///
        CollisionSystem(const LevelGeneratorSystem* levelGenerator) noexcept;
        
        /// Check for collisions and tell the player to perform any responses
        ///
        /// @param timeSinceLastUpdate
        ///     Time in seconds since last update
        ///
        void OnUpdate(f32 timeSinceLastUpdate) noexcept override;
        
    private:
        
        const PlayerController* m_playerController = nullptr;
        const LevelGeneratorSystem* m_levelGenerator;
        
        CS::Event<std::function<void(u32)>> m_onPlayerObstacleCollision;
    };
}

#endif
