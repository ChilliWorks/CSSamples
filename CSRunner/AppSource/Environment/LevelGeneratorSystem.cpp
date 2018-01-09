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

#include <Environment/LevelGeneratorSystem.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Sprite.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Texture.h>

#include <GameConfig.h>

#include <array>

namespace CSRunner
{
    CS_DEFINE_NAMEDTYPE(LevelGeneratorSystem);
    
    namespace
    {
        const std::array<std::string, 3> k_potentialObstacleIdsTop =
        {{
            "Balloons_Light",
            "Chandelier_Light",
            "Pinata_Light"
        }};
        
        const std::array<std::string, 5> k_potentialObstacleIdsBottom =
        {{
            "CakeStand_Light",
            "Champagne_Light",
            "Chicken_Light",
            "Flower_Light",
            "Plates_Light"
        }};
        
        /// Randomly picks an index based on the probability at each index
        ///
        u32 WeightedRandom(const std::vector<f32> probabilities) noexcept
        {
            f32 summedWeights = 0;
            
            for(f32 prob : probabilities)
            {
                summedWeights += prob;
            }
            
            f32 randomWeighting = CS::Random::Generate(0.0f, summedWeights);
            
            for(u32 i=0; i<(u32)probabilities.size(); ++i)
            {
                if(randomWeighting < probabilities[i])
                {
                    return i;
                }
                
                randomWeighting -= probabilities[i];
            }
            
            return 0;
        }
    }
    
    //------------------------------------------------------------
    LevelGeneratorSystemUPtr LevelGeneratorSystem::Create() noexcept
    {
        return LevelGeneratorSystemUPtr(new LevelGeneratorSystem());
    }
    
    //------------------------------------------------------------
    bool LevelGeneratorSystem::IsA(CS::InterfaceIDType interfaceId) const noexcept
    {
        return interfaceId == LevelGeneratorSystem::InterfaceID;
    }
    
