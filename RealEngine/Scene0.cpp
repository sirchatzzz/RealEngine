#include <glew.h>
#include <iostream>
#include <QMath.h>
#include <MMath.h>
#include "Scene0.h"
#include "Debug.h"

Scene0::Scene0(){
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene0::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	XML.ReadConfig();

	assetManager = std::make_shared<AssetManager>();

	skybox = assetManager->GetComponent<SkyboxActor>("SB_CNTower");

	//camera
	camera = assetManager->GetComponent<CameraActor>("Camera");
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -10.0f), Quaternion(), Vec3(1.0f, 1.0f, 1.0f));
	camera->UpdateViewMatrix();
	camera->OnCreate();

	//light
	light = assetManager->GetComponent<LightActor>("L_Default");
	light->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), Quaternion(), Vec3(1.0f, 1.0f, 1.0f));

	//checker board
	checkerBoard = std::make_shared<Actor>(nullptr);
	checkerBoard->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("SM_Plane"));
	checkerBoard->AddComponent<ShaderComponent>(assetManager->GetComponent<ShaderComponent>("S_Phong"));
	checkerBoard->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("M_CheckerBoard"));
	checkerBoard->AddComponent<CameraActor>(nullptr);
	checkerBoard->AddComponent<TransformComponent>(nullptr, XML.GetBoardPosition(), XML.GetBoardOrientation(), XML.GetBoardScale());
	checkerBoard->AddComponent<AudioComponent>(assetManager->GetComponent<AudioComponent>("SE_Ding"));
	checkerBoard->OnCreate();

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
		redCheckerPiece->AddComponent<TransformComponent>(nullptr, Vec3(xPos, yPos, 0.0f), Quaternion(), Vec3(0.15f, 0.15f, 0.1f));
		redCheckerPiece->OnCreate();

		xPos += 2.5f;

		redCheckers.push_back(redCheckerPiece);
	}

	xPos = -4.4f;
	yPos = -4.3f;

	//white checkers
	for (int i = 0; i < 12; ++i) {

		if (i == 4) {
			xPos = -3.2f;
			yPos = -3.0f;
		}
		if (i == 8) {
			xPos = -4.4f;
			yPos = -1.8f;
		}

		auto whiteCheckerPiece = std::make_shared<Actor>(nullptr);

		whiteCheckerPiece->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("SM_CheckerPiece"));
		whiteCheckerPiece->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("M_WhiteChecker"));
		whiteCheckerPiece->AddComponent<TransformComponent>(nullptr, Vec3(xPos, yPos, 0.0f), Quaternion(), Vec3(0.15f, 0.15f, 0.1f));
		whiteCheckerPiece->OnCreate();

		xPos += 2.5f;

		whiteCheckers.push_back(whiteCheckerPiece);
	}

	return true;
}

void Scene0::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	camera->HandleEvents(sdlEvent);
	switch( sdlEvent.type ) {
    case SDL_KEYDOWN:
		if (SDL_SCANCODE_W) {
			printf("Sound \n");
			Mix_PlayChannel(-1, assetManager->GetComponent<AudioComponent>("SE_Ding")->GetSoundEffect(), 0);
		}
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
	checkerBoard->GetComponent<TransformComponent>()->SetOrientation(QMath::angleAxisRotation(20.0f * cos(time), Vec3(0.0f, 1.0f, 1.0f)));
}
void Scene0::Render() const {
	/// Set the background color then clear the screen

	Ref<ShaderComponent> shaderComponent = checkerBoard->GetComponent<ShaderComponent>();

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
	glBindTexture(GL_TEXTURE_2D, checkerBoard->GetComponent<MaterialComponent>()->getTextureID());
	glUniformMatrix4fv(shaderComponent->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(shaderComponent->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetRotationMatrix());
	glUniform3fv(shaderComponent->GetUniformID("lightPos"), 1, light->GetComponent<TransformComponent>()->GetPosition());

	glUniformMatrix4fv(shaderComponent->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerBoard->getModelMatrix());
	checkerBoard->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	for (unsigned int i = 0; i < redCheckers.size(); ++i) {
		glBindTexture(GL_TEXTURE_2D, redCheckers[i]->GetComponent<MaterialComponent>()->getTextureID());
		glUniformMatrix4fv(shaderComponent->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerBoard->getModelMatrix() * redCheckers[i]->getModelMatrix());
		redCheckers[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	for (unsigned int i = 0; i < whiteCheckers.size(); ++i) {
		glBindTexture(GL_TEXTURE_2D, whiteCheckers[i]->GetComponent<MaterialComponent>()->getTextureID());
		glUniformMatrix4fv(shaderComponent->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerBoard->getModelMatrix() * whiteCheckers[i]->getModelMatrix());
		whiteCheckers[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	glUseProgram(0);
}



	
