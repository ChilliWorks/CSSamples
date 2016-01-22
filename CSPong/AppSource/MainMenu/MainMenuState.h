//
//  MainMenuState.h
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

#ifndef _APPSOURCE_MAINMENU_MAINMENUSTATE_H_
#define _APPSOURCE_MAINMENU_MAINMENUSTATE_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Tween.h>
#include <ChilliSource/UI/Base.h>

namespace CSPong
{
    //------------------------------------------------------------
    /// The main menu state displays the UI for accessing the game
    /// and the options menu.
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class MainMenuState final : public CSCore::State
    {
    public:
		//------------------------------------------------------------
        /// Called when its time to create our state systems.
        ///
        /// @author Ian Copland
		//------------------------------------------------------------
        void CreateSystems();
        //------------------------------------------------------------
        /// Called when the state is first placed onto the state manager
        /// stack. This is only called once and is mirrored by OnDestroy().
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
        void OnUpdate(f32 in_dt) override;
		//------------------------------------------------------------
		/// Adds the events and easing information needed for the main 
		/// menu's widgets.
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		void SetupMainMenu();
		//------------------------------------------------------------
		/// Adds the events and easing information needed for the 
		/// particle menu's widgets
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		void SetupParticleMenu();
		//------------------------------------------------------------
		/// Adds the events needed for the particle buttons in the 
		/// grid.
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		void SetupParticleMenuGrid();
		//------------------------------------------------------------
		/// Handler for when the MainMenuState is transitioned into.
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		void TransitionSystemFinished();
		//------------------------------------------------------------
		/// Handler for when the play button is pressed.
		///
		/// @author Angela Gross
		///
		/// @param Play button widget
		/// @param Captured pointer
		/// @param Pointer type
		//------------------------------------------------------------
		void PlayButtonPointerReleased(CSUI::Widget* in_playButton, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType);
		//------------------------------------------------------------
		/// Handler for when either of the arrows are pressed.
		///
		/// @author Angela Gross
		///
		/// @param Arrow button widget
		/// @param Captured pointer
		/// @param Pointer type
		//------------------------------------------------------------
		void ArrowButtonPointerReleased(CSUI::Widget* in_arrowButton, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType);
		//------------------------------------------------------------
		/// Handler to set all of the particle options for the ball,
		/// player, and opponent paddles.
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		void UpdateParticleOptions();
		//------------------------------------------------------------
		/// Darkens the widget.
		///
		/// @author Angela Gross
		///
		/// @param Desired widget to darken
		/// @param Pointer type
		//------------------------------------------------------------
		void DarkenWidget(CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer);
		//------------------------------------------------------------
		/// Resets the widget's color from DarkenWidget().
		///
		/// @author Angela Gross
		///
		/// @param Desired widget to darken
		/// @param Pointer type
		//------------------------------------------------------------
		void ResetDarkenWidget(CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer);
		//------------------------------------------------------------
		/// Sets the input enabled component on all widgets in the 
		/// menu.
		///
		/// @author Angela Gross
		///
		/// @param Whether or not the user should accept and respond
		/// to input.
		//------------------------------------------------------------
		void SetAllInputEnabled(const bool in_inputEnabled);
        
    private:

		CSCore::Colour const k_menuFadeColour = (1.0f, 1.0f, 1.0f, 0.75f);
        
        TransitionSystem* m_transitionSystem;

        CSCore::EventConnectionUPtr m_transitionInConnection;

        CSCore::EventConnectionUPtr m_playButtonReleasedConnection;
		CSCore::EventConnectionUPtr m_playButtonEnteredConnection;
		CSCore::EventConnectionUPtr m_playButtonExitedConnection;

		CSCore::EventConnectionUPtr m_rightArrowReleasedConnection;
		CSCore::EventConnectionUPtr m_rightArrowEnteredConnection;
		CSCore::EventConnectionUPtr m_rightArrowExitedConnection;

		CSCore::EventConnectionUPtr m_leftArrowReleasedConnection;
		CSCore::EventConnectionUPtr m_leftArrowEnteredConnection;
		CSCore::EventConnectionUPtr m_leftArrowExitedConnection;

		CSCore::EventConnectionUPtr m_playerMagmaToggleButtonEnteredConnection;
		CSCore::EventConnectionUPtr m_playerMagmaToggleButtonExitedConnection;
		CSCore::EventConnectionUPtr m_playerIceCreamToggleButtonEnteredConnection;
		CSCore::EventConnectionUPtr m_playerIceCreamToggleButtonExitedConnection;

		CSCore::EventConnectionUPtr m_opponentMagmaToggleButtonEnteredConnection;
		CSCore::EventConnectionUPtr m_opponentMagmaToggleButtonExitedConnection;
		CSCore::EventConnectionUPtr m_opponentIceCreamToggleButtonEnteredConnection;
		CSCore::EventConnectionUPtr m_opponentIceCreamToggleButtonExitedConnection;

		CSCore::EventConnectionUPtr m_ballSmokeToggleButtonEnteredConnection;
		CSCore::EventConnectionUPtr m_ballSmokeToggleButtonExitedConnection;
		CSCore::EventConnectionUPtr m_ballBeamToggleButtonEnteredConnection;
		CSCore::EventConnectionUPtr m_ballBeamToggleButtonExitedConnection;

		CSCore::EaseInOutBackTween<f32> m_mainMenuTween;
		CSCore::EaseInOutBackTween<f32> m_particleMenuTween;
		bool m_goingRight = true;

		CSUI::WidgetSPtr m_menuContainer;
        CSUI::WidgetSPtr m_mainMenu;
		CSUI::WidgetSPtr m_particleMenu;
    };
}

#endif
