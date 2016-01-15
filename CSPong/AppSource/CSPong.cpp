//
//  CSPong.cpp
//  CCSPong
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

#include <CSPong.h>

#include <Splash/SplashState.h>
#include <Game/Particles/ParticleEffectComponentFactory.h>

#include <ChilliSource/Input/Accelerometer.h>
#include <ChilliSource/Rendering/Model.h>

/*
 ==============================
 Chilli Source Tour: Application
 ==============================
 
 The main hub and entry point of a CS app is the Application class. The application class
 is a singleton (accessible via CSCore::Application::Get()) which receives lifecycle events from the OS.
 Each app must sub-class Application which allows them to override certain functionality.
 
 Application has the following lifecycle events:
 - CreateSystems: Called once on application launch (explained later)
 - OnInit: Called once on application launch and after the creation of all app systems.
 - PushInitialState: Called once after OnInit (explained later)
 - OnDestroy: Called once on application termination prior to the destruction of all app systems.
 
 ------------------------------
 
 Next: 'App Systems' in App::CreateSystems()
 */

//---------------------------------------------------------
/// Implements the body of the CreateApplication method
/// which creates the derviced CSPong application
///
/// @author S Downie
///
/// @return Instance of CSCore::Application
//---------------------------------------------------------
CSCore::Application* CreateApplication()
{
    return new CSPong::App();
}

#if defined(CS_TARGETPLATFORM_ANDROID) && defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)

std::string GetGooglePlayLvlPublicKey()
{
    //Enter your Google Play LVL public key here if you are building for Google Play on Android
    return "";
}

#endif

namespace CSPong
{
    //---------------------------------------------------------
    //---------------------------------------------------------
    void App::CreateSystems()
    {
        /*
         ==============================
         Chilli Source Tour: App Systems
         ==============================
         
         As well as receiving the lifecycle events, Application also owns all the app systems. A system in CS can be pretty much anything
         but usually relates to code that operates on multiple objects or different types of objects (i.e. physics system, particle system, etc.) or
         that has functionality used by various parts of the app (i.e. pointer input, accelerometer, video player, etc.). There are 2 system types
         in CS; app system and state systems. App systems are owned by the application and are tied to its lifecycle. Once created, app systems can be accessed via
         the following:
         
            CSCore::Application::Get()->GetSystem<MySystemClass>();
         
         Common systems have named accessors:
         
            CSCore::Application::Get()->GetFileSystem();
         
         
         In order to be as modular as possible, application will delegate some of the system creation to this function. 
         Many systems are created by default (i.e. renderer, input, etc) but other systems that are game specific should be created here. 
         This means that, for example; 2d games are not lumbered with model loading systems. Users are encouraged to create and add there own systems based on the needs of their game.
         
         App systems recieve the following lifecycle events:
         
         - OnInit: Called once during the OnInit phase of the application.
         - OnResume: Called once after OnInit and called subsequently after returning from a suspended state (i.e. the app has been resumed after being minimised).
         - OnForeground: Called once after OnResume and called subsequently after returning from a background state (i.e. the app has been foregrounded after a dialog pop-up).
         - OnUpdate: Called once per game loop with the time since last update in seconds.
         - OnFixedUpdate: Can be called multiple times per game loop in order to maintain a fixed time between updates. This is useful for physics calculations.
         - OnBackground: Called when the app is no longer at the front of the view stack (i.e. during a pop-up dialogue or prior to suspending).
         - OnSuspend: Called when the app is no longer active (i.e. it has been minimised or prior to being terminated).
         - OnDestroy: Called when the app is terminated.
         - OnMemoryWarning: Called whenever the OS wishes us to free some memory. Use this delegate to clear caches etc.
         
         Notice that most the events are mirrored. It is important that if resources are allocated in one they are destroyed in the mirrored equivalent. Also
         it is worth noting that systems recieves active events in the order they were created and inactive events in reverse order; i.e.
         
         OnInit -> InitA, InitB, InitC
         OnDestroy -> DestroyC, DestroyB, DestroyA
         
         ------------------------------
         
         Next: 'State Management' in App::PushInitialState()
         */
        
        CreateSystem<CSRendering::CSModelProvider>();
        CreateSystem<CSRendering::CSAnimProvider>();
        CreateSystem<CSInput::Accelerometer>();
		CreateSystem<ParticleEffectComponentFactory>();
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    void App::OnInit()
    {
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    void App::PushInitialState()
    {
        /*
         ==============================
         Chilli Source Tour: State Management
         ==============================
         
         States are very similar to application but with shorter lifecycles. States are used to encapsulate all the functionality
         associated with a particular section of the app and have their own lifecycle events and systems. Although the app can
         have many states only one of them is considered active at any time. The active state is always the one at the top of the state stack.
         The state stack is managed using the StateManager app system via Push and Pop commands.
         
         The state manager has ownership of the states which means it will never be destroyed until it is popped from the stack. It is good practice
         to pop a state before moving to the next one as this will free up any resources used by the state. The convenience method Change() will do a Pop()
         and then a Push(). For situations such as an in-game pause menu you may not want to have to recreate the game state after un-pausing and
         therefore it is better to push onto the top of the game state.
         
         This method here will push the first state and effectively begin the app.
         
         ------------------------------
         
         Next: 'State Lifecycles' in SplashState.cpp
         */
        
        GetStateManager()->Push(CSCore::StateSPtr(new SplashState()));
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    void App::OnDestroy()
    {
    }
}

