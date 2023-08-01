#include <glew.h>
#include <iostream>
#include <QMath.h>
#include <MMath.h>
#include "Scene0.h"
#include "Debug.h"

Scene0::Scene0(){
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
	glGetIntegerv(GL_VIEWPORT, viewport);
}

Scene0::~Scene0() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene0::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	XML.ReadConfig();

	assetManager = std::make_shared<AssetManager>();

	skybox = assetManager->GetComponent<SkyboxActor>("SB_NightSky");

	//camera
	camera = assetManager->GetComponent<CameraActor>("Camera");
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -10.0f), Quaternion(), Vec3(1.0f, 1.0f, 1.0f));
	camera->UpdateViewMatrix();
	camera->OnCreate();

	//light
	light = assetManager->GetComponent<LightActor>("L_Default");
	light->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), Quaternion(), Vec3(1.0f, 1.0f, 1.0f));

	float xPos = -3.2f;
	float yPos = 4.5f;

	//red checkers
	for (int i = 0; i < 12; ++i) {

		if (i == 4) {
			xPos = -4.4f;
			yPos = 3.2f;
		}
		if (i == 8) {
			xPos = -3.2f;
			yPos = 1.9f;
		}

		auto redCheckerPiece = std::make_shared<Actor>(nullptr);

		redCheckerPiece->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("SM_CheckerPiece"));
		redCheckerPiece->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("M_RedChecker"));
		redCheckerPiece->AddComponent<TransformComponent>(nullptr, Vec3(xPos, yPos, -20.0f), Quaternion(), Vec3(0.15f, 0.15f, 0.1f));
		redCheckerPiece->OnCreate();

		xPos += 2.5f;

		redCheckers.push_back(redCheckerPiece);
	}

	return true;
}

void Scene0::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	static Vec2 currentMousePos;
	static Vec2 lastMousePos;
	unsigned int objID = -1;
	//camera->HandleEvents(sdlEvent);
	switch( sdlEvent.type ) {
    case SDL_KEYDOWN:
		break;

	case SDL_MOUSEMOTION:
		break;

	case SDL_MOUSEBUTTONDOWN:
		break;

	case SDL_MOUSEBUTTONUP:   
		break;

	default:
		break;
    }
}

void Scene0::Update(const float deltaTime) {
	static float time = 0.0f;
	time += deltaTime / 2.0f; 
	//checkerBoard->GetComponent<TransformComponent>()->SetOrientation(QMath::angleAxisRotation(20.0f * cos(time), Vec3(0.0f, 1.0f, 1.0f)));
}

int Scene0::Pick(int x, int y) {
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); /// Paint the backgound white which is 0x00FFFFFF
	glClear(GL_COLOR_BUFFER_BIT);
	//glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("S_ColorPicker");
	glUseProgram(shader->GetProgram());

	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetRotationMatrix());

	for (GLuint i = 0; i < redCheckers.size(); i++) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, redCheckers[i]->getModelMatrix());
		glUniform1ui(shader->GetUniformID("colorID"), i);
		redCheckers[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	glUseProgram(0);

	GLuint colorIndex;
	glReadPixels(x, viewport.height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &colorIndex);
	colorIndex &= 0x00FFFFFF; /// This zeros out the alpha component
	printf("%x", colorIndex);
	if (colorIndex == 0x00FFFFFF) return -1; /// Picked nothing
	else return colorIndex;
}

void Scene0::Render() const {
	/// Set the background color then clear the screen

	Ref<ShaderComponent> shaderComponent = assetManager->GetComponent<ShaderComponent>("S_Phong");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUseProgram(skybox->GetShader()->GetProgram());
	glUniformMatrix4fv(skybox->GetShader()->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(skybox->GetShader()->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetRotationMatrix());
	skybox->Render();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glUseProgram(shaderComponent->GetProgram());
	glUniformMatrix4fv(shaderComponent->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(shaderComponent->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetRotationMatrix());
	glUniform3fv(shaderComponent->GetUniformID("lightPos"), 1, light->GetComponent<TransformComponent>()->GetPosition());

	for (unsigned int i = 0; i < redCheckers.size(); ++i) {
		glBindTexture(GL_TEXTURE_2D, redCheckers[i]->GetComponent<MaterialComponent>()->getTextureID());
		glUniformMatrix4fv(shaderComponent->GetUniformID("modelMatrix"), 1, GL_FALSE, redCheckers[i]->getModelMatrix());
		redCheckers[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	glUseProgram(0);
}