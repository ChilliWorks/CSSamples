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

#include <States/SplashState.h>

#include <Transition/TransitionSystem.h>
#include <States/MainMenuState.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/Rendering/Camera.h>


namespace CSRunner
{
    namespace
    {
        constexpr f32 k_timer = 1.0f;
    }
    
    //------------------------------------------------------------
    void SplashState::CreateSystems() noexcept
    {
        m_transitionSystem = CreateSystem<TransitionSystem>(0.0f, 1.0f);
    }
    
    //------------------------------------------------------------
    void SplashState::OnInit() noexcept
    {
        auto splashViewDef = CS::Application::Get()->GetResourcePool()->LoadResource<CS::WidgetTemplate>(CS::StorageLocation::k_package, "GUI/Splash.csui");
        m_splashView = CS::Application::Get()->GetWidgetFactory()->Create(splashViewDef);
        GetUICanvas()->AddWidget(m_splashView);
        
        GetMainScene()->SetClearColour(CS::Colour(0.424f, 0.365f, 0.357f));
    }
    
    //------------------------------------------------------------
    void SplashState::OnUpdate(f32 timeSinceLastUpdate) noexcept
    {
        if (m_transitionSystem->IsTransitioning() == false)
        {
            m_timer += timeSinceLastUpdate;
            if (m_timer > k_timer)
            {
                m_transitionSystem->Transition(std::make_shared<MainMenuState>());
            }
        }
    }
}
