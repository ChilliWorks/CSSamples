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

#include <Animation/KeyframeAnimation.h>

#include <ChilliSource/Rendering/Sprite.h>

namespace CSRunner
{
    //------------------------------------------------------------
    KeyframeAnimation::KeyframeAnimation(CS::SpriteComponent* spriteComponent, std::vector<std::string> frameAtlasIds, f32 frameRate) noexcept
    : m_spriteComponent(spriteComponent), m_frames(std::move(frameAtlasIds)), m_frameTime(1.0f/frameRate)
    {
        
    }
    
    //------------------------------------------------------------
    bool KeyframeAnimation::Update(f32 timeSinceLastUpdate, UpdateMode updateMode) noexcept
    {
        m_timeSinceLastFrame += timeSinceLastUpdate;
        
        // If the update FPS is less than the desired animation FPS we skip frames to keep the
        // animation a fixed length
        std::size_t numFramesElapsed = (std::size_t)(m_timeSinceLastFrame/m_frameTime);
        
        bool finished = false;
        
        if(numFramesElapsed > 0)
        {
            m_timeSinceLastFrame -= numFramesElapsed * m_frameTime;
            m_currentFrame += numFramesElapsed;
            if(m_currentFrame >= m_frames.size())
            {
                switch(updateMode)
                {
                    case UpdateMode::k_once:
                        m_currentFrame = m_frames.size() - 1;
                        finished = true;
                        break;
                    case UpdateMode::k_looping:
                        m_currentFrame = m_currentFrame % m_frames.size();
                        break;
                }
            }
            
            m_spriteComponent->SetTextureAtlasId(m_frames[m_currentFrame]);
        }
        
        return finished;
    }
    
    //------------------------------------------------------------
    void KeyframeAnimation::Reset() noexcept
    {
        m_currentFrame = 0;
        m_timeSinceLastFrame = 0.0f;
        m_spriteComponent->SetTextureAtlasId(m_frames[0]);
    }
}
