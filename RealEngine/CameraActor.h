#pragma once
#include <Matrix.h>
#include "Actor.h"
#include "Trackball.h"
using namespace MATH;

class CameraActor : public Actor {
	CameraActor(const CameraActor&) = delete;
	CameraActor(CameraActor&&) = delete;
	CameraActor& operator= (const CameraActor&) = delete;
	CameraActor& operator=(CameraActor&&) = delete;
private:
	Unq<Trackball> trackball;

	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 rotationMatrix;
	Matrix4 translationMatrix;
	Matrix4 skyboxMatrix;
	unsigned int uboMatriciesID;
	unsigned int bindingPoint;
	bool isCreated;
public:
	CameraActor(Component* parent_);
	~CameraActor();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	void HandleEvents(const SDL_Event& sdlEvent);
	virtual void Render() const override;
	Matrix4 GetProjectionMatrix() const { return projectionMatrix; }
	Matrix4 GetRotationMatrix() const { return rotationMatrix; }
	Matrix4 GetViewMatrix() const { return viewMatrix; }
	Matrix4 GetSkyboxMatrix() const { return skyboxMatrix; }
	void SetViewMatrix(Matrix4 view) { viewMatrix = view; }
	void SetRotationMatrix(Matrix4 rotation) { rotationMatrix = rotation; }
	unsigned int GetMatriciesID() const { return uboMatriciesID; }
	void UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far);
	void UpdateViewMatrix();
};
