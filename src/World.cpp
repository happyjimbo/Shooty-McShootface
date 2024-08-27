#include "World.h"
#include <Controllers/EnemyAircraftController.h>
#include <Controllers/ProjectileController.h>
#include <Controllers/PlayerAircraftController.h>
#include <Controllers/ProjectileCollisionController.h>
#include <Controllers/BackgroundController.h>
#include <Controllers/ScoreController.h>
#include <Gui/Label.h>

#include "CloudsController.h"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
{
    loadTextures();
    loadFonts();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

void World::draw()
{
    mWindow.setView(mWorldView);

    drawEntities(mSpriteEntitySystem);
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

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::loadTextures()
{
    // move these string values into a struct or similar
    mTextures.load(Textures::Eagle, MediaFiles::Eagle);
    mTextures.load(Textures::Raptor, MediaFiles::Raptor);
    mTextures.load(Textures::Background, MediaFiles::Background);
    mTextures.load(Textures::Bullet, MediaFiles::Bullet);
    mTextures.load(Textures::Clouds, MediaFiles::Clouds);
}

void World::loadFonts()
{
    mFonts.load(Fonts::Main, MediaFiles::Font);
}

void World::buildScene()
{
    // this is for the mEnemyAircraftController and should be internalised in that logic
    const auto startPosition = sf::Vector2f (mWorldBounds.width, (mSpawnPosition.y - mWorldView.getSize().y / 2)-100);

    mScoreController = new ScoreController(
        mLabelEntitySystem
    );
    mScoreController->create(mFonts);

    mProjectileController = new ProjectileController(
        mProjectileEntitySystem,
        mTextures,
        mWorldBounds
    );
    mEnemyAircraftController = new EnemyAircraftController(
        mEnemyAircraftEntitySystem,
        *mProjectileController,
        mTextures,
        AircraftEntity::Type::Raptor,
        startPosition,
        mWorldBounds,
        mScrollSpeed
    );
    mProjectileCollisionController = new ProjectileCollisionController(
        mProjectileEntitySystem,
        mEnemyAircraftEntitySystem,
        mPlayerAircraftEntitySystem,
        *mScoreController
    );

    mBackgroundController = new BackgroundController(
        mSpriteEntitySystem,
        mTextures,
        mWindow.getSize(),
        mScrollSpeed
    );
    mBackgroundController->create();

    mCloudsController = new CloudsController (
        mSpriteEntitySystem,
        mTextures,
        mScrollSpeed
    );
    mCloudsController->create();

    mPlayerAircraftController = new PlayerAircraftController(
        mPlayerAircraftEntitySystem,
        *mProjectileController,
        mCommandQueue,
        mWorldView.getCenter(),
        mWorldView.getSize(),
        mScrollSpeed
    );
    mPlayerAircraftController->create(mTextures, mSpawnPosition);
}

void World::update(const sf::Time delta)
{
    mPlayerAircraftController->tick(delta);
    mProjectileController->tick(delta, mScrollSpeed);
    mEnemyAircraftController->tick(delta);
    mProjectileCollisionController->tick(delta);
    mBackgroundController->tick(delta);
    mCloudsController->tick(delta);

    mProjectileEntitySystem.update(delta);
    mPlayerAircraftEntitySystem.update(delta);
    mEnemyAircraftEntitySystem.update(delta);
    mLabelEntitySystem.update(delta);
}

World::~World()
{
    delete mPlayerAircraftController;
    delete mScoreController;
    delete mBackgroundController;
    delete mCloudsController;
    delete mEnemyAircraftController;
    delete mProjectileController;
    delete mProjectileCollisionController;
}