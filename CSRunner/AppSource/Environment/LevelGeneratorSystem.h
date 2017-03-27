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

#ifndef _APPSOURCE_ENVIRONMENT_LEVELGENERATORSYSTEM_H_
#define _APPSOURCE_ENVIRONMENT_LEVELGENERATORSYSTEM_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/System.h>
#include <ChilliSource/Core/Math.h>

#include <Animation/KeyframeAnimation.h>

#include <vector>

namespace CSRunner
{
    /// Responsible for spawning obstacles and scrolling them
    /// to simulate the player running. Ensures only a certain number of objects are
    /// on screen at any time
    ///
    class LevelGeneratorSystem final : public CS::StateSystem
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(LevelGeneratorSystem);
        
        ///
        static LevelGeneratorSystemUPtr Create() noexcept;
        
        ///
        bool IsA(CS::InterfaceIDType interfaceId) const noexcept override;
        
        /// Called as a result of a collision between player and an obstacle.
        /// The system will destroy the obstacle at the given index
        ///
        /// @param index
        ///     Index of hit obstacle
        ///
        void DestroyObstacle(u32 index) noexcept;
        
        /// @return Axis aligned world collision boxes for current obstacles
        ///
        const std::vector<CS::Rectangle>& GetObstacleCollisionBoxes() const noexcept { return m_obstacleCollisionBoxes; }
        
    private:
        
        ///
        LevelGeneratorSystem() = default;
        
        /// Create obstacles
        ///
        void OnInit() noexcept override;
        
        /// Move the objects and spawn new ones
        ///
        void OnUpdate(f32 timeSinceLastUpdate) noexcept override;
        
        std::vector<CS::Rectangle> m_obstacleCollisionBoxes;
        
        std::vector<CS::EntitySPtr> m_activeObstacles;
        std::vector<CS::EntitySPtr> m_inactiveObstacles;
        
        std::vector<CS::EntitySPtr> m_bgTiles;
        std::vector<CS::EntitySPtr> m_groundTiles;
        std::vector<CS::EntitySPtr> m_ceilingTiles;
        
        CS::EntitySPtr m_explosionEffect;
        KeyframeAnimationUPtr m_explosionAnimation;
        
        f32 m_bgSizeX = 0.0f;
        f32 m_fgSizeX = 0.0f;
    };
}

#endif
