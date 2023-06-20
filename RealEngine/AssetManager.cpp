#include "AssetManager.h"

AssetManager::AssetManager(): isCreated(false) {
	ReadXML();

	ListAllComponents();

	OnCreate();
}

AssetManager::~AssetManager() {
	RemoveAllComponents();
}

bool AssetManager::OnCreate() {
	if (isCreated) return isCreated;
	for (std::pair<std::string, Ref<Component>> c : componentCatalog) {
		if (!c.second->OnCreate()) {
			isCreated = false;
		}
	}
	return isCreated;
}

void AssetManager::RemoveAllComponents() {
	componentCatalog.clear();
}

void AssetManager::ListAllComponents() const {
	std::cout <<"\n"<< typeid(*this).name() << " contain the following components:\n";
	for (auto c : componentCatalog) {
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

	std::string mesh = "Mesh";
	std::string shader = "Shader";
	std::string material = "Material";
	std::string sound = "Sound";

	XMLElement* rootData = XML.RootElement();

	XMLElement* assetsData = rootData->FirstChildElement("Assets");

	for (XMLElement* child = assetsData->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
		if (child->Name() == mesh) {
			AddComponent<MeshComponent>(child->Attribute("name"), nullptr, child->Attribute("path"));
		}
		if (child->Name() == shader) {
			AddComponent<ShaderComponent>(child->Attribute("name"), nullptr, child->Attribute("vert"), child->Attribute("frag"));
		}
		if (child->Name() == material) {
			AddComponent<MaterialComponent>(child->Attribute("name"), nullptr, child->Attribute("path"));
		}
		if (child->Name() == sound) {
			AddComponent<AudioComponent>(child->Attribute("name"), nullptr, child->Attribute("path"));
		}
	}
}
