//
//  TransitionSystem.h
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

#ifndef _APPSOURCE_COMMON_TRANSITIONSYSTEM_H_
#define _APPSOURCE_COMMON_TRANSITIONSYSTEM_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Event.h>
#include <ChilliSource/Core/System.h>
#include <ChilliSource/Core/Tween.h>

#include <functional>

namespace CSPong
{
    //---------------------------------------------------------
    /// A system for handling transitions into and out of states.
    /// This will fade the screen from black on transitioning in
    /// and fade to black on transition out. Events are provided
    /// for the end of the transition in and the start of the
    /// transition out.
    ///
    /// @author Ian Copland
    //---------------------------------------------------------
    class TransitionSystem final : public CSCore::StateSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(TransitionSystem);
        //-------------------------------------------------
        /// A delegate called when the transition in
        /// completes and the transition out start.
        ///
        /// @author Ian Copland
        //-------------------------------------------------
        using TransitionDelegate = std::function<void()>;
        //-------------------------------------------------
        /// Creates a new instance of the system.
        ///
        /// @author Ian Copland
        ///
        /// @param The time taken to fade in.
        /// @param The time taken to fade out.
        ///
        /// @return The new instance.
        //-------------------------------------------------
        static TransitionSystemUPtr Create(f32 in_fadeInTime, f32 in_fadeOutTime);
        //-------------------------------------------------
        /// Allows querying of whether or not this system
        /// implements the interface with the given Id.
        ///
        /// @author Ian Copland
        ///
        /// @param The interface Id.
        ///
        /// @return Whether the interface is implemented.
        //-------------------------------------------------
        bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
        //-------------------------------------------------
        /// Starts transitioning into the given state. This
        /// will fade the screen to black before changing
        /// to the given state. If a transition is already
        /// in progress--either out or in--this will have
        /// no effect.
        ///
        /// @author Ian Copland
        ///
        /// @return The state to transition to.
        //-------------------------------------------------
        void Transition(const CSCore::StateSPtr& in_newState);
        //-------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return Whether or not a transition is currently
        /// in progress.
        //-------------------------------------------------
        bool IsTransitioning() const;
        //-------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return An event that can be listened to in
        /// order to be notified when a transition in has
        /// completed.
        //-------------------------------------------------
        CSCore::IConnectableEvent<TransitionDelegate>& GetTransitionInFinishedEvent();
        //-------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return An event that can be listened to in
        /// order to be notified when a transition out has
        /// started.
        //-------------------------------------------------
        CSCore::IConnectableEvent<TransitionDelegate>& GetTransitionOutStartedEvent();
    private:
        //-------------------------------------------------
        /// An enum describing the possible transition
        /// states.
        ///
        /// @author Ian Copland
        //-------------------------------------------------
        enum class TransitionState
        {
            k_none,
            k_in,
            k_out
        };
        //-------------------------------------------------
        /// Constructor
        ///
        /// @author Ian Copland
        ///
        /// @param The time taken to fade in.
        /// @param The time taken to fade out.
        //-------------------------------------------------
        TransitionSystem(f32 in_fadeInTime, f32 in_fadeOutTime);
        //-------------------------------------------------
        /// Called when the state is initialised. This will
        /// start the fade in.
        ///
        /// @author Ian Copland
        //-------------------------------------------------
        void OnInit() override;
        //-------------------------------------------------
        /// Called every frame. This updates the current
        /// transition state.
        ///
        /// @author Ian Copland
        ///
        /// @param The time since the last frame.
        //-------------------------------------------------
        void OnUpdate(f32 in_deltaTime) override;
        //-------------------------------------------------
        /// Called when the state is destroyed. Removes the
        /// GUI from the scene.
        ///
        /// @author Ian Copland
        //-------------------------------------------------
        void OnDestroy() override;
        
        f32 m_fadeInTime;
        f32 m_fadeOutTime;
        TransitionState m_transitionState;
        CSCore::StateSPtr m_targetState;
        CSGUI::ImageViewSPtr m_fadeImageView;
        CSCore::SmoothStepTween<f32> m_fadeTween;
        CSCore::Event<TransitionDelegate> m_transitionInFinishedEvent;
        CSCore::Event<TransitionDelegate> m_transitionOutStartedEvent;
    };
}

#endif
