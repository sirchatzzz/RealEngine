#include "LightActor.h"

LightActor::LightActor(Component* parent_, const Vec3& pos_) : Actor(parent_) {
	pos = pos_;
}

void LightActor::OnDestroy() {
	//
}

bool LightActor::OnCreate() {
	return true;
}
