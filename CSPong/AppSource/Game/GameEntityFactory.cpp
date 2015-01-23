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
#include <ChilliSource/Rendering/Model.h>
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
    bool GameEntityFactory::IsA(CSCore::InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == GameEntityFactory::InterfaceID;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntityUPtr GameEntityFactory::CreateCamera() const
    {
        /*
         ==============================
         Chilli Source Tour: Components
         ==============================
         
         As mentioned adding functionality to an entity in Chilli Source is acheived by attaching components. A component
         is a module  that can provide any functionality that applies to a single game object, i.e rendering 
         a model or representing a collision body.
         
         Components are often created through factories like the Render Component Factory. This provides
         convienence methods for creating all rendering related components such as SpriteComponent
         ParticleComponent or AnimatedMeshComponent. The Entity class exposes methods for adding, removing
         and querying for components.
         
            CSRendering::StaticMeshComponentSPtr staticMeshComponent = renderComponentFactory->CreateAnimatedMeshComponent(mesh, material);
            entity->AddComponent(staticMeshComponent);
            staticMeshComponent = entity->GetComponent<StaticMeshComponent>();
            staticMeshComponent->RemoveFromEntity();
         
         Much like systems Components receive lifecycle events such as OnUpdate(), OnSuspend() and
         OnResume(). These are only received while the component is attached to an entity and that
         entity has been added to the scene. In addition to the standard lifecycle events there are
         also Component specific events: OnAddedToEntity(), OnAddedToScene(), OnRemovedFromScene() and
         OnRemovedFromEntity(). These lifecycle events are all governed by the owning entity.
         
         ------------------------------
         
         Next: 'Camera' in GameEntityFactory::CreateCamera()
         */
        
        CSCore::EntityUPtr camera(CSCore::Entity::Create());
        
        CameraTiltComponentSPtr cameraTiltComponent(new CameraTiltComponent(CSCore::Vector3::k_unitPositiveZ));
        camera->AddComponent(cameraTiltComponent);
        
        /*
         ==========================
         Chilli Source Tour: Camera
         ==========================
         
         In Chilli Source a camera is simply any entity with a Camera Component attached to it. There
         are two types of Camera Component each providing a different type of projection: Perspective
         or Orthographic. Both of them are created through the Render Component Factory.
         
            PerspectiveCameraComponentUPtr perspectiveCameraComponent = renderComponentFactory->CreatePerspectiveCameraComponent(fov, nearPlane, farPlane);
            OrthographicCameraComponentUPtr orthographicCameraComponent = renderComponentFactory->CreateOrthographicCameraComponent(viewportSize, nearPlane, farPlane);
         
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
        /*
         ============================
         Chilli Source Tour: Lighting
         ============================
         
         Like Cameras, Lights are entities with a Light Component attached to it. These can also be 
         created through the Render Component Factory. The engine provides 3 different types of 
         light: Ambient, Directional and Point.
         
             CSRendering::AmbientLightComponentSPtr ambientLightComponent = renderFactory->CreateAmbientLightComponent();
             ambientLightComponent->SetColour(CSCore::Colour(0.4f, 0.4f, 0.4f, 1.0f));
             
             CSRendering::DirectionalLightComponentSPtr directionalLightComponent = renderFactory->CreateDirectionalLightComponent();
             directionalLightComponent->SetColour(CSCore::Colour(0.6f, 0.6f, 0.6f, 1.0f));
             
             CSRendering::PointLightComponentSPtr pointLightComponent = renderFactory->CreateDirectionalLightComponent();
             pointLightComponent->SetColour(CSCore::Colour(0.6f, 0.6f, 0.6f, 1.0f));
         
         Ambient lights apply light evenly to all objects in the scene emulating light
         that has reflected multiple times and has no apparent source. Directional lights apply 
         light in a given direction to the entire scene, represening a far away light source such 
         as the sun.  Point lights are omni-directional light objects from a point source with attenuation as objects
         get further away, emulating light from a closer source such as a lamp.

         Chilli Source supports many lights in a scene as it uses a multi-pass renderer. It
         is still a forward renderer however and therefore each additional light will
         negatively affect performance.
         
         Again similar to a Camera Component, the position and (where revelant) the direction
         of a light come from the light Entities transform. The SetLookAt() method
         is also useful for setting the direction of a light.
         
         Directional lights can cast shadows. To enable shadows on a directional light simply
         pass a non-zero shadow map resolution when creating the component. Shadows require a
         small amount of configuration before use. First of all the shadow volume needs to be
         set. This is the area in which objects can cast shadows in front of the light. Secondly
         the shadow tolerence typically needs to be set. This is a small offset used in the shadow
         calculations to avoid shadow artifacts. By default all opaque objects will cast shadows.
         This can be disabled using SetShadowCastingEnabled() in Static and Animated Mesh Component.
         
         ----------------------------
         
         Next: 'Resources' in GameEntityFactory::CreateBall()
         */
        
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
    CSCore::EntityUPtr GameEntityFactory::CreateBall() const
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
         
            CSRendering::TextureCSPtr texture = resourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "Texture.png");
         
         The resource pool provides methods for asynchonous loading. In this case the resource is
         not returned immediately instead being returned through a callback provided when calling
         load.
         
             resourcePool->LoadResourceAsync<CSRendering::Texture>(CSCore::StorageLocation::k_package, "Texture.png", [](const CSRendering::TextureCSPtr& in_texture)
             {
                //Do something.
             });
         
         Resources can also be created manually using the CreateResource() method. In this case
         the resource is still pooled but it must be given a unique identifier and a mutable 
         instances is returned. This can be queried from the pool using GetResource().
         
             CSRendering::TextureSPtr mutableTexture = resourcePool->CreateResource("MyTexture");
             CSRendering::TextureCSPtr texture = resourcePool->GetResource("MyTexture");
         
         Resources that are created manually must also be built manually (for instance texture has a build method that takes in image data, format, etc).
         
         If the application receives a memory warning the Resource Pool will release any resources
         that are no-longer in use. This can also be performed manually by calling ReleaseAllUnused().
         
         -----------------------------
         
         Next: 'Materials' in GameEntityFactory::CreateBall()
         */
        
        auto resourcePool = CSCore::Application::Get()->GetResourcePool();
        CSRendering::MeshCSPtr mesh = resourcePool->LoadResource<CSRendering::Mesh>(CSCore::StorageLocation::k_package, "Models/Ball.csmodel");
        
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
         
            CSRendering::MaterialFactory materialFactory = CSCore::Application::Get()->GetSystem<MaterialFactory>();
            CSRendering::MaterialSPtr spriteMaterial = materialFactory->CreateSprite("UniqueId", texture);
         
         Objects with the same material can be rendered in the same batch and therefore it is important to reduce the number of materials by batching textures.
         
         -----------------------------
         
         Next: 'Texture Atlases' in GameEntityFactory::CreateScoreSprite()
         */
        
        CSRendering::MaterialCSPtr material = resourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/Models/Models.csmaterial");
        
        CSCore::EntityUPtr ball(CSCore::Entity::Create());
        
        auto renderFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
        CSRendering::StaticMeshComponentSPtr meshComponent = renderFactory->CreateStaticMeshComponent(mesh, material);
        ball->AddComponent(meshComponent);
        
        CSCore::Vector2 collisionSize = mesh->GetAABB().GetSize().XY();
        DynamicBodyComponentSPtr dynamicBody(new DynamicBodyComponent(m_physicsSystem, collisionSize, 1.0f, 0.0f, 1.0f));
        ball->AddComponent(dynamicBody);
        
        BallControllerComponentSPtr ballControllerComponent(new BallControllerComponent(dynamicBody.get()));
        ball->AddComponent(ballControllerComponent);
        
        m_scoringSystem->AddBallBody(dynamicBody);
        
        return ball;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntityUPtr GameEntityFactory::CreatePlayerPaddle(const CSCore::EntitySPtr& in_camera) const
    {
        CSCore::EntityUPtr paddle(CSCore::Entity::Create());
        
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
        
        return paddle;
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    CSCore::EntityUPtr GameEntityFactory::CreateOppositionPaddle(const CSCore::EntitySPtr& in_ball) const
    {
        CSCore::EntityUPtr paddle(CSCore::Entity::Create());
        
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
        /*
         ===================================
         Chilli Source Tour: Texture Atlases
         ===================================
         
         Texture Atlases (also called Spritesheets) provide a 'virtual' texture lookup within an actual texture. This
         allows multiple objects to share the same physical texture but render with different UVs and sizes, ultimately allowing them to share a material.
         
         Just like other resources a texture altas is loaded from the resource manager. Once loaded the 'virtual' texture (or sprite frame)
         can be accessed by querying the atlas using a 'Texture Id'. For example:
         
            CSRendering::TextureAltasCSPtr atlas = resourcePool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "Atlas.csatlas");
            CSRendering::UVs virtualTextureUVs = atlas->GetFrameUVs("Frame1");
         
         Typically when working with an atlas the component using it would deal with the virtual texture. For example
         When creating a SpriteComponent through the Render Component Factory the atlas and the virtual texture id are
         passed in.
         
         -----------------------------------
         
         This concludes the Chilli Source Tour!
         */
        
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