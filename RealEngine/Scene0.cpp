#include <glew.h>
#include <iostream>
#include <QMath.h>
#include <MMath.h>
#include "Scene0.h"
#include "Debug.h"

Scene0::Scene0() : shadowMap(0), shadowMapFBO(0), lightColor(Vec3(1.0f, 1.0f, 1.0f)),
backgroundColor(Vec4(0.0f, 0.0f, 0.0f, 0.0f)), currentSkybox(nullptr), openGUI(false), canRotate(false), assetsData(nullptr), rootData(nullptr),
objectSelected(false), selectedObject(0), objectPicker(false), meshesCount(-1)
{

	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
	glGetIntegerv(GL_VIEWPORT, viewport);

}

Scene0::~Scene0() 
{
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene0::OnCreate() 
{
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);

	CreateBuffer();

	assetManager = std::make_shared<AssetManager>();

	UpdateSkybox("SB_Beach");

	//camera
	camera = assetManager->GetComponent<CameraActor>("Camera");
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -5.0f), Quaternion(), Vec3(1.0f, 1.0f, 1.0f));
	camera->OnCreate();

	//light
	light = assetManager->GetComponent<LightActor>("L_Point");
	light->UpdatePosition(Vec3(-10.0f, 4.0f, 6.0f));
	lightPosition = light->GetPosition();

	cameraOrientationVector = Vec4(0.0f, 0.0f, 0.0f, 1.0f);

	for(auto c : sceneActors)
	{
		if(!sceneActors.empty()) c->OnCreate();
	}

	LoadSaveFile();

	return true;
}

void Scene0::OnDestroy() 
{
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
}

void Scene0::HandleEvents(const SDL_Event& sdlEvent)
{

	if (canRotate) camera->HandleEvents(sdlEvent);

	static Vec2 currentMousePos;
	static Vec2 lastMousePos;
	unsigned int objID = -1;
	switch (sdlEvent.type) 
	{
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_G:
			openGUI = !openGUI;
			break;
		case SDL_SCANCODE_LCTRL:
			Save();
			break;
		case SDL_SCANCODE_E:
			camera->GetComponent<TransformComponent>()->SetPosition(Vec3(camera->GetComponent<TransformComponent>()->GetPosition().x, camera->GetComponent<TransformComponent>()->GetPosition().y, camera->GetComponent<TransformComponent>()->GetPosition().z + 0.2f));
			break;
		case SDL_SCANCODE_C:
			camera->GetComponent<TransformComponent>()->SetPosition(Vec3(camera->GetComponent<TransformComponent>()->GetPosition().x, camera->GetComponent<TransformComponent>()->GetPosition().y, camera->GetComponent<TransformComponent>()->GetPosition().z - 0.2f));
			break;
		case SDL_SCANCODE_A:
			camera->GetComponent<TransformComponent>()->SetPosition(Vec3(camera->GetComponent<TransformComponent>()->GetPosition().x + 0.2f, camera->GetComponent<TransformComponent>()->GetPosition().y, camera->GetComponent<TransformComponent>()->GetPosition().z));
			break;
		case SDL_SCANCODE_D:
			camera->GetComponent<TransformComponent>()->SetPosition(Vec3(camera->GetComponent<TransformComponent>()->GetPosition().x - 0.2f, camera->GetComponent<TransformComponent>()->GetPosition().y, camera->GetComponent<TransformComponent>()->GetPosition().z));
			break;
		case SDL_SCANCODE_W:
			camera->GetComponent<TransformComponent>()->SetPosition(Vec3(camera->GetComponent<TransformComponent>()->GetPosition().x, camera->GetComponent<TransformComponent>()->GetPosition().y - 0.2f, camera->GetComponent<TransformComponent>()->GetPosition().z));
			break;
		case SDL_SCANCODE_S:
			camera->GetComponent<TransformComponent>()->SetPosition(Vec3(camera->GetComponent<TransformComponent>()->GetPosition().x, camera->GetComponent<TransformComponent>()->GetPosition().y + 0.2f, camera->GetComponent<TransformComponent>()->GetPosition().z));
			break;
		}
		break;

	case SDL_MOUSEMOTION:
		break;

	case SDL_MOUSEBUTTONDOWN:
		if(objectPicker)
		{
			currentMousePos = Vec2(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y));
			lastMousePos = currentMousePos;

			objID = Pick(sdlEvent.button.x, sdlEvent.button.y);
			printf("0x%X %d\n", objID, objID);
			selectedObject = objID;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		break;

	default:
		break;
	}

}

