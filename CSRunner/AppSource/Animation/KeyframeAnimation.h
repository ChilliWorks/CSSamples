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

#ifndef _APPSOURCE_ANIMATION_KEYFRAMEANIMATION_H_
#define _APPSOURCE_ANIMATION_KEYFRAMEANIMATION_H_

#include <ForwardDeclarations.h>

#include <vector>

namespace CSRunner
{
    /// Animates the atlas ids of a sprite component
    ///
    class KeyframeAnimation
    {
    public:
        
        enum class UpdateMode
        {
            k_once,
            k_looping
        };
        
        /// @param spriteComponent
        ///     The sprite to animate
        /// @param frameAtlasIds
        ///     Array containing the string ids for each frame of the animation
        /// @param frameRate
        ///     Used to calculate the time between frames
        ///
        KeyframeAnimation(CS::SpriteComponent* spriteComponent, std::vector<std::string> frameAtlasIds, f32 frameRate = 30.0f) noexcept;
        
        /// @param timeSinceLastUpdate
        ///     Time in seconds since last update
        /// @param updateMode
        ///     Whether to continuously loop or stop once the end is reached
        ///
        /// @return TRUE if the animation will no longer upddate (e.g. if mode is once at at end)
        ///
        bool Update(f32 timeSinceLastUpdate, UpdateMode updateMode) noexcept;
        
        /// Reset the animation back to the first frame
        ///
        void Reset() noexcept;
        
    private:
        
        CS::SpriteComponent* m_spriteComponent;
        std::vector<std::string> m_frames;
        f32 m_frameTime;
        
        f32 m_timeSinceLastFrame = 0.0f;
        std::size_t m_currentFrame = 0;
    };
}
#endif
