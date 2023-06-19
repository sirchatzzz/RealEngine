#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include "tinyxml2.h"
#include "Component.h"

using namespace tinyxml2;

class AudioComponent : public Component {
	bool isCreated;
	std::unordered_map<std::string, Ref<Component> > musicCatalog;
	std::unordered_map<std::string, Ref<Component> > chunkCatalog;
	XMLDocument XML;

public:
	AudioComponent(Component* parent_);
	~AudioComponent();
	bool OnCreate() override;
	void OnDestroy() override;

	void LoadConfig();
	void ReadConfig();

	template<typename ComponentTemplate, typename ... Args>
	void AddMusic(std::string name, Args&& ... args_) {
		Ref<ComponentTemplate> t = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		musicCatalog[name] = t;
	}

	template<typename ComponentTemplate, typename ... Args>
	void AddChunk(std::string name, Args&& ... args_) {
		Ref<ComponentTemplate> t = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		chunkCatalog[name] = t;
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetMusic(std::string name) const {
		auto id = musicCatalog.find(name);
		if (id == musicCatalog.end()) {
			std::cout<<("Can't fint requested component", __FILE__, __LINE__, "\n");
			return Ref<ComponentTemplate>(nullptr);
		}
		return std::dynamic_pointer_cast<ComponentTemplate>(id->second);
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetChunk(std::string name) const {
		auto id = chunkCatalog.find(name);
		if (id == chunkCatalog.end()) {
			std::cout<<("Can't fint requested component", __FILE__, __LINE__, "\n");
			return Ref<ComponentTemplate>(nullptr);
		}
		return std::dynamic_pointer_cast<ComponentTemplate>(id->second);
	}
};

