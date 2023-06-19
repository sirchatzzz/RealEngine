#include "TransformComponent.h"

using namespace MATH;

TransformComponent::TransformComponent(Component* parent_, const Vec3& pos_, const Quaternion& orientation_, const Vec3& scale_):
	Component(parent_), pos(pos_), orientation(orientation_), scale(scale_) {
}

TransformComponent::~TransformComponent() {
	OnDestroy();
}

bool TransformComponent::OnCreate() {
	if (isCreated) return true;
	return true;
}

void TransformComponent::OnDestroy() {}

void TransformComponent::Update(const float deltaTime){}

Matrix4 TransformComponent::GetTransformMatrix() const
{
	return MMath::translate(pos) * MMath::scale(scale) * MMath::toMatrix4(orientation);
}
