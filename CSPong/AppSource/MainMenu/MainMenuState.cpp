//
//  MainMenuState.cpp
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

#include <MainMenu/MainMenuState.h>

#include <Common/TransitionSystem.h>
#include <Game/GameState.h>
#include <Game/Particles/ParticleEffectComponentFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Button.h>

namespace CSPong
{
    //------------------------------------------------------
    //------------------------------------------------------
    void MainMenuState::CreateSystems()
    {
        m_transitionSystem = CreateSystem<TransitionSystem>(1.0f, 1.0f);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void MainMenuState::OnInit()
    {
		//Load parent widget container (only contains background image, main menu, and particle menu)
		auto menuContainerDef = CSCore::Application::Get()->GetResourcePool()->LoadResource<CSUI::WidgetTemplate>(CSCore::StorageLocation::k_package, "GUI/MenuContainer.csui");
		m_menuContainer = CSCore::Application::Get()->GetWidgetFactory()->Create(menuContainerDef);
		GetUICanvas()->AddWidget(m_menuContainer);

		//Get main and particle menus
		m_mainMenu = m_menuContainer->GetWidget("MainMenuTemplate")->GetWidget("MainMenuContainer");
		m_particleMenu = m_menuContainer->GetWidget("ParticleMenuTemplate")->GetWidget("ParticleMenuContainer");

		//Disable all user interaction until the animation is complete.
		SetAllInputEnabled(false);

		SetupMainMenu();
		SetupParticleMenu();

		//Transition system event
		m_transitionInConnection = m_transitionSystem->GetTransitionInFinishedEvent().OpenConnection([this]() { MainMenuState::TransitionSystemFinished(); });
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void MainMenuState::OnUpdate(f32 in_dt)
    {
		if (m_mainMenuTween.IsPlaying())
		{
			m_mainMenuTween.Update(in_dt);
			m_mainMenu->SetRelativePosition(CSCore::Vector2(m_mainMenuTween.GetValue(), m_mainMenu->GetLocalRelativePosition().y));
		}

		if (m_particleMenuTween.IsPlaying())
		{
			m_particleMenuTween.Update(in_dt);
			m_particleMenu->SetRelativePosition(CSCore::Vector2(m_particleMenuTween.GetValue(), m_particleMenu->GetLocalRelativePosition().y));
		}
    }
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::SetupMainMenu()
	{
		//Get play and right arrow buttons to set their events
		auto playButton = m_mainMenu->GetWidget("PlayButton");
		auto rightArrowButton = m_mainMenu->GetWidget("RightArrowButton");

		//Create main menu tween that will move its components from the left (offscreen) to the center
		m_mainMenuTween = CSCore::MakeEaseInOutBackTween(-1.0f, 0.0f, 1.0f);
		
		//Play button events
		m_playButtonReleasedConnection = playButton->GetReleasedInsideEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType) 
		{ 
			MainMenuState::PlayButtonPointerReleased(in_widget, in_pointer, in_inputType);
		});
		m_playButtonEnteredConnection = playButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer) 
		{ 
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_playButtonExitedConnection = playButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});

		//Right arrow button events
		m_rightArrowReleasedConnection = rightArrowButton->GetReleasedInsideEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
		{
			MainMenuState::ArrowButtonPointerReleased(in_widget, in_pointer, in_inputType);
		});
		m_rightArrowEnteredConnection = rightArrowButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_rightArrowExitedConnection = rightArrowButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::SetupParticleMenu()
	{
		//Get left arrow button to set its events
		auto leftArrowButton = m_particleMenu->GetWidget("LeftArrowButton");

		//Create particle menu tween that will move its components from the right (offscreen) to the center
		m_particleMenuTween = CSCore::MakeEaseInOutBackTween(1.0f, 0.0f, 1.0f);

		//Left arrow button events
		m_leftArrowReleasedConnection = leftArrowButton->GetReleasedInsideEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
		{
			MainMenuState::ArrowButtonPointerReleased(in_widget, in_pointer, in_inputType);
		});
		m_leftArrowEnteredConnection = leftArrowButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_leftArrowExitedConnection = leftArrowButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});

		SetupParticleMenuGrid();
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::SetupParticleMenuGrid()
	{
		auto particleMenuGrid = m_particleMenu->GetWidgetRecursive("ParticleMenuGridTemplate")->GetWidget("ParticleMenuGrid");
		auto playerParticleEffectDiv = particleMenuGrid->GetWidgetRecursive("PlayerParticleEffectDivTemplate")->GetWidget("PlayerParticleEffectDiv");
		auto opponentParticleEffectDiv = particleMenuGrid->GetWidgetRecursive("OpponentParticleEffectDivTemplate")->GetWidget("OpponentParticleEffectDiv");
		auto ballParticleEffectDiv = particleMenuGrid->GetWidgetRecursive("BallParticleEffectDivTemplate")->GetWidget("BallParticleEffectDiv");

		//Player magma toggle button events
		auto playerMagmaToggleButton = playerParticleEffectDiv->GetWidgetRecursive("PlayerMagmaToggleButton");
		m_playerMagmaToggleButtonEnteredConnection = playerMagmaToggleButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_playerMagmaToggleButtonExitedConnection = playerMagmaToggleButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});
		
		//Player ice cream toggle button events
		auto playerIceCreamToggleButton = playerParticleEffectDiv->GetWidgetRecursive("PlayerIceCreamToggleButton");
		m_playerIceCreamToggleButtonEnteredConnection = playerIceCreamToggleButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_playerIceCreamToggleButtonExitedConnection = playerIceCreamToggleButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});

		//Opponent magma toggle button events
		auto opponentMagmaToggleButton = opponentParticleEffectDiv->GetWidgetRecursive("OpponentMagmaToggleButton");
		m_opponentMagmaToggleButtonEnteredConnection = opponentMagmaToggleButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_opponentMagmaToggleButtonExitedConnection = opponentMagmaToggleButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});

		//Opponent ice cream toggle button events
		auto opponentIceCreamToggleButton = opponentParticleEffectDiv->GetWidgetRecursive("OpponentIceCreamToggleButton");
		m_opponentIceCreamToggleButtonEnteredConnection = opponentIceCreamToggleButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_opponentIceCreamToggleButtonExitedConnection = opponentIceCreamToggleButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});

		//Ball smoke toggle button events
		auto ballSmokeToggleButton = ballParticleEffectDiv->GetWidgetRecursive("BallSmokeToggleButton");
		m_ballSmokeToggleButtonEnteredConnection = ballSmokeToggleButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_ballSmokeToggleButtonExitedConnection = ballSmokeToggleButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});
		
		// Ball beam toggle button events
		auto ballBeamToggleButton = ballParticleEffectDiv->GetWidgetRecursive("BallBeamToggleButton");
		m_ballBeamToggleButtonEnteredConnection = ballBeamToggleButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_ballBeamToggleButtonExitedConnection = ballBeamToggleButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::TransitionSystemFinished()
	{
		//Set main menu tween to move to the right
		m_mainMenuTween.Play(CSCore::TweenPlayMode::k_once);

		//Once the animation is finished, allow the user to press the buttons
		m_mainMenuTween.SetOnEndDelegate([this](CSCore::EaseInOutBackTween<f32>* in_tween)
		{
			MainMenuState::SetAllInputEnabled(true);
		});
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::PlayButtonPointerReleased(CSUI::Widget* in_playButton, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
	{
		//Disable all user interaction until the animation is complete.
		SetAllInputEnabled(false);

		//Set main menu tween to move to the left
		m_mainMenuTween.Play(CSCore::TweenPlayMode::k_onceReverse);

		//Once the animation is finished, transition to the game
		m_mainMenuTween.SetOnEndDelegate([this](CSCore::EaseInOutBackTween<f32>* in_tween)
		{
			m_transitionSystem->Transition(CSCore::StateSPtr(new GameState()));
		});

		//Update the particle effect component factory system to use the chosen particles
		UpdateParticleOptions();
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::ArrowButtonPointerReleased(CSUI::Widget* in_arrowButton, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
	{
		//Disable all user interaction until the animation is complete.
		SetAllInputEnabled(false);

		if (m_goingRight)
		{
			//Set main menu and particle tweens to move left and right, respectively
			m_mainMenuTween.Play(CSCore::TweenPlayMode::k_onceReverse);
			m_particleMenuTween.Play(CSCore::TweenPlayMode::k_once);
		}
		else
		{
			//Set main menu and particle tweens to move right and left, respectively
			m_mainMenuTween.Play(CSCore::TweenPlayMode::k_once);
			m_particleMenuTween.Play(CSCore::TweenPlayMode::k_onceReverse);
		}

		//Reset the direction
		m_goingRight = !m_goingRight;

		//Once the animation is finished, allow the user to press the buttons
		m_mainMenuTween.SetOnEndDelegate([this](CSCore::EaseInOutBackTween<f32>* in_tween)
		{
			MainMenuState::SetAllInputEnabled(true);
		});
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::UpdateParticleOptions()
	{
		auto particleECFSystem = CSCore::Application::Get()->GetSystem<ParticleEffectComponentFactory>();

		auto particleMenuGrid = m_particleMenu->GetWidgetRecursive("ParticleMenuGridTemplate")->GetWidget("ParticleMenuGrid");
		auto playerParticleEffectDiv = particleMenuGrid->GetWidgetRecursive("PlayerParticleEffectDivTemplate")->GetWidget("PlayerParticleEffectDiv");
		auto opponentParticleEffectDiv = particleMenuGrid->GetWidgetRecursive("OpponentParticleEffectDivTemplate")->GetWidget("OpponentParticleEffectDiv");
		auto ballParticleEffectDiv = particleMenuGrid->GetWidgetRecursive("BallParticleEffectDivTemplate")->GetWidget("BallParticleEffectDiv");

		//Setting user's options for player particles
		bool playerMagmaToggled = playerParticleEffectDiv->GetWidgetRecursive("PlayerMagmaToggleButton")->GetProperty<bool>("ToggledOn");
		bool playerIceCreamToggled = playerParticleEffectDiv->GetWidgetRecursive("PlayerIceCreamToggleButton")->GetProperty<bool>("ToggledOn");
		particleECFSystem->SetPlayerParticles(playerMagmaToggled, playerIceCreamToggled);

		//Setting user's options for opponent particles
		bool opponentMagmaToggled = opponentParticleEffectDiv->GetWidgetRecursive("OpponentMagmaToggleButton")->GetProperty<bool>("ToggledOn");
		bool opponentIceCreamToggled = opponentParticleEffectDiv->GetWidgetRecursive("OpponentIceCreamToggleButton")->GetProperty<bool>("ToggledOn");
		particleECFSystem->SetOpponentParticles(opponentMagmaToggled, opponentIceCreamToggled);

		//Setting user's options for ball particles
		bool ballSmokeToggled = ballParticleEffectDiv->GetWidgetRecursive("BallSmokeToggleButton")->GetProperty<bool>("ToggledOn");
		bool ballBeamToggled = ballParticleEffectDiv->GetWidgetRecursive("BallBeamToggleButton")->GetProperty<bool>("ToggledOn");
		particleECFSystem->SetBallParticles(ballSmokeToggled, ballBeamToggled);
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::DarkenWidget(CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
	{
		in_widget->SetColour(in_widget->GetFinalColour() * k_menuFadeColour);
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::ResetDarkenWidget(CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
	{
		in_widget->SetColour(in_widget->GetFinalColour() / k_menuFadeColour);
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::SetAllInputEnabled(const bool in_inputEnabled)
	{
		m_mainMenu->GetWidget("PlayButton")->SetInputEnabled(in_inputEnabled);
		m_mainMenu->GetWidget("RightArrowButton")->SetInputEnabled(in_inputEnabled);
		m_particleMenu->GetWidget("LeftArrowButton")->SetInputEnabled(in_inputEnabled);
	}
}

