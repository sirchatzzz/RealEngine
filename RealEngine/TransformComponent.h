#pragma once
#include "Component.h"
#include <Quaternion.h>
#include <MMath.h>
#include <Vector.h>

using namespace MATH;

class TransformComponent : public Component {
public:
	TransformComponent(Component* parent_, const Vec3& pos_,const Quaternion& orientation_, const Vec3& scale_);
	~TransformComponent();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime);
	void SetTransform(Vec3 pos_, Quaternion orientation_, Vec3 scale_) { pos = pos_; orientation = orientation_; scale = scale_; }
private:
	Vec3 pos;
	Quaternion orientation;
	Vec3 scale;
public:
	Vec3 GetPosition() { return pos; }
	Quaternion GetOrientation() { return orientation; }
	Vec3 GetScale() { return scale; }
	Matrix4 GetTransformMatrix() const;
	void SetScale(Vec3 scale_) { scale = scale_; }
	void SetPosition(Vec3 pos_) { pos = pos_; }
	void SetOrientation(Quaternion orientation_) { orientation = orientation_; }

};

