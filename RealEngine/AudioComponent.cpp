#include "AudioComponent.h"
AudioComponent::AudioComponent(Component* parent_) :Component(parent_), isCreated(false){

}

AudioComponent::~AudioComponent() { 
	OnDestroy();
}

bool AudioComponent::OnCreate() {
	for (std::pair<std::string, Ref<Component>> c : musicCatalog) {
		if (!c.second->OnCreate()) isCreated = false;
	}

	for (std::pair<std::string, Ref<Component>> c : chunkCatalog) {
		if (!c.second->OnCreate()) isCreated = false;
	}

	return isCreated;
}

void AudioComponent::OnDestroy() {
}

void AudioComponent::LoadConfig() {
	XML.LoadFile("XMLs/Config.xml");
	bool status = XML.Error();
	if (status) {
		std::cout << XML.ErrorIDToName(XML.ErrorID()) << std::endl;
		return;
	}
}

void AudioComponent::ReadConfig() {
	LoadConfig();

	std::string music = "Music";
	std::string chunk = "Chunk";

	XMLElement* rootData = XML.RootElement();
	XMLElement* assetsData = rootData->FirstChildElement("Sound");

	for (XMLElement* child = assetsData->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
		if (child->Name() == music) {
			AddChunk<AudioComponent>("Ding", nullptr, "audio/ding.mp3");
		}
		if (child->Name() == chunk) {
		}
	}
}
