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

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Lighting.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Model.h>
#include <ChilliSource/Rendering/Sprite.h>
#include <ChilliSource/Rendering/Texture.h>

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
    bool GameEntityFactory::IsA(CS::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == GameEntityFactory::InterfaceID;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::EntityUPtr GameEntityFactory::CreateCamera() const
    {
        /*
         ==============================
         Chilli Source Tour: Components
         ==============================
         
         As mentioned, adding functionality to an entity in Chilli Source is achieved by attaching components. A component
         is a module that can provide any functionality that applies to a single game object, i.e rendering 
         a model or representing a collision body.
         
         Components are usually created via their constructors and the Entity class exposes methods for adding, removing
         and querying for components.
         
            CS::StaticModelComponentSPtr staticMeshComponent = std::make_shared<CS::StaticModelComponent>(mesh, material);
            entity->AddComponent(staticMeshComponent);
            staticMeshComponent = entity->GetComponent<StaticModelComponent>();
            staticMeshComponent->RemoveFromEntity();
         
         Much like systems, Components receive lifecycle events such as OnUpdate(), OnSuspend() and
         OnResume(). These are only received while the component is attached to an entity and that
         entity has been added to the scene. In addition to the standard lifecycle events there are
         also Component specific events: OnAddedToEntity(), OnAddedToScene(), OnRemovedFromScene() and
         OnRemovedFromEntity(). These lifecycle events are all governed by the owning entity.
         
         ------------------------------
         
         Next: 'Camera' in GameEntityFactory::CreateCamera()
         */
        
        CS::EntityUPtr camera(CS::Entity::Create());
        
        CameraTiltComponentSPtr cameraTiltComponent(new CameraTiltComponent(CS::Vector3::k_unitPositiveZ));
        camera->AddComponent(cameraTiltComponent);
        
        /*
         ==========================
         Chilli Source Tour: Camera
         ==========================
         
         In Chilli Source a camera is simply an entity with a Camera Component attached to it. There
         are two types of Camera Component each providing a different type of projection: Perspective
         or Orthographic.
         
            PerspectiveCameraComponentUPtr perspectiveCameraComponent = PerspectiveCameraComponentUPtr(new PerspectiveCameraComponent(aspect, fov, nearPlane, farPlane));
            OrthographicCameraComponentUPtr orthographicCameraComponent = OrthographicCameraComponentUPtr(new OrthographicCameraComponent(viewportSize, nearPlane, farPlane));
         
         The position and direction of the camera are based on the position and orientation of the
         camera entity. Methods such as SetLookAt() on an Entities Transform provide a convenient
         way of positioning a camera.
         
         If a scene has no camera in it, nothing will be rendered other than GUI.
         
         --------------------------
         
         Next: 'Lighting' in GameEntityFactory::CreateDiffuseLight()
         */
        
        const f32 k_fov = 3.14f / 3.0f;
        const f32 k_viewportHeight = 100.0f;
        f32 dist = (0.5f * k_viewportHeight) / std::tan(k_fov * 0.5f);
        
        auto screen = CS::Application::Get()->GetScreen();
        CS::CameraComponentSPtr cameraComponent = std::make_shared<CS::PerspectiveCameraComponent>(screen->GetResolution().x/screen->GetResolution().y, k_fov, 10.0f, 150.0f);
        camera->AddComponent(cameraComponent);
        camera->GetTransform().SetLookAt(CS::Vector3::k_unitNegativeZ * dist, CS::Vector3::k_zero, CS::Vector3::k_unitPositiveY);
        
        return camera;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::EntityUPtr GameEntityFactory::CreateDiffuseLight() const
    {
        /*
         ============================
         Chilli Source Tour: Lighting
         ============================
         
         Like Cameras, Lights are entities with a Light Component attached to it. The engine provides 3 different types of
         light: Ambient, Directional and Point.
         
             CS::AmbientLightComponentSPtr ambientLightComponent = std::make_shared<CS::AmbientLightComponent>(CS::Colour(0.4f, 0.4f, 0.4f, 1.0f)));
             
             CS::DirectionalLightComponentSPtr directionalLightComponent = std::make_shared<CS::DirectionalLightComponent>(CS::Colour(0.6f, 0.6f, 0.6f, 1.0f)));
             
             CS::PointLightComponentSPtr pointLightComponent = std::make_shared<CS::PointLightComponent>(CS::Colour(0.6f, 0.6f, 0.6f, 1.0f)));
         
         Ambient lights apply light evenly to all objects in the scene emulating light
         that has reflected multiple times and has no apparent source. Directional lights apply 
         light in a given direction to the entire scene, representing a far away light source such 
         as the sun. Point lights are omni-directional light objects from a point source with attenuation as objects
         get further away, emulating light from a closer source such as a lamp.

         Chilli Source supports many lights in a scene as it uses a multi-pass renderer. It
         is still a forward renderer however and therefore each additional light will
         negatively affect performance.
         
         Again similar to a Camera Component, the position and (where relevant) the direction
         of a light comes from the light Entities transform. The SetLookAt() method
         is also useful for setting the direction of a light.
         
         Directional lights can cast shadows. To enable shadows on a directional light, simply
         pass the shadow map resolution qualitu when creating the component. Shadows require a
         small amount of configuration before use. First of all the shadow volume needs to be
         set. This is the area in which objects can cast shadows in front of the light. Secondly
         the shadow tolerence typically needs to be set. This is a small offset used in the shadow
         calculations to avoid shadow artifacts. By default all opaque objects will cast shadows.
         This can be disabled using SetShadowCastingEnabled() in Static and Animated Mesh Component.
         
         ----------------------------
         
         Next: 'Resources' in GameEntityFactory::CreateBall()
         */
        
        CS::EntityUPtr light(CS::Entity::Create());
        
        CS::DirectionalLightComponentSPtr lightComponent = std::make_shared<CS::DirectionalLightComponent>(CS::DirectionalLightComponent::ShadowQuality::k_medium, CS::Colour::k_orange, 2.0f);
        
        const f32 k_distanceFromGround = 70.0f;
        CS::Vector3 lightDir(-1, 1, 1);
        lightDir.Normalise();
        CS::Vector3 lightPos = -lightDir * k_distanceFromGround;
        
        lightComponent->SetShadowVolume(130.0f, 130.0f, 1.0f, 130.0f);
        lightComponent->SetShadowTolerance(0.05f);
        light->GetTransform().SetLookAt(lightPos, CS::Vector3::k_zero, CS::Vector3::k_unitPositiveY);
        light->AddComponent(lightComponent);
        
        return light;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::EntityUPtr GameEntityFactory::CreateAmbientLight() const
    {
        CS::EntityUPtr light(CS::Entity::Create());
        
        CS::AmbientLightComponentSPtr lightComponent = std::make_shared<CS::AmbientLightComponent>(CS::Colour::k_white * 0.5f);
        light->AddComponent(lightComponent);
        
        return light;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::EntityUPtr GameEntityFactory::CreateBall() const
    {
        /*
         =============================
         Chilli Source Tour: Resources
         =============================
         
         In Chilli Source all resources are contained in the Resource Pool and loaded using a consistent API. When trying to load a
         resource the pool is queried instead of loading directly from disk. If the pool already
         contains a loaded version of the resource this will be returned. If not, the Resource Pool
         will load it from disk. All loaded resources are immutable and are therefore returned
         as const shared pointers.
         
            CS::TextureCSPtr texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_package, "Texture.png");
         
         The resource pool provides methods for asynchonous loading. In this case the resource is
         not returned immediately instead being returned through a callback provided when calling
         load.
         
             resourcePool->LoadResourceAsync<CS::Texture>(CS::StorageLocation::k_package, "Texture.png", [](const CS::TextureCSPtr& in_texture)
             {
                //Do something.
             });
         
         Resources can also be created manually using the CreateResource() method. In this case
         the resource is still pooled but it must be given a unique identifier and a mutable 
         instance is returned. This can be queried from the pool using GetResource().
         
             CS::TextureSPtr mutableTexture = resourcePool->CreateResource("MyTexture");
             CS::TextureCSPtr texture = resourcePool->GetResource("MyTexture");
         
         Resources that are created manually must also be built manually (for instance texture has a build method that takes in image data, format, etc).
         
         If the application receives a memory warning the Resource Pool will release any resources
         that are no-longer in use. This can also be performed manually by calling ReleaseAllUnused().
         
         -----------------------------
         
         Next: 'Materials' in GameEntityFactory::CreateBall()
         */
        
        auto resourcePool = CS::Application::Get()->GetResourcePool();
        CS::ModelCSPtr mesh = resourcePool->LoadResource<CS::Model>(CS::StorageLocation::k_package, "Models/Ball.csmodel");
        
        /*
         =============================
         Chilli Source Tour: Materials
         =============================
         
         In Chilli Source a material is a resource that contains a collection of rendering settings.
         This includes the textures that should be used, the lighting surface values and render options
         like the cull mode.
         
         Materials also have a type. This type determines which shaders should be used to render it.
         For example an AnimatedBlinn material will use the shaders required for applying blinn-phong 
         shading to an animated model or Sprite material is used to render standard 2D sprites. 
         The custom type can also be used to specify custom shaders.
         
         Unlike most resources it is often convenient to create materials in code instead of loading 
         from disk. To help with this a material factory has been provided with convienence
         methods for creating mutable instances of each of the material types.
         
            CS::MaterialFactory* materialFactory = CS::Application::Get()->GetSystem<MaterialFactory>();
            CS::MaterialSPtr spriteMaterial = materialFactory->CreateSprite("UniqueId", texture);
         
         Objects with the same material can be rendered in the same batch and therefore it is important to reduce the number of materials by batching textures.
         
         -----------------------------
         
         Next: 'Texture Atlases' in GameEntityFactory::CreateScoreSprite()
         */
        
        CS::MaterialCSPtr material = resourcePool->LoadResource<CS::Material>(CS::StorageLocation::k_package, "Materials/Models/Models.csmaterial");
        
        CS::EntityUPtr ball(CS::Entity::Create());
        
        CS::StaticModelComponentSPtr meshComponent = std::make_shared<CS::StaticModelComponent>(mesh, material);
        ball->AddComponent(meshComponent);
        
        CS::Vector2 collisionSize = mesh->GetAABB().GetSize().XY();
        DynamicBodyComponentSPtr dynamicBody(new DynamicBodyComponent(m_physicsSystem, collisionSize, 1.0f, 0.0f, 1.0f));
        ball->AddComponent(dynamicBody);
        
        BallControllerComponentSPtr ballControllerComponent(new BallControllerComponent(dynamicBody.get()));
        ball->AddComponent(ballControllerComponent);
        
        m_scoringSystem->AddBallBody(dynamicBody);
        
        return ball;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::EntityUPtr GameEntityFactory::CreatePlayerPaddle(const CS::EntitySPtr& in_camera) const
    {
        CS::EntityUPtr paddle(CS::Entity::Create());
        
        auto resourcePool = CS::Application::Get()->GetResourcePool();
        
        CS::ModelCSPtr mesh = resourcePool->LoadResource<CS::Model>(CS::StorageLocation::k_package, "Models/Paddle/PaddleLeft.csmodel");
        CS::MaterialCSPtr material = resourcePool->LoadResource<CS::Material>(CS::StorageLocation::k_package, "Materials/Models/Models.csmaterial");
        
        CS::StaticModelComponentSPtr meshComponent = std::make_shared<CS::StaticModelComponent>(mesh, material);
        paddle->AddComponent(meshComponent);
        
        CS::Vector2 collisionSize = mesh->GetAABB().GetSize().XY();
        DynamicBodyComponentSPtr dynamicBody(new DynamicBodyComponent(m_physicsSystem, collisionSize, 100.0f, 0.2f, 0.6f));
        paddle->AddComponent(dynamicBody);
        
        TouchControllerComponentSPtr touchComponent(new TouchControllerComponent(dynamicBody, in_camera->GetComponent<CS::CameraComponent>()));
        paddle->AddComponent(touchComponent);
        
        CS::ModelCSPtr arenaMesh = resourcePool->LoadResource<CS::Model>(CS::StorageLocation::k_package, "Models/Arena.csmodel");
        f32 offsetX = arenaMesh->GetAABB().GetSize().x * -k_paddlePercentageOffsetFromCentre;
        paddle->GetTransform().SetPosition(offsetX, 0.0f, 0.0f);
        
        return paddle;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::EntityUPtr GameEntityFactory::CreateOppositionPaddle(const CS::EntitySPtr& in_ball) const
    {
        CS::EntityUPtr paddle(CS::Entity::Create());
        
        auto resourcePool = CS::Application::Get()->GetResourcePool();
        
        CS::ModelCSPtr mesh = resourcePool->LoadResource<CS::Model>(CS::StorageLocation::k_package, "Models/Paddle/PaddleRight.csmodel");
        CS::MaterialCSPtr material = resourcePool->LoadResource<CS::Material>(CS::StorageLocation::k_package, "Materials/Models/Models.csmaterial");
        
        CS::StaticModelComponentSPtr meshComponent = std::make_shared<CS::StaticModelComponent>(mesh, material);
        paddle->AddComponent(meshComponent);
        
        CS::Vector2 collisionSize = mesh->GetAABB().GetSize().XY();
        DynamicBodyComponentSPtr dynamicBody(new DynamicBodyComponent(m_physicsSystem, collisionSize, 100.0f, 0.2f, 0.6f));
        paddle->AddComponent(dynamicBody);
        
        AIControllerComponentSPtr aiComponent(new AIControllerComponent(dynamicBody, in_ball));
        paddle->AddComponent(aiComponent);
        
        CS::ModelCSPtr arenaMesh = resourcePool->LoadResource<CS::Model>(CS::StorageLocation::k_package, "Models/Arena.csmodel");
        f32 offsetX = arenaMesh->GetAABB().GetSize().x * k_paddlePercentageOffsetFromCentre;
        paddle->GetTransform().SetPosition(offsetX, 0.0f, 0.0f);
        
        return paddle;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::EntityUPtr GameEntityFactory::CreateArena() const
    {
        CS::EntityUPtr arena(CS::Entity::Create());
        
        auto resourcePool = CS::Application::Get()->GetResourcePool();
        
        CS::ModelCSPtr mesh = resourcePool->LoadResource<CS::Model>(CS::StorageLocation::k_package, "Models/Arena.csmodel");
        CS::MaterialCSPtr material = resourcePool->LoadResource<CS::Material>(CS::StorageLocation::k_package, "Materials/Models/Models.csmaterial");
        
        const f32 k_border = 1.0f;
        const CS::Vector2 k_arenaDimensions(mesh->GetAABB().GetSize().XY() * 0.9f);
        
        CS::StaticModelComponentSPtr meshComponent = std::make_shared<CS::StaticModelComponent>(mesh, material);
        meshComponent->SetShadowCastingEnabled(false);
        arena->AddComponent(meshComponent);
        
        CS::EntitySPtr bottomEdge(CS::Entity::Create());
        bottomEdge->GetTransform().SetPosition(CS::Vector3(0.0f, -k_arenaDimensions.y * 0.5f - k_border * 0.5f, 0.0f));
        StaticBodyComponentSPtr bottomEdgeStaticBody(new StaticBodyComponent(m_physicsSystem, CS::Vector2(k_arenaDimensions.x + k_border * 2.0f, k_border)));
        bottomEdge->AddComponent(bottomEdgeStaticBody);
        arena->AddEntity(bottomEdge);
        
        CS::EntitySPtr topEdge(CS::Entity::Create());
        topEdge->GetTransform().SetPosition(CS::Vector3(0.0f, k_arenaDimensions.y * 0.5f + k_border * 0.5f, 0.0f));
        StaticBodyComponentSPtr topEdgeStaticBody(new StaticBodyComponent(m_physicsSystem, CS::Vector2(k_arenaDimensions.x + k_border * 2.0f, k_border)));
        topEdge->AddComponent(topEdgeStaticBody);
        arena->AddEntity(topEdge);
        
        CS::EntitySPtr leftEdge(CS::Entity::Create());
        leftEdge->GetTransform().SetPosition(CS::Vector3(-k_arenaDimensions.x * 0.5f - k_border * 0.5f, 0.0f, 0.0f));
        TriggerComponentSPtr leftEdgeTrigger(new TriggerComponent(m_physicsSystem, CS::Vector2(k_border, k_arenaDimensions.y)));
        leftEdge->AddComponent(leftEdgeTrigger);
        arena->AddEntity(leftEdge);
        
        CS::EntitySPtr rightEdge(CS::Entity::Create());
        rightEdge->GetTransform().SetPosition(CS::Vector3(k_arenaDimensions.x * 0.5f + k_border * 0.5f, 0.0f, 0.0f));
        TriggerComponentSPtr rightEdgeTrigger(new TriggerComponent(m_physicsSystem, CS::Vector2(k_border, k_arenaDimensions.y)));
        rightEdge->AddComponent(rightEdgeTrigger);
        arena->AddEntity(rightEdge);
        
        m_scoringSystem->AddGoalTrigger(leftEdge, 1);
        m_scoringSystem->AddGoalTrigger(rightEdge, 0);
        
        return arena;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CS::EntityUPtr GameEntityFactory::CreateScoreSprite(const CS::Vector2& in_size, CS::AlignmentAnchor in_alignmentAnchor) const
    {
        /*
         ===================================
         Chilli Source Tour: Texture Atlases
         ===================================
         
         Texture Atlases (also called Spritesheets) provide a 'virtual' texture lookup within an actual texture. This
         allows multiple objects to share the same physical texture but render with different UVs and sizes, ultimately allowing them to share a material.
         
         Just like other resources, a texture altas is loaded from the resource manager. Once loaded the 'virtual' texture (or sprite frame)
         can be accessed by querying the atlas using a 'Texture Id'. For example:
         
            CS::TextureAltasCSPtr atlas = resourcePool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "Atlas.csatlas");
            CS::UVs virtualTextureUVs = atlas->GetFrameUVs("Frame1");
         
         Typically when working with an atlas the component using it would deal with the virtual texture.
         When creating a SpriteComponent the atlas and the virtual texture id are passed in.
         
         -----------------------------------

         Next: 'Audio' in GameState::OnInit
         */
        
        auto resPool = CS::Application::Get()->GetResourcePool();
        auto digitAtlas = resPool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "TextureAtlases/Digits/Digits.csatlas");
        auto digitMaterial = resPool->LoadResource<CS::Material>(CS::StorageLocation::k_package, "Materials/Digits/Digits.csmaterial");
        CS::SpriteComponentSPtr spriteComponent = std::make_shared<CS::SpriteComponent>(digitMaterial, digitAtlas, "0", in_size, CS::SizePolicy::k_fitMaintainingAspect);
        spriteComponent->SetColour(0.5f, 0.5f, 0.5f, 0.5f);
        spriteComponent->SetOriginAlignment(in_alignmentAnchor);
        
        CS::EntityUPtr scoreEnt = CS::Entity::Create();
        scoreEnt->AddComponent(spriteComponent);
        return scoreEnt;
    }
}
