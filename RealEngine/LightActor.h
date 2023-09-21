#pragma once
#include "Actor.h"
class LightActor : public Actor {
	LightActor(const LightActor&) = delete;
	LightActor(LightActor&&) = delete;
	LightActor& operator = (const LightActor&) = delete;
	LightActor& operator = (LightActor&) = delete;
public:
	LightActor(Component* parent_);
	void OnDestroy() override;
	bool OnCreate() override;
	Vec3 GetPosition() { return position; }
	void UpdatePosition(Vec3 pos_);
	Matrix4 GetLightSpaceMatrix() { return lightSpaceMatrix; }
private:
	bool isCreated;
	Vec3 position;
	Matrix4 lightSpaceMatrix;
};

