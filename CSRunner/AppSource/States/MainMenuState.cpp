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

#include <States/MainMenuState.h>

#include <Transition/TransitionSystem.h>
#include <States/GameState.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Button.h>
#include <ChilliSource/Input/Gamepad.h>

namespace CSRunner
{
    //------------------------------------------------------------
    void MainMenuState::CreateSystems() noexcept
    {
        m_transitionSystem = CreateSystem<TransitionSystem>(1.0f, 1.0f);
    }
    
    //------------------------------------------------------------
    void MainMenuState::OnInit() noexcept
    {
        GetMainScene()->SetClearColour(CS::Colour(0.137f, 0.203f, 0.098f));
        
        //Load the GUI and add it to the window
        auto mainMenuViewDef = CS::Application::Get()->GetResourcePool()->LoadResource<CS::WidgetTemplate>(CS::StorageLocation::k_package, "GUI/MainMenu.csui");
        CS::WidgetSPtr mainMenuView = CS::Application::Get()->GetWidgetFactory()->Create(mainMenuViewDef);
        GetUICanvas()->AddWidget(mainMenuView);
        
        //Grab the button children by name and add this class as a listener for button events
        m_playButton = mainMenuView->GetWidget("PlayButton");
        
        //Disable user interaction until the animation is complete.
        m_playButton->SetInputEnabled(false);
        
        //Create a tween that will slide the UI onto the screen. This will be used to offset the button from its stated position.
        m_playButtonTween = CS::MakeEaseInOutBackTween(-1.0f, 0.0f, 1.0f);
        
        m_playButtonConnection = m_playButton->GetReleasedInsideEvent().OpenConnection([this](CS::Widget* in_widget, const CS::Pointer& in_pointer, CS::Pointer::InputType in_inputType)
        {
            StartTransitionOut();
        });
        
        m_transitionInConnection = m_transitionSystem->GetTransitionInFinishedEvent().OpenConnection([this]()
        {
            m_playButtonTween.Play(CS::TweenPlayMode::k_once);

            //Once the animation is finished allow the user to press the button
            m_playButtonTween.SetOnEndDelegate([this](CS::EaseInOutBackTween<f32>* in_tween)
            {
                m_playButton->SetInputEnabled(true);
                
                auto gamepadSystem = CS::Application::Get()->GetSystem<CS::GamepadSystem>();
                if(gamepadSystem != nullptr)
                {
                    m_gamepadButtonEventConnection = gamepadSystem->GetButtonPressureChangedEvent().OpenConnection([this](const CS::Gamepad& gamepad, f64 timestamp, u32 buttonIndex, f32 pressure)
                    {
                        if(pressure >= 0.5f)
                        {
                            StartTransitionOut();
                        }
                    });
                }
            });
        });
    }
    
    //-----------------------------------------------------------
    void MainMenuState::StartTransitionOut()
    {
        m_gamepadButtonEventConnection.reset();
        m_playButton->SetInputEnabled(false);
        m_playButtonTween.Play(CS::TweenPlayMode::k_onceReverse);
        m_playButtonTween.SetOnEndDelegate([this](CS::EaseInOutBackTween<f32>* in_tween)
        {
            m_transitionSystem->Transition(std::make_shared<GameState>());
        });
    }
    
    //------------------------------------------------------------
    void MainMenuState::OnUpdate(f32 timeSinceLastUpdate) noexcept
    {
        m_playButtonTween.Update(timeSinceLastUpdate);
        m_playButton->SetRelativePosition(CS::Vector2(m_playButtonTween.GetValue(), m_playButton->GetLocalRelativePosition().y));
    }
}

