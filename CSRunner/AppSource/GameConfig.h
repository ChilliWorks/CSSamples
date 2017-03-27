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

#ifndef _APPSOURCE_CONFIG_GAMECONFIG_H_
#define _APPSOURCE_CONFIG_GAMECONFIG_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Math/Vector2.h>

namespace CSRunner
{
    namespace GameConfig
    {
        //Most of these values are based on the sizes of the sprites
        constexpr f32 k_playerWidth = 210.0f;
        constexpr f32 k_playerHeight = 222.0f;
        
        constexpr f32 k_levelWidth = k_playerWidth * 7.0f;
        
        constexpr f32 k_playerJumpHeight = k_playerHeight * 2.0f;
        constexpr f32 k_playerJumpDuration = 0.7f;
        constexpr f32 k_playerBombDuration = 0.2f;
        constexpr f32 k_playerSlideDuration = 0.75f;
        constexpr f32 k_playerBumpDuration = 0.2f;
        constexpr f32 k_playerBumpBackDistance = k_levelWidth * 0.1f;
        constexpr f32 k_playerWidthCollisionScale = 0.8f;
        
        constexpr u32 k_maxObstacles = 8;
        constexpr f32 k_scrollSpeed = k_levelWidth * 0.7f;
        constexpr f32 k_minDistanceBetweenObstacles = k_levelWidth * 0.35f;
        constexpr f32 k_maxDistanceBetweenObstacles = k_levelWidth * 1.5f;
        constexpr f32 k_lowObstacleProbability = 2.0f;
        constexpr f32 k_highObstacleProbability = 1.0f;
        constexpr f32 k_startingX = 0.0f;
        
        constexpr f32 k_cullX = -k_levelWidth * 0.5f;
        constexpr f32 k_groundY = -k_playerHeight * 1.5f;
        constexpr f32 k_highObstacleSpawnOffset = k_playerHeight * 0.75f;
        constexpr f32 k_lowObstacleSpawnOffset = 0.0f;
        constexpr f32 k_playerSlideHeight = k_playerHeight * 0.5f;
        constexpr f32 k_ceilingY = k_groundY + k_highObstacleSpawnOffset + 420.0f;
    }
}

#endif
