#include "World.h"
#include "Label.h"
#include "EnemyAircraftController.h"
#include "ProjectileController.h"
#include "PlayerAircraftController.h"
#include "BackgroundController.h"
#include "GuiController.h"
#include "CloudsController.h"
#include "ExplosionController.h"
#include "MediaFiles.h"
#include "SpawnEnemyAircraftSystem.h"
#include "ProjectileSpawnSystem.h"
#include "ProjectileCollisionSystem.h"
#include "RemoveOffScreenEnemiesSystem.h"
#include "RemoveOffScreenProjectilesSystem.h"
#include "ExplosionAnimationSystem.h"
#include "PlayerKilledSystem.h"
#include "CloudMovementSystem.h"
#include "BackgroundMovementSystem.h"
#include "EnemyAircraftMovementSystem.h"
#include "PlayerAircraftMovementSystem.h"
#include "ProjectileMovementSystem.h"
#include "SoundEffects.h"

namespace
{
    constexpr float mScrollSpeed {-50.f};
}

World::World(sf::RenderWindow& window, const FontHolder& font, const std::function<void()>& endGameCallback)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mEndGameCallback(endGameCallback)
, mFonts(font)
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
{
    loadTextures();
    initLogic();

    mWorldView.setCenter(mSpawnPosition);
}

void World::draw()
{
    mWindow.setView(mWorldView);

    drawEntities(mBackgroundEntitySystem);
    drawEntities(mCloudEntitySystem);
    drawEntities(mExplosionEntitySystem);
    drawEntities(mProjectileEntitySystem);
    drawEntities(mEnemyAircraftEntitySystem);
    drawEntities(mPlayerAircraftEntitySystem);
    drawEntities(mLabelEntitySystem);
}

template <typename T>
void World::drawEntities(EntitySystem<T>& system)
{
    for (const auto* entity : system.getEntities())
    {
        mWindow.draw(*entity);
    }
}

void World::loadTextures()
{
    mTextures.load(Textures::Eagle, MediaFiles::Eagle);
    mTextures.load(Textures::Raptor, MediaFiles::Raptor);
    mTextures.load(Textures::Background, MediaFiles::Background);
    mTextures.load(Textures::Bullet, MediaFiles::Bullet);
    mTextures.load(Textures::EnemyBullet, MediaFiles::EnemyBullet);
    mTextures.load(Textures::Clouds, MediaFiles::Clouds);
    mTextures.load(Textures::Explosion, MediaFiles::Explosion);
    mTextures.load(Textures::PlayerExplosion, MediaFiles::PlayerExplosion);
}

void World::initLogic()
{
    mExplosionController = std::make_unique<ExplosionController>(
        mExplosionEntitySystem,
        mTextures
    );

    mProjectileController = std::make_unique<ProjectileController>(
        mProjectileEntitySystem,
        mTextures,
        mWorldBounds
    );

    mPlayerAircraftController = std::make_unique<PlayerAircraftController>(
       mPlayerAircraftEntitySystem
   );
    mPlayerAircraftController->create(mTextures, mSpawnPosition);

    mPlayerAircraftMovementSystem = std::make_unique<PlayerAircraftMovementSystem>(
    *mPlayerAircraftController->getPlayerAircaft(),
       mWorldView.getCenter(),
       mWorldView.getSize(),
       mScrollSpeed
    );

    mEnemyAircraftMovementSystem = std::make_unique<EnemyAircraftMovementSystem>(
        mEnemyAircraftEntitySystem,
        *mPlayerAircraftController->getPlayerAircaft(),
        mScrollSpeed
    );

    mProjectileMovementSystem = std::make_unique<ProjectileMovementSystem>(
        mProjectileEntitySystem
    );

    mScoreController = std::make_unique<GuiController>(
        mLabelEntitySystem,
        *mPlayerAircraftController->getPlayerAircaft(),
        mWorldBounds.width
    );
    mScoreController->create(mFonts);

    mProjectileCollisionSystem = std::make_unique<ProjectileCollisionSystem>(
        mProjectileEntitySystem,
        mEnemyAircraftEntitySystem,
        *mPlayerAircraftController->getPlayerAircaft(),
        *mExplosionController,
        *mScoreController
    );

    mSpawnEnemyAircraftSystem = std::make_unique<SpawnEnemyAircraftSystem>(
        mEnemyAircraftEntitySystem,
        mTextures,
        mWorldBounds.width
    );

    mEnemyProjectileSpawnSystem = std::make_unique<ProjectileSpawnSystem>(
        mEnemyAircraftEntitySystem,
        *mProjectileController
    );

    mPlayerProjectileSpawnSystem = std::make_unique<ProjectileSpawnSystem>(
        mPlayerAircraftEntitySystem,
        *mProjectileController
    );

    mRemoveOffScreenEnemiesSystem = std::make_unique<RemoveOffScreenEnemiesSystem>(
        mEnemyAircraftEntitySystem,
        mWorldBounds.height
    );

    mRemoveOffScreenProjectilesSystem = std::make_unique<RemoveOffScreenProjectilesSystem>(
        mProjectileEntitySystem,
        mWorldBounds.height
    );

    mExplosionAnimationSystem = std::make_unique<ExplosionAnimationSystem>(
        mExplosionEntitySystem
    );

    mEnemyAircraftController = std::make_unique<EnemyAircraftController>(
        mEnemyAircraftEntitySystem
    );

    mBackgroundController = std::make_unique<BackgroundController>(
        mBackgroundEntitySystem,
        mTextures,
        mWindow.getSize(),
        mScrollSpeed
    );
    mBackgroundController->create();

    mCloudsController = std::make_unique<CloudsController> (
        mCloudEntitySystem,
        mTextures,
        mScrollSpeed
    );
    mCloudsController->create();

    mCloudMovementSystem = std::make_unique<CloudMovementSystem> (
        mCloudEntitySystem
    );

    mBackgroundMovementSystem = std::make_unique<BackgroundMovementSystem> (
        mBackgroundEntitySystem
    );

    simpleControls.initializeActions(
        *mPlayerAircraftController->getPlayerAircaft()
    );

    mPlayerKilledSystem = std::make_unique<PlayerKilledSystem> (
        *mPlayerAircraftController->getPlayerAircaft(),
        mEndGameCallback
    );

    mSoundEffects = std::make_unique<SoundEffects>();
}

void World::update(const sf::Time delta)
{
    mProjectileCollisionSystem->execute();
    mSpawnEnemyAircraftSystem->execute(delta);
    mEnemyProjectileSpawnSystem->execute(delta);
    mPlayerProjectileSpawnSystem->execute(delta);
    mRemoveOffScreenEnemiesSystem->execute();
    mRemoveOffScreenProjectilesSystem->execute();
    mExplosionAnimationSystem->execute(delta);
    mCloudMovementSystem->execute(delta);
    mBackgroundMovementSystem->execute(delta);
    mPlayerAircraftMovementSystem->execute();
    mEnemyAircraftMovementSystem->execute(delta);
    mProjectileMovementSystem->execute(delta);

    simpleControls.handleRealtimeInput();

    mPlayerAircraftEntitySystem.update(delta);
    mProjectileEntitySystem.update(delta);
    mEnemyAircraftEntitySystem.update(delta);
    mLabelEntitySystem.update(delta);
    mExplosionEntitySystem.update(delta);

    // this needs to run last as it will tear this world object down
    mPlayerKilledSystem->execute();
}

World::~World() = default;