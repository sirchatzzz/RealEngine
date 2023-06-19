#ifndef SCENE0_H
#define SCENE0_H
#include "Scene.h"
#include <Vector.h>
#include <Matrix.h>
#include "LightActor.h"
#include "Actor.h"
#include "Component.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "CameraActor.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "AssetManager.h"
#include "ReadXML.h"
using namespace MATH;

class Scene0 : public Scene {
private:
	Ref<AssetManager> assetManager;
	Ref<Actor> checkerBoard;
	std::vector<Ref<Actor>> redCheckers;
	std::vector<Ref<Actor>> whiteCheckers;
	Ref<CameraActor>camera;
	Ref<LightActor>light;
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


#endif // SCENE0_H