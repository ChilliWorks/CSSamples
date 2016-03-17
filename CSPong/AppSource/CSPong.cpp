//
//  CSPong.cpp
//  CCSPong
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

#include <CSPong.h>

#include <Splash/SplashState.h>
#include <Game/Particles/ParticleEffectComponentFactory.h>

#include <ChilliSource/Input/Accelerometer.h>
#include <ChilliSource/Rendering/Model.h>

//---------------------------------------------------------
/// Implements the body of the CreateApplication method
/// which creates the derviced CSPong application
///
/// @author S Downie
///
/// @return Instance of CSCore::Application
//---------------------------------------------------------
CSCore::Application* CreateApplication()
{
    return new CSPong::App();
}

#if defined(CS_TARGETPLATFORM_ANDROID) && defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)

std::string GetGooglePlayLvlPublicKey()
{
    //Enter your Google Play LVL public key here if you are building for Google Play on Android
    return "";
}

#endif

namespace CSPong
{
    //---------------------------------------------------------
    //---------------------------------------------------------
    void App::CreateSystems()
    {
        CreateSystem<CSRendering::CSModelProvider>();
        CreateSystem<CSRendering::CSAnimProvider>();
        CreateSystem<CSInput::Accelerometer>();
		CreateSystem<ParticleEffectComponentFactory>();
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    void App::OnInit()
    {
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    void App::PushInitialState()
    {

        GetStateManager()->Push(CSCore::StateSPtr(new SplashState()));
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    void App::OnDestroy()
    {
    }
}

