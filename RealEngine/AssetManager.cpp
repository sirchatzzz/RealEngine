#include "AssetManager.h"

AssetManager::AssetManager(): isCreated(false), assetsData(nullptr), rootData(nullptr) {
	ReadXML();

	ListAllComponents();

	OnCreate();
}

AssetManager::~AssetManager() {
	OnDestroy();
}

bool AssetManager::OnCreate() {
	if (isCreated) return isCreated;
	for (std::pair<std::string, Ref<Component>> c : assetsCatalog) {
		if (!c.second->OnCreate()) {
			isCreated = false;
		}
	}
	return isCreated;
}

void AssetManager::OnDestroy() {
	RemoveAllComponents();
}

void AssetManager::RemoveAllComponents() {
	assetsCatalog.clear();
}

void AssetManager::ListAllComponents() const {
	std::cout <<"\n"<< typeid(*this).name() << " contain the following components:\n";
	for (auto c : assetsCatalog) {
		std::cout <<"\t" << c.first << ": " << typeid(*(c.second.get())).name() << "\n";
	}
	std::cout << "\n";
}

void AssetManager::LoadXML() {
	XML.LoadFile("XMLs/AssetManager.xml");
	bool status = XML.Error();
	if (status) {
		std::cout << XML.ErrorIDToName(XML.ErrorID()) << std::endl;
		return;
	}
}

void AssetManager::ReadXML() {
	LoadXML();

	rootData = XML.RootElement();

	assetsData = rootData->FirstChildElement("Assets");

	for (XMLElement* child = assetsData->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) 
	{
		if (!strcmp(child->Name(), "Mesh")) {
			AddComponent<MeshComponent>(child->Attribute("name"), nullptr, child->Attribute("path"));
		}
		if (!strcmp(child->Name(), "Shader")) {
			AddComponent<ShaderComponent>(child->Attribute("name"), nullptr, child->Attribute("vert"), child->Attribute("frag"));
		}
		if (!strcmp(child->Name(), "Material")) {
			AddComponent<MaterialComponent>(child->Attribute("name"), nullptr, child->Attribute("path"));
		}
		if (!strcmp(child->Name(), "Sound")) {
			AddComponent<AudioComponent>(child->Attribute("name"), nullptr, child->Attribute("path"));
		}
		if (!strcmp(child->Name(), "Skybox")) {
			AddComponent<SkyboxActor>(child->Attribute("name"), nullptr, child->Attribute("posX"), child->Attribute("negX"), child->Attribute("posY"), child->Attribute("negY"), child->Attribute("posZ"), child->Attribute("negZ"));
		}
		if (!strcmp(child->Name(), "Light")) {
			AddComponent<LightActor>(child->Attribute("name"), nullptr);
		}
		if (!strcmp(child->Name(), "Camera")) {
			AddComponent<CameraActor>(child->Attribute("name"), nullptr);
		}
	}
}
