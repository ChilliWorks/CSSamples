//
//  ScoringSystem.h
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

#ifndef _APPSOURCE_GAME_SCORINGSYSTEM_H_
#define _APPSOURCE_GAME_SCORINGSYSTEM_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Event.h>
#include <ChilliSource/Core/System.h>

#include <array>
#include <vector>

namespace CSPong
{
    //------------------------------------------------------------
    /// State system that holds the scores of the 2 players
    /// and has events for being notified of score updates
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class ScoringSystem final : public CSCore::StateSystem
    {
    public:
        
        static const u32 k_numPlayers = 2;
        
        CS_DECLARE_NAMEDTYPE(ScoringSystem);
        
        using Scores = std::array<u32, k_numPlayers>;
        //----------------------------------------------------
        /// A delegate that is used to receive events whenever
        /// a score changes
        ///
        /// @author S Downie
        ///
        /// @param Array of the current scores
        //----------------------------------------------------
        using ScoreChangedDelegate = std::function<void(const Scores&)>;
        //---------------------------------------------------
        /// Creates a new instance of the system.
        ///
        /// @author S Downie
        ///
        /// @return The new instance.
        //---------------------------------------------------
        static ScoringSystemUPtr Create();
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @return The score changed event.
        //----------------------------------------------------
        CSCore::IConnectableEvent<ScoreChangedDelegate>& GetScoreChangedEvent();
        //------------------------------------------------
        /// Add a ball body which will be checked against
        /// the goal triggers to detect if a goal has been
        /// scored
        ///
        /// @author S Downie
        ///
        /// @param Dynamic body
        //------------------------------------------------
        void AddBallBody(const DynamicBodyComponentSPtr& in_body);
        //------------------------------------------------
        /// Add a goal trigger which will be checked against
        /// the ball bodies to detect if a goal has been
        /// scored
        ///
        /// @author S Downie
        ///
        /// @param Trigger
        /// @param Player index that will benfit if goal hit
        //------------------------------------------------
        void AddGoalTrigger(const CSCore::EntitySPtr& in_trigger, u32 in_playerIndex);

    private:
        //----------------------------------------------------------
        /// Private constructor to enforce use of factory method
        ///
        /// @author S Downie
        //----------------------------------------------------------
        ScoringSystem() = default;
        //------------------------------------------------
        /// Create the views that display the score
        ///
        /// @author S Downie
        //------------------------------------------------
        void OnInit() override;
        //------------------------------------------------
        /// Called when any of the balls hit a trigger.
        /// This will check against registered goal triggers
        /// to see if the score should be increased.
        ///
        /// @author S Downie
        //------------------------------------------------
        void OnBallHitTrigger(CSCore::Entity* in_trigger);
        //------------------------------------------------
        /// Increment the score for the player at
        /// the given index by 1.
        ///
        /// @author S Downie
        ///
        /// @param Player Index
        //------------------------------------------------
        void IncrementScore(u32 in_playerIndex);
        
    private:
        
        CSCore::Event<ScoreChangedDelegate> m_scoreChangedEvent;
        
        Scores m_scores;
        
        std::array<std::pair<CSRendering::SpriteComponentSPtr, CSRendering::SpriteComponentSPtr>, k_numPlayers> m_scoreViews;
        std::vector<CSCore::EventConnectionUPtr> m_ballTriggerConnections;
        std::array<CSCore::Entity*, k_numPlayers> m_goaltriggers;
    };
}

#endif