    //------------------------------------------------------------
    void LevelGeneratorSystem::OnInit() noexcept
    {
        auto resourcePool = CS::Application::Get()->GetResourcePool();
        
        auto obstaclesTexture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_package, "TextureAtlases/Obstacles/Obstacles.csimage");
        auto obstaclesAtlas = resourcePool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "TextureAtlases/Obstacles/Obstacles.csatlas");
        
        auto effectsTexture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_package, "TextureAtlases/Effects/Effects.csimage");
        auto effectsAtlas = resourcePool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "TextureAtlases/Effects/Effects.csatlas");
        
        auto environmentTexture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_package, "TextureAtlases/Level/Level.csimage");
        auto environmentAtlas = resourcePool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "TextureAtlases/Level/Level.csatlas");
        
        auto materialFactory = CS::Application::Get()->GetSystem<CS::MaterialFactory>();
        
        auto obstaclesMaterial = resourcePool->GetResource<CS::Material>("Obstacles");
        if(obstaclesMaterial == nullptr)
        {
            obstaclesMaterial = materialFactory->CreateUnlit("Obstacles", obstaclesTexture, true);
        }
        
        auto effectsMaterial = resourcePool->GetResource<CS::Material>("Effects");
        if(effectsMaterial == nullptr)
        {
            effectsMaterial = materialFactory->CreateUnlit("Effects", effectsTexture, true);
        }
        
        auto environmentMaterial = resourcePool->GetResource<CS::Material>("Environment");
        if(environmentMaterial == nullptr)
        {
            environmentMaterial = materialFactory->CreateUnlit("Environment", environmentTexture, true);
        }
        
        //Sprites are aligned to the right so that we know they are fully offscreen when their origin as at the screen edge
        //Obstacles
        m_inactiveObstacles.reserve(GameConfig::k_maxObstacles);
        m_activeObstacles.reserve(GameConfig::k_maxObstacles);
        for(u32 i=0; i<GameConfig::k_maxObstacles; ++i)
        {
            CS::EntitySPtr entity = CS::Entity::Create();
            auto sprite = std::make_shared<CS::SpriteComponent>(obstaclesMaterial, obstaclesAtlas, k_potentialObstacleIdsBottom[0], CS::Vector2::k_one, CS::SizePolicy::k_usePreferredSize);
            sprite->SetOriginAlignment(CS::AlignmentAnchor::k_middleRight);
            entity->AddComponent(sprite);
            m_inactiveObstacles.push_back(entity);
        }
        
        //Effects
        m_explosionEffect = CS::Entity::Create();
        auto explosionSprite = std::make_shared<CS::SpriteComponent>(effectsMaterial, effectsAtlas, "Explosion_1", CS::Vector2::k_one, CS::SizePolicy::k_usePreferredSize);
        m_explosionEffect->AddComponent(explosionSprite);
        m_explosionAnimation = KeyframeAnimationUPtr(new KeyframeAnimation(explosionSprite.get(), {"Explosion_1","Explosion_2","Explosion_2"}, 15.0f));
        
        //BGs
        std::vector<std::string> bgAtlasIds = {"Background_1_Light","Background_2_Light"};
        auto bgSize = environmentAtlas->GetOriginalFrameSize(bgAtlasIds[0]);
        m_bgSizeX = bgSize.x;
        u32 numBGTiles = (u32)std::ceil(GameConfig::k_levelWidth/m_bgSizeX) + 1;
        m_bgTiles.reserve(numBGTiles);
        f32 xPos = GameConfig::k_cullX;
        
        for(u32 i=0; i<numBGTiles; ++i)
        {
            CS::EntitySPtr entity = CS::Entity::Create();
            auto sprite = std::make_shared<CS::SpriteComponent>(environmentMaterial, environmentAtlas, bgAtlasIds[i%bgAtlasIds.size()], CS::Vector2::k_one, CS::SizePolicy::k_usePreferredSize);
            sprite->SetOriginAlignment(CS::AlignmentAnchor::k_middleRight);
            entity->AddComponent(sprite);
            GetState()->GetMainScene()->Add(entity);
            entity->GetTransform().SetPosition(xPos, 0.0f, 2.0f);
            m_bgTiles.push_back(entity);
            xPos += m_bgSizeX;
        }
        
        //FGs
        m_fgSizeX = environmentAtlas->GetOriginalFrameSize("Table_Light").x;
        u32 numFGTiles = (u32)std::ceil(GameConfig::k_levelWidth/m_fgSizeX) + 1;
        m_groundTiles.reserve(numFGTiles);
        m_ceilingTiles.reserve(numFGTiles);
        xPos = GameConfig::k_cullX;
        
        for(u32 i=0; i<numFGTiles; ++i)
        {
            {
                CS::EntitySPtr entity = CS::Entity::Create();
                auto sprite = std::make_shared<CS::SpriteComponent>(environmentMaterial, environmentAtlas, "Table_Light", CS::Vector2::k_one, CS::SizePolicy::k_usePreferredSize);
                sprite->SetOriginAlignment(CS::AlignmentAnchor::k_topRight);
                entity->AddComponent(sprite);
                GetState()->GetMainScene()->Add(entity);
                entity->GetTransform().SetPosition(xPos, GameConfig::k_groundY + GameConfig::k_playerHeight * 0.1f, 1.0f);
                m_groundTiles.push_back(entity);
            }
            {
                CS::EntitySPtr entity = CS::Entity::Create();
                auto sprite = std::make_shared<CS::SpriteComponent>(environmentMaterial, environmentAtlas, "Table_Light", CS::Vector2::k_one, CS::SizePolicy::k_usePreferredSize);
                sprite->SetOriginAlignment(CS::AlignmentAnchor::k_bottomRight);
                sprite->SetColour(0.25f, 0.25f, 0.25f, 1.0f);
                sprite->SetFlippedVertically(true);
                entity->AddComponent(sprite);
                GetState()->GetMainScene()->Add(entity);
                entity->GetTransform().SetPosition(xPos, GameConfig::k_ceilingY, -1.0f);
                m_ceilingTiles.push_back(entity);
            }
            
            xPos += m_fgSizeX;
        }
    }
    
    //------------------------------------------------------------
    void LevelGeneratorSystem::OnUpdate(f32 timeSinceLastUpdate) noexcept
    {
        //Move all obstacles horizontally
        f32 scrollDist = -GameConfig::k_scrollSpeed * timeSinceLastUpdate;
        for(std::size_t i=0; i<m_activeObstacles.size(); ++i)
        {
            m_activeObstacles[i]->GetTransform().MoveBy(scrollDist, 0.0f, 0.0f);
            m_obstacleCollisionBoxes[i].vOrigin.x += scrollDist;
        }
        
        for(auto& bg : m_bgTiles)
        {
            bg->GetTransform().MoveBy(scrollDist, 0.0f, 0.0f);
        }
        
        for(auto& ground : m_groundTiles)
        {
            ground->GetTransform().MoveBy(scrollDist, 0.0f, 0.0f);
        }
        
        for(auto& ceiling : m_ceilingTiles)
        {
            ceiling->GetTransform().MoveBy(scrollDist, 0.0f, 0.0f);
        }
        
        //If the left most object exist the left hand side bound then make it disappear
        if(m_activeObstacles.size() > 0 && m_activeObstacles.front()->GetTransform().GetLocalPosition().x < GameConfig::k_cullX)
        {
            m_inactiveObstacles.push_back(m_activeObstacles.front());
            m_activeObstacles.front()->RemoveFromParent();
            m_activeObstacles.erase(m_activeObstacles.begin());
            m_obstacleCollisionBoxes.erase(m_obstacleCollisionBoxes.begin());
        }
        
        if(m_bgTiles.front()->GetTransform().GetLocalPosition().x < GameConfig::k_cullX)
        {
            auto tile = m_bgTiles.front();
            m_bgTiles.erase(m_bgTiles.begin());
            m_bgTiles.push_back(tile);
            m_bgTiles.back()->GetTransform().MoveBy(m_bgTiles.size() * m_bgSizeX, 0.0f, 0.0f);
        }
        
        if(m_groundTiles.front()->GetTransform().GetLocalPosition().x < GameConfig::k_cullX)
        {
            auto tile = m_groundTiles.front();
            m_groundTiles.erase(m_groundTiles.begin());
            m_groundTiles.push_back(tile);
            m_groundTiles.back()->GetTransform().MoveBy(m_groundTiles.size() * m_fgSizeX, 0.0f, 0.0f);
        }
        
        if(m_ceilingTiles.front()->GetTransform().GetLocalPosition().x < GameConfig::k_cullX)
        {
            auto tile = m_ceilingTiles.front();
            m_ceilingTiles.erase(m_ceilingTiles.begin());
            m_ceilingTiles.push_back(tile);
            m_ceilingTiles.back()->GetTransform().MoveBy(m_ceilingTiles.size() * m_fgSizeX, 0.0f, 0.0f);
        }
        
        //If we have some obstacles in the pool and there is a great enough distance to the last object then we can spawn a new obstacle
        if(m_activeObstacles.size() < GameConfig::k_maxObstacles && m_inactiveObstacles.size() > 0)
        {
            f32 minSpawnX = GameConfig::k_levelWidth * 1.2f;
            f32 lastPos = m_activeObstacles.size() > 0 ? m_activeObstacles.back()->GetTransform().GetLocalPosition().x : 0.0f;
            f32 distance = minSpawnX - lastPos;
            
            if(distance >= GameConfig::k_minDistanceBetweenObstacles)
            {
                auto spriteComponent = m_inactiveObstacles.back()->GetComponent<CS::SpriteComponent>();
                
                //Decide whether to spawn a low or high object and then randomly pick and object
                u32 index = WeightedRandom({GameConfig::k_lowObstacleProbability, GameConfig::k_highObstacleProbability});
                f32 spawnY = 0.0f;
                if(index == 0)
                {
                    spriteComponent->SetTextureAtlasId(k_potentialObstacleIdsBottom[CS::Random::Generate((std::size_t)0, k_potentialObstacleIdsBottom.size()-1)]);
                    spawnY = GameConfig::k_groundY + GameConfig::k_lowObstacleSpawnOffset + spriteComponent->GetSize().y * 0.5f;
                }
                else
                {
                    spriteComponent->SetTextureAtlasId(k_potentialObstacleIdsTop[CS::Random::Generate((std::size_t)0, k_potentialObstacleIdsTop.size()-1)]);
                    spawnY = GameConfig::k_groundY + GameConfig::k_highObstacleSpawnOffset + spriteComponent->GetSize().y * 0.5f;
                }
                
                CS::Vector3 spawnPos(minSpawnX + CS::Random::Generate(GameConfig::k_minDistanceBetweenObstacles, GameConfig::k_maxDistanceBetweenObstacles), spawnY, 0.0f);
                
                m_activeObstacles.push_back(m_inactiveObstacles.back());
                GetState()->GetMainScene()->Add(m_inactiveObstacles.back());
                m_inactiveObstacles.back()->GetTransform().SetPosition(spawnPos);
                m_obstacleCollisionBoxes.push_back(CS::Rectangle(spawnPos.XY() + CS::Vector2(-spriteComponent->GetSize().x * 0.5f, 0.0f), spriteComponent->GetSize()));
                
                m_inactiveObstacles.pop_back();
            }
        }
        
        //Update explosion animation
        if(m_explosionEffect->GetScene() != nullptr)
        {
            bool finished = m_explosionAnimation->Update(timeSinceLastUpdate, KeyframeAnimation::UpdateMode::k_once);
            if(finished == true)
            {
                m_explosionEffect->RemoveFromParent();
            }
        }
    }
    
    //------------------------------------------------------------
    void LevelGeneratorSystem::DestroyObstacle(u32 index) noexcept
    {
        if(m_explosionEffect->GetScene() == nullptr)
        {
            GetState()->GetMainScene()->Add(m_explosionEffect);
        }
        m_explosionAnimation->Reset();
        m_explosionEffect->GetTransform().SetPosition(m_activeObstacles[index]->GetTransform().GetLocalPosition());
        
        m_activeObstacles[index]->RemoveFromParent();
        m_inactiveObstacles.push_back(m_activeObstacles[index]);
        m_activeObstacles.erase(m_activeObstacles.begin() + index);
        m_obstacleCollisionBoxes.erase(m_obstacleCollisionBoxes.begin() + index);
    }
}
