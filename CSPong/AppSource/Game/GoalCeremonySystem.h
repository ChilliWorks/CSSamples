//
//  GoalCeremonySystem.h
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

#ifndef _APPSOURCE_GAME_GOALCEREMONYSYSTEM_H_
#define _APPSOURCE_GAME_GOALCEREMONYSYSTEM_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/System.h>
#include <ChilliSource/Core/Tween.h>

#include <functional>

namespace CSPong
{
    //------------------------------------------------------------
    /// Manages the lifecycle for the logic, entities and UI
    /// that make up the goal ceremony
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class GoalCeremonySystem final : public CSCore::StateSystem
    {
    public: 

        CS_DECLARE_NAMEDTYPE(GoalCeremonySystem);
        
        //---------------------------------------------------
        /// Delegate that is called on ceremony end
        //---------------------------------------------------
        using EndDelegate = std::function<void()>;
        //---------------------------------------------------
        /// Creates a new instance of the system.
        ///
        /// @author S Downie
        ///
        /// @return The new instance.
        //---------------------------------------------------
        static GoalCeremonySystemUPtr Create();
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
        //---------------------------------------------------
        /// Play the goal ceremony
        ///
        /// @author S Downie
        ///
        /// @param Delegate called when ceremony ends
        //---------------------------------------------------
        void PlayGoal(const EndDelegate& in_endDelegate);
        //---------------------------------------------------
        /// Play the win ceremony
        ///
        /// @author S Downie
        ///
        /// @param Delegate called when ceremony ends
        //---------------------------------------------------
        void PlayWin(const EndDelegate& in_endDelegate);
        //---------------------------------------------------
        /// Play the lose ceremony
        ///
        /// @author S Downie
        ///
        /// @param Delegate called when ceremony ends
        //---------------------------------------------------
        void PlayLose(const EndDelegate& in_endDelegate);
        
    private:
        //----------------------------------------------------------
        /// Private constructor to enforce use of factory method
        ///
        /// @author S Downie
        //----------------------------------------------------------
        GoalCeremonySystem() = default;
        //------------------------------------------------------------
        /// Called when the state is first placed onto the state manager
        /// stack. This is only called once and is mirrored by OnDestroy().
        ///
        /// @author S Downie
        //------------------------------------------------------------
        void OnInit() override;
        //------------------------------------------------------------
        /// This is called whenever the state is active (i.e. top of the
        /// state stack). A state is active between OnForeground and OnBackground
        /// lifecycle events.
        ///
        /// @author S Downie
        ///
        /// @param Time since last update in seconds
        //------------------------------------------------------------
        void OnUpdate(f32 in_timeSinceLastUpdate) override;
        //---------------------------------------------------
        /// Play the current set ceremony
        ///
        /// @author S Downie
        ///
        /// @param Delegate called when ceremony ends
        //---------------------------------------------------
        void Play(const EndDelegate& in_endDelegate);
        
    private:
        
        CSCore::EaseOutBackTween<CSCore::Vector2> m_scaleTween;
        CSCore::EaseOutBackTween<f32> m_rotationTween;
        CSGUI::GUIViewSPtr m_goalView;
        CSGUI::GUIViewSPtr m_winView;
        CSGUI::GUIViewSPtr m_loseView;
        CSGUI::GUIViewSPtr m_currentView;
    };
}

#endif
