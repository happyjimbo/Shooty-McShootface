#include "PlayerAircraftController.h"
#include "AircraftEntity.h"
#include "PlayerData.h"

using Aircraft::AircraftEntity;

PlayerAircraftController::PlayerAircraftController(
    EntitySystem<AircraftEntity>& entitySystem,
    const TextureHolder& textures,
    ShaderHolder& shaders,
    const PlayerData& playerData,
    const sf::Vector2f spawnPosition
) noexcept
{
    mPlayerAircraft = entitySystem.createObject(AircraftEntity::Eagle, textures, shaders, playerData.Health);
    mPlayerAircraft->setPosition(spawnPosition);
}

AircraftEntity* PlayerAircraftController::getPlayerAircaft() const
{
    return mPlayerAircraft;
}
