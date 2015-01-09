//
//  TransitionSystem.cpp
//  CSPong
//  Created by Ian Copland on 03/07/2014.
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

#include <Common/TransitionSystem.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Drawable.h>
#include <ChilliSource/Rendering/Texture.h>

namespace CSPong
{
    CS_DEFINE_NAMEDTYPE(TransitionSystem);
    //-------------------------------------------------
    //-------------------------------------------------
    TransitionSystemUPtr TransitionSystem::Create(f32 in_fadeInTime, f32 in_fadeOutTime)
    {
        return TransitionSystemUPtr(new TransitionSystem(in_fadeInTime, in_fadeOutTime));
    }
    //-------------------------------------------------
    //-------------------------------------------------
    TransitionSystem::TransitionSystem(f32 in_fadeInTime, f32 in_fadeOutTime)
    : m_transitionState(TransitionState::k_in), m_fadeInTime(in_fadeInTime), m_fadeOutTime(in_fadeOutTime)
    {
    }
    //-------------------------------------------------
    //-------------------------------------------------
    bool TransitionSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
    {
        return (TransitionSystem::InterfaceID == in_interfaceId);
    }
    //-------------------------------------------------
    //-------------------------------------------------
    void TransitionSystem::Transition(const CSCore::StateSPtr& in_newState)
    {
        if (m_transitionState == TransitionState::k_none)
        {
            m_transitionState = TransitionState::k_out;
            
            m_fadeImageView->BringToFront();
            
            m_fadeTween = CSCore::MakeSmoothStepTween<f32>(0.0f, 1.0f, m_fadeOutTime);
            m_fadeTween.SetOnEndDelegate([=](CSCore::SmoothStepTween<f32>* in_tween)
            {
                CSCore::Application::Get()->GetStateManager()->Change(m_targetState);
            });
            m_fadeTween.Play(CSCore::TweenPlayMode::k_once);
            
            m_targetState = in_newState;
            m_transitionOutStartedEvent.NotifyConnections();
        }
    }
    //-------------------------------------------------
    //-------------------------------------------------
    bool TransitionSystem::IsTransitioning() const
    {
        return (m_transitionState != TransitionState::k_none);
    }
    //-------------------------------------------------
    //-------------------------------------------------
    CSCore::IConnectableEvent<TransitionSystem::TransitionDelegate>& TransitionSystem::GetTransitionInFinishedEvent()
    {
        return m_transitionInFinishedEvent;
    }
    //-------------------------------------------------
    //-------------------------------------------------
    CSCore::IConnectableEvent<TransitionSystem::TransitionDelegate>& TransitionSystem::GetTransitionOutStartedEvent()
    {
        return m_transitionOutStartedEvent;
    }
    //-------------------------------------------------
    //-------------------------------------------------
    void TransitionSystem::OnInit()
    {
        auto resPool = CSCore::Application::Get()->GetResourcePool();
        auto texture = resPool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_chilliSource, "Textures/Blank.csimage");
        
        m_fadeImageView = CSCore::Application::Get()->GetWidgetFactory()->CreateImage();
        CSUI::DrawableDefCSPtr drawableDef(new CSUI::StandardDrawableDef(texture, CSRendering::UVs(), CSCore::Colour::k_white));
        m_fadeImageView->GetComponent<CSUI::DrawableComponent>()->ApplyDrawableDef(drawableDef);
        
        GetState()->GetUICanvas()->AddWidget(m_fadeImageView);
        
        m_fadeTween = CSCore::MakeSmoothStepTween<f32>(1.0f, 0.0f, m_fadeInTime);
        m_fadeTween.SetOnEndDelegate([=](CSCore::SmoothStepTween<f32>* in_tween)
        {
            m_transitionState = TransitionState::k_none;
            m_transitionInFinishedEvent.NotifyConnections();
        });
        m_fadeTween.Play(CSCore::TweenPlayMode::k_once);
    }
    //-------------------------------------------------
    //-------------------------------------------------
    void TransitionSystem::OnUpdate(f32 in_deltaTime)
    {
        if (m_transitionState == TransitionState::k_in || m_transitionState == TransitionState::k_out)
        {
            f32 value = m_fadeTween.Update(in_deltaTime);
            m_fadeImageView->SetColour(CSCore::Colour(0.0f, 0.0f, 0.0f, value));
            m_fadeImageView->BringToFront();
        }
        else
        {
            m_fadeImageView->SetColour(CSCore::Colour(0.0f, 0.0f, 0.0f, 0.0f));
        }
    }
    //-------------------------------------------------
    //-------------------------------------------------
    void TransitionSystem::OnDestroy()
    {
        m_fadeImageView->RemoveFromParent();
    }
}
