#pragma once
#include <box2d.h>

class MyContactListener : public b2ContactListener {
public:
    bool projectileHitTarget = false;

    void BeginContact(b2Contact* contact) override {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        // Identificar por tipo de cuerpo y forma
        bool isDynamicA = (bodyA->GetType() == b2_dynamicBody);
        bool isDynamicB = (bodyB->GetType() == b2_dynamicBody);
        bool isStaticA = (bodyA->GetType() == b2_staticBody);
        bool isStaticB = (bodyB->GetType() == b2_staticBody);

        bool isCircleA = (fixtureA->GetShape()->GetType() == b2Shape::e_circle);
        bool isCircleB = (fixtureB->GetShape()->GetType() == b2Shape::e_circle);
        bool isPolygonA = (fixtureA->GetShape()->GetType() == b2Shape::e_polygon);
        bool isPolygonB = (fixtureB->GetShape()->GetType() == b2Shape::e_polygon);

        // Proyectil = dinámico + círculo
        bool isProjectileA = isDynamicA && isCircleA;
        bool isProjectileB = isDynamicB && isCircleB;

        // Objetivo = estático + polígono
        bool isTargetA = isStaticA && isPolygonA;
        bool isTargetB = isStaticB && isPolygonB;

        // Si proyectil toca objetivo
        if ((isProjectileA && isTargetB) || (isProjectileB && isTargetA)) {
            projectileHitTarget = true;
        }
    }

    void EndContact(b2Contact* contact) override {}
};