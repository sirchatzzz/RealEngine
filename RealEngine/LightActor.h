#pragma once
#include "Actor.h"
class LightActor : public Actor {
	LightActor(const LightActor&) = delete;
	LightActor(LightActor&&) = delete;
	LightActor& operator = (const LightActor&) = delete;
	LightActor& operator = (LightActor&) = delete;
public:
	LightActor(Component* parent_, const Vec3 &pos_);
	void OnDestroy() override;
	bool OnCreate() override;
	Vec3 getPosition() { return pos; }
private:
	Vec3 pos;
	Vec4 color;
	float intensity;
	Vec3 falOff;
};

