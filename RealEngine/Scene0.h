#pragma once
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
#include "Core.h"

using namespace MATH;

class Scene0 : public Scene {
private:
	std::vector<Ref<Actor>> redCheckers;

	Ref<AssetManager> assetManager;
	Ref<CameraActor>camera;
	Ref<LightActor>light;
	Ref<SkyboxActor> skybox;
	ReadXML XML;
	ViewPort viewport;

public:
	explicit Scene0();
	virtual ~Scene0();

	int Pick(int x, int y);
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};