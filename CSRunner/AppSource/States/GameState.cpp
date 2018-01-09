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

#include <States/GameState.h>

#include <ChilliSource/Audio/CricketAudio.h>
#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Delegate.h>
#include <ChilliSource/Core/Event.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Sprite.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>
#include <ChilliSource/Input/Keyboard.h>
#include <ChilliSource/Input/Gamepad.h>

#include <Collision/CollisionSystem.h>
#include <Player/PlayerController.h>
#include <Environment/LevelGeneratorSystem.h>
#include <Transition/TransitionSystem.h>
#include <States/GameoverState.h>

#include <GameConfig.h>

namespace CSRunner
{
    namespace
    {
        constexpr u32 k_moveGamepadAction = 0;
        
        /// The viewport of the camera has a fixed width so that you always see the same distance in front
        /// but the height will adapt to maintain display aspect ratio
        ///
        CS::EntityUPtr CreateCamera() noexcept
        {
            auto screen = CS::Application::Get()->GetScreen();
            auto cameraEntity = CS::Entity::Create();
            CS::Vector2 viewportSize = CS::AspectRatioUtils::KeepOriginalWidthAdaptHeight(CS::Vector2(GameConfig::k_levelWidth, 0.0f), screen->GetResolution().x/screen->GetResolution().y);
            auto cameraComponent = std::make_shared<CS::OrthographicCameraComponent>(viewportSize, 1.0f, 20.0f, CS::CameraComponent::ViewportResizePolicy::k_none);
            cameraEntity->AddComponent(cameraComponent);
            cameraEntity->GetTransform().SetLookAt(CS::Vector3(0.0f, 0.0f, -5.0f), CS::Vector3::k_zero, CS::Vector3::k_unitPositiveY);
            return cameraEntity;
        }
        
        /// @param audioPlayer
        ///     Used by the player controller to play player audio
        ///
        CS::EntityUPtr CreatePlayer(CS::CkAudioPlayer* audioPlayer) noexcept
        {
            auto playerEntity = CS::Entity::Create();
            
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_package, "TextureAtlases/Player/Player.csimage");
            auto atlas = resourcePool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "TextureAtlases/Player/Player.csatlas");
  
            auto materialFactory = CS::Application::Get()->GetSystem<CS::MaterialFactory>();
            
            auto material = resourcePool->GetResource<CS::Material>("Player");
            if(material == nullptr)
            {
                material = materialFactory->CreateUnlit("Player", texture, true);
            }
            
            auto playerSprite = std::make_shared<CS::SpriteComponent>(material, atlas, "Run_01", CS::Vector2(GameConfig::k_playerWidth, GameConfig::k_playerHeight), CS::SizePolicy::k_none);
            playerEntity->AddComponent(playerSprite);

#ifndef CS_TARGETPLATFORM_RPI
            auto audioBank = resourcePool->LoadResource<CS::CkBank>(CS::StorageLocation::k_package, "SFX/SFX.ckb");
            auto playerController = std::make_shared<PlayerController>(playerSprite.get(), audioPlayer, audioBank);
#else
            auto playerController = std::make_shared<PlayerController>(playerSprite.get());
#endif
            playerEntity->AddComponent(playerController);
            
