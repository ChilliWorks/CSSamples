//
//  GameEntityFactory.h
//  CSPong
//  Created by Scott Downie on 02/07/2014.
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

#ifndef _APPSOURCE_GAME_GAMEENTITYFACTORY_H_
#define _APPSOURCE_GAME_GAMEENTITYFACTORY_H_

#include <ForwardDeclarations.h>

#include <ChilliSource/Core/System.h>
#include <ChilliSource/Rendering/Base.h> 

namespace CSPong
{
    //------------------------------------------------------------
    /// System for creating entities with the correct
    /// view and behaviour components attached
    ///
    /// @author S Downie
    //------------------------------------------------------------
    class GameEntityFactory final : public CSCore::StateSystem
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(GameEntityFactory);
        
        //---------------------------------------------------
        /// Creates a new instance of the system.
        ///
        /// @author S Downie
        ///
        /// @param Physics system to register physics entities
        /// with
        /// @param Scoring system
        ///
        /// @return The new instance.
        //---------------------------------------------------
        static GameEntityFactoryUPtr Create(PhysicsSystem* in_physicsSystem, ScoringSystem* in_scoringSystem);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
        //------------------------------------------------------------
        /// Create the camera entity. The camera is a perspective camera
        ///
        /// @author S Downie
        ///
        /// @return Camera entity (NOTE: It returns a unique ptr which
        /// tells the caller that they take ownership)
        //------------------------------------------------------------
        CSCore::EntityUPtr CreateCamera() const;
        //------------------------------------------------------------
        /// Create the light entity. The light is ambient
        ///
        /// @author S Downie
        ///
        /// @return Light entity (NOTE: It returns a unique ptr which
        /// tells the caller that they take ownership)
        //------------------------------------------------------------
        CSCore::EntityUPtr CreateAmbientLight() const;
        //------------------------------------------------------------
        /// Create the light entity. The light is directional
        ///
        /// @author S Downie
        ///
        /// @return Light entity (NOTE: It returns a unique ptr which
        /// tells the caller that they take ownership)
        //------------------------------------------------------------
        CSCore::EntityUPtr CreateDiffuseLight() const;
        //------------------------------------------------------------
        /// Create the ball entity. The ball entity consists of
        /// a static model and logic that controls its movement. These
        /// are added to the entity as components.
        ///
        /// @author S Downie
        ///
        /// @return Ball entity (NOTE: It returns a unique ptr which
        /// tells the caller that they take ownership)
        //------------------------------------------------------------
        CSCore::EntitySPtr CreateBall() const;
        //------------------------------------------------------------
        /// Create the player paddle. This has components for displaying
        /// the paddle and allowing the player to move it via the
        /// user input
        ///
        /// @author S Downie
        ///
        /// @param Camera used for movement
        ///
        /// @return Paddle entity (NOTE: It returns a unique ptr which
        /// tells the caller that they take ownership)
        //------------------------------------------------------------
        CSCore::EntitySPtr CreatePlayerPaddle(const CSCore::EntitySPtr& in_camera) const;
        //------------------------------------------------------------
        /// Create the oppositions paddle. This has components for displaying
        /// the paddle and AI logic that allows the paddle to move
        /// autonomously
        ///
        /// @author S Downie
        ///
        /// @param Ball to chase
        ///
        /// @return Paddle entity (NOTE: It returns a unique ptr which
        /// tells the caller that they take ownership)
        //------------------------------------------------------------
        CSCore::EntitySPtr CreateOppositionPaddle(const CSCore::EntitySPtr& in_ball) const;
        //------------------------------------------------------------
        /// Creates the arean. This contains a component for displaying
        /// the stage and child entities representing the edges of the
        /// stage. The top and bottom edge child entities have static
        /// body components and the left and right edge child entities
        /// have trigger body components.
        ///
        /// @author Ian Copland
        ///
        /// @return The arena entity (NOTE: It returns a unique ptr which
        /// tells the caller that they take ownership)
        //------------------------------------------------------------
        CSCore::EntityUPtr CreateArena() const;
        //------------------------------------------------------------
        /// Creates the score sprite entity. This simply contains
        /// a sprite component.
        ///
        /// @author Ian Copland
        ///
        /// @param The size of the sprite.
        /// @param The sprites anchor.
        ///
        /// @return The sprite entity (NOTE: It returns a unique ptr which
        /// tells the caller that they take ownership)
        //------------------------------------------------------------
        CSCore::EntityUPtr CreateScoreSprite(const CSCore::Vector2& in_size, CSRendering::AlignmentAnchor in_alignmentAnchor) const;
        
    private:
        //----------------------------------------------------------
        /// Private constructor to enforce use of factory method
        ///
        /// @author S Downie
        ///
        /// @param Physics system to register physics entities
        /// with
        /// @param Scoring system
        //----------------------------------------------------------
        GameEntityFactory(PhysicsSystem* in_physicsSystem, ScoringSystem* in_scoringSystem);
        
    private:
        
        PhysicsSystem* m_physicsSystem;
        ScoringSystem* m_scoringSystem;
    };
}

#endif
