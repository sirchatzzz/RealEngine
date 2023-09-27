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
#include "SaveSystem.h"
#include "imgui_impl_opengl3.h"
#include "tinyxml2.h"

using namespace MATH;

class Scene0 : public Scene 
{
private:
	std::vector<Ref<Actor>> sceneActors;

	Ref<Actor> cube1;
	Ref<Actor> cube2;

	Ref<AssetManager> assetManager;
	Ref<CameraActor>camera;
	Ref<LightActor>light;
	Ref<SkyboxActor> skybox;
	const char* currentSkybox;
	Ref<Actor> plane, sphere;
	ViewPort viewport;
	SaveSystem saveSystem;

	GLuint shadowMapFBO;
	GLuint shadowMap;

	//GUI stuff
	Vec3 lightPosition;
	Vec4 backgroundColor;
	Vec4 lightColor;
	Vec4 cameraOrientationVector;
	const char* materialName;
	const char* meshName;
	bool objectPicker;
	bool openGUI;
	bool canRotate;
	bool objectSelected;
	unsigned int selectedObject;
	int meshesCount;

	//save system
	tinyxml2::XMLDocument XML;
	tinyxml2::XMLElement* rootData;
	tinyxml2::XMLElement* assetsData;

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
	void LoadSaveFile();
	void Save();
};