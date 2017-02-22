//
//  GameState.cpp
//  CSPong
//  Created by Scott Downie on 30/06/2014.
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

#include <Game/GameState.h>

#include <Common/TransitionSystem.h>
#include <Game/GameEntityFactory.h>
#include <Game/GoalCeremonySystem.h>
#include <Game/Ball/BallControllerComponent.h>
#include <Game/Paddle/AIControllerComponent.h>
#include <Game/Physics/PhysicsSystem.h>
#include <MainMenu/MainMenuState.h>

#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Delegate.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Lighting.h>
#include <ChilliSource/Audio/CricketAudio.h>

namespace CSPong
{
    namespace
    {
        const u32 k_targetScore = 5;
    }
    //------------------------------------------------------
    //------------------------------------------------------
    void GameState::CreateSystems()
    {
        m_transitionSystem = CreateSystem<TransitionSystem>(1.0f, 1.0f);
        m_physicsSystem = CreateSystem<PhysicsSystem>();
        m_scoringSystem = CreateSystem<ScoringSystem>();
        m_goalCeremonySystem = CreateSystem<GoalCeremonySystem>();
        m_gameEntityFactory = CreateSystem<GameEntityFactory>(m_physicsSystem, m_scoringSystem);
        m_audioPlayer = CreateSystem<CS::CkAudioPlayer>();
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GameState::OnInit()
    {
        /*
         ==============================
         Chilli Source Tour: Entities
         ==============================
         
         Entites (also known as game objects) are used to represent all the objects in the world. Entities have no functionality other than a transform which is used
         to determine their world space position, scale and orientation. 
         
         The behaviour of an entity is goverened by the components it has attached to it. An example of this is the opposition paddle
         entity which has a StaticModelComponent which describes how it is rendered and an AIControllerComponent which moves the entity towards the ball. This is really powerful as it allows
         behaviour to be customised from building blocks without having to change exisiting code. For instance the player paddle is controlled by pointer input via the TouchControllerComponent 
         however this could easily be detached and replaced by an AccelerometerControllerComponent; while the opposition paddle is controlled by an AIComponent but both player and opposition have the same render component.
         Another example could be a player entity in an RPG game that collects abilities as they progress, each ability's logic could be controlled by a component.
         
         Entites can be added to other entites to form transform hierarchies (i.e. moving the parent entity will move its children).
         
         Entities that have render components (meshes, cameras, lights, etc) can only have these components rendered by adding them to the scene. Similarly, components on each entity are only updated if the
         entity is added to the scene.
         
         ------------------------------
         
         Next: 'Components' in GameEntityFactory::CreateCamera
         */
        
        GetMainScene()->SetClearColour(CS::Colour::k_black);
        
        CS::EntitySPtr camera = m_gameEntityFactory->CreateCamera();
        GetMainScene()->Add(camera);

        CS::EntitySPtr ambientLight = m_gameEntityFactory->CreateAmbientLight();
        GetMainScene()->Add(ambientLight);
        
        CS::EntitySPtr diffuseLight = m_gameEntityFactory->CreateDiffuseLight();
        GetMainScene()->Add(diffuseLight);
        
        CS::EntitySPtr arena = m_gameEntityFactory->CreateArena();
        GetMainScene()->Add(arena);
        
        m_ball = m_gameEntityFactory->CreateBall();
        GetMainScene()->Add(m_ball);
        
        CS::EntitySPtr playerPaddle = m_gameEntityFactory->CreatePlayerPaddle(camera);
        GetMainScene()->Add(playerPaddle);
        
        m_oppositionPaddle = m_gameEntityFactory->CreateOppositionPaddle(m_ball);
        GetMainScene()->Add(m_oppositionPaddle);
        
        /*
         ===================================
         Chilli Source Tour: Audio
         ===================================
         
         Chilli Source has an optional Cricket audio module http://www.crickettechnology.com/. The modules are created in CSPong.cpp.
         
         For short SFX samples, Cricket works by loading audio banks (which are a collection of sounds batched together). The bank is created as a simple xml file with a .ckbk extension (see PrebuiltResources/SFX for an example).
         The audio bank is then built into a binary format using the CkTool.jar (this is incorporated into the "BuildAll" content process).
         
         At runtime the banks are loaded like any other resource and sound effects are then created from the bank. As you would expect the SFX has a play function that allows you to optionally loop the sound and 
         other functions for changing the volume, pitch, etc. For simple fire and forget sound effects you can use the CkAudioPlayer for convenience (as we are doing here).
         
         If you need to play larger audio samples (such as music), it makes no sense to load the entire sample into memory, instead you can use the CreateFromStream method to stream the file directly (or again use the CkAudioPlayer::PlayMusic).
         
         In this demo we have SFX for when a goal is scored and background music
         
         -----------------------------------
         
         This concludes the Chilli Source Tour!
         */
        auto resourcePool = CS::Application::Get()->GetResourcePool();
        m_audioBank = resourcePool->LoadResource<CS::CkBank>(CS::StorageLocation::k_package, "SFX/SFX.ckb");
        m_audioPlayer->SetMusicVolume(0.5f);
        m_audioPlayer->PlayMusic(CS::StorageLocation::k_package, "Music/GameMusic.cks");
        
        
        
        
        m_scoreChangedConnection = m_scoringSystem->GetScoreChangedEvent().OpenConnection(CS::MakeDelegate(this, &GameState::OnGoalScored));
        
        m_transitionInConnection = m_transitionSystem->GetTransitionInFinishedEvent().OpenConnection([=]()
        {
            m_ball->GetComponent<BallControllerComponent>()->Activate();
        });
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GameState::OnGoalScored(const ScoringSystem::Scores& in_scores)
    {
        m_audioPlayer->PlayEffect(m_audioBank, "GoalScored");
        
        BallControllerComponentSPtr ballController = m_ball->GetComponent<BallControllerComponent>();
        ballController->Deactivate();
        
        AIControllerComponentSPtr aiController = m_oppositionPaddle->GetComponent<AIControllerComponent>();
        aiController->Reset();
        
        if(in_scores[0] >= k_targetScore)
        {
            m_goalCeremonySystem->PlayWin([=]()
            {
                m_transitionSystem->Transition(CS::StateSPtr(new MainMenuState()));
            });
        }
        else if(in_scores[1] >= k_targetScore)
        {
            m_goalCeremonySystem->PlayLose([=]()
            {
                m_transitionSystem->Transition(CS::StateSPtr(new MainMenuState()));
            });
        }
        else
        {
            m_goalCeremonySystem->PlayGoal([ballController]()
            {
                ballController->Activate();
            });
        }
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GameState::OnDestroy()
    {
        m_scoreChangedConnection.reset();
    }
}
