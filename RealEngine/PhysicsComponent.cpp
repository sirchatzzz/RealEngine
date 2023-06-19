#include "PhysicsComponent.h"
PhysicsComponent::PhysicsComponent(Component* parent_) : Component(parent_), pos(Vec3(0.0f, 0.0f, 0.0f)), vel(Vec3(0.0f, 0.0f, 0.0f)), accel(Vec3(0.0f, 0.0f, 0.0f)), mass(1.0f), radius(0.0f), isCreated(false) {}

PhysicsComponent::~PhysicsComponent() {}

void PhysicsComponent::Update(float deltaTime) {
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;
	vel += accel * deltaTime;
}

bool PhysicsComponent::OnCreate() {
	if (isCreated) return true;
	return true;
}

void PhysicsComponent::OnDestroy(){ /*just making it concrete*/ }

void PhysicsComponent::ApplyForce(Vec3 force) {
	accel = force / mass;
}