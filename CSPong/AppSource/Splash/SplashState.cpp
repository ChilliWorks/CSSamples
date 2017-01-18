//
//  SplashState.cpp
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

#include <Splash/SplashState.h>

#include <Common/TransitionSystem.h>
#include <MainMenu/MainMenuState.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/Rendering/Camera.h>

/*
 ==============================
 Chilli Source Tour: State Lifecycles
 ==============================
 
 As mentioned in the last section states have very similar lifecycles to application but unlike application which is tied to the OS, the lifecycles
 of states are tied to the state manager app system. This state manages the code for displaying and animating the splash screen and the transition to
 the next state of the app.
 
 - CreateSystems: Called once on pushing the state onto the state manager (explained later)
 - OnInit: Called once after CreateSystems.
 - OnResume: Called once after OnInit and called subsequently after returning from a suspended state (i.e. the app has been resumed after being minimised). This is also called when the state moves back to the top of the stack.
 - OnForeground: Called once after OnResume and called subsequently after returning from a background state (i.e. the app has been foregrounded after a dialog pop-up).
 - OnUpdate: Called once per game loop with the time since last update in seconds.
 - OnFixedUpdate: Can be called multiple times per game loop in order to maintain a fixed time between updates. This is useful for physics calculations.
 - OnBackground: Called when the app is no longer at the front of the view stack (i.e. during a pop-up dialogue or prior to suspending) or when the state is no longer top of the state stack.
 - OnSuspend: Called when the state is no longer active
 - OnDestroy: Called when the state is removed from the state stack.
 - OnMemoryWarning: Called whenever the OS wishes us to free some memory. Use this delegate to clear caches etc.
 
 As with all lifecycle events it is important to obey the mirroring rules.
 
 ------------------------------
 
 Next: 'State Systems' in SplashState::CreateSystems
 */

namespace CSPong
{
    namespace
    {
        f32 k_timer = 1.0f;
    }
    
    //------------------------------------------------------
    //------------------------------------------------------
    void SplashState::CreateSystems()
    {
        /*
         ==============================
         Chilli Source Tour: State Systems
         ==============================
         
         State systems are really equivalent to app systems but their lifecycles are tied to the state rather than the application. State systems
         share all the same lifecycle events as state. States have default system that are created on behalf of the user (such as Scene which is explained later)
         but also allows the app to create its own systems. State systems are accessible via State::GetSystem<MySystemClass> and again named accessors exist for common systems i.e. State::GetScene().
         
         State systems should be used when the objects that the system is working on are tied to the lifetime of the state. A good example of this is a selection system that holds pointers
         to all currently selected objects. Managing this system across multiple states would become complicated:
         
         StateStack: StateA
         SelectionSystem: ItemA, ItemB, ItemC
         
         StateStack: StateB, StateA -> New state pushed on
         SelectionSystem: ItemA, ItemB, ItemC -> These would have to be manually removed as they do no exist in StateB
         
         StateStack: StateA -> StateB popped off
         SelectionSystem: -> Once StateB had been popped we would need to manually repopulate the system
         
         It is much simpler to implement this as a state system.
         
         ------------------------------
         
         Next: 'GUI' in MainMenuState::OnInit
         */
        
        m_transitionSystem = CreateSystem<TransitionSystem>(0.0f, 1.0f);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void SplashState::OnInit()
    {
        //Load the splash screen UI from a csui and add it to this states UI canvas. More info on GUI can be found in MainMenuState.cpp
        //Chilli source has several "Storage Locations". These correspond to the sandbox locations
        //of most mobile platforms. k_package is the most commonly used and refers to the bundle on iOS and the
        //APK on Android.
        auto splashViewDef = CS::Application::Get()->GetResourcePool()->LoadResource<CS::WidgetTemplate>(CS::StorageLocation::k_package, "GUI/Splash.csui");
        m_splashView = CS::Application::Get()->GetWidgetFactory()->Create(splashViewDef);
        
        //Each state has a ui canvas. The canvas is rendered after the update loop.
        //Adding a widget to the canvas will cause it to be rendered.
        GetUICanvas()->AddWidget(m_splashView);
        
        //Set the clear colour of the screen
        GetMainScene()->SetClearColour(CS::Colour(0.424f, 0.365f, 0.357f, 1.0f));
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void SplashState::OnUpdate(f32 in_timeSinceLastUpdate)
    {
        if (m_transitionSystem->IsTransitioning() == false)
        {
            m_timer += in_timeSinceLastUpdate;
            if (m_timer > k_timer)
            {
                m_transitionSystem->Transition(CS::StateSPtr(new MainMenuState()));
            }
        }
    }
}