void Scene0::HandleGUI()
{
	objectPicker = !ImGui::IsAnyItemHovered();

	ImGui::Begin("Frame rate", &openGUI, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
	ImGui::Text("%.1f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin("Settings");

	//camera position
	Vec3 cameraPos = camera->GetComponent<TransformComponent>()->GetPosition();
	ImGui::SliderFloat3("Camera Position", cameraPos, -20.0f, 20.0f);
	camera->GetComponent<TransformComponent>()->SetPosition(cameraPos);

	//camera orientation

	ImGui::SliderFloat4("Camera Rotation", cameraOrientationVector, -1.0f, 1.0f);

	//light position
	ImGui::SliderFloat3("Light Position", lightPosition, -20.0f, 20.0f);

	//light color
	ImGui::ColorEdit4("Light Color", lightColor);

	//bg color
	ImGui::ColorEdit4("Background Color", backgroundColor);

	//skyBoxes
	if (ImGui::Button("Beach")) UpdateSkybox("SB_Beach");
	ImGui::SameLine();
	if (ImGui::Button("Sunset")) UpdateSkybox("SB_Sunset");
	ImGui::SameLine();
	if (ImGui::Button("Niagara")) UpdateSkybox("SB_Niagara");
	ImGui::SameLine();
	if (ImGui::Button("Snow")) UpdateSkybox("SB_Snow");
	ImGui::SameLine();
	if (ImGui::Button("None")) skybox = nullptr;
	if (ImGui::Button("Reset", ImVec2(306.0, 30.0)))
	{
		camera->GetComponent<TransformComponent>()->SetPosition(Vec3(0.0f, 0.0f, -5.0f));
		cameraOrientationVector = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		light->UpdatePosition(Vec3(-10.0f, 4.0f, 6.0f));
		lightColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
		backgroundColor = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
		skybox = assetManager->GetComponent<SkyboxActor>("SB_Beach");
		lightPosition = Vec3(-10.0f, 4.0f, 6.0f);
		for(int i = 0; i < sceneActors.size(); ++i)
		{
			sceneActors[i]->RemoveAllComponents();
			sceneActors.clear();
			meshesCount = -1;
		}
		saveSystem.ClearXML();
		Save();
	}
	ImGui::End();

	//meshes
	ImGui::Begin("Meshes");
	if (ImGui::Button("SM_Cube")) 
	{
		++meshesCount;
		meshName = "SM_Cube";
		Ref<Actor> actor = std::make_shared<Actor>(nullptr);
		actor->AddComponent(assetManager->GetComponent<MeshComponent>(meshName));
		actor->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -5.0f), Quaternion(), Vec3(1.0f, 1.0f, 1.0f));
		actor->AddComponent(assetManager->GetComponent<MaterialComponent>("M_CheckerBoard"));
		sceneActors.push_back(actor);
		saveSystem.SaveChar(("StaticMesh" + std::to_string(meshesCount)).c_str(), meshName);
	}
	ImGui::SameLine();
	if (ImGui::Button("SM_Sphere"))
	{
		++meshesCount;
		meshName = "SM_Sphere";
		Ref<Actor> actor = std::make_shared<Actor>(nullptr);
		actor->AddComponent(assetManager->GetComponent<MeshComponent>(meshName));
		actor->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -5.0f), Quaternion(), Vec3(1.0f, 1.0f, 1.0f));
		actor->AddComponent(assetManager->GetComponent<MaterialComponent>("M_CheckerBoard"));
		sceneActors.push_back(actor);
		saveSystem.SaveChar(("StaticMesh" + std::to_string(meshesCount)).c_str(), meshName);
	}
	ImGui::End();

	//meshes settings
	if (objectSelected) 
	{
		ImGui::Begin("Mesh Settings");

		//position
		if(sceneActors.size() > selectedObject)
		{
			Vec3 position = sceneActors[selectedObject]->GetComponent<TransformComponent>()->GetPosition();
			ImGui::SliderFloat3("Position", position, -20.0f, 20.0f);
			if (selectedObject != -1) sceneActors[selectedObject]->GetComponent<TransformComponent>()->SetPosition(position);
		}

		//rotation
		if (sceneActors.size() > selectedObject)
		{
			Vec3 scale = sceneActors[selectedObject]->GetComponent<TransformComponent>()->GetScale();
			ImGui::SliderFloat3("Scale", scale, -20.0f, 20.0f);
			if (selectedObject != -1) sceneActors[selectedObject]->GetComponent<TransformComponent>()->SetScale(scale);
		}

		//scale
		if (sceneActors.size() > selectedObject)
		{
			Vec4 orient = Vec4(sceneActors[selectedObject]->GetComponent<TransformComponent>()->GetOrientation().ijk.x,
				sceneActors[selectedObject]->GetComponent<TransformComponent>()->GetOrientation().ijk.y,
				sceneActors[selectedObject]->GetComponent<TransformComponent>()->GetOrientation().ijk.z,
				sceneActors[selectedObject]->GetComponent<TransformComponent>()->GetOrientation().w);
			ImGui::InputFloat4("Orientation", orient);
			if (selectedObject != -1) sceneActors[selectedObject]->GetComponent<TransformComponent>()->SetOrientation(Quaternion(orient.w, Vec3(orient.x, orient.y, orient.z)));
		}
		if (ImGui::Button("Delete Object"))
		{
			saveSystem.DeleteMesh(("StaticMesh" + std::to_string(selectedObject)).c_str());
			sceneActors[selectedObject]->RemoveAllComponents();
			if (selectedObject < sceneActors.size()) sceneActors.erase(sceneActors.begin() + selectedObject);
			--meshesCount;
			saveSystem.SaveInt("MeshesCount", meshesCount);
		}
		ImGui::End();

		//material settings
		ImGui::Begin("Materials");
		if(ImGui::Button("M_CheckerBoard"))
		{
			materialName = "M_CheckerBoard";
			sceneActors[selectedObject]->RemoveComponent<MaterialComponent>();
			sceneActors[selectedObject]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>(materialName));
			saveSystem.SaveChar(("Material" + std::to_string(selectedObject)).c_str(), materialName);
		}
		ImGui::SameLine();
		if (ImGui::Button("M_EvilEye")) 
		{
			materialName = "M_EvilEye";
			sceneActors[selectedObject]->RemoveComponent<MaterialComponent>();
			sceneActors[selectedObject]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>(materialName));
			saveSystem.SaveChar(("Material" + std::to_string(selectedObject)).c_str(), materialName);
		}
		ImGui::SameLine();
		if (ImGui::Button("M_WhiteChecker"))
		{
			materialName = "M_WhiteChecker";
			sceneActors[selectedObject]->RemoveComponent<MaterialComponent>();
			sceneActors[selectedObject]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>(materialName));
			saveSystem.SaveChar(("Material" + std::to_string(selectedObject)).c_str(), materialName);
		}
		if (ImGui::Button("M_Wood"))
		{
			materialName = "M_Wood";
			sceneActors[selectedObject]->RemoveComponent<MaterialComponent>();
			sceneActors[selectedObject]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>(materialName));
			saveSystem.SaveChar(("Material" + std::to_string(selectedObject)).c_str(), materialName);
		}
		ImGui::SameLine();
		if (ImGui::Button("M_ChippedPaint"))
		{
			materialName = "M_ChippedPaint";
			sceneActors[selectedObject]->RemoveComponent<MaterialComponent>();
			sceneActors[selectedObject]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>(materialName));
			saveSystem.SaveChar(("Material" + std::to_string(selectedObject)).c_str(), materialName);
		}
		ImGui::SameLine();
		if (ImGui::Button("M_SciFi"))
		{
			materialName = "M_SciFi";
			sceneActors[selectedObject]->RemoveComponent<MaterialComponent>();
			sceneActors[selectedObject]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>(materialName));
			saveSystem.SaveChar(("Material" + std::to_string(selectedObject)).c_str(), materialName);
		}
		if (ImGui::Button("M_Concrete"))
		{
			materialName = "M_Concrete";
			sceneActors[selectedObject]->RemoveComponent<MaterialComponent>();
			sceneActors[selectedObject]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>(materialName));
			saveSystem.SaveChar(("Material" + std::to_string(selectedObject)).c_str(), materialName);
		}
		ImGui::SameLine();
		if (ImGui::Button("M_Pavement"))
		{
			materialName = "M_Pavement";
			sceneActors[selectedObject]->RemoveComponent<MaterialComponent>();
			sceneActors[selectedObject]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>(materialName));
			saveSystem.SaveChar(("Material" + std::to_string(selectedObject)).c_str(), materialName);
		}
		ImGui::End();
	}

	light->UpdatePosition(lightPosition);
}

