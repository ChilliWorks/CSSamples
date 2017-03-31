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

#ifndef _APP_STATES_GAMESTATE_H_
#define _APP_STATES_GAMESTATE_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/State.h>
#include <ChilliSource/Input/Gesture.h>

namespace CSRunner
{
    /// The main controller for the runner part of the game.
    /// Responsible for creating and updating all the main systems
    /// including the HUD, player and "physics".
    ///
    class GameState final : public CS::State
    {
    private:

        /// Create the collision and level generation systems
        ///
        void CreateSystems() noexcept override;
        
        /// Create the UI, camera and player
        ///
        void OnInit() noexcept override;
        
        /// Updates the player, level and collisions
        ///
        /// @param timeSinceLastUpdate
        ///     Time since last update in seconds
        ///
        void OnUpdate(f32 timeSinceLastUpdate) noexcept override;
        
        /// Called by the gesture system when the player drags used to make the runner jump or slide
        /// depending on the direction of the drag.
        ///
        /// @param gesture
        ///     Gesture that was triggered
        /// @param info
        ///     Distance since drag start, distance since last update and current position
        ///
        void OnDragged(const CS::DragGesture* gesture, const CS::DragGesture::DragInfo& info) noexcept;
        
        /// Called by the collision system when the player collides with the obstacle and the given index
        ///
        /// @param index
        ///     Index of colliding obstacle in the generator system
        ///
        void OnPlayerObstacleCollision(u32 index) noexcept;
        
        ///
        void OnDestroy() noexcept override;
        
    private:
        
        CS::GestureSystem* m_gestureSystem;
        LevelGeneratorSystem* m_levelGenerator;
        CollisionSystem* m_collisionSystem;
        TransitionSystem* m_transitionSystem;
        PlayerController* m_playerController;
        CS::CkAudioPlayer* m_audioPlayer = nullptr;
        CS::Keyboard* m_keyboard;
        
        CS::EventConnectionUPtr m_dragMovedEventConnection;
        CS::EventConnectionUPtr m_obstacleCollisionEventConnection;
        
        CS::TextUIComponent* m_timeSurvivedLabel = nullptr;
        
        f32 m_timeSurvived = 0.0f;
        f32 m_minDragDistance = 0.0f;
    };
}

#endif
