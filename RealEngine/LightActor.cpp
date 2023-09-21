#include "LightActor.h"

LightActor::LightActor(Component* parent_) : Actor(parent_), isCreated(false), position(Vec3(1.0f,1.0f,1.0f))
{
	//
}

void LightActor::OnDestroy() 
{
	//
}

bool LightActor::OnCreate() 
{

	if (isCreated) return true;
	for (const auto& c : components) {
		if (!c->OnCreate()) return isCreated = false;
	}
	isCreated = true;
	return isCreated;
}

void LightActor::UpdatePosition(Vec3 pos_)
{
	position = pos_;

	if (position <= Vec3(0.0f, 0.0f, 0.0f)) printf("Position cannot be < than 0");

	Matrix4 projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	Matrix4 lightTranslation = MMath::lookAt(position, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f));
	lightSpaceMatrix = projectionMatrix * lightTranslation;

}