            playerEntity->GetTransform().SetPosition(GameConfig::k_startingX, GameConfig::k_groundY, 0.0f);
            return playerEntity;
        }
    }
    
    //------------------------------------------------------------
    void GameState::CreateSystems() noexcept
    {
        m_transitionSystem = CreateSystem<TransitionSystem>(0.0f, 1.0f);
        m_levelGenerator = CreateSystem<LevelGeneratorSystem>();
        m_collisionSystem = CreateSystem<CollisionSystem>(m_levelGenerator);
        m_gestureSystem = CreateSystem<CS::GestureSystem>();
        
#ifndef CS_TARGETPLATFORM_RPI
        m_audioPlayer = CreateSystem<CS::CkAudioPlayer>();
#endif
    }
    
    //------------------------------------------------------------
    void GameState::OnInit() noexcept
    {
        //Create an orthographic camera. The camera doesn't move instead the level moves. This means we don't get
        //floating point precision issues as the runner moves through the level
        GetMainScene()->Add(CreateCamera());

        //Create the player "runner"
        auto player = CreatePlayer(m_audioPlayer);
        m_playerController = player->GetComponent<PlayerController>().get();
        m_collisionSystem->SetPlayer(m_playerController);
        GetMainScene()->Add(std::move(player));
        
#ifndef CS_TARGETPLATFORM_RPI
        m_audioPlayer->SetMusicVolume(0.5f);
        m_audioPlayer->PlayMusic(CS::StorageLocation::k_package, "Music/GameMusic.cks");
#endif

        //Load the GUI and add it to the window
        auto gameViewDef = CS::Application::Get()->GetResourcePool()->LoadResource<CS::WidgetTemplate>(CS::StorageLocation::k_package, "GUI/Game.csui");
        CS::WidgetSPtr gameView = CS::Application::Get()->GetWidgetFactory()->Create(gameViewDef);
        GetUICanvas()->AddWidget(gameView);
        m_timeSurvivedLabel = gameView->GetWidgetRecursive("Time")->GetComponent<CS::TextUIComponent>();
        
        
        //Subscribe for collision events
        m_obstacleCollisionEventConnection = m_collisionSystem->GetPlayerObstacleCollisionEvent().OpenConnection(CS::MakeDelegate(this, &GameState::OnPlayerObstacleCollision));
        
        m_minDragDistance = CS::Application::Get()->GetScreen()->GetResolution().y * 0.025f;
        
        
        //Subscribe for events that we use to make the player jump and slide on the various platforms
        auto gesture = std::make_shared<CS::DragGesture>();
        m_dragMovedEventConnection = gesture->GetDragMovedEvent().OpenConnection(CS::MakeDelegate(this, &GameState::OnDragged));
        m_gestureSystem->AddGesture(gesture);
        
        auto gamepadSystem = CS::Application::Get()->GetSystem<CS::GamepadSystem>();
        if(gamepadSystem != nullptr)
        {
            CS::Gamepad gamepad;
            if(gamepadSystem->TryGetGamepadWithIndex(0, gamepad))
            {
                if(gamepad.IsAxisSupported(CS::GamepadAxis::k_y) == false)
                {
                    CS_LOG_WARNING("Gamepad doesn't have the required y axis input");
                }
            }
            
            //Map the jump/slide command for each supported controller.
            gamepadSystem->SetDefaultActionMapping(k_moveGamepadAction, CS::GamepadAxis::k_y);
            gamepadSystem->SetActionMapping(k_moveGamepadAction, CS::GamepadMappings::PS4::k_name, CS::GamepadMappings::PS4::k_dpadY, CS::GamepadMappings::PS4::k_dpadYShouldInvert);
            gamepadSystem->SetActionMapping(k_moveGamepadAction, CS::GamepadMappings::PS4::k_name, CS::GamepadMappings::PS4::k_lStickY, CS::GamepadMappings::PS4::k_lStickYShouldInvert);
    
            m_gamepadAxisEventConnection = gamepadSystem->GetMappedAxisPositionChangedEvent().OpenConnection(CS::MakeDelegate(this, &GameState::OnGamepadAxisMoved));
        }
        
        m_keyboard = CS::Application::Get()->GetSystem<CS::Keyboard>();
    }

    //------------------------------------------------------------
    void GameState::OnUpdate(f32 timeSinceLastUpdate) noexcept
    {
        if(m_transitionSystem->IsTransitioning() == false)
        {
            m_timeSurvived += timeSinceLastUpdate;
            m_timeSurvivedLabel->SetText("Survived for " + CS::ToString((u32)m_timeSurvived) + " seconds");
            
            if(m_keyboard != nullptr)
            {
                if(m_keyboard->IsKeyDown(CS::KeyCode::k_up) || m_keyboard->IsKeyDown(CS::KeyCode::k_w))
                {
                    m_playerController->Jump();
                }
                else if(m_keyboard->IsKeyDown(CS::KeyCode::k_down) || m_keyboard->IsKeyDown(CS::KeyCode::k_s))
                {
                    m_playerController->Slide();
                }
            }
        }
    }
    
    //------------------------------------------------------------
    void GameState::OnPlayerObstacleCollision(u32 index) noexcept
    {
        if(m_transitionSystem->IsTransitioning() == false)
        {
            m_playerController->Bump();
            m_levelGenerator->DestroyObstacle(index);
            
            //Check to see if the player has been bumped offscreen
            if(m_playerController->GetEntity()->GetTransform().GetLocalPosition().x <= GameConfig::k_cullX)
            {
                //Close our connection so we don't get any more events
                m_dragMovedEventConnection.reset();
                m_obstacleCollisionEventConnection.reset();
                m_gamepadAxisEventConnection.reset();
                
                m_transitionSystem->Transition(std::make_shared<GameoverState>(m_timeSurvived));
            }
        }
    }

    //------------------------------------------------------------
    void GameState::OnDragged(const CS::DragGesture* gesture, const CS::DragGesture::DragInfo& info) noexcept
    {
        if(info.m_delta.y > m_minDragDistance)
        {
            m_playerController->Jump();
        }
        else if(info.m_delta.y < -m_minDragDistance)
        {
            m_playerController->Slide();
        }
    }
    
    //------------------------------------------------------------
    void GameState::OnGamepadAxisMoved(const CS::Gamepad& gamepad, f64 timestamp, u32 actionId, f32 position) noexcept
    {
        if(gamepad.GetIndex() == 0 && actionId == k_moveGamepadAction)
        {
            if(position > 0.1f)
            {
                m_playerController->Slide();
            }
            else if(position < -0.1f)
            {
                m_playerController->Jump();
            }
        }
    }
    
    //------------------------------------------------------------
    void GameState::OnDestroy() noexcept
    {
        //Close our connection so we don't get any more events
        m_dragMovedEventConnection.reset();
        m_obstacleCollisionEventConnection.reset();
        m_gamepadAxisEventConnection.reset();
    }
}
