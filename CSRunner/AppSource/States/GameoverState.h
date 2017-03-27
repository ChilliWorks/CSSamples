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

#ifndef _APPSOURCE_STATES_GAMEOVERSTATE_H_
#define _APPSOURCE_STATES_GAMEOVERSTATE_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/State.h>

namespace CSRunner
{
    /// Shows how long you surived for before taking you to main menu
    ///
    class GameoverState final : public CS::State
    {
    public:
        
        /// @param timeSurvived
        ///     How long the player survived in the game state
        ///
        GameoverState(f32 timeSurvived) noexcept;
        
    private:

        /// Creates the system that manages the transition to the main menu
        ///
        void CreateSystems() noexcept override;

        /// Creates the UI
        ///
        void OnInit() noexcept override;
        
    private:
        
        CS::EventConnectionUPtr m_tapEventConnection;
        
        TransitionSystem* m_transitionSystem;
        CS::GestureSystem* m_gestureSystem;
        
        f32 m_timeSurvived;
    };
}

#endif
