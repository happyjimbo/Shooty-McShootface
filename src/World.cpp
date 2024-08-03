#include "World.h"

#include <iostream>

#include "SpriteNode.h"
#include "EnemyAircraftController.h"
#include "ProjectileCollisionController.h"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
, mSceneGraph()
, mSceneLayer()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
{
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

void World::draw() const
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
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

void World::buildScene()
{
    for (std::size_t i = 0; i < LayerCount; i++)
    {
        SceneNode::Ptr layer(std::make_unique<SceneNode>());
        mSceneLayer[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    // Prepare tiled background
    sf::Texture& texture = mTextures.get(Textures::Desert);
    sf::IntRect textureRect(mWorldBounds);
    texture.setRepeated(true);

    std::shared_ptr<SpriteNode> backgroundSprite(std::make_shared<SpriteNode>(texture, textureRect));
    backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
    mSceneLayer[Background]->attachChild(backgroundSprite);

    std::shared_ptr<Aircraft> leader(std::make_shared<Aircraft>(Aircraft::Eagle, mTextures));
    mPlayerAircraft = leader.get();
    mPlayerAircraft->setPosition(mSpawnPosition);
    mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
    mSceneLayer[Air]->attachChild(leader);

    auto startPosition = sf::Vector2f (mWorldBounds.width, mWorldBounds.top);

    mEnemyAircraftController = std::make_shared<EnemyAircraftController>(mTextures, Aircraft::Type::Raptor, startPosition, mWorldBounds);
    mSceneLayer[Air]->attachChild(mEnemyAircraftController);

    mProjectileController = std::make_shared<ProjectileController>(mTextures, mWorldBounds);
    mSceneLayer[Air]->attachChild(mProjectileController);

    mProjectileCollisionController =
        std::make_unique<ProjectileCollisionController>(mProjectileController, mEnemyAircraftController);
}

void World::update(sf::Time delta)
{
    mWorldView.move(0.f, mScrollSpeed * delta.asSeconds());
    mPlayerAircraft->setVelocity(0.f, 0.f);

    while (!mCommandQueue.isEmpty())
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), delta);
    }

    adaptPlayerVelocity();
    mProjectileController->tick(delta, mPlayerAircraft->getPosition(), mScrollSpeed);
    mEnemyAircraftController->tick(delta, mScrollSpeed);
    mProjectileCollisionController->tick(delta);

    mSceneGraph.update(delta);
    adaptPlayerPosition();
}

void World::adaptPlayerPosition() const
{
    sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
    const float borderDistance = 40.f;

    sf::Vector2f position = mPlayerAircraft->getPosition();
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

    mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}