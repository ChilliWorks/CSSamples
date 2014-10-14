//
//  ScoringSystem.cpp
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

#include <Game/ScoringSystem.h>


#include <Game/GameEntityFactory.h>
#include <Game/Physics/DynamicBodyComponent.h>
#include <Game/Physics/TriggerComponent.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Delegate.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Sprite.h>
#include <ChilliSource/Rendering/Texture.h>

namespace CSPong
{
    CS_DEFINE_NAMEDTYPE(ScoringSystem);
    
    //---------------------------------------------------
    //---------------------------------------------------
    ScoringSystemUPtr ScoringSystem::Create()
    {
        return ScoringSystemUPtr(new ScoringSystem());
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool ScoringSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == ScoringSystem::InterfaceID;
    }
    //------------------------------------------------
    //------------------------------------------------
    void ScoringSystem::OnInit()
    {
        for(auto& score : m_scores)
        {
            score = 0;
        }
        
        auto entityFactory = GetState()->GetSystem<GameEntityFactory>();
        auto scene = GetState()->GetScene();
        const CSCore::Vector2 k_digitSize(20, 20);
        const f32 distanceFromGround = 2.0f;
        
        const f32 k_paddingFromCentre = 10.0f;
        
        CSCore::EntitySPtr player1TensEntity = entityFactory->CreateScoreSprite(k_digitSize, CSRendering::AlignmentAnchor::k_middleRight);
        m_scoreViews[0].first = player1TensEntity->GetComponent<CSRendering::SpriteComponent>();
        player1TensEntity->GetTransform().SetPosition(-k_paddingFromCentre - k_digitSize.x, 0.0f, distanceFromGround);
        scene->Add(player1TensEntity);
        
        CSCore::EntitySPtr player1SinglesEntity = entityFactory->CreateScoreSprite(k_digitSize, CSRendering::AlignmentAnchor::k_middleRight);
        m_scoreViews[0].second = player1SinglesEntity->GetComponent<CSRendering::SpriteComponent>();
        player1SinglesEntity->GetTransform().SetPosition(-k_paddingFromCentre, 0.0f, distanceFromGround);
        scene->Add(player1SinglesEntity);

        CSCore::EntitySPtr player2TensEntity = entityFactory->CreateScoreSprite(k_digitSize, CSRendering::AlignmentAnchor::k_middleLeft);
        m_scoreViews[1].first = player2TensEntity->GetComponent<CSRendering::SpriteComponent>();
        player2TensEntity->GetTransform().SetPosition(k_paddingFromCentre, 0.0f, distanceFromGround);
        scene->Add(player2TensEntity);
        
        CSCore::EntitySPtr player2SinglesEntity = entityFactory->CreateScoreSprite(k_digitSize, CSRendering::AlignmentAnchor::k_middleLeft);
        m_scoreViews[1].second = player2SinglesEntity->GetComponent<CSRendering::SpriteComponent>();
        player2SinglesEntity->GetTransform().SetPosition(k_paddingFromCentre + k_digitSize.x, 0.0f, distanceFromGround);
        scene->Add(player2SinglesEntity);
    }
    //------------------------------------------------
    //------------------------------------------------
    void ScoringSystem::AddBallBody(const DynamicBodyComponentSPtr& in_body)
    {
        m_ballTriggerConnections.push_back(in_body->GetTriggerEvent().OpenConnection(CSCore::MakeDelegate(this, &ScoringSystem::OnBallHitTrigger)));
    }
    //------------------------------------------------
    //------------------------------------------------
    void ScoringSystem::AddGoalTrigger(const CSCore::EntitySPtr& in_trigger, u32 in_playerIndex)
    {
        CS_ASSERT(in_playerIndex < k_numPlayers, "Index out of range");
        
        m_goaltriggers[in_playerIndex] = in_trigger.get();
    }
    //------------------------------------------------
    //------------------------------------------------
    void ScoringSystem::OnBallHitTrigger(CSCore::Entity* in_trigger)
    {
        for(u32 i=0; i<m_goaltriggers.size(); ++i)
        {
            if(m_goaltriggers[i] == in_trigger)
            {
                IncrementScore(i);
                return;
            }
        }
    }
    //------------------------------------------------
    //------------------------------------------------
    void ScoringSystem::IncrementScore(u32 in_playerIndex)
    {
        CS_ASSERT(in_playerIndex < k_numPlayers, "Index out of range");
        
        m_scores[in_playerIndex]++;
        
        if(m_scores[in_playerIndex] <= 99)
        {
            u32 tens = m_scores[in_playerIndex]/10;
            m_scoreViews[in_playerIndex].first->SetTextureAtlasId(CSCore::ToString(tens));
            
            u32 singles = m_scores[in_playerIndex]%10;
            m_scoreViews[in_playerIndex].second->SetTextureAtlasId(CSCore::ToString(singles));
        }
        else
        {
            CS_LOG_WARNING("We can only currently display scores up to 99");
        }
        
        m_scoreChangedEvent.NotifyConnections(m_scores);
    }
    //----------------------------------------------------
    //----------------------------------------------------
    CSCore::IConnectableEvent<ScoringSystem::ScoreChangedDelegate>& ScoringSystem::GetScoreChangedEvent()
    {
        return m_scoreChangedEvent;
    }
}