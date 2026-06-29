#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
    : world(b2Vec2(0.0f, 9.8f)), projectileLaunched(false) {

    world.SetContactListener(&contactListener);

    int screenWidth = 1000;
    int screenHeight = 600;

    // ============================================
    // SUELO
    // ============================================
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(screenWidth / 2.0f, screenHeight - 40.0f);
    groundBody = world.CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(screenWidth / 2.0f, 20.0f);
    groundBody->CreateFixture(&groundShape, 0.0f);

    // ============================================
    // OBJETIVO
    // ============================================
    b2BodyDef targetDef;
    targetDef.type = b2_staticBody;
    targetDef.position.Set(800.0f, screenHeight - 140.0f);
    targetBody = world.CreateBody(&targetDef);

    b2PolygonShape targetShape;
    targetShape.SetAsBox(40.0f, 40.0f);

    b2FixtureDef targetFixture;
    targetFixture.shape = &targetShape;
    targetFixture.density = 0.0f;
    targetFixture.friction = 0.5f;
    targetFixture.restitution = 0.3f;

    targetBody->CreateFixture(&targetFixture);

    // ============================================
    // PROYECTIL
    // ============================================
    b2BodyDef projectileDef;
    projectileDef.type = b2_dynamicBody;
    projectileDef.position.Set(-100.0f, -100.0f);
    projectileDef.bullet = true;
    projectileBody = world.CreateBody(&projectileDef);

    b2CircleShape projectileShape;
    projectileShape.m_radius = 15.0f;

    b2FixtureDef projectileFixture;
    projectileFixture.shape = &projectileShape;
    projectileFixture.density = 0.5f;
    projectileFixture.friction = 0.3f;
    projectileFixture.restitution = 0.5f;

    projectileBody->CreateFixture(&projectileFixture);

}

PhysicsWorld::~PhysicsWorld() {}

void PhysicsWorld::Update() {
    world.Step(1.0f / 60.0f, 8, 3);
}

void PhysicsWorld::Draw() {
    int screenWidth = 1000;
    int screenHeight = 600;

    // Suelo visual
    DrawRectangle(0, screenHeight - 60, screenWidth, 40, Fade(DARKGREEN, 0.6f));

    // OBJETIVO
    b2Vec2 targetPos = targetBody->GetPosition();
    DrawRectangle(targetPos.x - 40, targetPos.y - 40, 80, 80, ORANGE);
    Rectangle targetRect = { targetPos.x - 40, targetPos.y - 40, 80, 80 };
    DrawRectangleLinesEx(targetRect, 3, DARKBROWN);
    DrawText("OBJETIVO", targetPos.x - 35, targetPos.y - 8, 16, DARKBROWN);

    // PROYECTIL (solo si fue lanzado)
    if (projectileLaunched) {
        b2Vec2 projPos = projectileBody->GetPosition();
        DrawCircleV({ projPos.x, projPos.y }, 15.0f, RED);
        DrawCircleLines((int)projPos.x, (int)projPos.y, 15.0f, MAROON);
    }
}

void PhysicsWorld::LaunchProjectile(float x, float y, float forceX, float forceY) {
    projectileBody->SetTransform(b2Vec2(x, y), 0);
    projectileBody->SetLinearVelocity(b2Vec2(forceX, forceY));
    projectileLaunched = true;
    contactListener.projectileHitTarget = false;
}

void PhysicsWorld::ResetScene() {
    projectileBody->SetTransform(b2Vec2(-100.0f, -100.0f), 0);
    projectileBody->SetLinearVelocity(b2Vec2(0, 0));
    projectileLaunched = false;
    contactListener.projectileHitTarget = false;
}

bool PhysicsWorld::HasProjectileHitTarget() const {
    return contactListener.projectileHitTarget;
}

b2Vec2 PhysicsWorld::GetProjectilePosition() const {
    return projectileBody->GetPosition();
}