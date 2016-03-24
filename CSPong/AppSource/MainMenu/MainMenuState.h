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
		//------------------------------------------------------------
		/// This struct is to help describe the movement of components
		///
		/// The possible movements include:
		/// L --> C (movingCenter = true, leftTween = true)
		/// L <-- C (movingCenter = false, leftTween = true) 
		/// C --> R (movingCenter = false, leftTween = false)
		/// C <-- R (movingCenter = true, leftTween = false)
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		struct MovementData
		{
			bool m_moving = false;
			bool m_movingCenter = false;
			bool m_leftTween = false;
		};

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
        /// @author  Angela Gross
        //------------------------------------------------------------
        void OnInit() override;
        //------------------------------------------------------------
        /// This is called whenever the state is active (i.e. top of the
        /// state stack). A state is active between OnForeground and 
		/// OnBackground lifecycle events.
        ///
        /// @author Angela Gross
        ///
        /// @param Time since last update in seconds
        //------------------------------------------------------------
        void OnUpdate(f32 in_dt) override;
		//------------------------------------------------------------
		/// Method that handles the movement of widget components that
		/// have MovementData associated with them.
		///
		/// @author Angela Gross
		///
		/// @param Time since last update in seconds
		/// @param Widget to animate
		/// @param Widget's movement data that helps determine direction
		/// and nature of widget's movement
		/// @param Whether or not it's a benchmarking widget, or a 
		/// widget that is activated by the "move" button
		//------------------------------------------------------------
		void AnimateWidget(f32 in_dt, CSUI::WidgetSPtr in_widget, MovementData* in_widgetMovementData, bool in_isBenchmarkingWidget = false);
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
		/// Adds the events and easing information needed for the 
		/// benchmarking menu's widgets
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		void SetupBenchmarkingMenu();
		//------------------------------------------------------------
		/// Adds the events needed for the particle buttons in the 
		/// grid.
		///
		/// @author Angela Gross
		//------------------------------------------------------------
		void SetupParticleMenuGrid();
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

        CSCore::EventConnectionUPtr m_playButtonReleasedConnection, m_playButtonEnteredConnection, m_playButtonExitedConnection;
		CSCore::EventConnectionUPtr m_mainRArrowReleasedConnection, m_mainRArrowEnteredConnection, m_mainRArrowExitedConnection;

		CSCore::EventConnectionUPtr m_particleLArrowReleasedConnection, m_particleLArrowEnteredConnection, m_particleLArrowExitedConnection;
		CSCore::EventConnectionUPtr m_particleRArrowReleasedConnection, m_particleRArrowEnteredConnection, m_particleRArrowExitedConnection;

		CSCore::EventConnectionUPtr m_benchLArrowReleasedConnection, m_benchLArrowEnteredConnection, m_benchLArrowExitedConnection;
		CSCore::EventConnectionUPtr m_benchMoveButtonReleasedConnection, m_benchMoveButtonEnteredConnection, m_benchMoveButtonExitedConnection;

		CSCore::EventConnectionUPtr m_playerMagmaToggleButtonEnteredConnection, m_playerMagmaToggleButtonExitedConnection;
		CSCore::EventConnectionUPtr m_playerIceCreamToggleButtonEnteredConnection, m_playerIceCreamToggleButtonExitedConnection;

		CSCore::EventConnectionUPtr m_opponentMagmaToggleButtonEnteredConnection, m_opponentMagmaToggleButtonExitedConnection;
		CSCore::EventConnectionUPtr m_opponentIceCreamToggleButtonEnteredConnection, m_opponentIceCreamToggleButtonExitedConnection;

		CSCore::EventConnectionUPtr m_ballSmokeToggleButtonEnteredConnection, m_ballSmokeToggleButtonExitedConnection;
		CSCore::EventConnectionUPtr m_ballBeamToggleButtonEnteredConnection, m_ballBeamToggleButtonExitedConnection;

		CSCore::EaseInOutBackTween<f32> m_rightToCenterTween, m_leftToCenterTween;
		CSCore::EaseInOutBackTween<f32> m_upAndDownTween, m_rightAndLeftTween;
		MovementData m_mainMenuMovementData, m_particleMenuMovementData, m_benchmarkingMenuMovementData;
		MovementData m_playerParticleDivMovementData, m_opponentParticleDivMovementData, m_ballParticleMovementData, m_optionParticleMovementData;
		bool m_isPlayButtonPressed = false;

		CSUI::WidgetSPtr m_menuContainer, m_mainMenu, m_particleMenu, m_benchmarkingMenu;
		CSUI::WidgetSPtr m_benchPlayerParticleDiv, m_benchOpponentParticleDiv, m_benchBallParticleDiv, m_benchOptionParticleDiv;
    };
}

#endif
