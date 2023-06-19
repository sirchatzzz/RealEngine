#pragma once
#include <Vector.h>
#include <Matrix.h>
#include <MMath.h>
#include "Component.h"

using namespace MATH;

class PhysicsComponent : public Component {
public:
	PhysicsComponent(Component* parent_);
	~PhysicsComponent();
private: // Physics stuff
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	float mass;
	float radius;
	bool isCreated;
public:
	void Update(float deltaTime);
	bool OnCreate() override;
	void OnDestroy() override;
	void ApplyForce(Vec3 force);

	Vec3 GetAccel() const { return accel; }
	Vec3 GetPos() const { return pos; }
	Vec3 GetVel() const { return vel; }
	float GetRadius() const { return radius; }

	void SetAccel(Vec3& accel_) { accel = accel_; }
	void SetPos(Vec3& pos_) { pos = pos_; }
	void SetVel(Vec3& vel_) { vel = vel_; }
	void SetRadius(float& radius_) { radius = radius_; }
};