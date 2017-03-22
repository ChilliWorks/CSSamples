//
//  GameState.h
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

#ifndef _APPSOURCE_GAME_GAMESTATE_H_
#define _APPSOURCE_GAME_GAMESTATE_H_

#include <ForwardDeclarations.h>
#include <Game/ScoringSystem.h>

#include <ChilliSource/Core/State.h>

namespace CSPong
{
    //------------------------------------------------------------
    /// Manages the lifecycle for the logic, entities and UI
    /// that constitutes the pong game
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class GameState final : public CS::State
    {
    public: 

        //------------------------------------------------------
        /// Called when its time to create our state systems
        ///
        /// @author S Downie
        //------------------------------------------------------
        void CreateSystems() override;
        
    private:
        //------------------------------------------------------------
        /// Called when the state is first placed onto the state manager
        /// stack. This is only called once and is mirrored by OnDestroy().
        ///
        /// Use this for one time initialisation such as creating
        /// the camera and entities
        ///
        /// @author S Downie
        //------------------------------------------------------------
        void OnInit() override;
        //------------------------------------------------------------
        /// Called when a goal is scored and plays the ceremony
        ///
        /// @author S Downie
        //------------------------------------------------------------
        void OnGoalScored(const ScoringSystem::Scores& in_scores);
        //------------------------------------------------------------
        /// Called when the state is removed from the state manager
        /// stack.
        ///
        /// @author S Downie
        //------------------------------------------------------------
        void OnDestroy() override;
        
    private:
        
        CS::EventConnectionUPtr m_scoreChangedConnection;
        CS::EventConnectionUPtr m_transitionInConnection;
        
        CS::EntitySPtr m_ball;
        CS::EntitySPtr m_oppositionPaddle;
        
#ifndef CS_TARGETPLATFORM_RPI
        CS::CkBankCSPtr m_audioBank;
#endif
        
        TransitionSystem* m_transitionSystem;
        PhysicsSystem* m_physicsSystem;
        ScoringSystem* m_scoringSystem;
        GoalCeremonySystem* m_goalCeremonySystem;
        GameEntityFactory* m_gameEntityFactory;
        CS::CkAudioPlayer* m_audioPlayer;
    };
}

#endif
