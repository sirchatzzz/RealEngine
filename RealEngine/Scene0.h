#pragma once
#include <Vector.h>
#include <Matrix.h>
#include "Scene.h"
#include "Actor.h"
#include "Component.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "AssetManager.h"
#include "ReadXML.h"

using namespace MATH;

class Scene0 : public Scene {
private:
	std::vector<Ref<Actor>> redCheckers;
	std::vector<Ref<Actor>> whiteCheckers;

	Ref<AssetManager> assetManager;
	Ref<Actor> checkerBoard;
	Ref<CameraActor>camera;
	Ref<LightActor>light;
	Ref<SkyboxActor> skybox;
	ReadXML XML;

public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};