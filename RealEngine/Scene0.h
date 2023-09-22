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
#include "Core.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "tinyxml2.h"

using namespace MATH;

class Scene0 : public Scene {
private:

	std::vector<Ref<Actor>> sceneMeshes;
	std::vector<Vec3> meshesPosition;
	std::vector<Vec3> meshesNewPosition;

	Ref<AssetManager> assetManager;
	Ref<CameraActor>camera;
	Ref<LightActor>light;
	Ref<SkyboxActor> skybox;
	const char* currentSkybox;
	Ref<Actor> plane, sphere;
	ViewPort viewport;

	GLuint shadowMapFBO;
	GLuint shadowMap;

	//GUI stuff
	Vec3 lightPosition;
	Vec3 cameraPosition;
	Vec3 position;
	Vec4 cameraOrientationVector;
	Vec4 backgroundColor;
	Vec4 lightColor;
	Quaternion cameraOrientation;
	bool openGUI;
	bool canRotate;

	//save system
	XMLDocument XML;
	XMLElement* rootData;
	XMLElement* assetsData;

public:
	explicit Scene0();
	virtual ~Scene0();

	int Pick(int x, int y);
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
	void UpdateSkybox(const char* name);
	void CreateBuffer();
	void RenderShadowMap();
	void HandleGUI();
	void LoadXML();
	void ReadXML();
	void WriteXML();
};