void Scene0::Update(const float deltaTime)
{
	static float saveTime = 0.0f;
	saveTime += deltaTime;

	camera->UpdateViewMatrix();

	RenderShadowMap();
	if(openGUI)	HandleGUI();
	camera->GetComponent<TransformComponent>()->SetOrientation(Quaternion(cameraOrientationVector.w, Vec3(cameraOrientationVector.x, cameraOrientationVector.y, cameraOrientationVector.z)));

}

void Scene0::Render() const
{

	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("S_ShadowPhong");

	glViewport(0, 0, 1280, 720);
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (skybox != nullptr) 
	{

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glUseProgram(skybox->GetShader()->GetProgram());
		glUniformMatrix4fv(skybox->GetShader()->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
		glUniformMatrix4fv(skybox->GetShader()->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetRotationMatrix());
		skybox->Render();

	}

	glEnable(GL_DEPTH_TEST);

	glUseProgram(shader->GetProgram());

	glUniformMatrix4fv(shader->GetUniformID("view"), 1, GL_FALSE, camera->GetViewMatrix());
	glUniformMatrix4fv(shader->GetUniformID("projection"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(shader->GetUniformID("lightSpaceMatrix"), 1, GL_FALSE, light->GetLightSpaceMatrix());
	glUniform3fv(shader->GetUniformID("lightPos"), 1, light->GetPosition());
	glUniform3fv(shader->GetUniformID("viewPos"), 1, camera->GetComponent<TransformComponent>()->GetPosition());
	glUniform3fv(shader->GetUniformID("lightColor"), 1, lightColor);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "shadowMap"), 0);

	glActiveTexture(GL_TEXTURE1);

	for (int i = 0; i < sceneActors.size(); ++i)
	{
		if (sceneActors[i]->GetComponent<MaterialComponent>() != nullptr) 
		{
			glBindTexture(GL_TEXTURE_2D, sceneActors[i]->GetComponent<MaterialComponent>()->getTextureID());
			glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTexture"), 1);
		}
		if(sceneActors[i]->getModelMatrix() != nullptr) glUniformMatrix4fv(shader->GetUniformID("model"), 1, GL_FALSE, sceneActors[i]->getModelMatrix());
		if(sceneActors[i]->GetComponent<MeshComponent>() != nullptr) sceneActors[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	glUseProgram(0);
}

void Scene0::CreateBuffer()
{

	glGenFramebuffers(1, &shadowMapFBO);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Scene0::RenderShadowMap()
{

	Ref<ShaderComponent> shadowShader = assetManager->GetComponent<ShaderComponent>("S_Shadow");
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glViewport(0, 0, 4096, 4096);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glUseProgram(shadowShader->GetProgram());

	glUniformMatrix4fv(shadowShader->GetUniformID("lightSpaceMatrix"), 1, GL_FALSE, light->GetLightSpaceMatrix());

	for (int i = 0; i < sceneActors.size(); ++i)
	{
		glUniformMatrix4fv(shadowShader->GetUniformID("model"), 1, GL_FALSE, sceneActors[i]->getModelMatrix());
		if (sceneActors[i]->GetComponent<MeshComponent>() != nullptr) sceneActors[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);

}

int Scene0::Pick(int x, int y) {
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); /// Paint the backgound white which is 0x00FFFFFF
	glClear(GL_COLOR_BUFFER_BIT);
	Ref<ShaderComponent> colorPickerShader = assetManager->GetComponent<ShaderComponent>("S_ColorPicker");
	glUseProgram(colorPickerShader->GetProgram());

	glUniformMatrix4fv(colorPickerShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(colorPickerShader->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());

	for (int i = 0; i < sceneActors.size(); ++i)
	{
		glUniformMatrix4fv(colorPickerShader->GetUniformID("modelMatrix"), 1, GL_FALSE, sceneActors[i]->getModelMatrix());
		glUniform1ui(colorPickerShader->GetUniformID("colorID"), i);
		if (sceneActors[i]->GetComponent<MeshComponent>() != nullptr) sceneActors[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	glUseProgram(0);

	GLuint colorIndex;
	glReadPixels(x, viewport.height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &colorIndex);
	colorIndex &= 0x00FFFFFF; /// This zeros out the alpha component
	if (colorIndex == 0x00FFFFFF) 
	{
		objectSelected = false;
		return -1;
	}
	else
	{
		objectSelected = true;
		return colorIndex;
	}
}

void Scene0::UpdateSkybox(const char* name)
{
	skybox = assetManager->GetComponent<SkyboxActor>(name);
	currentSkybox = name;
}

void Scene0::LoadSaveFile()
{
	XML.LoadFile("XMLs/SaveFile.xml");
	bool status = XML.Error();
	if (status) {
		std::cout << XML.ErrorIDToName(XML.ErrorID()) << std::endl;
		return;
	}

	rootData = XML.RootElement();

	assetsData = rootData->FirstChildElement("Data");

	for (XMLElement* child = assetsData->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
	{
		if (!strcmp(child->Name(), "MeshesCount"))
		{
			int i;
			sscanf_s(child->Attribute("MeshesCount"), "%i", &i);
			meshesCount = i;
		}

		for (int i = 0; i <= meshesCount + 1; ++i) 
		{
			if (!strcmp(child->Name(), ("StaticMesh" + std::to_string(i)).c_str()))
			{
				Ref<Actor> actor = std::make_shared<Actor>(nullptr);
				actor->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>(child->Attribute(("StaticMesh" + std::to_string(i)).c_str())));
				actor->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -5.0f), Quaternion(), Vec3(1.0f, 1.0f, 1.0f));
				actor->AddComponent(assetManager->GetComponent<MaterialComponent>("M_CheckerBoard"));
				sceneActors.push_back(actor);
			}
		}
	}

	for (XMLElement* child = assetsData->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
	{
		if (!strcmp(child->Name(), "CameraPosition"))
		{
			float x, y, z;
			sscanf_s(child->Attribute("CameraPosition"), "%f,%f,%f", &x, &y, &z);
			camera->GetComponent<TransformComponent>()->SetPosition(Vec3(x, y, z));
		}

		if (!strcmp(child->Name(), "CameraRotation"))
		{
			float x, y, z, w;
			sscanf_s(child->Attribute("CameraRotation"), "%f,%f,%f,%f", &x, &y, &z, &w);
			cameraOrientationVector = Vec4(x, y, z, w);
		}

		if (!strcmp(child->Name(), "LightPosition"))
		{
			float x, y, z;
			sscanf_s(child->Attribute("LightPosition"), "%f,%f,%f", &x, &y, &z);
			lightPosition = Vec3(x, y, z);
			light->UpdatePosition(Vec3(x, y, z));
		}

		if (!strcmp(child->Name(), "LightColor"))
		{
			float x, y, z, w;
			sscanf_s(child->Attribute("LightColor"), "%f,%f,%f,%f", &x, &y, &z, &w);
			lightColor = Vec4(x, y, z, w);
		}

		if (!strcmp(child->Name(), "BackgroundColor"))
		{
			float x, y, z, w;
			sscanf_s(child->Attribute("BackgroundColor"), "%f,%f,%f,%f", &x, &y, &z, &w);
			backgroundColor = Vec4(x, y, z, w);
		}

		if (!strcmp(child->Name(), "Skybox"))
		{
			UpdateSkybox(child->Attribute("Skybox"));
		}

		for (int i = 0; i < sceneActors.size(); ++i)
		{
			if (!strcmp(child->Name(), ("MeshPosition" + std::to_string(i)).c_str()))
			{
				float x, y, z;
				sscanf_s(child->Attribute(("MeshPosition" + std::to_string(i)).c_str()), "%f, %f, %f", &x, &y, &z);
				sceneActors[i]->GetComponent<TransformComponent>()->SetPosition(Vec3(x, y, z));
			}

			if (!strcmp(child->Name(), ("MeshOrientation" + std::to_string(i)).c_str()))
			{
				float x, y, z, w;
				sscanf_s(child->Attribute(("MeshOrientation" + std::to_string(i)).c_str()), "%f,%f,%f,%f", &x, &y, &z, &w);
				sceneActors[i]->GetComponent<TransformComponent>()->SetOrientation(Quaternion(w, Vec3(x, y, z)));
			}

			if (!strcmp(child->Name(), ("MeshScale" + std::to_string(i)).c_str()))
			{
				float x, y, z;
				sscanf_s(child->Attribute(("MeshScale" + std::to_string(i)).c_str()), "%f,%f,%f", &x, &y, &z);
				sceneActors[i]->GetComponent<TransformComponent>()->SetScale(Vec3(x, y, z));
			}

			if (!strcmp(child->Name(), ("Material" + std::to_string(i)).c_str()))
			{
				sceneActors[i]->RemoveComponent<MaterialComponent>();
				sceneActors[i]->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>(child->Attribute(("Material" + std::to_string(i)).c_str())));
			}
		}
	}
}

void Scene0::Save()
{
	for (int i = 0; i < sceneActors.size(); ++i)
	{
		saveSystem.SaveVec3(("MeshPosition" + std::to_string(i)).c_str(), sceneActors[i]->GetComponent<TransformComponent>()->GetPosition());
		saveSystem.SaveVec4(("MeshOrientation" + std::to_string(i)).c_str(), Vec4(sceneActors[i]->GetComponent<TransformComponent>()->GetOrientation().ijk.x,
			sceneActors[i]->GetComponent<TransformComponent>()->GetOrientation().ijk.y,
			sceneActors[i]->GetComponent<TransformComponent>()->GetOrientation().ijk.z,
			sceneActors[i]->GetComponent<TransformComponent>()->GetOrientation().w));
		saveSystem.SaveVec3(("MeshScale" + std::to_string(i)).c_str(), sceneActors[i]->GetComponent<TransformComponent>()->GetScale());
	}
	saveSystem.SaveInt("MeshesCount", meshesCount);
	saveSystem.SaveVec3("CameraPosition", camera->GetComponent<TransformComponent>()->GetPosition());
	saveSystem.SaveVec4("CameraRotation", cameraOrientationVector);
	saveSystem.SaveVec3("LightPosition", lightPosition);
	saveSystem.SaveVec4("LightColor", lightColor);
	saveSystem.SaveVec4("BackgroundColor", backgroundColor);
	saveSystem.SaveSkybox("Skybox", currentSkybox);
	printf("Progress Saved \n");
}
