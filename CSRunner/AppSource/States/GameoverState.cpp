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

#include <States/GameoverState.h>

#include <Transition/TransitionSystem.h>
#include <States/MainMenuState.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Input/Gamepad.h>
#include <ChilliSource/Input/Gesture.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

#include <json/json.h>

namespace CSRunner
{
    namespace
    {
        const std::string k_saveDataPath = "SavedData.json";
        const std::string k_saveDataBestTimeKey = "BestTime";
    }
    //------------------------------------------------------------
    GameoverState::GameoverState(f32 timeSurvived) noexcept
    : m_timeSurvived((u32)timeSurvived)
    {
        
    }
    //------------------------------------------------------------
    void GameoverState::CreateSystems() noexcept
    {
        m_transitionSystem = CreateSystem<TransitionSystem>(0.0f, 1.0f);
        m_gestureSystem = CreateSystem<CS::GestureSystem>();
    }
    
    //------------------------------------------------------------
    void GameoverState::OnInit() noexcept
    {
        GetMainScene()->SetClearColour(CS::Colour(0.137f, 0.203f, 0.098f));
        
        auto gameoverViewDef = CS::Application::Get()->GetResourcePool()->LoadResource<CS::WidgetTemplate>(CS::StorageLocation::k_package, "GUI/Gameover.csui");
        CS::WidgetSPtr gameoverView = CS::Application::Get()->GetWidgetFactory()->Create(gameoverViewDef);
        GetUICanvas()->AddWidget(gameoverView);

        //Grab the current best time
        Json::Value root;
        u32 bestTimeSurvived = 0;
        if(CS::JsonUtils::ReadJson(CS::StorageLocation::k_saveData, k_saveDataPath, root) == true)
        {
            bestTimeSurvived = root.get(k_saveDataBestTimeKey, 0u).asUInt();
        }
        
        auto timeSurvivedLabel = gameoverView->GetWidgetRecursive("LastTime")->GetComponent<CS::TextUIComponent>();
        timeSurvivedLabel->SetText("You survived for " + CS::ToString(m_timeSurvived) + " seconds");
        
        std::string bestTimePrefix;
        if(m_timeSurvived > bestTimeSurvived)
        {
            bestTimeSurvived = m_timeSurvived;
            bestTimePrefix = "NEW ";
            root[k_saveDataBestTimeKey] = bestTimeSurvived;
            CS::JsonUtils::WriteJson(CS::StorageLocation::k_saveData, k_saveDataPath, root);
        }
        
        auto bestTimeLabel = gameoverView->GetWidgetRecursive("BestTime")->GetComponent<CS::TextUIComponent>();
        bestTimeLabel->SetText(bestTimePrefix + "Best time " + CS::ToString(bestTimeSurvived) + " seconds");
        
        auto tapGesture = std::make_shared<CS::TapGesture>();
        m_tapEventConnection = tapGesture->GetTappedEvent().OpenConnection([this](const CS::TapGesture* gesture, const CS::Vector2& pos)
        {
            m_transitionSystem->Transition(std::make_shared<MainMenuState>());
            m_tapEventConnection.reset();
        });
        m_gestureSystem->AddGesture(tapGesture);
        
        auto gamepadSystem = CS::Application::Get()->GetSystem<CS::GamepadSystem>();
        if(gamepadSystem != nullptr)
        {
            m_gamepadButtonEventConnection = gamepadSystem->GetButtonPressureChangedEvent().OpenConnection([this](const CS::Gamepad& gamepad, f64 timestamp, u32 buttonIndex, f32 pressure)
            {
                if(pressure >= 0.5f)
                {
                    m_transitionSystem->Transition(std::make_shared<MainMenuState>());
                    m_gamepadButtonEventConnection.reset();
                }
            });
        }
    }
}
