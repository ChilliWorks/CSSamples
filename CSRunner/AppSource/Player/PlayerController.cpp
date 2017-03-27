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

#include <Player/PlayerController.h>

#include <ChilliSource/Audio/CricketAudio.h>
#include <ChilliSource/Rendering/Sprite.h>

#include <GameConfig.h>

namespace CSRunner
{
    CS_DEFINE_NAMEDTYPE(PlayerController);
    
    //------------------------------------------------------------
    PlayerController::PlayerController(CS::SpriteComponent* spriteComponent) noexcept
    :
    m_spriteComponent(spriteComponent),
    m_runningAnimation(spriteComponent, {"Run_01","Run_02","Run_03","Run_04","Run_05","Run_06","Run_07","Run_08","Run_09"}),
    m_jumpingAnimation(spriteComponent, {"Jump_01"}),
    m_bombingAnimation(spriteComponent, {"Jump_01"}),
    m_slidingAnimation(spriteComponent, {"Slide_01"}),
    m_collidingAnimation(spriteComponent, {"Collide_01","Collide_02","Collide_03","Collide_04","Collide_05","Collide_06","Collide_07","Collide_08","Collide_09"})
    {
        //Align the player on the ground
        m_spriteComponent->SetOriginAlignment(CS::AlignmentAnchor::k_bottomCentre);
        m_localCollisionBox.vOrigin = CS::Vector2(0.0f, GameConfig::k_playerHeight * 0.5f);
        m_localCollisionBox.vSize = CS::Vector2(GameConfig::k_playerWidth * GameConfig::k_playerWidthCollisionScale, GameConfig::k_playerHeight);
    }
    
#ifndef CS_TARGETPLATFORM_RPI
    //------------------------------------------------------------
    PlayerController::PlayerController(CS::SpriteComponent* spriteComponent, CS::CkAudioPlayer* audioPlayer, CS::CkBankCSPtr audioBank) noexcept
    : PlayerController(spriteComponent)
    {
        m_audioPlayer = audioPlayer;
        m_audioBank = audioBank;
    }
#endif
    
    //------------------------------------------------------------
    bool PlayerController::IsA(CS::InterfaceIDType interfaceId) const noexcept
    {
        return interfaceId == PlayerController::InterfaceID;
    }
    
    //------------------------------------------------------------
    CS::Rectangle PlayerController::GetWorldCollisionBox() const noexcept
    {
        CS::Rectangle worldBox = m_localCollisionBox;
        worldBox.vOrigin += GetEntity()->GetTransform().GetWorldPosition().XY();
        return worldBox;
    }
    
    //------------------------------------------------------------
    void PlayerController::Jump() noexcept
    {
        if(m_state == PlayerState::k_running || m_state == PlayerState::k_sliding)
        {
            m_state = PlayerState::k_jumping;
            m_localCollisionBox.vSize.y = GameConfig::k_playerHeight;
            
            m_jumpTween = CS::MakeEaseOutBackTween(GameConfig::k_groundY, GameConfig::k_groundY + GameConfig::k_playerJumpHeight, GameConfig::k_playerJumpDuration);
            m_jumpTween.Play(CS::TweenPlayMode::k_pingPong);
            
#ifndef CS_TARGETPLATFORM_RPI
            m_audioPlayer->PlayEffect(m_audioBank, "Jump");
#endif
        }
    }
    
    //------------------------------------------------------------
    void PlayerController::Slide() noexcept
    {
        if(m_state == PlayerState::k_running)
        {
            m_slidingTimer = 0.0f;
            m_localCollisionBox.vSize.y = GameConfig::k_playerSlideHeight;
            m_state = PlayerState::k_sliding;
            
#ifndef CS_TARGETPLATFORM_RPI
            m_audioPlayer->PlayEffect(m_audioBank, "Slide");
#endif
        }
        else if(m_state == PlayerState::k_jumping)
        {
            m_state = PlayerState::k_bombing;
            m_localCollisionBox.vSize.y = GameConfig::k_playerHeight;
       
            m_bombTween = CS::MakeEaseOutBackTween(GetEntity()->GetTransform().GetLocalPosition().y, GameConfig::k_groundY, GameConfig::k_playerBombDuration);
            m_bombTween.Play(CS::TweenPlayMode::k_once);
            
#ifndef CS_TARGETPLATFORM_RPI
            m_audioPlayer->PlayEffect(m_audioBank, "Slide");
#endif
        }
    }

    //------------------------------------------------------------
    void PlayerController::Bump() noexcept
    {
        if(m_state != PlayerState::k_colliding)
        {
            m_state = PlayerState::k_colliding;
            m_localCollisionBox.vSize.y = GameConfig::k_playerHeight;
       
            CS::Vector3 pos = GetEntity()->GetTransform().GetLocalPosition();
            m_bumpTween = CS::MakeEaseInOutQuadTween(pos, CS::Vector3(pos.x - GameConfig::k_playerBumpBackDistance, GameConfig::k_groundY, 0.0f), GameConfig::k_playerBumpDuration);
            m_bumpTween.Play(CS::TweenPlayMode::k_once);
            
#ifndef CS_TARGETPLATFORM_RPI
            m_audioPlayer->PlayEffect(m_audioBank, "Break");
#endif
        }
    }
    
    //------------------------------------------------------------
    void PlayerController::Run() noexcept
    {
        m_state = PlayerState::k_running;
        m_localCollisionBox.vSize.y = GameConfig::k_playerHeight;
    }
    
    //------------------------------------------------------------
    void PlayerController::OnUpdate(f32 timeSinceLastUpdate) noexcept
    {
        bool transformDirty = false;
        CS::Vector3 currentPos = GetEntity()->GetTransform().GetLocalPosition();
        
        if(m_state == PlayerState::k_running)
        {
            m_runningAnimation.Update(timeSinceLastUpdate, KeyframeAnimation::UpdateMode::k_looping);
        }
        else if(m_state == PlayerState::k_jumping)
        {
            m_jumpingAnimation.Update(timeSinceLastUpdate, KeyframeAnimation::UpdateMode::k_looping);
            currentPos.y = m_jumpTween.Update(timeSinceLastUpdate);
            transformDirty = true;
            if(m_jumpTween.IsFinished() == true)
            {
                Run();
            }
        }
        else if(m_state == PlayerState::k_bombing)
        {
            m_bombingAnimation.Update(timeSinceLastUpdate, KeyframeAnimation::UpdateMode::k_looping);
            currentPos.y = m_bombTween.Update(timeSinceLastUpdate);
            transformDirty = true;
            if(m_bombTween.IsFinished() == true)
            {
                Run();
            }
        }
        else if(m_state == PlayerState::k_sliding)
        {
            m_slidingTimer += timeSinceLastUpdate;
            m_slidingAnimation.Update(timeSinceLastUpdate, KeyframeAnimation::UpdateMode::k_looping);
            
            if(m_slidingTimer >= GameConfig::k_playerSlideDuration)
            {
                Run();
            }
        }
        else if(m_state == PlayerState::k_colliding)
        {
            m_collidingAnimation.Update(timeSinceLastUpdate, KeyframeAnimation::UpdateMode::k_once);
            currentPos = m_bumpTween.Update(timeSinceLastUpdate);
            transformDirty = true;
            if(m_bumpTween.IsFinished() == true)
            {
                Run();
            }
        }
        
        if(transformDirty == true)
        {
            GetEntity()->GetTransform().SetPosition(currentPos);
        }
    }
}

