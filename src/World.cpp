#include "World.h"

#include "Entity/SpriteEntity.h"
#include "EnemyAircraftController.h"
#include "ProjectileController.h"
#include "ProjectileCollisionController.h"
#include "BackgroundController.h"
#include "ScoreController.h"
#include <Gui/Label.h>

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft()
, mProjectileController()
, mEnemyAircraftController()
, mProjectileCollisionController()
, mBackgroundController()
{
    loadTextures();
    loadFonts();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

void World::draw() const
{
    mWindow.setView(mWorldView);

    for(const auto* entity : mSpriteEntitySystem.getEntities()) {
        mWindow.draw(*entity);
    }

    for(const auto* entity : mProjectileEntitySystem.getEntities()) {
        mWindow.draw(*entity);
    }

    for(const auto* entity : mEnemyAircraftEntitySystem.getEntities()) {
        mWindow.draw(*entity);
    }

    for(const auto* entity : mPlayerAircraftEntitySystem.getEntities()) {
        mWindow.draw(*entity);
    }

    for(const auto* entity : mLabelEntitySystem.getEntities()) {
        mWindow.draw(*entity);
    }

}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::loadTextures()
{
    mTextures.load(Textures::Eagle, "Media/Textures/Eagle.png");
    mTextures.load(Textures::Raptor, "Media/Textures/Raptor.png");
    mTextures.load(Textures::Desert, "Media/Textures/Desert.png");
    mTextures.load(Textures::Bullet, "Media/Textures/Bullet.png");
}

void World::loadFonts()
{
    mFonts.load(Fonts::Main, "Media/Sansation.ttf");
}

void World::buildScene()
{
    const auto scoreTextLable = mLabelEntitySystem.createObject("Score:", mFonts);
    scoreTextLable->setPosition(10, 10);

    const auto scoreAmountLabel = mLabelEntitySystem.createObject("0", mFonts);
    scoreAmountLabel->setPosition(60, 10);

    const auto startPosition = sf::Vector2f (mWorldBounds.width, (mSpawnPosition.y - mWorldView.getSize().y / 2)-100);

    mScoreController = new ScoreController(*scoreAmountLabel);
    mProjectileController = new ProjectileController(mProjectileEntitySystem, mTextures, mWorldBounds);
    mEnemyAircraftController = new EnemyAircraftController(mEnemyAircraftEntitySystem, *mProjectileController, mTextures, AircraftEntity::Type::Raptor, startPosition, mWorldBounds);
    mProjectileCollisionController = new ProjectileCollisionController(mProjectileEntitySystem, mEnemyAircraftEntitySystem, mPlayerAircraftEntitySystem, *mScoreController);

    mBackgroundController = new BackgroundController(mSpriteEntitySystem, mTextures, mWindow.getSize(), mScrollSpeed);
    mBackgroundController->createBackground();

    mPlayerAircraft = mPlayerAircraftEntitySystem.createObject(*mProjectileController, AircraftEntity::Eagle, mTextures);
    mPlayerAircraft->setPosition(mSpawnPosition);
}

void World::update(const sf::Time delta)
{
    // mWorldView.move(0.f, mScrollSpeed * delta.asSeconds());
    mPlayerAircraft->setVelocity(0.f, 0.f);

    while (!mCommandQueue.isEmpty())
    {
        mPlayerAircraftEntitySystem.onCommand(mCommandQueue.pop(), delta);
    }

    adaptPlayerVelocity();
    mProjectileController->tick(delta, mScrollSpeed);
    mEnemyAircraftController->tick(delta, mScrollSpeed);
    mProjectileCollisionController->tick(delta);
    mBackgroundController->tick(delta);

    mProjectileEntitySystem.update(delta);
    mPlayerAircraftEntitySystem.update(delta);
    mEnemyAircraftEntitySystem.update(delta);
    mLabelEntitySystem.update(delta);

    adaptPlayerPosition();
}

void World::adaptPlayerPosition() const
{
    const sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
    constexpr float borderDistance = 40.f;

    auto position = mPlayerAircraft->getPosition();
    position.x = std::max(position.x, viewBounds.left + borderDistance);
    position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
    position.y = std::max(position.y, viewBounds.top + borderDistance);
    position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
    mPlayerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity() const
{
    sf::Vector2f velocity = mPlayerAircraft->getVelocity();

    if (velocity.x != 0.f && velocity.y != 0.f)
        mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));

    mPlayerAircraft->accelerate(0.f, mScrollSpeed/2);
}

World::~World()
{
    delete mScoreController;
    delete mPlayerAircraft;
    delete mBackgroundSprite;
    delete mEnemyAircraftController;
    delete mProjectileController;
    delete mProjectileCollisionController;
    delete mBackgroundController;
}