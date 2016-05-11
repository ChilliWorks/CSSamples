//
//  CSPong.h
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

#ifndef _APPSOURCE_CSPONG_H_
#define _APPSOURCE_CSPONG_H_

#include <ChilliSource/Core/Base.h>

namespace CSPong
{
    //---------------------------------------------------------
    /// Entry point into any Chilli Source app. This
    /// class is delegated to by the Application class for
    /// system creation and for pushing the initial state onto
    /// the state manager hierarchy
    //---------------------------------------------------------
    class App final : public CS::Application
    {
    public:
        //---------------------------------------------------------
        /// Overloaded method in which app systems can be created
        ///
        /// @author S Downie
        //---------------------------------------------------------
        void CreateSystems() override;
        //---------------------------------------------------------
        /// Called after all app system have been created allowing
        /// any system dependent initialisation
        ///
        /// @author S Downie
        //---------------------------------------------------------
        void OnInit() override;
        //---------------------------------------------------------
        /// This is where the first app state is pushed onto
        /// the state hierarchy.
        ///
        /// @author S Downie
        //---------------------------------------------------------
        void PushInitialState() override;
        //---------------------------------------------------------
        /// Mirrors OnInit and is called when the app is destroyed
        ///
        /// @author S Downie
        //---------------------------------------------------------
        void OnDestroy() override;
    };
}

#endif
