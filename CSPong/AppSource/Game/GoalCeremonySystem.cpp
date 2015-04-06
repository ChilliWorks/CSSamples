//
//  GoalCeremonySystem.cpp
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

#include <Game/GoalCeremonySystem.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Drawable.h>
#include <ChilliSource/Rendering/Texture.h>

namespace CSPong
{
    CS_DEFINE_NAMEDTYPE(GoalCeremonySystem);
    
    namespace
    {
        const f32 k_ceremonyDuration = 1.0f;
        const f32 k_ceremonyStartDelay = 0.2f;
        const f32 k_ceremonyEndDelay = 0.7f;
        
        //------------------------------------------------------
        /// Creates the image view that is displayed when a
        /// goal is scored
        ///
        /// @author S Downie
        ///
        /// @return View
        //------------------------------------------------------
        CSUI::WidgetSPtr CreateGoalCelebrationView()
        {
            auto resPool = CSCore::Application::Get()->GetResourcePool();
            auto texture = resPool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/GUI/GUI.csimage");
            auto atlas = resPool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/GUI/GUI.csatlas");
            
            CSUI::WidgetSPtr widget = CSCore::Application::Get()->GetWidgetFactory()->CreateImage();
            CSUI::DrawableDefCSPtr drawableDef(new CSUI::StandardDrawableDef(texture, atlas, "Goal"));
            widget->GetComponent<CSUI::DrawableComponent>()->ApplyDrawableDef(drawableDef);
            widget->ScaleTo(CSCore::Vector2(0.0f, 0.0f));
            widget->SetRelativeSize(CSCore::Vector2(0.5f, 0.0f));
            widget->SetSizePolicy(CSUI::SizePolicy::k_useWidthMaintainingAspect);
            
            return widget;
        }
        //------------------------------------------------------
        /// Creates the image view that is displayed when the
        /// game is won
        ///
        /// @author S Downie
        ///
        /// @return View
        //------------------------------------------------------
        CSUI::WidgetSPtr CreateWinCelebrationView()
        {
            auto resPool = CSCore::Application::Get()->GetResourcePool();
            auto texture = resPool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/GUI/GUI.csimage");
            auto atlas = resPool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/GUI/GUI.csatlas");
            
            CSUI::WidgetSPtr widget = CSCore::Application::Get()->GetWidgetFactory()->CreateImage();
            CSUI::DrawableDefCSPtr drawableDef(new CSUI::StandardDrawableDef(texture, atlas, "Win"));
            widget->GetComponent<CSUI::DrawableComponent>()->ApplyDrawableDef(drawableDef);
            widget->ScaleTo(CSCore::Vector2(0.0f, 0.0f));
            widget->SetRelativeSize(CSCore::Vector2(0.5f, 0.0f));
            widget->SetSizePolicy(CSUI::SizePolicy::k_useWidthMaintainingAspect);
            
            return widget;
        }
        //------------------------------------------------------
        /// Creates the image view that is displayed when the
        /// game is lost
        ///
        /// @author S Downie
        ///
        /// @return View
        //------------------------------------------------------
        CSUI::WidgetSPtr CreateLoseCelebrationView()
        {
            auto resPool = CSCore::Application::Get()->GetResourcePool();
            auto texture = resPool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/GUI/GUI.csimage");
            auto atlas = resPool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/GUI/GUI.csatlas");
            
            CSUI::WidgetSPtr widget = CSCore::Application::Get()->GetWidgetFactory()->CreateImage();
            CSUI::DrawableDefCSPtr drawableDef(new CSUI::StandardDrawableDef(texture, atlas, "Lose"));
            widget->GetComponent<CSUI::DrawableComponent>()->ApplyDrawableDef(drawableDef);
            widget->ScaleTo(CSCore::Vector2(0.0f, 0.0f));
            widget->SetRelativeSize(CSCore::Vector2(0.5f, 0.0f));
            widget->SetSizePolicy(CSUI::SizePolicy::k_useWidthMaintainingAspect);
            
            return widget;
        }
    }
    //---------------------------------------------------
    //---------------------------------------------------
    GoalCeremonySystemUPtr GoalCeremonySystem::Create()
    {
        return GoalCeremonySystemUPtr(new GoalCeremonySystem());
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool GoalCeremonySystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == GoalCeremonySystem::InterfaceID;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GoalCeremonySystem::OnInit()
    {
        m_goalView = CreateGoalCelebrationView();
        m_winView = CreateWinCelebrationView();
        m_loseView = CreateLoseCelebrationView();
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GoalCeremonySystem::PlayGoal(const EndDelegate& in_endDelegate)
    {
        m_currentView = m_goalView;
        Play(in_endDelegate);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GoalCeremonySystem::PlayWin(const EndDelegate& in_endDelegate)
    {
        m_currentView = m_winView;
        Play(in_endDelegate);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GoalCeremonySystem::PlayLose(const EndDelegate& in_endDelegate)
    {
        m_currentView = m_loseView;
        Play(in_endDelegate);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GoalCeremonySystem::Play(const EndDelegate& in_endDelegate)
    {
        GetState()->GetUICanvas()->AddWidget(m_currentView);
        
        m_rotationTween = CSCore::MakeEaseOutBackTween(0.0f, CSCore::MathUtils::k_pi * 2.0f, k_ceremonyDuration, k_ceremonyStartDelay, k_ceremonyEndDelay);
        m_scaleTween = CSCore::MakeEaseOutBackTween(CSCore::Vector2::k_zero, CSCore::Vector2::k_one, k_ceremonyDuration, k_ceremonyStartDelay, k_ceremonyEndDelay);
        m_scaleTween.SetOnEndDelegate([this, in_endDelegate](CSCore::EaseOutBackTween<CSCore::Vector2>*)
        {
            m_currentView->RemoveFromParent();
            m_currentView->ScaleTo(CSCore::Vector2(0.0f, 0.0f));
            m_currentView.reset();
            in_endDelegate();
        });
        m_rotationTween.Play(CSCore::TweenPlayMode::k_pingPong);
        m_scaleTween.Play(CSCore::TweenPlayMode::k_pingPong);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GoalCeremonySystem::OnUpdate(f32 in_timeSinceLastUpdate)
    {
        m_rotationTween.Update(in_timeSinceLastUpdate);
        m_scaleTween.Update(in_timeSinceLastUpdate);
        if(m_currentView != nullptr)
        {
            m_currentView->ScaleTo(CSCore::Vector2::Max(m_scaleTween.GetValue(), CSCore::Vector2::k_zero));
            m_currentView->RotateTo(m_rotationTween.GetValue());
        }
    }
}