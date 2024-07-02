#ifndef CMAKESFMLPROJECT_SCENENODE_H
#define CMAKESFMLPROJECT_SCENENODE_H

#include <SFML/Graphics.hpp>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> Ptr;

public:
                    SceneNode();
    void            attachChild(Ptr);
    Ptr             detachChild(const SceneNode&);

    void            update(sf::Time);

    sf::Vector2f    getWorldPosition() const;
    sf::Transform   getWorldTransform() const;

private:
    virtual void    updateCurrent(sf::Time dt);
    void            updateChildren(sf::Time dt);

    virtual void    draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void    drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::vector<Ptr> mChildren;
    SceneNode* mParent;
};

#endif //CMAKESFMLPROJECT_SCENENODE_H