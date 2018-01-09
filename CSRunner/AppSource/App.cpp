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

#include <App.h>

#include <States/SplashState.h>

#include <ChilliSource/Audio/CricketAudio.h>

//------------------------------------------------------------
CS::Application* CreateApplication(CS::SystemInfoCUPtr systemInfo) noexcept
{
    return new CSRunner::App(std::move(systemInfo));
}

#if defined(CS_TARGETPLATFORM_ANDROID) && defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)

std::string GetGooglePlayLvlPublicKey() noexcept
{
    //Enter your Google Play LVL public key here if you are building for Google Play on Android
    return "";
}

#endif

namespace CSRunner
{
    //------------------------------------------------------------
    App::App(CS::SystemInfoCUPtr systemInfo) noexcept
        : Application(std::move(systemInfo))
    { 
    }

    //------------------------------------------------------------
    void App::CreateSystems() noexcept
    {
#ifndef CS_TARGETPLATFORM_RPI
        //If you have purchased a source code license from Cricket Technology, this product is covered by the source code license (http://www.crickettechnology.com/source_license).
        //Otherwise, this product is covered by the free license (http://www.crickettechnology.com/free_license).
        CreateSystem<CS::CkBankProvider>();
        CreateSystem<CS::CricketAudioSystem>();
#endif
    }

    //------------------------------------------------------------
    void App::PushInitialState() noexcept
    {
        CS_LOG_VERBOSE("Thanks to CodeManu for the game music. https://opengameart.org/content/platformer-game-music-pack");
        GetStateManager()->Push(std::make_shared<SplashState>());
    }
}

