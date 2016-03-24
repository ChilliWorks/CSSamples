//
//  GameEntityFactory.cpp
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

#include <Game/GameEntityFactory.h>

#include <Game/ScoringSystem.h>
#include <Game/Ball/BallControllerComponent.h>
#include <Game/Camera/CameraTiltComponent.h>
#include <Game/Paddle/AIControllerComponent.h>
#include <Game/Paddle/TouchControllerComponent.h>
#include <Game/Physics/DynamicBodyComponent.h>
#include <Game/Physics/StaticBodyComponent.h>
#include <Game/Physics/TriggerComponent.h>
#include <Game/Particles/ParticleEffectComponentFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Lighting.h>
#include <ChilliSource/Rendering/Model.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/Rendering/Particle.h>

namespace CSPong
{
    CS_DEFINE_NAMEDTYPE(GameEntityFactory);
    
    namespace
    {
        const f32 k_paddlePercentageOffsetFromCentre = 0.4f;
    }
    
    //---------------------------------------------------
    //---------------------------------------------------
    GameEntityFactoryUPtr GameEntityFactory::Create(PhysicsSystem* in_physicsSystem, ScoringSystem* in_scoringSystem)
    {
        return GameEntityFactoryUPtr(new GameEntityFactory(in_physicsSystem, in_scoringSystem));
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    GameEntityFactory::GameEntityFactory(PhysicsSystem* in_physicsSystem, ScoringSystem* in_scoringSystem)
    : m_physicsSystem(in_physicsSystem), m_scoringSystem(in_scoringSystem)
    {
        
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool GameEntityFactory::IsA(CSCore::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == GameEntityFactory::InterfaceID;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntityUPtr GameEntityFactory::CreateCamera() const
    {   
        CSCore::EntityUPtr camera(CSCore::Entity::Create());
        
        CameraTiltComponentSPtr cameraTiltComponent(new CameraTiltComponent(CSCore::Vector3::k_unitPositiveZ));
        camera->AddComponent(cameraTiltComponent);
        
        const f32 k_fov = 3.14f / 3.0f;
        const f32 k_viewportHeight = 100.0f;
        f32 dist = (0.5f * k_viewportHeight) / std::tan(k_fov * 0.5f);
        auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
        
        CSRendering::CameraComponentSPtr cameraComponent = renderFactory->CreatePerspectiveCameraComponent(k_fov, 10.0f, 150.0f);
        camera->AddComponent(cameraComponent);
        camera->GetTransform().SetLookAt(CSCore::Vector3::k_unitNegativeZ * dist, CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY);
        
        return camera;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntityUPtr GameEntityFactory::CreateDiffuseLight() const
    { 
        CSCore::EntityUPtr light(CSCore::Entity::Create());
        
        auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
        
        CSRendering::DirectionalLightComponentSPtr lightComponent = renderFactory->CreateDirectionalLightComponent(1024);
        lightComponent->SetIntensity(2.0f);
        lightComponent->SetColour(CSCore::Colour::k_orange);
        
        const f32 k_distanceFromGround = 70.0f;
        CSCore::Vector3 lightDir(-1, 1, 1);
        lightDir.Normalise();
        CSCore::Vector3 lightPos = -lightDir * k_distanceFromGround;
        
        lightComponent->SetShadowVolume(130.0f, 130.0f, 1.0f, 130.0f);
        lightComponent->SetShadowTolerance(0.05f);
        light->GetTransform().SetLookAt(lightPos, CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY);
        light->AddComponent(lightComponent);
        
        return light;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntityUPtr GameEntityFactory::CreateAmbientLight() const
    {
        CSCore::EntityUPtr light(CSCore::Entity::Create());
        
        auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
        
        CSRendering::AmbientLightComponentSPtr lightComponent = renderFactory->CreateAmbientLightComponent();
        lightComponent->SetColour(CSCore::Colour::k_white * 0.5f);
        light->AddComponent(lightComponent);
        
        return light;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntitySPtr GameEntityFactory::CreateBall() const
    {   
        auto resourcePool = CSCore::Application::Get()->GetResourcePool();
        CSRendering::MeshCSPtr mesh = resourcePool->LoadResource<CSRendering::Mesh>(CSCore::StorageLocation::k_package, "Models/Ball.csmodel");
        CSRendering::MaterialCSPtr material = resourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/Models/Models.csmaterial");
        
        CSCore::EntitySPtr ball(CSCore::Entity::Create());
        
        auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
        CSRendering::StaticMeshComponentSPtr meshComponent = renderFactory->CreateStaticMeshComponent(mesh, material);
        ball->AddComponent(meshComponent);
        
        CSCore::Vector2 collisionSize = mesh->GetAABB().GetSize().XY();
        DynamicBodyComponentSPtr dynamicBody(new DynamicBodyComponent(m_physicsSystem, collisionSize, 1.0f, 0.0f, 1.0f));
        ball->AddComponent(dynamicBody);
        
        BallControllerComponentSPtr ballControllerComponent(new BallControllerComponent(dynamicBody.get()));
        ball->AddComponent(ballControllerComponent);
        
        m_scoringSystem->AddBallBody(dynamicBody);

		auto particleECFSystem = CSCore::Application::Get()->GetSystem<ParticleEffectComponentFactory>();
		particleECFSystem->AddBallParticles(ball);
        
        return ball;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntitySPtr GameEntityFactory::CreatePlayerPaddle(const CSCore::EntitySPtr& in_camera) const
    {
        CSCore::EntitySPtr paddle(CSCore::Entity::Create());
        
        auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
        auto resourcePool = CSCore::Application::Get()->GetResourcePool();
        
        CSRendering::MeshCSPtr mesh = resourcePool->LoadResource<CSRendering::Mesh>(CSCore::StorageLocation::k_package, "Models/Paddle/PaddleLeft.csmodel");
        CSRendering::MaterialCSPtr material = resourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/Models/Models.csmaterial");
        
        CSRendering::StaticMeshComponentSPtr meshComponent = renderFactory->CreateStaticMeshComponent(mesh, material);
        paddle->AddComponent(meshComponent);
        
        CSCore::Vector2 collisionSize = mesh->GetAABB().GetSize().XY();
        DynamicBodyComponentSPtr dynamicBody(new DynamicBodyComponent(m_physicsSystem, collisionSize, 100.0f, 0.2f, 0.6f));
        paddle->AddComponent(dynamicBody);
        
        TouchControllerComponentSPtr touchComponent(new TouchControllerComponent(dynamicBody, in_camera->GetComponent<CSRendering::CameraComponent>()));
        paddle->AddComponent(touchComponent);
        
        CSRendering::MeshCSPtr arenaMesh = resourcePool->LoadResource<CSRendering::Mesh>(CSCore::StorageLocation::k_package, "Models/Arena.csmodel");
        f32 offsetX = arenaMesh->GetAABB().GetSize().x * -k_paddlePercentageOffsetFromCentre;
        paddle->GetTransform().SetPosition(offsetX, 0.0f, 0.0f);

		auto particleECFSystem = CSCore::Application::Get()->GetSystem<ParticleEffectComponentFactory>();
		particleECFSystem->AddPlayerParticlesOnCollision(paddle, dynamicBody->GetCollisionEvent());
        
        return paddle;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntitySPtr GameEntityFactory::CreateOppositionPaddle(const CSCore::EntitySPtr& in_ball) const
    {
        CSCore::EntitySPtr paddle(CSCore::Entity::Create());
        
        auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
        auto resourcePool = CSCore::Application::Get()->GetResourcePool();
        
        CSRendering::MeshCSPtr mesh = resourcePool->LoadResource<CSRendering::Mesh>(CSCore::StorageLocation::k_package, "Models/Paddle/PaddleRight.csmodel");
        CSRendering::MaterialCSPtr material = resourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/Models/Models.csmaterial");
        
        CSRendering::StaticMeshComponentSPtr meshComponent = renderFactory->CreateStaticMeshComponent(mesh, material);
        paddle->AddComponent(meshComponent);
        
        CSCore::Vector2 collisionSize = mesh->GetAABB().GetSize().XY();
        DynamicBodyComponentSPtr dynamicBody(new DynamicBodyComponent(m_physicsSystem, collisionSize, 100.0f, 0.2f, 0.6f));
        paddle->AddComponent(dynamicBody);
        
        AIControllerComponentSPtr aiComponent(new AIControllerComponent(dynamicBody, in_ball));
        paddle->AddComponent(aiComponent);
        
        CSRendering::MeshCSPtr arenaMesh = resourcePool->LoadResource<CSRendering::Mesh>(CSCore::StorageLocation::k_package, "Models/Arena.csmodel");
        f32 offsetX = arenaMesh->GetAABB().GetSize().x * k_paddlePercentageOffsetFromCentre;
        paddle->GetTransform().SetPosition(offsetX, 0.0f, 0.0f);

		auto particleECFSystem = CSCore::Application::Get()->GetSystem<ParticleEffectComponentFactory>();
		particleECFSystem->AddOpponentParticlesOnCollision(paddle, dynamicBody->GetCollisionEvent());
		
        return paddle;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntityUPtr GameEntityFactory::CreateArena() const
    {
        CSCore::EntityUPtr arena(CSCore::Entity::Create());
        
        auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
        auto resourcePool = CSCore::Application::Get()->GetResourcePool();
        
        CSRendering::MeshCSPtr mesh = resourcePool->LoadResource<CSRendering::Mesh>(CSCore::StorageLocation::k_package, "Models/Arena.csmodel");
        CSRendering::MaterialCSPtr material = resourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/Models/Models.csmaterial");
        
        const f32 k_border = 1.0f;
        const CSCore::Vector2 k_arenaDimensions(mesh->GetAABB().GetSize().XY() * 0.9f);
        
        CSRendering::StaticMeshComponentSPtr meshComponent = renderFactory->CreateStaticMeshComponent(mesh, material);
        meshComponent->SetShadowCastingEnabled(false);
        arena->AddComponent(meshComponent);
        
        CSCore::EntitySPtr bottomEdge(CSCore::Entity::Create());
        bottomEdge->GetTransform().SetPosition(CSCore::Vector3(0.0f, -k_arenaDimensions.y * 0.5f - k_border * 0.5f, 0.0f));
        StaticBodyComponentSPtr bottomEdgeStaticBody(new StaticBodyComponent(m_physicsSystem, CSCore::Vector2(k_arenaDimensions.x + k_border * 2.0f, k_border)));
        bottomEdge->AddComponent(bottomEdgeStaticBody);
        arena->AddEntity(bottomEdge);
        
        CSCore::EntitySPtr topEdge(CSCore::Entity::Create());
        topEdge->GetTransform().SetPosition(CSCore::Vector3(0.0f, k_arenaDimensions.y * 0.5f + k_border * 0.5f, 0.0f));
        StaticBodyComponentSPtr topEdgeStaticBody(new StaticBodyComponent(m_physicsSystem, CSCore::Vector2(k_arenaDimensions.x + k_border * 2.0f, k_border)));
        topEdge->AddComponent(topEdgeStaticBody);
        arena->AddEntity(topEdge);
        
        CSCore::EntitySPtr leftEdge(CSCore::Entity::Create());
        leftEdge->GetTransform().SetPosition(CSCore::Vector3(-k_arenaDimensions.x * 0.5f - k_border * 0.5f, 0.0f, 0.0f));
        TriggerComponentSPtr leftEdgeTrigger(new TriggerComponent(m_physicsSystem, CSCore::Vector2(k_border, k_arenaDimensions.y)));
        leftEdge->AddComponent(leftEdgeTrigger);
        arena->AddEntity(leftEdge);
        
        CSCore::EntitySPtr rightEdge(CSCore::Entity::Create());
        rightEdge->GetTransform().SetPosition(CSCore::Vector3(k_arenaDimensions.x * 0.5f + k_border * 0.5f, 0.0f, 0.0f));
        TriggerComponentSPtr rightEdgeTrigger(new TriggerComponent(m_physicsSystem, CSCore::Vector2(k_border, k_arenaDimensions.y)));
        rightEdge->AddComponent(rightEdgeTrigger);
        arena->AddEntity(rightEdge);
        
        m_scoringSystem->AddGoalTrigger(leftEdge, 1);
        m_scoringSystem->AddGoalTrigger(rightEdge, 0);
        
        return arena;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntityUPtr GameEntityFactory::CreateScoreSprite(const CSCore::Vector2& in_size, CSRendering::AlignmentAnchor in_alignmentAnchor) const
    {
        auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
        auto resPool = CSCore::Application::Get()->GetResourcePool();
        auto digitAtlas = resPool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/Digits/Digits.csatlas");
        auto digitMaterial = resPool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/Digits/Digits.csmaterial");
        CSRendering::SpriteComponentSPtr spriteComponent = renderFactory->CreateSpriteComponent(in_size, digitAtlas, "0", digitMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
        spriteComponent->SetColour(0.5f, 0.5f, 0.5f, 0.5f);
        spriteComponent->SetOriginAlignment(in_alignmentAnchor);
        
        CSCore::EntityUPtr scoreEnt = CSCore::Entity::Create();
        scoreEnt->AddComponent(spriteComponent);
        return scoreEnt;
    }
}