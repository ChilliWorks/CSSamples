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
		m_benchmarkingMenu = m_menuContainer->GetWidget("BenchmarkingMenuTemplate")->GetWidget("BenchmarkingMenuContainer");

		//Disable all user interaction until the animation is complete.
		SetAllInputEnabled(false);

		//Create tweens that will move its components from the right and left (offscreen) to the center
		m_rightToCenterTween = CSCore::MakeEaseInOutBackTween(1.0f, 0.0f, 1.0f);
		m_leftToCenterTween = CSCore::MakeEaseInOutBackTween(-1.0f, 0.0f, 1.0f);

		SetupMainMenu();
		SetupParticleMenu();
		SetupBenchmarkingMenu();

		//Transition system event
		m_transitionInConnection = m_transitionSystem->GetTransitionInFinishedEvent().OpenConnection([this]() 
		{ 
			m_mainMenuMovementData.m_leftTween = true;
			m_mainMenuMovementData.m_movingCenter = true;
			m_mainMenuMovementData.m_moving = true;
		});
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void MainMenuState::OnUpdate(f32 in_dt)
    {
		if (m_mainMenuMovementData.m_moving)
		{
			//Get correct directional tween based on movement data
			CSCore::EaseInOutBackTween<f32>* mainMenuTween = m_mainMenuMovementData.m_leftTween ? &m_leftToCenterTween : &m_rightToCenterTween;

			//Play tween based on its center movement data if it's not playing already
			if (!mainMenuTween->IsPlaying())
			{
				if (m_mainMenuMovementData.m_movingCenter)
				{
					mainMenuTween->Play(CSCore::TweenPlayMode::k_once);
				}
				else
				{
					mainMenuTween->Play(CSCore::TweenPlayMode::k_onceReverse);
				}
			}

			//If the play button is pressed then make sure we will transition into the next state at the end of the animation
			if (m_isPlayButtonPressed)
			{
				mainMenuTween->SetOnEndDelegate([this](CSCore::EaseInOutBackTween<f32>* in_tween)
				{
					m_transitionSystem->Transition(CSCore::StateSPtr(new GameState()));
					m_mainMenuMovementData.m_moving = false;
				});
			}
			//Make sure that the menu stops moving at the end of the animation
			else
			{
				mainMenuTween->SetOnEndDelegate([this](CSCore::EaseInOutBackTween<f32>* in_tween)
				{
					m_mainMenuMovementData.m_moving = false;
					MainMenuState::SetAllInputEnabled(true);
				});
			}

			mainMenuTween->Update(in_dt);
			m_mainMenu->SetRelativePosition(CSCore::Vector2(mainMenuTween->GetValue(), m_mainMenu->GetLocalRelativePosition().y));
		}

		if (m_particleMenuMovementData.m_moving)
		{
			//Get correct directional tween based on movement data
			CSCore::EaseInOutBackTween<f32>* particleMenuTween = m_particleMenuMovementData.m_leftTween ? &m_leftToCenterTween : &m_rightToCenterTween;

			//Play tween based on its center movement data if it's not playing already
			if (!particleMenuTween->IsPlaying())
			{
				if (m_particleMenuMovementData.m_movingCenter)
				{
					particleMenuTween->Play(CSCore::TweenPlayMode::k_once);
				}
				else
				{
					particleMenuTween->Play(CSCore::TweenPlayMode::k_onceReverse);
				}
			}

			//Make sure that the menu stops moving at the end of the animation
			particleMenuTween->SetOnEndDelegate([this](CSCore::EaseInOutBackTween<f32>* in_tween)
			{
				m_particleMenuMovementData.m_moving = false;
				MainMenuState::SetAllInputEnabled(true);
			});

			particleMenuTween->Update(in_dt);
			m_particleMenu->SetRelativePosition(CSCore::Vector2(particleMenuTween->GetValue(), m_particleMenu->GetLocalRelativePosition().y));
		}

		if (m_benchmarkingMenuMovementData.m_moving)
		{
			//Get correct directional tween based on movement data
			CSCore::EaseInOutBackTween<f32>* benchmarkingMenuTween = m_benchmarkingMenuMovementData.m_leftTween ? &m_leftToCenterTween : &m_rightToCenterTween;

			//Play tween based on its center movement data if it's not playing already
			if (!benchmarkingMenuTween->IsPlaying())
			{
				if (m_benchmarkingMenuMovementData.m_movingCenter)
				{
					benchmarkingMenuTween->Play(CSCore::TweenPlayMode::k_once);
				}
				else
				{
					benchmarkingMenuTween->Play(CSCore::TweenPlayMode::k_onceReverse);
				}
			}

			//Make sure that the menu stops moving at the end of the animation
			benchmarkingMenuTween->SetOnEndDelegate([this](CSCore::EaseInOutBackTween<f32>* in_tween)
			{
				m_benchmarkingMenuMovementData.m_moving = false;
				MainMenuState::SetAllInputEnabled(true);
			});

			benchmarkingMenuTween->Update(in_dt);
			m_benchmarkingMenu->SetRelativePosition(CSCore::Vector2(benchmarkingMenuTween->GetValue(), m_benchmarkingMenu->GetLocalRelativePosition().y));
		}
    }
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::SetupMainMenu()
	{
		//Get play and right arrow buttons to set their events
		auto playButton = m_mainMenu->GetWidget("PlayButton");
		auto rightArrowButton = m_mainMenu->GetWidget("RightArrowButton");
		
		//Play button events
		m_playButtonReleasedConnection = playButton->GetReleasedInsideEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType) 
		{ 
			//L <-- C
			m_mainMenuMovementData.m_leftTween = true;
			m_mainMenuMovementData.m_movingCenter = false;
			m_mainMenuMovementData.m_moving = true;

			//Update the particle effect component factory system to use the chosen particles
			UpdateParticleOptions();

			//Update member variable so we transition into the next state after the animation 
			m_isPlayButtonPressed = true;

			//Disable input until the end of the animation
			MainMenuState::SetAllInputEnabled(false);
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
		m_mainRArrowReleasedConnection = rightArrowButton->GetReleasedInsideEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
		{
			//L <-- C
			m_mainMenuMovementData.m_leftTween = true;
			m_mainMenuMovementData.m_movingCenter = false;
			m_mainMenuMovementData.m_moving = true;

			//C <-- R
			m_particleMenuMovementData.m_leftTween = false;
			m_particleMenuMovementData.m_movingCenter = true;
			m_particleMenuMovementData.m_moving = true;

			//Disable input until the end of the animation
			MainMenuState::SetAllInputEnabled(false);
		});
		m_mainRArrowEnteredConnection = rightArrowButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_mainRArrowExitedConnection = rightArrowButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::SetupParticleMenu()
	{
		//Get arrow buttons to set their events
		auto leftArrowButton = m_particleMenu->GetWidget("LeftArrowButton");
		auto rightArrowButton = m_particleMenu->GetWidget("RightArrowButton");

		//Left arrow button events
		m_particleLArrowReleasedConnection = leftArrowButton->GetReleasedInsideEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
		{
			//L --> C
			m_mainMenuMovementData.m_leftTween = true;
			m_mainMenuMovementData.m_movingCenter = true;
			m_mainMenuMovementData.m_moving = true;

			//C --> R
			m_particleMenuMovementData.m_leftTween = false;
			m_particleMenuMovementData.m_movingCenter = false;
			m_particleMenuMovementData.m_moving = true;

			//Disable input until the end of the animation
			MainMenuState::SetAllInputEnabled(false);
		});
		m_particleLArrowEnteredConnection = leftArrowButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_particleLArrowExitedConnection = leftArrowButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});

		//Right arrow button events
		m_particleRArrowReleasedConnection = rightArrowButton->GetReleasedInsideEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
		{
			//C <-- R
			m_benchmarkingMenuMovementData.m_leftTween = false;
			m_benchmarkingMenuMovementData.m_movingCenter = true;
			m_benchmarkingMenuMovementData.m_moving = true;

			//L <-- C
			m_particleMenuMovementData.m_leftTween = true;
			m_particleMenuMovementData.m_movingCenter = false;
			m_particleMenuMovementData.m_moving = true;

			//Disable input until the end of the animation
			MainMenuState::SetAllInputEnabled(false);
		});
		m_particleRArrowEnteredConnection = rightArrowButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_particleRArrowExitedConnection = rightArrowButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});

		SetupParticleMenuGrid();
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MainMenuState::SetupBenchmarkingMenu()
	{
		//Get arrow buttons to set their events
		auto leftArrowButton = m_benchmarkingMenu->GetWidget("LeftArrowButton");
		auto rightArrowButton = m_benchmarkingMenu->GetWidget("RightArrowButton");

		//Left arrow button events
		m_benchLArrowReleasedConnection = leftArrowButton->GetReleasedInsideEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
		{
			//L --> C
			m_particleMenuMovementData.m_leftTween = true;
			m_particleMenuMovementData.m_movingCenter = true;
			m_particleMenuMovementData.m_moving = true;

			//C --> R
			m_benchmarkingMenuMovementData.m_leftTween = false;
			m_benchmarkingMenuMovementData.m_movingCenter = false;
			m_benchmarkingMenuMovementData.m_moving = true;

			//Disable input until the end of the animation
			MainMenuState::SetAllInputEnabled(false);
		});
		m_benchLArrowEnteredConnection = leftArrowButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_benchLArrowExitedConnection = leftArrowButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});

		//Right arrow button events
		m_benchRArrowReleasedConnection = rightArrowButton->GetReleasedInsideEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType)
		{
			//TODO: nothing will happen
		});
		m_benchRArrowEnteredConnection = rightArrowButton->GetMoveEnteredEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::DarkenWidget(in_widget, in_pointer);
		});
		m_benchRArrowExitedConnection = rightArrowButton->GetMoveExitedEvent().OpenConnection([this](CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer)
		{
			MainMenuState::ResetDarkenWidget(in_widget, in_pointer);
		});
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

