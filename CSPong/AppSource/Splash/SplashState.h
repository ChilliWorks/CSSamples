//
//  SplashState.h
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

#ifndef _APPSOURCE_SPLASH_SPLASHSTATE_H_
#define _APPSOURCE_SPLASH_SPLASHSTATE_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Tween.h>

namespace CSPong
{
    //------------------------------------------------------------
    /// A state class receieves lifecycle events from the application
    /// when it is the active state (i.e. top of the stack). This state
    /// controls the splash intro animation and then transitions to
    /// the main menu state.
    ///
    /// See ChilliSource/Core/State/State.h for all the lifecycle events.
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class SplashState final : public CSCore::State
    {
    public: 
        //------------------------------------------------------
        /// Called when its time to create our state systems
        ///
        /// @author Ian Copland
        //------------------------------------------------------
        void CreateSystems() override;
        //------------------------------------------------------------
        /// Called when the state is first placed onto the state manager
        /// stack. This is only called once and is mirrored by OnDestroy().
        ///
        /// Use this for one time initialisation
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
        
    private:
        
        TransitionSystem* m_transitionSystem;
        f32 m_timer = 0.0f;
        CSUI::WidgetSPtr m_splashView;
    };
}

#endif
