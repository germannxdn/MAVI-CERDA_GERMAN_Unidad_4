#pragma once
#include <box2d.h>
#include "raylib.h"
#include "ContactListener.h"

class PhysicsWorld {
private:
    b2World world;
    b2Body* groundBody;
    b2Body* targetBody;
    b2Body* projectileBody;
    MyContactListener contactListener;
    bool projectileLaunched;

public:
    PhysicsWorld();
    ~PhysicsWorld();

    void Update();
    void Draw();

    void LaunchProjectile(float x, float y, float forceX, float forceY);
    void ResetScene();
    bool HasProjectileHitTarget() const;
    bool IsProjectileLaunched() const { return projectileLaunched; }
    b2Vec2 GetProjectilePosition() const;
};