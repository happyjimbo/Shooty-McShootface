#include "Bullet.h"

class BulletController : public SceneNode {

    public:
                                                    BulletController(const TextureHolder& textures);
        void                                        spawnBullet(Bullet::Type type);
        void                                        tick(sf::Vector2f position, float speed);
        virtual unsigned int	                    getCategory() const;

    private:
        void                                        accelerateBullets(float speed);

    private:
        std::vector<Bullet*> mBullets;
        const TextureHolder& mTextures;
        sf::Vector2f        mSpawnPosition;
};