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

#ifndef _APPSOURCE_PLAYER_PLAYERCONTROLLER_H_
#define _APPSOURCE_PLAYER_PLAYERCONTROLLER_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Tween.h>

#include <Animation/KeyframeAnimation.h>

namespace CSRunner
{
    enum class PlayerState
    {
        k_running,
        k_jumping,
        k_sliding,
        k_bombing,
        k_colliding,
    };
    
    /// Component that controls the actions of the runner (player).
    ///
    /// Handles the player actions: run, jump, slide, bump, etc in response
    /// to actions of the game or player
    ///
    /// NOTE: The player doesn't really move horizontally (except for bumping) instead the level moves
    ///
    class PlayerController final : public CS::Component
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(PlayerController);
        
        /// @param spriteComponent
        ///     Used to change the animation of the player and to grab the collision box
        ///
        PlayerController(CS::SpriteComponent* spriteComponent) noexcept;
        
        /// @param spriteComponent
        ///     Used to change the animation of the player and to grab the collision box
        /// @param audioPlayer
        ///     Used to play the player's action audio
        /// @param audioBank
        ///     Bank containing audio for the player
        ///
#ifndef CS_TARGETPLATFORM_RPI
        PlayerController(CS::SpriteComponent* spriteComponent, CS::CkAudioPlayer* audioPlayer, CS::CkBankCSPtr audioBank) noexcept;
#endif
        
        ///
        bool IsA(CS::InterfaceIDType interfaceId) const noexcept override;
        
        /// @return Player's axis-aligned collision box in world space
        ///
        CS::Rectangle GetWorldCollisionBox() const noexcept;

        /// Tell the player to jump. Depending on the current state
        /// the player may or may not jump
        ///
        void Jump() noexcept;
        
        /// Tell the player to slide. Depending on the current state
        /// the player may or may not slide
        ///
        void Slide() noexcept;
        
        /// Tell the player to bump back. Depending on the current state
        /// the player may or may not happen
        ///
        void Bump() noexcept;
        
        /// Tell the player to begin running
        ///
        void Run() noexcept;
        
        ///
        ~PlayerController() noexcept { }
        
    private:
        
        /// Update the state machine and process any commands
        ///
        /// @param timeSinceLastUpdate
        ////    Time since last update in seconds
        ///
        void OnUpdate(float timeSinceLastUpdate) noexcept override;
        
    private:
        
        CS::Rectangle m_localCollisionBox;
        CS::SpriteComponent* m_spriteComponent;
        
#ifndef CS_TARGETPLATFORM_RPI
        CS::CkAudioPlayer* m_audioPlayer = nullptr;
        CS::CkBankCSPtr m_audioBank;
#endif
        
        KeyframeAnimation m_runningAnimation;
        KeyframeAnimation m_jumpingAnimation;
        KeyframeAnimation m_bombingAnimation;
        KeyframeAnimation m_slidingAnimation;
        KeyframeAnimation m_collidingAnimation;
        
        PlayerState m_state = PlayerState::k_running;
        
        CS::EaseOutBackTween<f32> m_jumpTween;
        CS::EaseOutBackTween<f32> m_bombTween;
        CS::EaseInOutQuadTween<CS::Vector3> m_bumpTween;
        
        f32 m_slidingTimer = 0.0f;
    };
};

#endif
