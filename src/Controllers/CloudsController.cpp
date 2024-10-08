#include "CloudsController.h"
#include "ResourceHolder.h"

namespace
{
    const std::vector<float> scrollSpeedOffsets = {0.3f, 0.6f, 0.9f, 1.2f};
}

CloudsController::CloudsController (EntitySystem<CloudEntity>& entitySystem, TextureHolder& textures, ShaderHolder& shaders, const float scrollSpeed)
{
    const auto& cloudTexture = textures.get(Textures::Clouds);
    const auto rect = sf::IntRect(0, 0, cloudTexture.getSize().x, cloudTexture.getSize().y);

    for (std::size_t i = 0; i < scrollSpeedOffsets.size(); i++)
    {
        CloudEntity* cloudSprite = entitySystem.createObject(cloudTexture, textures, shaders, rect, scrollSpeed);

        CloudData& cloudData = cloudSprite->getCloudData();
        cloudData.scrollSpeedOffset = scrollSpeedOffsets[i];

        const float yPos = static_cast<float>(-rect.getSize().y * i);
        cloudSprite->setPosition(cloudData.generateRandomXpos(), (yPos * scrollSpeedOffsets[i]));
    }
}
